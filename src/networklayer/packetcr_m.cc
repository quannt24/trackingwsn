//
// Generated file, do not edit! Created by opp_msgc 4.3 from networklayer/packetcr.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "packetcr_m.h"

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
    cEnum *e = cEnum::find("PacketType");
    if (!e) enums.getInstance()->add(e = new cEnum("PacketType"));
    e->insert(PK_PAYLOAD_TO_AN, "PK_PAYLOAD_TO_AN");
    e->insert(PK_PAYLOAD_TO_BS, "PK_PAYLOAD_TO_BS");
    e->insert(PK_REQ_RELAY, "PK_REQ_RELAY");
    e->insert(PK_RELAY_INFO, "PK_RELAY_INFO");
    e->insert(PK_ENERGY_INFO, "PK_ENERGY_INFO");
);

Register_Class(PacketCR);

PacketCR::PacketCR(const char *name, int kind) : Packet802154(name,kind)
{
    this->setTxType(TX_PPP);

    this->pkType_var = 0;
    this->hopLimit_var = 64;
    this->pkSize_var = 20;
}

PacketCR::PacketCR(const PacketCR& other) : Packet802154(other)
{
    copy(other);
}

PacketCR::~PacketCR()
{
}

PacketCR& PacketCR::operator=(const PacketCR& other)
{
    if (this==&other) return *this;
    Packet802154::operator=(other);
    copy(other);
    return *this;
}

void PacketCR::copy(const PacketCR& other)
{
    this->pkType_var = other.pkType_var;
    this->hopLimit_var = other.hopLimit_var;
    this->pkSize_var = other.pkSize_var;
}

void PacketCR::parsimPack(cCommBuffer *b)
{
    Packet802154::parsimPack(b);
    doPacking(b,this->pkType_var);
    doPacking(b,this->hopLimit_var);
    doPacking(b,this->pkSize_var);
}

void PacketCR::parsimUnpack(cCommBuffer *b)
{
    Packet802154::parsimUnpack(b);
    doUnpacking(b,this->pkType_var);
    doUnpacking(b,this->hopLimit_var);
    doUnpacking(b,this->pkSize_var);
}

int PacketCR::getPkType() const
{
    return pkType_var;
}

void PacketCR::setPkType(int pkType)
{
    this->pkType_var = pkType;
}

int PacketCR::getHopLimit() const
{
    return hopLimit_var;
}

void PacketCR::setHopLimit(int hopLimit)
{
    this->hopLimit_var = hopLimit;
}

int PacketCR::getPkSize() const
{
    return pkSize_var;
}

void PacketCR::setPkSize(int pkSize)
{
    this->pkSize_var = pkSize;
}

class PacketCRDescriptor : public cClassDescriptor
{
  public:
    PacketCRDescriptor();
    virtual ~PacketCRDescriptor();

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

Register_ClassDescriptor(PacketCRDescriptor);

PacketCRDescriptor::PacketCRDescriptor() : cClassDescriptor("PacketCR", "Packet802154")
{
}

PacketCRDescriptor::~PacketCRDescriptor()
{
}

bool PacketCRDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketCR *>(obj)!=NULL;
}

const char *PacketCRDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketCRDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int PacketCRDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PacketCRDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pkType",
        "hopLimit",
        "pkSize",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int PacketCRDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkType")==0) return base+0;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopLimit")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkSize")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketCRDescriptor::getFieldTypeString(void *object, int field) const
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

const char *PacketCRDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketCRDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketCR *pp = (PacketCR *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketCRDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketCR *pp = (PacketCR *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPkType());
        case 1: return long2string(pp->getHopLimit());
        case 2: return long2string(pp->getPkSize());
        default: return "";
    }
}

bool PacketCRDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketCR *pp = (PacketCR *)object; (void)pp;
    switch (field) {
        case 0: pp->setPkType(string2long(value)); return true;
        case 1: pp->setHopLimit(string2long(value)); return true;
        case 2: pp->setPkSize(string2long(value)); return true;
        default: return false;
    }
}

const char *PacketCRDescriptor::getFieldStructName(void *object, int field) const
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

void *PacketCRDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketCR *pp = (PacketCR *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


