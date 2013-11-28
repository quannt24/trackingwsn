//
// Generated file, do not edit! Created by opp_msgc 4.3 from sensors/sensereq.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "sensereq_m.h"

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




Register_Class(SenseReq);

SenseReq::SenseReq(const char *name, int kind) : cMessage(name,kind)
{
}

SenseReq::SenseReq(const SenseReq& other) : cMessage(other)
{
    copy(other);
}

SenseReq::~SenseReq()
{
}

SenseReq& SenseReq::operator=(const SenseReq& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void SenseReq::copy(const SenseReq& other)
{
    this->srcAss_var = other.srcAss_var;
    this->srcMob_var = other.srcMob_var;
}

void SenseReq::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->srcAss_var);
    doPacking(b,this->srcMob_var);
}

void SenseReq::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->srcAss_var);
    doUnpacking(b,this->srcMob_var);
}

AssPtr& SenseReq::getSrcAss()
{
    return srcAss_var;
}

void SenseReq::setSrcAss(const AssPtr& srcAss)
{
    this->srcAss_var = srcAss;
}

MobPtr& SenseReq::getSrcMob()
{
    return srcMob_var;
}

void SenseReq::setSrcMob(const MobPtr& srcMob)
{
    this->srcMob_var = srcMob;
}

class SenseReqDescriptor : public cClassDescriptor
{
  public:
    SenseReqDescriptor();
    virtual ~SenseReqDescriptor();

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

Register_ClassDescriptor(SenseReqDescriptor);

SenseReqDescriptor::SenseReqDescriptor() : cClassDescriptor("SenseReq", "cMessage")
{
}

SenseReqDescriptor::~SenseReqDescriptor()
{
}

bool SenseReqDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SenseReq *>(obj)!=NULL;
}

const char *SenseReqDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SenseReqDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int SenseReqDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SenseReqDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "srcAss",
        "srcMob",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int SenseReqDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAss")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcMob")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SenseReqDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "AssPtr",
        "MobPtr",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *SenseReqDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SenseReqDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SenseReq *pp = (SenseReq *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SenseReqDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SenseReq *pp = (SenseReq *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSrcAss(); return out.str();}
        case 1: {std::stringstream out; out << pp->getSrcMob(); return out.str();}
        default: return "";
    }
}

bool SenseReqDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SenseReq *pp = (SenseReq *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *SenseReqDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "AssPtr",
        "MobPtr",
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *SenseReqDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SenseReq *pp = (SenseReq *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSrcAss()); break;
        case 1: return (void *)(&pp->getSrcMob()); break;
        default: return NULL;
    }
}


