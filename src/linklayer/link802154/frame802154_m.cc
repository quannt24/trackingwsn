//
// Generated file, do not edit! Created by opp_msgc 4.4 from linklayer/link802154/frame802154.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "frame802154_m.h"

USING_NAMESPACE

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
    cEnum *e = cEnum::find("FrType");
    if (!e) enums.getInstance()->add(e = new cEnum("FrType"));
    e->insert(FR_PAYLOAD, "FR_PAYLOAD");
    e->insert(FR_PREAMBLE, "FR_PREAMBLE");
    e->insert(FR_PREAMBLE_ACK, "FR_PREAMBLE_ACK");
);

Register_Class(Frame802154);

Frame802154::Frame802154(const char *name, int kind) : ::cPacket(name,kind)
{
    this->type_var = FR_PAYLOAD;
    this->srcAddr_var = 0;
    this->desAddr_var = 0;
}

Frame802154::Frame802154(const Frame802154& other) : ::cPacket(other)
{
    copy(other);
}

Frame802154::~Frame802154()
{
}

Frame802154& Frame802154::operator=(const Frame802154& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Frame802154::copy(const Frame802154& other)
{
    this->type_var = other.type_var;
    this->srcAddr_var = other.srcAddr_var;
    this->desAddr_var = other.desAddr_var;
}

void Frame802154::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->type_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->desAddr_var);
}

void Frame802154::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->type_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->desAddr_var);
}

int Frame802154::getType() const
{
    return type_var;
}

void Frame802154::setType(int type)
{
    this->type_var = type;
}

int Frame802154::getSrcAddr() const
{
    return srcAddr_var;
}

void Frame802154::setSrcAddr(int srcAddr)
{
    this->srcAddr_var = srcAddr;
}

int Frame802154::getDesAddr() const
{
    return desAddr_var;
}

void Frame802154::setDesAddr(int desAddr)
{
    this->desAddr_var = desAddr;
}

class Frame802154Descriptor : public cClassDescriptor
{
  public:
    Frame802154Descriptor();
    virtual ~Frame802154Descriptor();

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

Register_ClassDescriptor(Frame802154Descriptor);

Frame802154Descriptor::Frame802154Descriptor() : cClassDescriptor("Frame802154", "cPacket")
{
}

Frame802154Descriptor::~Frame802154Descriptor()
{
}

bool Frame802154Descriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Frame802154 *>(obj)!=NULL;
}

const char *Frame802154Descriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Frame802154Descriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int Frame802154Descriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *Frame802154Descriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "type",
        "srcAddr",
        "desAddr",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int Frame802154Descriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "desAddr")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Frame802154Descriptor::getFieldTypeString(void *object, int field) const
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
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *Frame802154Descriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Frame802154Descriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Frame802154 *pp = (Frame802154 *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Frame802154Descriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Frame802154 *pp = (Frame802154 *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getType());
        case 1: return long2string(pp->getSrcAddr());
        case 2: return long2string(pp->getDesAddr());
        default: return "";
    }
}

bool Frame802154Descriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Frame802154 *pp = (Frame802154 *)object; (void)pp;
    switch (field) {
        case 0: pp->setType(string2long(value)); return true;
        case 1: pp->setSrcAddr(string2long(value)); return true;
        case 2: pp->setDesAddr(string2long(value)); return true;
        default: return false;
    }
}

const char *Frame802154Descriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *Frame802154Descriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Frame802154 *pp = (Frame802154 *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


