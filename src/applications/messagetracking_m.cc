//
// Generated file, do not edit! Created by opp_msgc 4.3 from applications/messagetracking.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "messagetracking_m.h"

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
    cEnum *e = cEnum::find("MessageType");
    if (!e) enums.getInstance()->add(e = new cEnum("MessageType"));
    e->insert(MSG_SYNC_REQUEST, "MSG_SYNC_REQUEST");
    e->insert(MSG_SENSE_RESULT, "MSG_SENSE_RESULT");
);

Register_Class(MsgTracking);

MsgTracking::MsgTracking(const char *name, int kind) : MessageCR(name,kind)
{
    this->msgType_var = 0;
    this->msgSize_var = 10;
}

MsgTracking::MsgTracking(const MsgTracking& other) : MessageCR(other)
{
    copy(other);
}

MsgTracking::~MsgTracking()
{
}

MsgTracking& MsgTracking::operator=(const MsgTracking& other)
{
    if (this==&other) return *this;
    MessageCR::operator=(other);
    copy(other);
    return *this;
}

void MsgTracking::copy(const MsgTracking& other)
{
    this->msgType_var = other.msgType_var;
    this->msgSize_var = other.msgSize_var;
}

void MsgTracking::parsimPack(cCommBuffer *b)
{
    MessageCR::parsimPack(b);
    doPacking(b,this->msgType_var);
    doPacking(b,this->msgSize_var);
}

void MsgTracking::parsimUnpack(cCommBuffer *b)
{
    MessageCR::parsimUnpack(b);
    doUnpacking(b,this->msgType_var);
    doUnpacking(b,this->msgSize_var);
}

int MsgTracking::getMsgType() const
{
    return msgType_var;
}

void MsgTracking::setMsgType(int msgType)
{
    this->msgType_var = msgType;
}

int MsgTracking::getMsgSize() const
{
    return msgSize_var;
}

void MsgTracking::setMsgSize(int msgSize)
{
    this->msgSize_var = msgSize;
}

class MsgTrackingDescriptor : public cClassDescriptor
{
  public:
    MsgTrackingDescriptor();
    virtual ~MsgTrackingDescriptor();

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

Register_ClassDescriptor(MsgTrackingDescriptor);

MsgTrackingDescriptor::MsgTrackingDescriptor() : cClassDescriptor("MsgTracking", "MessageCR")
{
}

MsgTrackingDescriptor::~MsgTrackingDescriptor()
{
}

bool MsgTrackingDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MsgTracking *>(obj)!=NULL;
}

const char *MsgTrackingDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MsgTrackingDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MsgTrackingDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MsgTrackingDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "msgType",
        "msgSize",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MsgTrackingDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgSize")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MsgTrackingDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MsgTrackingDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int MsgTrackingDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MsgTracking *pp = (MsgTracking *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MsgTrackingDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MsgTracking *pp = (MsgTracking *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMsgType());
        case 1: return long2string(pp->getMsgSize());
        default: return "";
    }
}

bool MsgTrackingDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MsgTracking *pp = (MsgTracking *)object; (void)pp;
    switch (field) {
        case 0: pp->setMsgType(string2long(value)); return true;
        case 1: pp->setMsgSize(string2long(value)); return true;
        default: return false;
    }
}

const char *MsgTrackingDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MsgTrackingDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MsgTracking *pp = (MsgTracking *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MsgSyncRequest);

MsgSyncRequest::MsgSyncRequest(const char *name, int kind) : MsgTracking(name,kind)
{
    this->routingType_var = RT_BROADCAST;
    this->msgType_var = MSG_SYNC_REQUEST;
}

MsgSyncRequest::MsgSyncRequest(const MsgSyncRequest& other) : MsgTracking(other)
{
    copy(other);
}

MsgSyncRequest::~MsgSyncRequest()
{
}

MsgSyncRequest& MsgSyncRequest::operator=(const MsgSyncRequest& other)
{
    if (this==&other) return *this;
    MsgTracking::operator=(other);
    copy(other);
    return *this;
}

void MsgSyncRequest::copy(const MsgSyncRequest& other)
{
    this->routingType_var = other.routingType_var;
    this->msgType_var = other.msgType_var;
}

void MsgSyncRequest::parsimPack(cCommBuffer *b)
{
    MsgTracking::parsimPack(b);
    doPacking(b,this->routingType_var);
    doPacking(b,this->msgType_var);
}

void MsgSyncRequest::parsimUnpack(cCommBuffer *b)
{
    MsgTracking::parsimUnpack(b);
    doUnpacking(b,this->routingType_var);
    doUnpacking(b,this->msgType_var);
}

int MsgSyncRequest::getRoutingType() const
{
    return routingType_var;
}

void MsgSyncRequest::setRoutingType(int routingType)
{
    this->routingType_var = routingType;
}

int MsgSyncRequest::getMsgType() const
{
    return msgType_var;
}

void MsgSyncRequest::setMsgType(int msgType)
{
    this->msgType_var = msgType;
}

