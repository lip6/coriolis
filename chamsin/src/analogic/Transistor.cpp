#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Pad.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

namespace {

Layer* getLayer(Technology* technology, const string& layerStr) {
   Layer* layer = technology->getLayer(layerStr);
   if (!layer) {
       throw Error("Unknown Layer : " + layerStr);
   }
   return layer; 
}

Pad* createPad(Technology* technology, Net* net, const string& layerName) {
    static Box emptyBox(0, 0, 0, 0);
    Layer* layer = getLayer(technology, layerName);
    Pad* pad = Pad::create(net, layer, emptyBox);
    return pad;
}


}

const Name Transistor::DrainName("DRAIN");
const Name Transistor::SourceName("SOURCE");
const Name Transistor::GridName("GRID");
const Name Transistor::BulkName("BULK");
const Name Transistor::AnonymousName("ANONYMOUS");

Transistor::Type::Type(const Code& code):
    _code(code)
{}

Transistor::Type::Type(const Type& type):
    _code(type._code)
{}

Transistor::Type& Transistor::Type::operator=(const Type& type) {
    _code = type._code;
    return *this;
}

Transistor::Transistor(Library* library, const Name& name):
    AnalogComponent(library, name),
    _drain(NULL),
    _source(NULL),
    _grid(NULL),
    _bulk(NULL),
    _anonymous(NULL),
    _type(),
    _l(0),
    _w(0),
    _source20(NULL), _source22(NULL),
    _drain40(NULL), _drain42(NULL),
    _grid00(NULL), _grid01(NULL), _grid30(NULL), _grid31(NULL),
    _anonymous10(NULL), _anonymous11(NULL), _anonymous12(NULL), _anonymous50(NULL)
{}


Transistor* Transistor::create(Library* library, const Name& name) {
    Transistor* transistor = new Transistor(library, name);

    transistor->_postCreate();

    return transistor;
}

void Transistor::_postCreate() {
    Inherit::_postCreate();

    //ChoiceParameter<Transistor>::Choices choices;
    //choices.push_back(string("N"));
    //choices.push_back(string("P"));
    //addParameter(ChoiceParameter<Transistor>("type", choices, 0, new CallBack<Transistor>(this, &Transistor::updateType)));

    DataBase* db = DataBase::getDB();
    Technology* technology = db->getTechnology();
    _drain = Net::create(this, DrainName);
    _drain->setExternal(true);
    _source = Net::create(this, SourceName);
    _source->setExternal(true);
    _grid = Net::create(this, GridName);
    _grid->setExternal(true);
    _bulk = Net::create(this, BulkName);
    _bulk->setExternal(true);
    _anonymous = Net::create(this, AnonymousName);
    _source20    = createPad(technology, _source,    "cut0");
    _source22    = createPad(technology, _source,    "cut1");
    _drain40     = createPad(technology, _drain,     "cut0");
    _drain42     = createPad(technology, _drain,     "cut1");
    _grid00      = createPad(technology, _grid,      "poly");
    _grid01      = createPad(technology, _grid,      "poly");
    _grid30      = createPad(technology, _grid,      "cut0");
    _grid31      = createPad(technology, _grid,      "metal1");
    _anonymous10 = createPad(technology, _anonymous, "active");
    if (_type == Type::NMOS) { 
        _anonymous11 = createPad(technology, _anonymous, "nImplant");
        _anonymous12 = createPad(technology, _anonymous, "nImplant");
    } else {
        _anonymous11 = createPad(technology, _anonymous, "pImplant");
        _anonymous12 = createPad(technology, _anonymous, "pImplant");
    }
    setTerminal(false);
}

void Transistor::setType(Type type) {
    UpdateSession::open();
    if (type != _type) {
        _type = type;
        DataBase* db = DataBase::getDB();
        Technology* technology = db->getTechnology();
        if (_type == Type::NMOS) { 
            _anonymous11->setLayer(getLayer(technology, "nImplant"));
            _anonymous12->setLayer(getLayer(technology, "nImplant"));
        } else {
            _anonymous11->setLayer(getLayer(technology, "pImplant"));
            _anonymous12->setLayer(getLayer(technology, "pImplant"));
        }
        updateLayout();
    }
    UpdateSession::close();
}

