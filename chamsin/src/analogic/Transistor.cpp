#include "UpdateSession.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Transistor.h"

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
   _drain = Net::create(this, DrainName);
   _drain->setExternal(true);
   _source = Net::create(this, SourceName);
   _source->setExternal(true);
   _grid = Net::create(this, GridName);
   _grid->setExternal(true);
   _bulk = Net::create(this, BulkName);
   _bulk->setExternal(true);
   //_source20 = Contact::create(_source);
   //_source22 = Contact::create(_source);
   //_drain40 = Contact::create(_drain);
   //_drain42 = Contact::create(_drain);
}

void Transistor::createLayout() {
    ATechnology* techno = AEnv::getATechnology();

    Unit rwCont = getUnit(techno->getPhysicalRule("RW_CONT")->getValue());
    Unit rdCont = getUnit(techno->getPhysicalRule("RD_CONT")->getValue());

    UpdateSession::open();
    
    

    UpdateSession::close();
}
