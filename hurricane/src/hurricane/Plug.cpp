// ****************************************************************************************************
// File: ./Plug.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#include "hurricane/Plug.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Plug_IsConnectedFilter : public Filter<Plug*> {
// ************************************************

    public: Plug_IsConnectedFilter() {};

    public: Plug_IsConnectedFilter(const Plug_IsConnectedFilter& filter) {};

    public: Plug_IsConnectedFilter& operator=(const Plug_IsConnectedFilter& filter) {return *this;};

    public: virtual Filter<Plug*>* getClone() const {return new Plug_IsConnectedFilter(*this);};

    public: virtual bool accept(Plug* plug) const {return plug->isConnected();};

    public: virtual string _getString() const {return "<" + _TName("Plug::IsConnectedFilter>");};

};



// ****************************************************************************************************
// Plug implementation
// ****************************************************************************************************

Plug::Plug(Instance* instance, Net* masterNet)
// *******************************************
:    Inherit(NULL, true),
    _instance(instance),
    _masterNet(masterNet),
    _nextOfInstancePlugMap(NULL)
{
    if (!_instance)
        throw Error("Can't create " + _TName("Plug") + " : null instance");

    if (!_masterNet)
        throw Error("Can't create " + _TName("Plug") + " : null master net");

    if (_masterNet->getCell() != _instance->getMasterCell())
        throw Error("Can't create " + _TName("Plug") + " : incompatible master net");

    if (!_masterNet->isExternal())
        throw Error("Can't create " + _TName("Plug") + " : not an external master net");

    if (_instance->getPlug(_masterNet))
        throw Error("Can't create " + _TName("Plug") + " : already exists");
}

void Plug::destroy()
// ****************
{
    throw Error("Abnormal deletion of " + _TName("Plug"));
}

Cell* Plug::getCell() const
// ************************
{
    return _instance->getCell();
}

DbU::Unit Plug::getX() const
// ********************
{
    return _instance->getTransformation().getX(_masterNet->getPosition());
}

DbU::Unit Plug::getY() const
// ********************
{
    return _instance->getTransformation().getY(_masterNet->getPosition());
}

Point Plug::getPosition() const
// ****************************
{
    return _instance->getTransformation().getPoint(_masterNet->getPosition());
}

Box Plug::getBoundingBox() const
// *****************************
{
    return _instance->getTransformation().getBox(_masterNet->getPosition());
}

Box Plug::getBoundingBox(const BasicLayer* basicLayer) const
// ***************************************************
{
    return Box();
}

PlugFilter Plug::getIsConnectedFilter()
// ************************************
{
    return Plug_IsConnectedFilter();
}

PlugFilter Plug::getIsUnconnectedFilter()
// **************************************
{
    return !Plug_IsConnectedFilter();
}

void Plug::setNet(Net* net)
// ************************
{
  if (net != getNet()) {
    if (net and (getCell() != net->getCell()))
      throw Error( "Plug::setNet(): Can't set Net of plug %s to %s\n"
                   "Net belong to %s instead of %s."
                 , getString(this).c_str()
                 , getString(net).c_str()
                 , getString(net->getCell()).c_str()
                 , getString(getCell()).c_str()
                 );

    if (not getBodyHook()->getSlaveHooks().isEmpty())
      throw Error( "Plug::setNet(): Can't change net of %s: Component(s) are still attached upon it."
                 , getString(this).c_str()
                 );

    if (not net) 
      cdebug_log(18,0) << "Plug::setNet(): About to disconnect " << this << endl;

    _setNet( net );
  }
}

Plug* Plug::_create(Instance* instance, Net* masterNet)
// ****************************************************
{
  if (Entity::inForcedIdMode()) Entity::setNextId( 0 );

  Plug* plug = new Plug(instance, masterNet);

  plug->_postCreate();
  return plug;
}

void Plug::_postCreate()
// *********************
{
    _instance->_getPlugMap()._insert(this);

    Inherit::_postCreate();
}

void Plug::_destroy()
// *****************
{
    _preDestroy();

    delete this;
}

void Plug::_preDestroy()
// ********************
{
  cdebug_log(18,1) << "entering Plug::_preDestroy: " << this << endl;

  Inherit::_preDestroy();

  _instance->_getPlugMap()._remove(this);

  cdebug_log(18,0) << "exiting Plug::_preDestroy:" << endl;
  cdebug_tabw(18,-1);
}

void Plug::_toJson(JsonWriter* writer) const
// *****************************************
{
  if (writer->issetFlags(JsonWriter::UsePlugReference)) {
    jsonWrite( writer, "_id"      , getId() );
    jsonWrite( writer, "_instance", getInstance()->getName() );
    jsonWrite( writer, "_bodyHook", (const_cast<Plug*>(this)->getBodyHook())->getNextHook()->toJson() );
  } else {
    Inherit::_toJson( writer );
    jsonWrite( writer, "_masterNet", getMasterNet()->getName() );
  }
}

