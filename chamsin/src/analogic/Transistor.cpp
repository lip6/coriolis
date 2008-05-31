#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

namespace {


Pad* createPad(Technology* technology, Net* net, const string& layerName) {
    Layer* layer = technology->getLayer(Name(layerName));
    Pad* pad = Pad::create(net, layer, 0, 0);
    return pad;
}


void createContactMatrix(Net* net, const Layer* layer, const Box& box, unsigned columns) {
    unsigned contacts = 0;
    long xCenter = 0;
    long yCenter = 0;
    if (box.getHeight() < RW_CONT__) {
        contacts = 0;
    } else {
        contacts = (box.getHeight()-rw_cont)/(rw_cont + rd_cont) + 1;
    }

    Point xCenter(box.getXMin() + (rw_cont/2));
    Point yCenter(box.getYMin() + (rw_cont/2));

    for (unsigned i=0; i<columns; i++) {
        for (unsigned j=0; j<columns; j++) {
            Pad::create(net, layer, xCenter, yCenter, rw_cont , rw_cont);
            yCenter += rw_cont + rd_cont;
        }
        xCenter += rw_cont + rd_cont;
        yCenter = box.getYMin() + (rw_cont/2);
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
    _drain40(NULL), _drain42(NULL)
    _grid00(NULL), _grid01(NULL), _grid(30), _grid(31)
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
