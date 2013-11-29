//
// Generated file, do not edit! Created by opp_msgc 4.3 from linklayer/packet802154.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "packet802154_m.h"

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
    cEnum *e = cEnum::find("TxType");
    if (!e) enums.getInstance()->add(e = new cEnum("TxType"));
    e->insert(TX_PPP, "TX_PPP");
    e->insert(TX_BROADCAST, "TX_BROADCAST");
);

Register_Class(Packet802154);

Packet802154::Packet802154(const char *name, int kind) : cPacket(name,kind)
{
    this->txType_var = 0;
    this->srcMacAddr_var = 0;
    this->desMacAddr_var = 0;
    this->strobeFlag_var = false;
    this->pkSize_var = 18;
}

Packet802154::Packet802154(const Packet802154& other) : cPacket(other)
{
    copy(other);
}

Packet802154::~Packet802154()
{
}

Packet802154& Packet802154::operator=(const Packet802154& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void Packet802154::copy(const Packet802154& other)
{
    this->txType_var = other.txType_var;
    this->srcMacAddr_var = other.srcMacAddr_var;
    this->desMacAddr_var = other.desMacAddr_var;
    this->strobeFlag_var = other.strobeFlag_var;
    this->pkSize_var = other.pkSize_var;
}

void Packet802154::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->txType_var);
    doPacking(b,this->srcMacAddr_var);
    doPacking(b,this->desMacAddr_var);
    doPacking(b,this->strobeFlag_var);
    doPacking(b,this->pkSize_var);
}

void Packet802154::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->txType_var);
    doUnpacking(b,this->srcMacAddr_var);
    doUnpacking(b,this->desMacAddr_var);
    doUnpacking(b,this->strobeFlag_var);
    doUnpacking(b,this->pkSize_var);
}

int Packet802154::getTxType() const
{
    return txType_var;
}

void Packet802154::setTxType(int txType)
{
    this->txType_var = txType;
}

int Packet802154::getSrcMacAddr() const
{
    return srcMacAddr_var;
}

void Packet802154::setSrcMacAddr(int srcMacAddr)
{
    this->srcMacAddr_var = srcMacAddr;
}

int Packet802154::getDesMacAddr() const
{
    return desMacAddr_var;
}

void Packet802154::setDesMacAddr(int desMacAddr)
{
    this->desMacAddr_var = desMacAddr;
}

bool Packet802154::getStrobeFlag() const
{
    return strobeFlag_var;
}

void Packet802154::setStrobeFlag(bool strobeFlag)
{
    this->strobeFlag_var = strobeFlag;
}

int Packet802154::getPkSize() const
{
    return pkSize_var;
}

void Packet802154::setPkSize(int pkSize)
{
    this->pkSize_var = pkSize;
}

class Packet802154Descriptor : public cClassDescriptor
{
  public:
    Packet802154Descriptor();
    virtual ~Packet802154Descriptor();

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

Register_ClassDescriptor(Packet802154Descriptor);

Packet802154Descriptor::Packet802154Descriptor() : cClassDescriptor("Packet802154", "cPacket")
{
}

Packet802154Descriptor::~Packet802154Descriptor()
{
}

bool Packet802154Descriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Packet802154 *>(obj)!=NULL;
}

const char *Packet802154Descriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Packet802154Descriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int Packet802154Descriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *Packet802154Descriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "txType",
        "srcMacAddr",
        "desMacAddr",
        "strobeFlag",
        "pkSize",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int Packet802154Descriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "txType")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcMacAddr")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "desMacAddr")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "strobeFlag")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkSize")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Packet802154Descriptor::getFieldTypeString(void *object, int field) const
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
        "bool",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *Packet802154Descriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "TxType";
            return NULL;
        default: return NULL;
    }
}

int Packet802154Descriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Packet802154 *pp = (Packet802154 *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Packet802154Descriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Packet802154 *pp = (Packet802154 *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getTxType());
        case 1: return long2string(pp->getSrcMacAddr());
        case 2: return long2string(pp->getDesMacAddr());
        case 3: return bool2string(pp->getStrobeFlag());
        case 4: return long2string(pp->getPkSize());
        default: return "";
    }
}

bool Packet802154Descriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Packet802154 *pp = (Packet802154 *)object; (void)pp;
    switch (field) {
        case 0: pp->setTxType(string2long(value)); return true;
        case 1: pp->setSrcMacAddr(string2long(value)); return true;
        case 2: pp->setDesMacAddr(string2long(value)); return true;
        case 3: pp->setStrobeFlag(string2bool(value)); return true;
        case 4: pp->setPkSize(string2long(value)); return true;
        default: return false;
    }
}

const char *Packet802154Descriptor::getFieldStructName(void *object, int field) const
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
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *Packet802154Descriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Packet802154 *pp = (Packet802154 *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


