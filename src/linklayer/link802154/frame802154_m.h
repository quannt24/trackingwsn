//
// Generated file, do not edit! Created by opp_msgc 4.4 from linklayer/link802154/frame802154.msg.
//

#ifndef _FRAME802154_M_H_
#define _FRAME802154_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Enum generated from <tt>linklayer/link802154/frame802154.msg</tt> by opp_msgc.
 * <pre>
 * enum FrType {
 *     FR_PAYLOAD = 0; 
 *     FR_PREAMBLE = 1; 
 *     FR_PREAMBLE_ACK = 2; 
 * };
 * </pre>
 */
enum FrType {
    FR_PAYLOAD = 0,
    FR_PREAMBLE = 1,
    FR_PREAMBLE_ACK = 2
};

/**
 * Class generated from <tt>linklayer/link802154/frame802154.msg</tt> by opp_msgc.
 * <pre>
 * packet Frame802154 {
 *     int type = FR_PAYLOAD; 
 *     int srcAddr; 
 *     int desAddr; 
 * }
 * </pre>
 */
class Frame802154 : public ::cPacket
{
  protected:
    int type_var;
    int srcAddr_var;
    int desAddr_var;

  private:
    void copy(const Frame802154& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Frame802154&);

  public:
    Frame802154(const char *name=NULL, int kind=0);
    Frame802154(const Frame802154& other);
    virtual ~Frame802154();
    Frame802154& operator=(const Frame802154& other);
    virtual Frame802154 *dup() const {return new Frame802154(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getType() const;
    virtual void setType(int type);
    virtual int getSrcAddr() const;
    virtual void setSrcAddr(int srcAddr);
    virtual int getDesAddr() const;
    virtual void setDesAddr(int desAddr);
};

inline void doPacking(cCommBuffer *b, Frame802154& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, Frame802154& obj) {obj.parsimUnpack(b);}


#endif // _FRAME802154_M_H_
