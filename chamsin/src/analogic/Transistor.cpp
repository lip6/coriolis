#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Pad.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

namespace {


Pad* createPad(Technology* technology, Net* net, const string& layerName) {
    Layer* layer = technology->getLayer(Name(layerName));
    Pad* pad = Pad::create(net, layer, Box());
    return pad;
}


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

Layer* getLayer(const string& layerStr) {
}


}

const Name Transistor::DrainName("DRAIN");
const Name Transistor::SourceName("SOURCE");
const Name Transistor::GridName("GRID");
const Name Transistor::BulkName("BULK");
const Name Transistor::AnonymousName("ANONYMOUS");

Transistor::Transistor(Library* library, const Name& name, const Polarity& polarity):
    Cell(library, name),
    _drain(NULL),
    _source(NULL),
    _grid(NULL),
    _bulk(NULL),
    _anonymous(NULL),
    _polarity(polarity),
    _abutmentType(),
    _l(DbU::Min),
    _w(DbU::Min),
    _source20(NULL), _source22(NULL),
    _drain40(NULL), _drain42(NULL),
    _grid00(NULL), _grid01(NULL), _grid30(NULL), _grid31(NULL),
    _anonymous10(NULL), _anonymous11(NULL), _anonymous12(NULL)
{}


Transistor* Transistor::create(Library* library, const Name& name, const Polarity& polarity) {
    Transistor* transistor = new Transistor(library, name, polarity);

    transistor->_postCreate();

    return transistor;
}

void Transistor::_postCreate() {
   Inherit::_postCreate();
   DataBase* db = getDataBase();
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
   _anonymous11 = createPad(technology, _anonymous, "nimp");
   _anonymous12 = createPad(technology, _anonymous, "nimp");
}

void Transistor::createLayout() {
    ATechnology* techno = AEnv::getATechnology();

    DbU::Unit rwCont = techno->getPhysicalRule("RW_CONT")->getValue();
    DbU::Unit rdCont = techno->getPhysicalRule("RD_CONT")->getValue();
    DbU::Unit reGateActiv  = techno->getPhysicalRule("RE", getLayer("poly"), getLayer("active"))->getValue(); 
    DbU::Unit rePolyCont   = techno->getPhysicalRule("RE", getLayer("poly"), getLayer("cut"))->getValue();
    DbU::Unit rdActiveCont = techno->getPhysicalRule("RD", getLayer("active"), getLayer("cut"))->getValue();
    DbU::Unit rdActivePoly = techno->getPhysicalRule("RD", getLayer("active"), getLayer("poly"))->getValue();

    UpdateSession::open();

    //grid 00
    DbU::Unit x00 = 0;
    DbU::Unit y00 = -reGateActiv;
    DbU::Unit dx00 = _l;
    DbU::Unit dy00 = _w - y00*2; 
    Box box00(x00, y00, dx00, dy00);
    _grid00->setBoundingBox(box00);

    //grid30
    DbU::Unit toto = rwCont + 2*rePolyCont;
    DbU::Unit x30 = 0, dx30 = 0, y30 = 0, dy30 = 0;
    if (toto > _l) {
        dx30 = rwCont;
        dy30 = dx30;
        y30 = _w + max(rdActiveCont, rdActivePoly + rePolyCont); 
    } else {
        dx30 = dx00 - 2*rePolyCont;
        dy30 = rwCont;
        y30 = _w + rdActiveCont;
    }
    x30 = x00 + dx00/2 - dx30/2;
    Box box30(x30, y30, dx30, dy30);
    _grid30->setBoundingBox(box30);

    //grid31
    DbU::Unit dx31 = dx30 + 2*rePolyCont;
    DbU::Unit dy31 = dy30 + 2*rePolyCont;
    DbU::Unit x31 = x30 - rePolyCont;
    DbU::Unit y31 = y30 - rePolyCont;
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

    //12

    UpdateSession::close();
}
