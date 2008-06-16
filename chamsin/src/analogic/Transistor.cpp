#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Pad.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

namespace {



void createContactMatrix(Net* net, const Layer* layer, const Box& box, unsigned columns,
        const DbU::Unit& rwCont, const DbU::Unit& rdCont) {
    unsigned contacts = 0;
    if (box.getHeight() < rwCont) {
        contacts = 0;
    } else {
        contacts = (box.getHeight() - rwCont) / (rwCont + rdCont) + 1;
    }

    Point padMin(box.getXMin(), box.getYMin());
    Point padMax(padMin);
    padMax += Point(rwCont, rwCont);

    for (unsigned i=0; i<columns; i++) {
        for (unsigned j=0; j<contacts; j++) {
            Box padBox(padMin, padMax);
            Pad::create(net, layer, padBox);
            padMin.setY(padMin.getY() + rwCont + rdCont);
            padMax.setY(padMax.getY() + rwCont + rdCont);
        }
        padMin.setX(padMin.getX() + rwCont + rdCont);
        padMax.setX(padMax.getX() + rwCont + rdCont);
        padMin.setY(box.getYMin());
        padMax.setY(box.getYMax());
    }
}

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

Transistor::Transistor(Library* library, const Name& name, const Polarity& polarity, DbU::Unit l, DbU::Unit w):
    Cell(library, name),
    _drain(NULL),
    _source(NULL),
    _grid(NULL),
    _bulk(NULL),
    _anonymous(NULL),
    _polarity(polarity),
    _abutmentType(),
    _l(l),
    _w(w),
    _source20(NULL), _source22(NULL),
    _drain40(NULL), _drain42(NULL),
    _grid00(NULL), _grid01(NULL), _grid30(NULL), _grid31(NULL),
    _anonymous10(NULL), _anonymous11(NULL), _anonymous12(NULL)
{}


Transistor* Transistor::create(Library* library, const Name& name, const Polarity& polarity,
        DbU::Unit l, DbU::Unit w) {
    Transistor* transistor = new Transistor(library, name, polarity, l, w);

    transistor->_postCreate();

    return transistor;
}

void Transistor::_postCreate() {
   Inherit::_postCreate();
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
   if (_polarity == N) { 
       _anonymous11 = createPad(technology, _anonymous, "nImplant");
       _anonymous12 = createPad(technology, _anonymous, "nImplant");
   } else {
       _anonymous11 = createPad(technology, _anonymous, "pImplant");
       _anonymous12 = createPad(technology, _anonymous, "pImplant");
   }
   setTerminal(false);
}

void Transistor::createLayout() {
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
    DbU::Unit extGateActive     = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "poly"), getLayer(techno, "active"))->getValue(); 
    DbU::Unit extPolyCut0       = atechno->getPhysicalRule("minExtension",
            getLayer(techno, "poly"), getLayer(techno, "cut0"))->getValue();
    DbU::Unit spacingActiveCut0 = atechno->getPhysicalRule("minSpacing",
            getLayer(techno, "active"), getLayer(techno, "cut0"))->getValue();
    DbU::Unit spacingActivePoly = atechno->getPhysicalRule("minSpacing",
            getLayer(techno, "active"), getLayer(techno, "poly"))->getValue();
    DbU::Unit enclosureImplantPoly = 0;
    DbU::Unit extImplantActive = 0;
    DbU::Unit extImplantCut0 = 0;
    if (_polarity == N) {
        enclosureImplantPoly   = atechno->getPhysicalRule("minEnclosure",
                getLayer(techno, "nImplant"), getLayer(techno, "poly"))->getValue();
        extImplantActive = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "nImplant"), getLayer(techno, "active"))->getValue();
        extImplantCut0   = atechno->getPhysicalRule("minExtension",
                getLayer(techno, "nImplant"), getLayer(techno, "cut0"))->getValue();
    } else {
        enclosureImplantPoly   = atechno->getPhysicalRule("minEnclosure",
                getLayer(techno, "pImplant"), getLayer(techno, "poly"))->getValue();
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
    DbU::Unit dy00 = _w - y00*2; 
    Box box00(x00, y00, dx00, dy00);
    _grid00->setBoundingBox(box00);
    Contact* contact = Contact::create(_grid, getLayer(techno, "metal1"), box00.getXCenter(), box00.getYCenter(), box00.getWidth(), box00.getHeight());
    Pad* pad = Pad::create(_grid, getLayer(techno, "metal1"), box00);

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
    Box box30(x30, y30, dx30, dy30);
    _grid30->setBoundingBox(box30);

    //grid31
    DbU::Unit dx31 = dx30 + 2*extPolyCut0;
    DbU::Unit dy31 = dy30 + 2*extPolyCut0;
    DbU::Unit x31 = x30 - extPolyCut0;
    DbU::Unit y31 = y30 - extPolyCut0;
    Box box31(x31, y31, dx31, dy31);
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
    Box box01(x01, y01, dx01, dy01);
    _grid01->setBoundingBox(box01);

    //anonymous12
    DbU::Unit x12 = min(x31, x00) - enclosureImplantPoly;
    DbU::Unit y12 = min(0 - extImplantActive, y00 - enclosureImplantPoly);
    DbU::Unit dx12 = max(dx31, dx00) + 2 * enclosureImplantPoly;
    DbU::Unit yMax = max( max(y30 + dy30 + extImplantCut0, max(y31 + dy31, y00 + dy00) + enclosureImplantPoly), _w + extImplantActive);
    DbU::Unit dy12 = yMax - y12;

    Box box12(x12, y12, dx12, dy12);
    _anonymous12->setBoundingBox(box12);

    //setAbutmentBox(getAbutmentBox());
    UpdateSession::close();
}
