#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

namespace {

const Name Transistor::DrainName("DRAIN");
const Name Transistor::SourceName("SOURCE");
const Name Transistor::GridName("GRID");
const Name Transistor::BulkName("BULK");

Contact* createContact(Technology* technology, Net* net, const string& layerName) {
    Layer* layer = technology->getLayer(Name(layerName));
    Contact* contact = Contact::create(net, layer, 0, 0);
    return contact;
}

}

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
    _source20(NULL),
    _source22(NULL),
    _drain40(NULL),
    _drain42(NULL)
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
   _source20 = createContact(technology, _source, "cut0");
   _source22 = createContact(technology, _source, "cut1");
   _drain40  = createContact(technology, _drain,  "cut0");
   _drain42  = createContact(technology, _drain,  "cut1");
}

void Transistor::createLayout() {
    ATechnology* techno = AEnv::getATechnology();

    Unit rwCont = getUnit(techno->getPhysicalRule("RW_CONT")->getValue());
    Unit rdCont = getUnit(techno->getPhysicalRule("RD_CONT")->getValue());

    UpdateSession::open();
    
    

    UpdateSession::close();
}