void Transistor::updateLayout() {
    DataBase* db = DataBase::getDB();
    if (!db) {
        throw Error("Error : no DataBase");
    }
    Technology* techno = db->getTechnology();
    if (!techno) {
        throw Error("Error : no Technology");
    }
    ATechnology* atechno = AEnv::getATechnology();

    DbU::Unit widthCut0         = atechno->getPhysicalRule("minWidth", getLayer(techno, "cut0"))->getValue();
    DbU::Unit spacingCut0       = atechno->getPhysicalRule("minSpacing", getLayer(techno, "cut0"))->getValue();
    DbU::Unit extGateActive     = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "poly"), getLayer(techno, "active"))->getValue(); 
    DbU::Unit extPolyCut0       = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "poly"), getLayer(techno, "cut0"))->getValue();
    DbU::Unit spacingActiveCut0 = atechno->getPhysicalRule("minSpacing",
            getLayer(techno, "active"), getLayer(techno, "cut0"))->getValue();
    DbU::Unit spacingGateCut0 = atechno->getPhysicalRule("minGateSpacing",
            getLayer(techno, "cut0"), getLayer(techno, "active"))->getValue();
    DbU::Unit spacingActivePoly = atechno->getPhysicalRule("minSpacing",
            getLayer(techno, "active"), getLayer(techno, "poly"))->getValue();
    DbU::Unit sourceDrainWidth = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "active"), getLayer(techno, "poly"))->getValue();
    DbU::Unit extActiveCut0      = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "active"), getLayer(techno, "cut0"))->getValue();
    DbU::Unit enclosurePPlusActive = atechno->getPhysicalRule("minEnclosure",
            getLayer(techno, "nWell"), getLayer(techno, "active"))->getValue();
    DbU::Unit enclosureImplantPoly = 0;
    DbU::Unit enclosureGateImplant = 0;
    DbU::Unit extImplantActive = 0;
    DbU::Unit extImplantCut0 = 0;

    if (_type == Type::NMOS) {
        enclosureImplantPoly   = atechno->getPhysicalRule("minEnclosure",
                getLayer(techno, "nImplant"), getLayer(techno, "poly"))->getValue();
        enclosureGateImplant   = atechno->getPhysicalRule("minGateEnclosure",
                getLayer(techno, "nImplant"), getLayer(techno, "poly"))->getValue();
        extImplantActive = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "nImplant"), getLayer(techno, "active"))->getValue();
        extImplantCut0   = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "nImplant"), getLayer(techno, "cut0"))->getValue();
    } else {
        enclosureImplantPoly   = atechno->getPhysicalRule("minEnclosure",
                getLayer(techno, "pImplant"), getLayer(techno, "poly"))->getValue();
        enclosureGateImplant   = atechno->getPhysicalRule("minGateEnclosure",
                getLayer(techno, "nImplant"), getLayer(techno, "poly"))->getValue();
        extImplantActive = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "pImplant"), getLayer(techno, "active"))->getValue();
        extImplantCut0   = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "pImplant"), getLayer(techno, "cut0"))->getValue();
    }

    UpdateSession::open();
    DbU::setStringMode(1);

    //grid 00
    DbU::Unit x00 = 0;
    DbU::Unit y00 = -extGateActive;
    DbU::Unit dx00 = _l;
    DbU::Unit dy00 = _w + extGateActive;
    Box box00(x00, y00, x00 + dx00, y00 + dy00);
    _grid00->setBoundingBox(box00);

    //grid30
    DbU::Unit maxValue = widthCut0 + 2*extPolyCut0;
    DbU::Unit x30 = 0, dx30 = 0, y30 = 0, dy30 = 0;
    if (maxValue > _l) {
        dx30 = widthCut0;
        dy30 = dx30;
        y30 = _w + max(spacingActiveCut0, spacingActivePoly + extPolyCut0); 
    } else {
        dx30 = dx00 - 2*extPolyCut0;
        dy30 = widthCut0;
        y30 = _w + spacingActiveCut0;
    }
    x30 = x00 + dx00/2 - dx30/2;
    Box box30(x30, y30, x30 + dx30, y30 + dy30);
    _grid30->setBoundingBox(box30);

    //grid31
    DbU::Unit dx31 = dx30 + 2*extPolyCut0;
    DbU::Unit dy31 = dy30 + 2*extPolyCut0;
    DbU::Unit x31 = x30 - extPolyCut0;
    DbU::Unit y31 = y30 - extPolyCut0;
    Box box31(x31, y31, x31 + dx31, y31 + dy31);
    _grid31->setBoundingBox(box31);

    //grid01
    DbU::Unit x01 = 0, y01 = 0, dx01 = 0, dy01 = 0;
    if (y31 <= y00+dy00) {
        x01 = 0; y01 = 0; dx01 = 0; dy01 = 0;
    } else {
        x01 = x00; 
        y01 = y00 + dy00; 
        dx01 = dx00; 
        dy01 = y31 - (y00 + dy00);
    }
    Box box01(x01, y01, x01 + dx01, y01 + dy01);
    _grid01->setBoundingBox(box01);

    //anonymous12
    DbU::Unit x12 = min(x31, x00) - enclosureImplantPoly;
    DbU::Unit y12 = min(0 - extImplantActive, y00 - enclosureImplantPoly);
    DbU::Unit dx12 = max(dx31, dx00) + 2 * enclosureImplantPoly;
    DbU::Unit yMax = max( max(y30 + dy30 + extImplantCut0, max(y31 + dy31, y00 + dy00) + enclosureImplantPoly), _w + extImplantActive);
    DbU::Unit dy12 = yMax - y12;

    Box box12(x12, y12, x12 + dx12, y12 + dy12);
    _anonymous12->setBoundingBox(box12);

    //_source20
    DbU::Unit y20 = extActiveCut0;
    DbU::Unit dy20 = _w - 2 * extActiveCut0;
    unsigned sourceColumnNumber = 1;
    DbU::Unit dx20 = sourceColumnNumber * widthCut0 + (sourceColumnNumber - 1) * spacingCut0;
    DbU::Unit x20 = -(dx20 + spacingGateCut0);
    Box box20(x20, y20, x20 + dx20, y20 + dy20);
    _source20->setBoundingBox(box20);

    //_drain40
    DbU::Unit y40 = y20;
    DbU::Unit x40 = x00 + dx00 + spacingGateCut0;
    unsigned drainColumnNumber = 1;
    DbU::Unit dx40 = drainColumnNumber * widthCut0 + (drainColumnNumber - 1) * (spacingCut0);
    DbU::Unit dy40 = dy20;

    Box box40(x40, y40, x40 + dx40, y40 + dy40);
    _drain40->setBoundingBox(box40);

    //_anonymous10
    DbU::Unit y10 = 0;
    DbU::Unit x10 = min(x20 - spacingActiveCut0, sourceDrainWidth); 
    DbU::Unit dy10 = _w;
    DbU::Unit extension10 = max(x40 + dx40 + spacingActiveCut0, dx00 + sourceDrainWidth);
    DbU::Unit dx10 = -x10 + extension10;

    Box box10(x10, y10, x10 + dx10, y10 + dy10);
    _anonymous10->setBoundingBox(box10);

    //Rectangle 23
    DbU::Unit x23 = x10;
    DbU::Unit y23 = y10;
    DbU::Unit dx23 = x10;
    DbU::Unit dy23 = _w;


    //_anonymous11
    DbU::Unit extension11_1 = enclosureGateImplant;
    DbU::Unit extension11_2 = extImplantCut0 - x20;
    DbU::Unit extension11_3 = extImplantActive - x10;

    DbU::Unit extension11_4 = max(max(extension11_1, extension11_2), extension11_3);

    DbU::Unit x11 = -extension11_4;

    extension11_1 = enclosureGateImplant + x00 + dx00;
    extension11_2 = extImplantCut0 + x40 + dx40;
    extension11_3 = extImplantActive + x10 + dx10;

    extension11_4 = max(max(extension11_1, extension11_2), extension11_3);
  
    DbU::Unit dx11 = -x11 + extension11_4;  
  
    DbU::Unit y11 = min(y20 - extImplantCut0, y23 - extImplantActive);
    DbU::Unit dy11 = max(y20 + dy20 + extImplantCut0, y23 + dy23 + extImplantActive) - y11;

    Box box11(x11, y11, x11 + dx11, y11 + dy11);
    _anonymous11->setBoundingBox(box11);

    if (_type == Type::PMOS) { 
        DbU::Unit x50 = x10 - enclosurePPlusActive;
        DbU::Unit y50 = y10 - enclosurePPlusActive;
        DbU::Unit dx50 = dx10 + 2 * enclosurePPlusActive;
        DbU::Unit dy50 = dy10 + 2 * enclosurePPlusActive;
        Box box50(x50, y50, x50 + dx50, y50 + dy50);
        if (!_anonymous50) {
            _anonymous50 = createPad(techno, _anonymous, "nWell");
        }
       _anonymous50->setBoundingBox(box50);
    } else {
        if (_anonymous50) {
            _anonymous50->destroy();
            _anonymous50 = NULL;
        }
    }

    //setAbutmentBox(getAbutmentBox());
    UpdateSession::close();
}

Record* Transistor::_getRecord() const {
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Drain", _drain));
        record->add(getSlot("Source", _source));
        record->add(getSlot("Grid", _grid));
        record->add(getSlot("Bulk", _bulk));
        record->add(getSlot("L", &_l));
        record->add(getSlot("W", &_w));
        record->add(getSlot("Source20", _source20));
        record->add(getSlot("Source22", _source22));
        record->add(getSlot("Drain40", _drain40));
        record->add(getSlot("Drain42", _drain42));
        record->add(getSlot("Grid00", _grid00));
        record->add(getSlot("Grid01", _grid01));
        record->add(getSlot("Grid30", _grid30));
        record->add(getSlot("Grid31", _grid31));
        record->add(getSlot("10", _anonymous10));
        record->add(getSlot("11", _anonymous11));
        record->add(getSlot("12", _anonymous12));
    }
    return record;
}
