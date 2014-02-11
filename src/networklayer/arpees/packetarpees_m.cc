//
// Generated file, do not edit! Created by opp_msgc 4.3 from networklayer/arpees/packetarpees.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "packetarpees_m.h"

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




Register_Class(PacketARPEES);

PacketARPEES::PacketARPEES(const char *name, int kind) : PacketCR(name,kind)
{
}

PacketARPEES::PacketARPEES(const PacketARPEES& other) : PacketCR(other)
{
    copy(other);
}

PacketARPEES::~PacketARPEES()
{
}

PacketARPEES& PacketARPEES::operator=(const PacketARPEES& other)
{
    if (this==&other) return *this;
    PacketCR::operator=(other);
    copy(other);
    return *this;
}

void PacketARPEES::copy(const PacketARPEES& other)
{
}

void PacketARPEES::parsimPack(cCommBuffer *b)
{
    PacketCR::parsimPack(b);
}

void PacketARPEES::parsimUnpack(cCommBuffer *b)
{
    PacketCR::parsimUnpack(b);
}

class PacketARPEESDescriptor : public cClassDescriptor
{
  public:
    PacketARPEESDescriptor();
    virtual ~PacketARPEESDescriptor();

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

Register_ClassDescriptor(PacketARPEESDescriptor);

PacketARPEESDescriptor::PacketARPEESDescriptor() : cClassDescriptor("PacketARPEES", "PacketCR")
{
}

PacketARPEESDescriptor::~PacketARPEESDescriptor()
{
}

bool PacketARPEESDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketARPEES *>(obj)!=NULL;
}

