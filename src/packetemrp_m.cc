//
// Generated file, do not edit! Created by opp_msgc 4.3 from packetemrp.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "packetemrp_m.h"

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

Register_Class(PacketEMRP);

PacketEMRP::PacketEMRP(const char *name, int kind) : Packet802154(name,kind)
{
    this->pkType_var = 0;
}

PacketEMRP::PacketEMRP(const PacketEMRP& other) : Packet802154(other)
{
    copy(other);
}

PacketEMRP::~PacketEMRP()
{
}

PacketEMRP& PacketEMRP::operator=(const PacketEMRP& other)
{
    if (this==&other) return *this;
    Packet802154::operator=(other);
    copy(other);
    return *this;
}

void PacketEMRP::copy(const PacketEMRP& other)
{
    this->pkType_var = other.pkType_var;
}

void PacketEMRP::parsimPack(cCommBuffer *b)
{
    Packet802154::parsimPack(b);
    doPacking(b,this->pkType_var);
}

void PacketEMRP::parsimUnpack(cCommBuffer *b)
{
    Packet802154::parsimUnpack(b);
    doUnpacking(b,this->pkType_var);
}

int PacketEMRP::getPkType() const
{
    return pkType_var;
}

void PacketEMRP::setPkType(int pkType)
{
    this->pkType_var = pkType;
}

class PacketEMRPDescriptor : public cClassDescriptor
{
  public:
    PacketEMRPDescriptor();
    virtual ~PacketEMRPDescriptor();

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

Register_ClassDescriptor(PacketEMRPDescriptor);

PacketEMRPDescriptor::PacketEMRPDescriptor() : cClassDescriptor("PacketEMRP", "Packet802154")
{
}

PacketEMRPDescriptor::~PacketEMRPDescriptor()
{
}

bool PacketEMRPDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketEMRP *>(obj)!=NULL;
}

const char *PacketEMRPDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketEMRPDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int PacketEMRPDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *PacketEMRPDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pkType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int PacketEMRPDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketEMRPDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *PacketEMRPDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketEMRPDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP *pp = (PacketEMRP *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketEMRPDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP *pp = (PacketEMRP *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPkType());
        default: return "";
    }
}

bool PacketEMRPDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP *pp = (PacketEMRP *)object; (void)pp;
    switch (field) {
        case 0: pp->setPkType(string2long(value)); return true;
        default: return false;
    }
}

const char *PacketEMRPDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *PacketEMRPDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP *pp = (PacketEMRP *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketEMRP_RelayInfo);

PacketEMRP_RelayInfo::PacketEMRP_RelayInfo(const char *name, int kind) : PacketEMRP(name,kind)
{
    this->setPkType(PK_RELAY_INFO);

    this->bsFlag_var = 0;
    this->energy_var = 0;
    this->posX_var = 0;
    this->posY_var = 0;
    this->dBS_var = 0;
}

PacketEMRP_RelayInfo::PacketEMRP_RelayInfo(const PacketEMRP_RelayInfo& other) : PacketEMRP(other)
{
    copy(other);
}

PacketEMRP_RelayInfo::~PacketEMRP_RelayInfo()
{
}

PacketEMRP_RelayInfo& PacketEMRP_RelayInfo::operator=(const PacketEMRP_RelayInfo& other)
{
    if (this==&other) return *this;
    PacketEMRP::operator=(other);
    copy(other);
    return *this;
}

void PacketEMRP_RelayInfo::copy(const PacketEMRP_RelayInfo& other)
{
    this->bsFlag_var = other.bsFlag_var;
    this->energy_var = other.energy_var;
    this->posX_var = other.posX_var;
    this->posY_var = other.posY_var;
    this->dBS_var = other.dBS_var;
}

void PacketEMRP_RelayInfo::parsimPack(cCommBuffer *b)
{
    PacketEMRP::parsimPack(b);
    doPacking(b,this->bsFlag_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->posX_var);
    doPacking(b,this->posY_var);
    doPacking(b,this->dBS_var);
}

void PacketEMRP_RelayInfo::parsimUnpack(cCommBuffer *b)
{
    PacketEMRP::parsimUnpack(b);
    doUnpacking(b,this->bsFlag_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->posX_var);
    doUnpacking(b,this->posY_var);
    doUnpacking(b,this->dBS_var);
}

bool PacketEMRP_RelayInfo::getBsFlag() const
{
    return bsFlag_var;
}

void PacketEMRP_RelayInfo::setBsFlag(bool bsFlag)
{
    this->bsFlag_var = bsFlag;
}

double PacketEMRP_RelayInfo::getEnergy() const
{
    return energy_var;
}

void PacketEMRP_RelayInfo::setEnergy(double energy)
{
    this->energy_var = energy;
}

double PacketEMRP_RelayInfo::getPosX() const
{
    return posX_var;
}

void PacketEMRP_RelayInfo::setPosX(double posX)
{
    this->posX_var = posX;
}

double PacketEMRP_RelayInfo::getPosY() const
{
    return posY_var;
}

void PacketEMRP_RelayInfo::setPosY(double posY)
{
    this->posY_var = posY;
}

double PacketEMRP_RelayInfo::getDBS() const
{
    return dBS_var;
}

void PacketEMRP_RelayInfo::setDBS(double dBS)
{
    this->dBS_var = dBS;
}

class PacketEMRP_RelayInfoDescriptor : public cClassDescriptor
{
  public:
    PacketEMRP_RelayInfoDescriptor();
    virtual ~PacketEMRP_RelayInfoDescriptor();

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

Register_ClassDescriptor(PacketEMRP_RelayInfoDescriptor);

PacketEMRP_RelayInfoDescriptor::PacketEMRP_RelayInfoDescriptor() : cClassDescriptor("PacketEMRP_RelayInfo", "PacketEMRP")
{
}

PacketEMRP_RelayInfoDescriptor::~PacketEMRP_RelayInfoDescriptor()
{
}

bool PacketEMRP_RelayInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketEMRP_RelayInfo *>(obj)!=NULL;
}

