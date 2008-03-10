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


BEGIN_NAMESPACE_HURRICANE

// ****************************************************************************************************
// MetaTransistor implementation
// ****************************************************************************************************

MetaTransistor::MetaTransistor(Library* library, const Name& name, char type)
// **************************************************************************
:	Inherit(library, name),
        _type(type),
	_m(1),
	_le(0.0),
	_we(0.0)
{
}


MetaTransistor* MetaTransistor::Create(Library* library, const Name& name, char type)
// **********************************************************************************
{
	MetaTransistor* metatransistor = new MetaTransistor(library, name, type);

	metatransistor->_PostCreate();

	return metatransistor;
}


void MetaTransistor::_PreDelete()
// ******************************
{
   // do something
   // ************
  
   Inherit::_PreDelete();
}


void MetaTransistor::_PostCreate()
// *******************************
{
   Inherit::_PostCreate();

   (Net::Create(this, Name("DRAIN")))->SetExternal(true);
   (Net::Create(this, Name("SOURCE")))->SetExternal(true);
   (Net::Create(this, Name("GRID")))->SetExternal(true);
   (Net::Create(this, Name("BULK")))->SetExternal(true);
}


void MetaTransistor::CreateConnection()
// ***********************************
{
  for_each_instance(instance, this->GetInstances())   
     Cell * mastercell = instance->GetMasterCell();

     // Assurance of unique instanciation 
     // *********************************
     if(mastercell->_GetSlaveInstanceSet()._GetSize()!=1) {
       string err_msg = "Can't create connection : " + GetString(mastercell) + " hasn't only one slave instance";
       assert(err_msg.c_str());
     }

     instance->GetPlug(mastercell->GetNet(Name("DRAIN")))->SetNet(GetNet(Name("DRAIN")));
     instance->GetPlug(mastercell->GetNet(Name("SOURCE")))->SetNet(GetNet(Name("SOURCE")));
     instance->GetPlug(mastercell->GetNet(Name("GRID")))->SetNet(GetNet(Name("GRID")));
     instance->GetPlug(mastercell->GetNet(Name("BULK")))->SetNet(GetNet(Name("BULK")));
  end_for
}  


void MetaTransistor::CreateLayout()
// ********************************
{
//  OpenUpdateSession();
  
  if((_le == 0.0) || (_we == 0.0)) {
     throw Error("Can't generate layout : " + GetString(this) + " hasn't been dimensionned");
  }

  SetTerminal(false);
  
  Transistor * internal_ref = NULL;
  Transistor * left_ref = NULL;
  Transistor * right_ref = NULL;

  for_each_instance(instance, this->GetInstances())
     Cell * mastercell = instance->GetMasterCell();

     // Assurance of unique instanciation 
     // *********************************
     if(mastercell->_GetSlaveInstanceSet()._GetSize()!=1) {
       string err_msg = "Can't generate layout : " + GetString(mastercell) + " hasn't only one slave instance";
       assert(err_msg.c_str());
     }
    
     Transistor * trans = dynamic_cast<Transistor*>(mastercell);
     if(!trans){
       string err_msg = "Can't genrate layout : " + GetString(mastercell) + " isn't a Transistor"; 
     }
     
     if(trans->IsInternal()) {
       if(!internal_ref) {
         trans->CreateLayout();
	 internal_ref = trans;
       } 
       else {
         trans->DuplicateLayout(internal_ref);
       }
     }
     else if(trans->IsLeft()) {
       if(!left_ref) {
	 trans->CreateLayout();
	 left_ref=trans;
       }
       else
	 trans->DuplicateLayout(left_ref);
     }  
     else if(trans->IsRight()) {
       if(!right_ref) {
	 trans->CreateLayout();
	 right_ref=trans;
       }
       else
	 trans->DuplicateLayout(right_ref);
     }
     else 
        trans->CreateLayout();
  end_for    
  

  Materialize();
 // CloseUpdateSession();

}


void MetaTransistor::Flush()
// *************************
{
  OpenUpdateSession();
  for_each_instance(instance, this->GetInstances())   
    Cell * mastercell = instance->GetMasterCell();
    instance->Delete();     
    mastercell->Delete();
  end_for
  CloseUpdateSession();
}  



string MetaTransistor::_GetString() const
// ***************************************
{
     string s= Inherit::_GetString();
     s.insert(s.length()-1, " " + GetString(GetType()) );
     s.insert(s.length()-1, " " + GetString(GetM()) );
     return s;
}

Record* MetaTransistor::_GetRecord() const
// ***************************************
{
	Record* record = Inherit::_GetRecord();
	return record;
}

END_NAMESPACE_HURRICANE


