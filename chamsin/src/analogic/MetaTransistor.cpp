// ****************************************************************************************************
// File: MetaTransistor.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "UpdateSession.h"
using namespace Hurricane;

#include "Transistor.h"
#include "MetaTransistor.h"

namespace Hurricane {

// ****************************************************************************************************
// MetaTransistor implementation
// ****************************************************************************************************

MetaTransistor::MetaTransistor(Library* library, const Name& name, char type)
:	Inherit(library, name),
        _type(type),
	_m(1),
	_le(0.0),
	_we(0.0)
{
}


MetaTransistor* MetaTransistor::create(Library* library, const Name& name, char type) {
	MetaTransistor* metatransistor = new MetaTransistor(library, name, type);

	metatransistor->_postCreate();

	return metatransistor;
}


void MetaTransistor::_preDestroy() {
   // do something
   // ************
  
   Inherit::_preDestroy();
}


void MetaTransistor::_postCreate() {
   Inherit::_postCreate();

   (Net::create(this, Name("DRAIN")))->setExternal(true);
   (Net::create(this, Name("SOURCE")))->setExternal(true);
   (Net::create(this, Name("GRID")))->setExternal(true);
   (Net::create(this, Name("BULK")))->setExternal(true);
}


void MetaTransistor::createConnection()
// ***********************************
{
  for_each_instance(instance, this->getInstances())   
     Cell * mastercell = instance->getMasterCell();

     // Assurance of unique instanciation 
     // *********************************
     if(mastercell->_getSlaveInstanceSet()._getSize()!=1) {
       string err_msg = "Can't create connection : " + getString(mastercell) + " hasn't only one slave instance";
       assert(err_msg.c_str());
     }

     instance->getPlug(mastercell->getNet(Name("DRAIN")))->setNet(getNet(Name("DRAIN")));
     instance->getPlug(mastercell->getNet(Name("SOURCE")))->setNet(getNet(Name("SOURCE")));
     instance->getPlug(mastercell->getNet(Name("GRID")))->setNet(getNet(Name("GRID")));
     instance->getPlug(mastercell->getNet(Name("BULK")))->setNet(getNet(Name("BULK")));
  end_for
}  


void MetaTransistor::createLayout() {
  
  if((_le == 0.0) || (_we == 0.0)) {
     throw Error("Can't generate layout : " + getString(this) + " hasn't been dimensionned");
  }

  setTerminal(false);
  
  Transistor* internal_ref = NULL;
  Transistor* left_ref = NULL;
  Transistor* right_ref = NULL;

  for_each_instance(instance, this->getInstances())
     Cell * mastercell = instance->getMasterCell();

     // Assurance of unique instanciation 
     // *********************************
     if(mastercell->_getSlaveInstanceSet()._getSize()!=1) {
       string err_msg = "Can't generate layout : " + getString(mastercell) + " hasn't only one slave instance";
       assert(err_msg.c_str());
     }
    
     Transistor * trans = dynamic_cast<Transistor*>(mastercell);
     if(!trans){
       string err_msg = "Can't genrate layout : " + getString(mastercell) + " isn't a Transistor"; 
     }
     
     if(trans->isInternal()) {
       if(!internal_ref) {
         cerr << "akecoucou" << endl;
         trans->createLayout();
	 internal_ref = trans;
       } else {
         trans->duplicateLayout(internal_ref);
       }
     } else if(trans->isLeft()) {
       if(!left_ref) {
	 trans->createLayout();
	 left_ref=trans;
       } else {
	 trans->duplicateLayout(left_ref);
       }
     }  else if(trans->isRight()) {
       if(!right_ref) {
	 trans->createLayout();
	 right_ref=trans;
       } else {
	 trans->duplicateLayout(right_ref);
       }
     } else { 
        trans->createLayout();
     }
  end_for    
  

  materialize();

}


void MetaTransistor::Flush()
// *************************
{
  UpdateSession::open();
  for_each_instance(instance, this->getInstances()) { 
    Cell * mastercell = instance->getMasterCell();
    instance->destroy();     
    mastercell->destroy();
    end_for
  }
  UpdateSession::close();
}  



string MetaTransistor::_getString() const
// ***************************************
{
     string s= Inherit::_getString();
     s.insert(s.length()-1, " " + getString(getType()) );
     s.insert(s.length()-1, " " + getString(getM()) );
     return s;
}

Record* MetaTransistor::_getRecord() const
// ***************************************
{
	Record* record = Inherit::_getRecord();
	return record;
}

}
