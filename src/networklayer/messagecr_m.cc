//
// Generated file, do not edit! Created by opp_msgc 4.3 from networklayer/messagecr.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "messagecr_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("RoutingType");
    if (!e) enums.getInstance()->add(e = new cEnum("RoutingType"));
    e->insert(RT_TO_AN, "RT_TO_AN");
    e->insert(RT_TO_BS, "RT_TO_BS");
    e->insert(RT_BROADCAST, "RT_BROADCAST");
);

Register_Class(MessageCR);

MessageCR::MessageCR(const char *name, int kind) : cPacket(name,kind)
{
    this->routingType_var = 0;
    this->desMacAddr_var = 0;
    this->preambleFlag_var = false;
    this->msgSize_var = 10;
}

MessageCR::MessageCR(const MessageCR& other) : cPacket(other)
{
    copy(other);
}

MessageCR::~MessageCR()
{
}

MessageCR& MessageCR::operator=(const MessageCR& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void MessageCR::copy(const MessageCR& other)
{
    this->routingType_var = other.routingType_var;
    this->desMacAddr_var = other.desMacAddr_var;
    this->preambleFlag_var = other.preambleFlag_var;
    this->msgSize_var = other.msgSize_var;
}

void MessageCR::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->routingType_var);
    doPacking(b,this->desMacAddr_var);
    doPacking(b,this->preambleFlag_var);
    doPacking(b,this->msgSize_var);
}

void MessageCR::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->routingType_var);
    doUnpacking(b,this->desMacAddr_var);
    doUnpacking(b,this->preambleFlag_var);
    doUnpacking(b,this->msgSize_var);
}

int MessageCR::getRoutingType() const
{
    return routingType_var;
}

void MessageCR::setRoutingType(int routingType)
{
    this->routingType_var = routingType;
}

int MessageCR::getDesMacAddr() const
{
    return desMacAddr_var;
}

void MessageCR::setDesMacAddr(int desMacAddr)
{
    this->desMacAddr_var = desMacAddr;
}

bool MessageCR::getPreambleFlag() const
{
    return preambleFlag_var;
}

void MessageCR::setPreambleFlag(bool preambleFlag)
{
    this->preambleFlag_var = preambleFlag;
}

int MessageCR::getMsgSize() const
{
    return msgSize_var;
}

void MessageCR::setMsgSize(int msgSize)
{
    this->msgSize_var = msgSize;
}

class MessageCRDescriptor : public cClassDescriptor
{
  public:
    MessageCRDescriptor();
    virtual ~MessageCRDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MessageCRDescriptor);

MessageCRDescriptor::MessageCRDescriptor() : cClassDescriptor("MessageCR", "cPacket")
{
}

MessageCRDescriptor::~MessageCRDescriptor()
{
}

bool MessageCRDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MessageCR *>(obj)!=NULL;
}

const char *MessageCRDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MessageCRDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int MessageCRDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *MessageCRDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "routingType",
        "desMacAddr",
        "preambleFlag",
        "msgSize",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int MessageCRDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingType")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "desMacAddr")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "preambleFlag")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgSize")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MessageCRDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "bool",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *MessageCRDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int MessageCRDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MessageCR *pp = (MessageCR *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MessageCRDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MessageCR *pp = (MessageCR *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRoutingType());
        case 1: return long2string(pp->getDesMacAddr());
        case 2: return bool2string(pp->getPreambleFlag());
        case 3: return long2string(pp->getMsgSize());
        default: return "";
    }
}

bool MessageCRDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MessageCR *pp = (MessageCR *)object; (void)pp;
    switch (field) {
        case 0: pp->setRoutingType(string2long(value)); return true;
        case 1: pp->setDesMacAddr(string2long(value)); return true;
        case 2: pp->setPreambleFlag(string2bool(value)); return true;
        case 3: pp->setMsgSize(string2long(value)); return true;
        default: return false;
    }
}

const char *MessageCRDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *MessageCRDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MessageCR *pp = (MessageCR *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