string Plug::_getString() const
// ****************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getName());
    return s;
}

Record* Plug::_getRecord() const
// ***********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Instance", _instance));
        record->add(getSlot("MasterNet", _masterNet));
    }
    return record;
}

string Plug::getName() const
// *************************
{
    return getString(_instance->getName())
         + "."
         + getString(_masterNet->getName());
}



// ****************************************************************************************************
// JsonPlug implementation
// ****************************************************************************************************

Initializer<JsonPlug>  jsonPlugInit ( 0 );


void  JsonPlug::initialize()
// *************************
{ JsonTypes::registerType( new JsonPlug (JsonWriter::RegisterMode) ); }

JsonPlug::JsonPlug(unsigned long flags)
// ************************************
  : JsonComponent(flags)
{
  add( "_masterNet", typeid(string) );
}

string JsonPlug::getTypeName() const
// *********************************
{ return "Plug"; }

JsonPlug* JsonPlug::clone(unsigned long flags) const
// *************************************************
{ return new JsonPlug ( flags ); }

void JsonPlug::toData(JsonStack& stack)
// ************************************
{
// We are parsing an Instance.
// Plugs are automatically createds whith the Instance.
// Here we add the Plug to the stack's entity table that
// associates the Json Ids to the actual Hurricane ones.
// At the same time, we perform some coherency checking
// (i.e. the same Plug already exists).
    
// For now, simply discard "_id" & "_propertySet".
  check( stack, "JsonPlug::toData" );

  Instance* instance = get<Instance*>(stack,".Instance");
  Plug*     plug     = NULL;
  if (instance) {
    Net* masterNet = instance->getMasterCell()->getNet( get<string>(stack,"_masterNet") );
    if (not masterNet) {
      cerr << Error( "JsonPlug::toData(): Incoherency, instance %s master doesn't containt net %s."
                   , getString(instance->getName()).c_str()
                   , get<string>(stack,"_masterNet").c_str()
                   ) << endl;
    } else {
      unsigned int jsonId = get<int64_t>(stack,"_id");
      plug = instance->getPlug( masterNet );
      if (issetFlags(JsonWriter::DesignBlobMode))
        plug->forceId( jsonId );
    }
  } else {
    cerr << Error( "JsonPlug::toData(): Cannot find \".Instance\" in stack, skipping." ) << endl;
  }
  cdebug_log(19,0) << "Instance Plug contents ignored for now." << endl;

  update( stack, plug );
}



// ****************************************************************************************************
// JsonPlugRef implementation
// ****************************************************************************************************

Initializer<JsonPlugRef>  jsonPlugRefInit ( 0 );

void  JsonPlugRef::initialize()
// ****************************
{ JsonTypes::registerType( new JsonPlugRef (JsonWriter::RegisterMode) ); }

JsonPlugRef::JsonPlugRef(unsigned long flags)
// ******************************************
  : JsonObject(flags)
{
  add( ".Net"     , typeid(Net*)     );
  add( "_id"      , typeid(uint64_t) );
  add( "_instance", typeid(string)   );
  add( "_bodyHook", typeid(string)   );
}

string JsonPlugRef::getTypeName() const
// ************************************
{ return "&Plug"; }

JsonPlugRef* JsonPlugRef::clone(unsigned long flags) const
// *******************************************************
{ return new JsonPlugRef ( flags ); }

void JsonPlugRef::toData(JsonStack& stack)
// ***************************************
{
  check( stack, "JsonPlugRef::toData" );

// We are parsing a Net, perform only a "Plug::setNet()".
  Net*          net      = get<Net*>(stack,".Net");
  Instance*     instance = net->getCell()->getInstance( get<string>(stack,"_instance") );
  unsigned int  id       = get<int64_t>(stack,"_id");

  Plug* plug = stack.getEntity<Plug*>(id);
  if (plug) {
    plug->setNet( net );

    JsonNet* jnet = jget<JsonNet>( stack );
    if (jnet) {
      jnet->addHookLink( plug->getBodyHook  (), id, get<string>(stack,"_bodyHook"  ) );
    } else {
      cerr << Error( "JsonPlug::toData(): Missing (Json)Net in stack context." ) << endl;
    }
  } else {
    cerr << Error( "JsonPlugRef::toData(): No Plug id:%u in instance %s, while building net %s."
                 , id, getString(instance->getName()).c_str(), getString(net->getName()).c_str()
                 ) << endl;
  }

  update( stack, plug );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