const char *PacketEMRP_RelayInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketEMRP_RelayInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int PacketEMRP_RelayInfoDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PacketEMRP_RelayInfoDescriptor::getFieldName(void *object, int field) const
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

int PacketEMRP_RelayInfoDescriptor::findField(void *object, const char *fieldName) const
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

const char *PacketEMRP_RelayInfoDescriptor::getFieldTypeString(void *object, int field) const
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

const char *PacketEMRP_RelayInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketEMRP_RelayInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_RelayInfo *pp = (PacketEMRP_RelayInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketEMRP_RelayInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_RelayInfo *pp = (PacketEMRP_RelayInfo *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getBsFlag());
        case 1: return double2string(pp->getEnergy());
        case 2: return double2string(pp->getPosX());
        case 3: return double2string(pp->getPosY());
        case 4: return double2string(pp->getDBS());
        default: return "";
    }
}

bool PacketEMRP_RelayInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_RelayInfo *pp = (PacketEMRP_RelayInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setBsFlag(string2bool(value)); return true;
        case 1: pp->setEnergy(string2double(value)); return true;
        case 2: pp->setPosX(string2double(value)); return true;
        case 3: pp->setPosY(string2double(value)); return true;
        case 4: pp->setDBS(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketEMRP_RelayInfoDescriptor::getFieldStructName(void *object, int field) const
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

void *PacketEMRP_RelayInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_RelayInfo *pp = (PacketEMRP_RelayInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(PacketEMRP_EnergyInfo);

PacketEMRP_EnergyInfo::PacketEMRP_EnergyInfo(const char *name, int kind) : PacketEMRP(name,kind)
{
    this->setPkType(PK_ENERGY_INFO);

    this->energy_var = 0;
    this->consumedEnergy_var = 0;
}

PacketEMRP_EnergyInfo::PacketEMRP_EnergyInfo(const PacketEMRP_EnergyInfo& other) : PacketEMRP(other)
{
    copy(other);
}

PacketEMRP_EnergyInfo::~PacketEMRP_EnergyInfo()
{
}

PacketEMRP_EnergyInfo& PacketEMRP_EnergyInfo::operator=(const PacketEMRP_EnergyInfo& other)
{
    if (this==&other) return *this;
    PacketEMRP::operator=(other);
    copy(other);
    return *this;
}

void PacketEMRP_EnergyInfo::copy(const PacketEMRP_EnergyInfo& other)
{
    this->energy_var = other.energy_var;
    this->consumedEnergy_var = other.consumedEnergy_var;
}

void PacketEMRP_EnergyInfo::parsimPack(cCommBuffer *b)
{
    PacketEMRP::parsimPack(b);
    doPacking(b,this->energy_var);
    doPacking(b,this->consumedEnergy_var);
}

void PacketEMRP_EnergyInfo::parsimUnpack(cCommBuffer *b)
{
    PacketEMRP::parsimUnpack(b);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->consumedEnergy_var);
}

double PacketEMRP_EnergyInfo::getEnergy() const
{
    return energy_var;
}

void PacketEMRP_EnergyInfo::setEnergy(double energy)
{
    this->energy_var = energy;
}

double PacketEMRP_EnergyInfo::getConsumedEnergy() const
{
    return consumedEnergy_var;
}

void PacketEMRP_EnergyInfo::setConsumedEnergy(double consumedEnergy)
{
    this->consumedEnergy_var = consumedEnergy;
}

class PacketEMRP_EnergyInfoDescriptor : public cClassDescriptor
{
  public:
    PacketEMRP_EnergyInfoDescriptor();
    virtual ~PacketEMRP_EnergyInfoDescriptor();

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

Register_ClassDescriptor(PacketEMRP_EnergyInfoDescriptor);

PacketEMRP_EnergyInfoDescriptor::PacketEMRP_EnergyInfoDescriptor() : cClassDescriptor("PacketEMRP_EnergyInfo", "PacketEMRP")
{
}

PacketEMRP_EnergyInfoDescriptor::~PacketEMRP_EnergyInfoDescriptor()
{
}

bool PacketEMRP_EnergyInfoDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<PacketEMRP_EnergyInfo *>(obj)!=NULL;
}

const char *PacketEMRP_EnergyInfoDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int PacketEMRP_EnergyInfoDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int PacketEMRP_EnergyInfoDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *PacketEMRP_EnergyInfoDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "energy",
        "consumedEnergy",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int PacketEMRP_EnergyInfoDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "consumedEnergy")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *PacketEMRP_EnergyInfoDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "double",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *PacketEMRP_EnergyInfoDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int PacketEMRP_EnergyInfoDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_EnergyInfo *pp = (PacketEMRP_EnergyInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PacketEMRP_EnergyInfoDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_EnergyInfo *pp = (PacketEMRP_EnergyInfo *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getEnergy());
        case 1: return double2string(pp->getConsumedEnergy());
        default: return "";
    }
}

bool PacketEMRP_EnergyInfoDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_EnergyInfo *pp = (PacketEMRP_EnergyInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->setEnergy(string2double(value)); return true;
        case 1: pp->setConsumedEnergy(string2double(value)); return true;
        default: return false;
    }
}

const char *PacketEMRP_EnergyInfoDescriptor::getFieldStructName(void *object, int field) const
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

void *PacketEMRP_EnergyInfoDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    PacketEMRP_EnergyInfo *pp = (PacketEMRP_EnergyInfo *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


