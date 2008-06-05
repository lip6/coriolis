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


void createContactMatrix(Net* net, const Layer* layer, const Box& box, unsigned columns, const Unit& rwCont, const Unit& rdCont) {
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


}

const Name Transistor::DrainName("DRAIN");
const Name Transistor::SourceName("SOURCE");
const Name Transistor::GridName("GRID");
const Name Transistor::BulkName("BULK");

Transistor::Transistor(Library* library, const Name& name, const Polarity& polarity):
    Cell(library, name),
    _drain(NULL),
    _source(NULL),
    _grid(NULL),
    _bulk(NULL),
    _polarity(polarity),
    _abutmentType(),
    _l(0.0),
    _w(0.0),
    _source20(NULL), _source22(NULL),
    _drain40(NULL), _drain42(NULL),
    _grid00(NULL), _grid01(NULL), _grid30(NULL), _grid31(NULL)
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
   _source20 = createPad(technology, _source, "cut0");
   _source22 = createPad(technology, _source, "cut1");
   _drain40  = createPad(technology, _drain,  "cut0");
   _drain42  = createPad(technology, _drain,  "cut1");
   _grid00   = createPad(technology, _grid,   "poly");
   _grid01   = createPad(technology, _grid,   "poly");
   _grid30   = createPad(technology, _grid,   "cut0");
   _grid31   = createPad(technology, _grid,   "metal1");
   
}

void Transistor::createLayout() {
    ATechnology* techno = AEnv::getATechnology();

    Unit rwCont = getUnit(techno->getPhysicalRule("RW_CONT")->getValue());
    Unit rdCont = getUnit(techno->getPhysicalRule("RD_CONT")->getValue());

    UpdateSession::open();
    
    

    UpdateSession::close();
}
