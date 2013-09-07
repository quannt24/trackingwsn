//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "link802154.h"
#include "frame802154_m.h"
#include "msgkind.h"
#include "wsnexception.h"
#include "channelutil.h"

Define_Module(Link802154);

void Link802154::initialize()
{
    macAddress = this->getId(); // Address is also module id of link module
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == csmaMsg) {
            // Start CSMA transmission
            csmaTransmit();
        } else if (msg == releaseChannelMsg) {
            releaseChannel();
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            // Packet from upper layer, assemble frame and add to sending queue
            queueFrame(createFrame((cPacket*) msg));
        } else if (msg->getArrivalGate() == gate("radioIn")) {
            // Frame from other node
            recvFrame((Frame802154*) msg);
        }
    }
}

Link802154::Link802154()
{
    numAdjNode = 0;
    txFrame = NULL;
    csmaMsg = new cMessage("CSMAMsg");
    releaseChannelMsg = new cMessage("ReleaseChannelMsg");
}

Link802154::~Link802154()
{
    cancelAndDelete(csmaMsg);
    cancelAndDelete(releaseChannelMsg);
    if (txFrame != NULL) delete txFrame;
    outQueue.clear();
}

/*
 * Get MAC address
 */
int Link802154::getAddr()
{
    return macAddress;
}

/*
 * Check if connection list is full or not.
 */
bool Link802154::isFullConn()
{
    return numAdjNode >= MAX_CONNECTIONS;
}

/*
 * Add a node to connection list.
 * Return: 0 on success, -1 when error
 */
int Link802154::addAdjNode(int addr)
{
    if (numAdjNode >= MAX_CONNECTIONS) return -1;

    adjNode[numAdjNode] = addr;
    numAdjNode++;
    return 0;
}

/*
 * Assemble a frame to encapsulate a packet.
 */
Frame802154* Link802154::createFrame(cPacket* packet)
{
    // TODO Process long packet
    if (packet->getByteLength() > par("maxPacketSize").longValue()) throw PACKET_TOO_LONG;

    Frame802154 *frm = new Frame802154("Frame802154");
    frm->setSrcAddr(macAddress);
    if (packet->getKind() == PK_BROADCAST) {
        // This packet is intended to be broadcasted
        frm->setDesAddr(BROADCAST_ADDR);
    } else {
        frm->setDesAddr(adjNode[intuniform(0, numAdjNode - 1)]); // TODO Test
    }
    frm->setByteLength(par("fldFrameControl").longValue() + par("fldSequenceId").longValue()
            + par("fldAddr").longValue() + par("fldFooter").longValue() + par("phyHeaderSize").longValue());
    frm->encapsulate(packet); // Frame length will be increased by length of packet

    return frm;
}

/*
 * Add Frame to sending queue and start a transmit timer.
 */
void Link802154::queueFrame(Frame802154 *frame)
{
    outQueue.insert(frame);
    // If CSMA transmission is not in process, start new one
    if (!csmaMsg->isScheduled() && !releaseChannelMsg->isScheduled()) scheduleAt(simTime(), csmaMsg);
}

/*
 * Receive frame from other node, forward to upper layer
 */
void Link802154::recvFrame(Frame802154* frame)
{
    // TODO Test
    EV << "Link802154::recvFrame : Frame size " << frame->getByteLength() << "\n";
    // Forward to upper layer
    send(frame->decapsulate(), "netGate$o");
    delete frame;
}

/*
 * Transmit a prepared frame using unslotted CSMA/CA.
 * This function is entry point of unslotted CSMA/CA algorithm.
 */
void Link802154::csmaTransmit()
{
    if (txFrame == NULL) {
        // Initialize when a frame is transmitted for first time
        if (outQueue.isEmpty()) return; // Nothing to transmit
        // Pop new frame from queue
        txFrame = (Frame802154*) outQueue.pop();
        // Initialize first CSMA attempt
        NB = 0;
        BE = par("macMinBE").longValue();
    } else {
        // Re-attempt to transmit prepared frame
        NB++;
        if (BE < par("aMaxBE").longValue()) {
            BE++;
        } else {
            BE = par("aMaxBE").longValue();
        }
    }

    if (performCCA()) {
        transmit();
        txFrame = NULL;
    } else {
        if (NB < par("aMaxNB").longValue()) {
            backoff();
        } else {
            // TODO Transmission failure
            delete txFrame;
            txFrame = NULL;
            EV << "Link802154::csmaTransmit : Transmission failure\n";
        }
    }
}

/*
 * Perform a Clear Channel Assessment (CCA).
 * In simulation, this function will also acquire the channel if possible.
 * Return true if channel is idle, false if channel is busy or error.
 */
bool Link802154::performCCA()
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");
    int ret = cu->acquireChannel(this);
    EV << "Link802154::performCCA : result " << ret << '\n';
    return ret == 0;
}

/*
 * Release channel
 */
void Link802154::releaseChannel()
{
    ChannelUtil *cu = (ChannelUtil*) simulation.getModuleByPath("Wsn.cu");
    cu->releaseChannel(this);
    EV << "Link802154::releaseChannel\n";
}

/*
 * Transmit prepared frame, set a timer to release channel when transmission completes.
 * If outgoing frame queue is not empty, set a timer for next CSMA transmission.
 */
void Link802154::transmit()
{
    double txDuration = 0;

    if (txFrame != NULL) {
        txDuration = ((double) txFrame->getBitLength()) / par("bitRate").doubleValue();
        int desAddr = txFrame->getDesAddr();
        Link802154 *des;

        if (desAddr == BROADCAST_ADDR) {
            // Broadcast frame. In simulation, we send frame to all connected nodes
            Frame802154 *copy;
            for (int i = 0; i < numAdjNode; i++) {
                des = (Link802154*) simulation.getModule(adjNode[i]);
                if (des != NULL) {
                    copy = txFrame->dup();
                    sendDirect(copy, 0, txDuration, des, "radioIn");
                } else {
                    EV << "Link802154::transmitFrames : destination error, ID " << adjNode[i] << '\n';
                    delete copy;
                }
            }
            delete txFrame; // Original frame is redundant
        } else {
            // Transmit to specific destination
            des = (Link802154*) simulation.getModule(desAddr);
            if (des != NULL) {
                sendDirect(txFrame, 0, txDuration, des, "radioIn");
            } else {
                EV << "Link802154::transmitFrames : destination error, ID " << desAddr << '\n';
                delete txFrame;
            }
        }

    }

    // Set a timer to release channel
    scheduleAt(simTime() + txDuration, releaseChannelMsg);

    if (!outQueue.isEmpty()) {
        // Set a timer to transmit next frame in queue
        scheduleAt(simTime() + txDuration, csmaMsg); // TODO A process time may need to be added to timer
    }
}

/*
 * Back-off when channel is not idle.
 */
void Link802154::backoff()
{
    double backoffDur = intuniform(0, (int)(pow(2, BE) - 1)) * aUnitBP;
    scheduleAt(simTime() + backoffDur, csmaMsg);
    EV << "Link802154::backoff : duration " << backoffDur << '\n';
}
