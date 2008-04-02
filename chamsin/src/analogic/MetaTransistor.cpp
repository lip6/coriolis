// ****************************************************************************************************
// File: MetaTransistor.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#include "MetaTransistor.h"
#include "Transistor.h"
#include "Instances.h"
#include "UpdateSession.h"
#include "Vertical.h"

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


void MetaTransistor::createLayout()
// ********************************
{
//  OpenUpdateSession();
  
  if((_le == 0.0) || (_we == 0.0)) {
     throw Error("Can't generate layout : " + getString(this) + " hasn't been dimensionned");
  }

  setTerminal(false);
  
  Transistor * internal_ref = NULL;
  Transistor * left_ref = NULL;
  Transistor * right_ref = NULL;

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
     
     if(trans->IsInternal()) {
       if(!internal_ref) {
         trans->createLayout();
	 internal_ref = trans;
       } 
       else {
         trans->DuplicateLayout(internal_ref);
       }
     }
     else if(trans->IsLeft()) {
       if(!left_ref) {
	 trans->createLayout();
	 left_ref=trans;
       }
       else
	 trans->DuplicateLayout(left_ref);
     }  
     else if(trans->IsRight()) {
       if(!right_ref) {
	 trans->createLayout();
	 right_ref=trans;
       }
       else
	 trans->DuplicateLayout(right_ref);
     }
     else 
        trans->createLayout();
  end_for    
  

  materialize();
 // CloseUpdateSession();

}


void MetaTransistor::Flush()
// *************************
{
  OpenUpdateSession();
  for_each_instance(instance, this->getInstances())   
    Cell * mastercell = instance->getMasterCell();
    instance->destroy();     
    mastercell->destroy();
  end_for
  CloseUpdateSession();
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