const char *PacketARPEESDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketARPEESDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int PacketARPEESDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *PacketARPEESDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int PacketARPEESDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketARPEESDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *PacketARPEESDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketARPEESDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES *pp = (PacketARPEES *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketARPEESDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES *pp = (PacketARPEES *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool PacketARPEESDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES *pp = (PacketARPEES *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *PacketARPEESDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *PacketARPEESDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES *pp = (PacketARPEES *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketARPEES_RelayReq);

PacketARPEES_RelayReq::PacketARPEES_RelayReq(const char *name, int kind) : PacketARPEES(name,kind)
{
    this->setTxType(TX_BROADCAST);
    this->setPkType(PK_REQ_RELAY);
}

PacketARPEES_RelayReq::PacketARPEES_RelayReq(const PacketARPEES_RelayReq& other) : PacketARPEES(other)
{
    copy(other);
}

PacketARPEES_RelayReq::~PacketARPEES_RelayReq()
{
}

PacketARPEES_RelayReq& PacketARPEES_RelayReq::operator=(const PacketARPEES_RelayReq& other)
{
    if (this==&other) return *this;
    PacketARPEES::operator=(other);
    copy(other);
    return *this;
}

void PacketARPEES_RelayReq::copy(const PacketARPEES_RelayReq& other)
{
}

void PacketARPEES_RelayReq::parsimPack(cCommBuffer *b)
{
    PacketARPEES::parsimPack(b);
}

void PacketARPEES_RelayReq::parsimUnpack(cCommBuffer *b)
{
    PacketARPEES::parsimUnpack(b);
}

class PacketARPEES_RelayReqDescriptor : public cClassDescriptor
{
  public:
    PacketARPEES_RelayReqDescriptor();
    virtual ~PacketARPEES_RelayReqDescriptor();

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

Register_ClassDescriptor(PacketARPEES_RelayReqDescriptor);

PacketARPEES_RelayReqDescriptor::PacketARPEES_RelayReqDescriptor() : cClassDescriptor("PacketARPEES_RelayReq", "PacketARPEES")
{
}

PacketARPEES_RelayReqDescriptor::~PacketARPEES_RelayReqDescriptor()
{
}

bool PacketARPEES_RelayReqDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketARPEES_RelayReq *>(obj)!=NULL;
}

const char *PacketARPEES_RelayReqDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketARPEES_RelayReqDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int PacketARPEES_RelayReqDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *PacketARPEES_RelayReqDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int PacketARPEES_RelayReqDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketARPEES_RelayReqDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *PacketARPEES_RelayReqDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketARPEES_RelayReqDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayReq *pp = (PacketARPEES_RelayReq *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketARPEES_RelayReqDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayReq *pp = (PacketARPEES_RelayReq *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool PacketARPEES_RelayReqDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayReq *pp = (PacketARPEES_RelayReq *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *PacketARPEES_RelayReqDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *PacketARPEES_RelayReqDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayReq *pp = (PacketARPEES_RelayReq *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketARPEES_RelayInfo);

PacketARPEES_RelayInfo::PacketARPEES_RelayInfo(const char *name, int kind) : PacketARPEES(name,kind)
{
    this->setPkType(PK_RELAY_INFO);
    this->setPkSize(37);

    this->bsFlag_var = 0;
    this->energy_var = 0;
    this->posX_var = 0;
    this->posY_var = 0;
    this->dBS_var = 0;
}

PacketARPEES_RelayInfo::PacketARPEES_RelayInfo(const PacketARPEES_RelayInfo& other) : PacketARPEES(other)
{
    copy(other);
}

PacketARPEES_RelayInfo::~PacketARPEES_RelayInfo()
{
}

PacketARPEES_RelayInfo& PacketARPEES_RelayInfo::operator=(const PacketARPEES_RelayInfo& other)
{
    if (this==&other) return *this;
    PacketARPEES::operator=(other);
    copy(other);
    return *this;
}

void PacketARPEES_RelayInfo::copy(const PacketARPEES_RelayInfo& other)
{
    this->bsFlag_var = other.bsFlag_var;
    this->energy_var = other.energy_var;
    this->posX_var = other.posX_var;
    this->posY_var = other.posY_var;
    this->dBS_var = other.dBS_var;
}

void PacketARPEES_RelayInfo::parsimPack(cCommBuffer *b)
{
    PacketARPEES::parsimPack(b);
    doPacking(b,this->bsFlag_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->posX_var);
    doPacking(b,this->posY_var);
    doPacking(b,this->dBS_var);
}

void PacketARPEES_RelayInfo::parsimUnpack(cCommBuffer *b)
{
    PacketARPEES::parsimUnpack(b);
    doUnpacking(b,this->bsFlag_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->posX_var);
    doUnpacking(b,this->posY_var);
    doUnpacking(b,this->dBS_var);
}

bool PacketARPEES_RelayInfo::getBsFlag() const
{
    return bsFlag_var;
}

void PacketARPEES_RelayInfo::setBsFlag(bool bsFlag)
{
    this->bsFlag_var = bsFlag;
}

double PacketARPEES_RelayInfo::getEnergy() const
{
    return energy_var;
}

void PacketARPEES_RelayInfo::setEnergy(double energy)
{
    this->energy_var = energy;
}

double PacketARPEES_RelayInfo::getPosX() const
{
    return posX_var;
}

void PacketARPEES_RelayInfo::setPosX(double posX)
{
    this->posX_var = posX;
}

double PacketARPEES_RelayInfo::getPosY() const
{
    return posY_var;
}

void PacketARPEES_RelayInfo::setPosY(double posY)
{
    this->posY_var = posY;
}

double PacketARPEES_RelayInfo::getDBS() const
{
    return dBS_var;
}

void PacketARPEES_RelayInfo::setDBS(double dBS)
{
    this->dBS_var = dBS;
}

class PacketARPEES_RelayInfoDescriptor : public cClassDescriptor
{
  public:
    PacketARPEES_RelayInfoDescriptor();
    virtual ~PacketARPEES_RelayInfoDescriptor();

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

Register_ClassDescriptor(PacketARPEES_RelayInfoDescriptor);

PacketARPEES_RelayInfoDescriptor::PacketARPEES_RelayInfoDescriptor() : cClassDescriptor("PacketARPEES_RelayInfo", "PacketARPEES")
{
}

PacketARPEES_RelayInfoDescriptor::~PacketARPEES_RelayInfoDescriptor()
{
}

bool PacketARPEES_RelayInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketARPEES_RelayInfo *>(obj)!=NULL;
}

const char *PacketARPEES_RelayInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketARPEES_RelayInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int PacketARPEES_RelayInfoDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PacketARPEES_RelayInfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "bsFlag",
        "energy",
        "posX",
        "posY",
        "dBS",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int PacketARPEES_RelayInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bsFlag")==0) return base+0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "posX")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "posY")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "dBS")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketARPEES_RelayInfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *PacketARPEES_RelayInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketARPEES_RelayInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayInfo *pp = (PacketARPEES_RelayInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketARPEES_RelayInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayInfo *pp = (PacketARPEES_RelayInfo *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getBsFlag());
        case 1: return double2string(pp->getEnergy());
        case 2: return double2string(pp->getPosX());
        case 3: return double2string(pp->getPosY());
        case 4: return double2string(pp->getDBS());
        default: return "";
    }
}

bool PacketARPEES_RelayInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayInfo *pp = (PacketARPEES_RelayInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setBsFlag(string2bool(value)); return true;
        case 1: pp->setEnergy(string2double(value)); return true;
        case 2: pp->setPosX(string2double(value)); return true;
        case 3: pp->setPosY(string2double(value)); return true;
        case 4: pp->setDBS(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketARPEES_RelayInfoDescriptor::getFieldStructName(void *object, int field) const
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

void *PacketARPEES_RelayInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketARPEES_RelayInfo *pp = (PacketARPEES_RelayInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


