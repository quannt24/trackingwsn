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

Define_Module(Link802154);

void Link802154::initialize()
{
    macAddress = this->getId(); // Address is also module id of link module
}

void Link802154::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == txMsg) {
            // Transmit timer
            transmitFrames();
        }
    } else {
        if (msg->getArrivalGate() == gate("netGate$i")) {
            // Packet from upper layer, assemble frame and send to next hop
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
    txMsg = new cMessage("TxMsg");
}

Link802154::~Link802154()
{
    cancelAndDelete(txMsg);
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
    frm->setDesAddr(adjNode[intuniform(0, numAdjNode - 1)]); // TODO Test
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

    // If transmit timer is not set, set it immediately
    if (!txMsg->isScheduled()) scheduleAt(simTime(), txMsg);
}

/*
 * Check if channel is idle, then transmit queued frames to the air.
 * If channel is busy, back-off.
 */
void Link802154::transmitFrames()
{
    Frame802154 *frm;
    Link802154 *des;
    double txDuration;

    while (!outQueue.isEmpty()) {
        // TODO Sense channel, back-off if channel is busy
        frm = (Frame802154*) outQueue.pop();
        des = (Link802154*) simulation.getModule(frm->getDesAddr());
        if (des == NULL) {
            EV << "Link802154::transmitFrames : destination error\n";
            delete frm;
        } else {
            txDuration = ((double) frm->getBitLength()) / par("bitRate").doubleValue();
            EV << "Link802154::transmitFrames : Tx Duration " << txDuration << "\n";
            sendDirect(frm, 0, txDuration, des, "radioIn");
        }
    }
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