class MsgSyncRequestDescriptor : public cClassDescriptor
{
  public:
    MsgSyncRequestDescriptor();
    virtual ~MsgSyncRequestDescriptor();

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

Register_ClassDescriptor(MsgSyncRequestDescriptor);

MsgSyncRequestDescriptor::MsgSyncRequestDescriptor() : cClassDescriptor("MsgSyncRequest", "MsgTracking")
{
}

MsgSyncRequestDescriptor::~MsgSyncRequestDescriptor()
{
}

bool MsgSyncRequestDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MsgSyncRequest *>(obj)!=NULL;
}

const char *MsgSyncRequestDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MsgSyncRequestDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MsgSyncRequestDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MsgSyncRequestDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "routingType",
        "msgType",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MsgSyncRequestDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingType")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MsgSyncRequestDescriptor::getFieldTypeString(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MsgSyncRequestDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int MsgSyncRequestDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MsgSyncRequest *pp = (MsgSyncRequest *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MsgSyncRequestDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MsgSyncRequest *pp = (MsgSyncRequest *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRoutingType());
        case 1: return long2string(pp->getMsgType());
        default: return "";
    }
}

bool MsgSyncRequestDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MsgSyncRequest *pp = (MsgSyncRequest *)object; (void)pp;
    switch (field) {
        case 0: pp->setRoutingType(string2long(value)); return true;
        case 1: pp->setMsgType(string2long(value)); return true;
        default: return false;
    }
}

const char *MsgSyncRequestDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *MsgSyncRequestDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MsgSyncRequest *pp = (MsgSyncRequest *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(MsgSenseResult);

MsgSenseResult::MsgSenseResult(const char *name, int kind) : MsgTracking(name,kind)
{
    this->routingType_var = RT_BROADCAST;
    this->msgType_var = MSG_SENSE_RESULT;
}

MsgSenseResult::MsgSenseResult(const MsgSenseResult& other) : MsgTracking(other)
{
    copy(other);
}

MsgSenseResult::~MsgSenseResult()
{
}

MsgSenseResult& MsgSenseResult::operator=(const MsgSenseResult& other)
{
    if (this==&other) return *this;
    MsgTracking::operator=(other);
    copy(other);
    return *this;
}

void MsgSenseResult::copy(const MsgSenseResult& other)
{
    this->routingType_var = other.routingType_var;
    this->msgType_var = other.msgType_var;
    this->meaList_var = other.meaList_var;
}

void MsgSenseResult::parsimPack(cCommBuffer *b)
{
    MsgTracking::parsimPack(b);
    doPacking(b,this->routingType_var);
    doPacking(b,this->msgType_var);
    doPacking(b,this->meaList_var);
}

void MsgSenseResult::parsimUnpack(cCommBuffer *b)
{
    MsgTracking::parsimUnpack(b);
    doUnpacking(b,this->routingType_var);
    doUnpacking(b,this->msgType_var);
    doUnpacking(b,this->meaList_var);
}

int MsgSenseResult::getRoutingType() const
{
    return routingType_var;
}

void MsgSenseResult::setRoutingType(int routingType)
{
    this->routingType_var = routingType;
}

int MsgSenseResult::getMsgType() const
{
    return msgType_var;
}

void MsgSenseResult::setMsgType(int msgType)
{
    this->msgType_var = msgType;
}

MeasurementList& MsgSenseResult::getMeaList()
{
    return meaList_var;
}

void MsgSenseResult::setMeaList(const MeasurementList& meaList)
{
    this->meaList_var = meaList;
}

class MsgSenseResultDescriptor : public cClassDescriptor
{
  public:
    MsgSenseResultDescriptor();
    virtual ~MsgSenseResultDescriptor();

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

Register_ClassDescriptor(MsgSenseResultDescriptor);

MsgSenseResultDescriptor::MsgSenseResultDescriptor() : cClassDescriptor("MsgSenseResult", "MsgTracking")
{
}

MsgSenseResultDescriptor::~MsgSenseResultDescriptor()
{
}

bool MsgSenseResultDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MsgSenseResult *>(obj)!=NULL;
}

const char *MsgSenseResultDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MsgSenseResultDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int MsgSenseResultDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *MsgSenseResultDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "routingType",
        "msgType",
        "meaList",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int MsgSenseResultDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingType")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "meaList")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MsgSenseResultDescriptor::getFieldTypeString(void *object, int field) const
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
        "MeasurementList",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *MsgSenseResultDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int MsgSenseResultDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MsgSenseResult *pp = (MsgSenseResult *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MsgSenseResultDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MsgSenseResult *pp = (MsgSenseResult *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getRoutingType());
        case 1: return long2string(pp->getMsgType());
        case 2: {std::stringstream out; out << pp->getMeaList(); return out.str();}
        default: return "";
    }
}

bool MsgSenseResultDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MsgSenseResult *pp = (MsgSenseResult *)object; (void)pp;
    switch (field) {
        case 0: pp->setRoutingType(string2long(value)); return true;
        case 1: pp->setMsgType(string2long(value)); return true;
        default: return false;
    }
}

const char *MsgSenseResultDescriptor::getFieldStructName(void *object, int field) const
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
        "MeasurementList",
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *MsgSenseResultDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MsgSenseResult *pp = (MsgSenseResult *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getMeaList()); break;
        default: return NULL;
    }
}


