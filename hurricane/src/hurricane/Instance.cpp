// ****************************************************************************************************
// File: ./Instance.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#include "hurricane/Warning.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/SharedPath.h"
#include "hurricane/DataBase.h"
#include "hurricane/Instance.h"
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Plug.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Error.h"

namespace Hurricane {


// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Instance_IsUnderFilter : public Filter<Instance*> {
// ****************************************************

    public: Box _area;

    public: Instance_IsUnderFilter(const Box& area)
    // ********************************************
    : _area(area)
    {
    };

    public: Instance_IsUnderFilter(const Instance_IsUnderFilter& filter)
    // *****************************************************************
    :    _area(filter._area)
    {
    };

    public: Instance_IsUnderFilter& operator=(const Instance_IsUnderFilter& filter)
    // ****************************************************************************
    {
        _area = filter._area;
        return *this;
    };

    public: virtual Filter<Instance*>* getClone() const
    // ************************************************
    {
        return new Instance_IsUnderFilter(*this);
    };

    public: virtual bool accept(Instance* instance) const
    // **************************************************
    {
        return _area.intersect(instance->getBoundingBox());
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Instance::IsUnderFilter") + " " + getString(_area) + ">";
    };

};

class Instance_IsTerminalFilter : public Filter<Instance*> {
// *******************************************************

    public: Instance_IsTerminalFilter() {};

    public: Instance_IsTerminalFilter(const Instance_IsTerminalFilter& filter) {};

    public: Instance_IsTerminalFilter& operator=(const Instance_IsTerminalFilter& filter) {return *this;};

    public: virtual Filter<Instance*>* getClone() const {return new Instance_IsTerminalFilter(*this);};

    public: virtual bool accept(Instance* instance) const {return instance->isTerminal();};

    public: virtual string _getString() const {return "<" + _TName("Instance::IsTerminalFilter") + ">";};

};

class Instance_IsTerminalNetlistFilter : public Filter<Instance*>
{
    public:
                                                Instance_IsTerminalNetlistFilter () {};
                                                Instance_IsTerminalNetlistFilter ( const Instance_IsTerminalNetlistFilter& filter ) {};
              Instance_IsTerminalNetlistFilter& operator=             ( const Instance_IsTerminalNetlistFilter& filter )
                                                  { return *this; };
      virtual Filter<Instance*>*                getClone              () const
                                                  { return new Instance_IsTerminalNetlistFilter( *this ); };
      virtual bool                              accept                ( Instance* instance ) const
                                                  { return instance->isTerminalNetlist(); };
      virtual string                            _getString            () const
                                                  { return "<" + _TName("Instance::IsTerminalNetlistFilter") + ">"; };
  private:
    uint32_t _flags;

};

class Instance_IsUnplacedFilter : public Filter<Instance*> {
// *******************************************************

    public: Instance_IsUnplacedFilter() {};

    public: Instance_IsUnplacedFilter(const Instance_IsUnplacedFilter& filter) {};

    public: Instance_IsUnplacedFilter& operator=(const Instance_IsUnplacedFilter& filter) {return *this;};

    public: virtual Filter<Instance*>* getClone() const {return new Instance_IsUnplacedFilter(*this);};

    public: virtual bool accept(Instance* instance) const {return instance->isUnplaced();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsUnplacedFilter>");};

};

class Instance_IsPlacedFilter : public Filter<Instance*> {
// *****************************************************

    public: Instance_IsPlacedFilter() {};

    public: Instance_IsPlacedFilter(const Instance_IsPlacedFilter& filter) {};

    public: Instance_IsPlacedFilter& operator=(const Instance_IsPlacedFilter& filter) {return *this;};

    public: virtual Filter<Instance*>* getClone() const {return new Instance_IsPlacedFilter(*this);};

    public: virtual bool accept(Instance* instance) const {return instance->isPlaced();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsPlacedFilter>");};

};

class Instance_IsFixedFilter : public Filter<Instance*> {
// *****************************************************

    public: Instance_IsFixedFilter() {};

    public: Instance_IsFixedFilter(const Instance_IsFixedFilter& filter) {};

    public: Instance_IsFixedFilter& operator=(const Instance_IsFixedFilter& filter) {return *this;};

    public: virtual Filter<Instance*>* getClone() const {return new Instance_IsFixedFilter(*this);};

    public: virtual bool accept(Instance* instance) const {return instance->isFixed();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsFixedFilter>");};

};

// ****************************************************************************************************
// Instance implementation
// ****************************************************************************************************

Instance::Instance(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag)
// ****************************************************************************************************
:    Inherit(),
    _cell(cell),
    _name(name),
    _masterCell(masterCell),
    _transformation(transformation),
    _placementStatus(placementstatus),
    _plugMap(),
    _sharedPathMap(),
    _nextOfCellInstanceMap(NULL),
    _nextOfCellSlaveInstanceSet(NULL)
{
    if (!_cell)
        throw Error("Instance::Instance(): Can't create " + _TName("Instance") + ", NULL cell");

    if (name.isEmpty())
        throw Error("Instance::Instance(): Can't create " + _TName("Instance") + ", empty name");

    if (_cell->getInstance(_name))
        throw Error("Instance::Instance(): Can't create " + _TName("Instance") + " " + getString(_name) + ", already exists");

    if (!_masterCell)
        throw Error("Instance::Instance(): Can't create " + _TName("Instance") + ", NULL master cell");

    if (_masterCell == _cell)
        throw Error( "Instance::Instance(): Can't create " + _TName("Instance") + ", immediate cyclic construction\n"
                   + "        (Instance \"" + getString(name).c_str()
                   + "\" in Cell \"" + getString(_cell->getName()).c_str() + "\" uses it's owner as model)"
                   );

    if (secureFlag && _cell->isCalledBy(_masterCell))
        throw Error("Instance::Instance(): Can't create " + _TName("Instance") + ", cyclic construction");
}

Instance* Instance::create(Cell* cell, const Name& name, Cell* masterCell, bool secureFlag)
// ****************************************************************************************
{
  if (not cell)
    throw Error( "Instance::create(): NULL owner Cell argument for %s.", getString(name).c_str() );

  if (not masterCell)
    throw Error( "Instance::create(): NULL master Cell argument for %s.", getString(name).c_str() );

  // if (cell->isUniquified())
  //   throw Error( "Instance::create(): %s master Cell is an uniquified copy.", getString(cell).c_str() );

  Instance* instance =
    new Instance(cell, name, masterCell, Transformation(), PlacementStatus(), secureFlag);

  instance->_postCreate();
  return instance;
}

Instance* Instance::create(Cell* cell, const Name& name, Cell* masterCell, const Transformation& transformation, const PlacementStatus& placementstatus, bool secureFlag)
// **********************************************************************************************************************************************************************
{
  if (not cell)
    throw Error( "Instance::create(): NULL owner Cell argument for %s.", getString(name).c_str() );

  if (not masterCell)
    throw Error( "Instance::create(): NULL master Cell argument for %s.", getString(name).c_str() );

  // if (cell->isUniquified())
  //   throw Error( "Instance::create(): %s master Cell is an uniquified copy.", getString(cell).c_str() );

  Instance* instance =
    new Instance(cell, name, masterCell, transformation, placementstatus, secureFlag);

  instance->_postCreate();
  return instance;
}

Box Instance::getBoundingBox() const
// *********************************
{
    return  _transformation.getBox(_masterCell->getBoundingBox());
}

Plugs Instance::getConnectedPlugs() const
// **************************************
{
    return getPlugs().getSubSet(Plug::getIsConnectedFilter());
}

Plugs Instance::getUnconnectedPlugs() const
// ****************************************
{
    return getPlugs().getSubSet(Plug::getIsUnconnectedFilter());
}

Path Instance::getPath(const Path& tailPath) const
// ***********************************************
{
    return Path((Instance*)this, tailPath);
}

Box Instance::getAbutmentBox() const
// *********************************
{
    return _transformation.getBox(_masterCell->getAbutmentBox());
}

bool Instance::isTerminal() const
// ******************************
{
    return getMasterCell()->isTerminal();
}

bool Instance::isTerminalNetlist() const
// *************************************
{
    return getMasterCell()->isTerminalNetlist();
}

bool Instance::isUnique() const
// ****************************
{
  return _masterCell->isUnique();
}

bool Instance::isUniquified() const
// ********************************
{
  return _masterCell->isUniquified();
}

bool Instance::isUniquifyMaster() const
// ************************************
{
  return _masterCell->isUniquifyMaster();
}


InstanceFilter Instance::getIsUnderFilter(const Box& area)
// *******************************************************
{
    return Instance_IsUnderFilter(area);
}

InstanceFilter Instance::getIsTerminalFilter()
// *******************************************
{
    return Instance_IsTerminalFilter();
}

InstanceFilter Instance::getIsTerminalNetlistFilter()
// **************************************************
{
    return Instance_IsTerminalNetlistFilter();
}

InstanceFilter Instance::getIsUnplacedFilter()
// *******************************************
{
    return Instance_IsUnplacedFilter();
}

InstanceFilter Instance::getIsPlacedFilter()
// *****************************************
{
    return Instance_IsPlacedFilter();
}

InstanceFilter Instance::getIsFixedFilter()
// ****************************************
{
    return Instance_IsFixedFilter();
}

InstanceFilter Instance::getIsNotUnplacedFilter()
// **********************************************
{
    return !Instance_IsUnplacedFilter();
}

void Instance::materialize()
// *************************
{
  if (not isMaterialized()) {
    Box boundingBox = getBoundingBox();
    if (!boundingBox.isEmpty()) {
      QuadTree* quadTree = _cell->_getQuadTree();
      quadTree->insert(this);
      _cell->_fit(quadTree->getBoundingBox());
    }
  }
}

void Instance::unmaterialize()
// ***************************
{
    if (isMaterialized()) {
        _cell->_unfit(getBoundingBox());
        _cell->_getQuadTree()->remove(this);
    }
}

void Instance::invalidate(bool propagateFlag)
// ******************************************
{
    Inherit::invalidate(false);

    if (propagateFlag) {
        for_each_plug(plug, getConnectedPlugs()) {
            plug->invalidate(true);
            end_for;
        }
    }
}

void Instance::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *****************************************************
{
    if ((dx != 0) || (dy !=0)) {
        Point translation = _transformation.getTranslation();
        DbU::Unit x = translation.getX() + dx;
        DbU::Unit y = translation.getY() + dy;
        Transformation::Orientation orientation = _transformation.getOrientation();
        setTransformation(Transformation(x, y, orientation));
    }
}

void Instance::setName(const Name& name)
// *************************************
{
    if (name != _name) {
        if (name.isEmpty())
            throw Error("Can't change instance name : empty name");

        if (_cell->getInstance(name))
            throw Error("Can't change instance name : already exists");

        _cell->_getInstanceMap()._remove(this);
        _name = name;
        _cell->_getInstanceMap()._insert(this);
    }
}

void Instance::setTransformation(const Transformation& transformation)
// *******************************************************************
{
    if (transformation != _transformation) {
        invalidate(true);
        _transformation = transformation;
    }
}

void Instance::setPlacementStatus(const PlacementStatus& placementStatus)
// **********************************************************************
{
  if (placementStatus != _placementStatus) {
    invalidate(true);

    if (placementStatus & (PlacementStatus::PLACED|PlacementStatus::FIXED)) {
      materialize ();
    } else if (placementStatus == PlacementStatus::UNPLACED)
      unmaterialize ();

    _placementStatus = placementStatus;
  }
}

void Instance::setMasterCell(Cell* masterCell, bool secureFlag)
// ************************************************************
{
  cdebug_log(18,1) << "Instance::setMasterCell() on " << this << endl;
  cdebug_log(18,0) << "NEW masterCell:" << masterCell << endl;

  if (masterCell != _masterCell) {
    UpdateSession::open();

    if (!masterCell)
      throw Error("Can't set master : null master cell");

    if (secureFlag && _cell->isCalledBy(masterCell))
      throw Error("Can't set master : cyclic construction");

    list<Plug*> connectedPlugList;
    list<Net*> masterNetList;
    for_each_plug(plug, getConnectedPlugs()) {
      Net* masterNet = masterCell->getNet(plug->getMasterNet()->getName());
      if (!masterNet || !masterNet->isExternal())
        throw Error("Can't set master (bad master net matching)");
      connectedPlugList.push_back(plug);
      masterNetList.push_back(masterNet);
      end_for;
    }

    for_each_shared_path(sharedPath, _getSharedPathes()) {
      if (!sharedPath->getTailSharedPath())
      // if the tail is empty the SharedPath isn't impacted by the change
        delete sharedPath;
      end_for;
    }

    invalidate(true);

    for_each_plug(plug, getUnconnectedPlugs()) {
      plug->_destroy();
      end_for;
    }

    while (!connectedPlugList.empty() && !masterNetList.empty()) {
      Plug* plug = connectedPlugList.front();
      Net* masterNet = masterNetList.front();
      _plugMap._remove(plug);
      plug->_setMasterNet(masterNet);
      _plugMap._insert(plug);
      connectedPlugList.pop_front();
      masterNetList.pop_front();
    }

    cdebug_log(18,0) << "Remove " << this << " from " << _masterCell << endl;
    _masterCell->_getSlaveInstanceSet()._remove(this);
    _masterCell = masterCell;

    cdebug_log(18,0) << "Add (before) " << this << " to " << _masterCell << endl;
    _masterCell->isUnique();
    _masterCell->_getSlaveInstanceSet()._insert(this);
    cdebug_log(18,0) << "Add (after) " << this << " to " << _masterCell << endl;
    _masterCell->isUnique();

    for_each_net(externalNet, _masterCell->getExternalNets()) {
      if (!getPlug(externalNet)) Plug::_create(this, externalNet);
      end_for;
    }

    UpdateSession::close();
  }

  cdebug_tabw(19,-1);
}

void Instance::uniquify()
// **********************
{
  if (_masterCell->isUnique()) {
    cerr << Warning( "Instance::uniquify(): Master Cell %s of %s is unique or already uniquified, cancelled."
                   , getString(_masterCell->getName()).c_str()
                   , getString(getName()).c_str()
                   ) << endl;
    return;
  }

  if (not _getSharedPathMap().isEmpty()) {
    cerr << Warning( "Instance::uniquify(): While uniquifying model %s of instance %s, SharedPathMap is not empty.\n"
                     "          (%u Entity's Occurrences will still uses the original master Cell)"
                   , getString(_masterCell->getName()).c_str()
                   , getString(getName()).c_str()
                   , _getSharedPathMap()._getSize()
                   ) << endl;
  }

  setMasterCell( _masterCell->getClone() );
}

void Instance::slaveAbutmentBox()
// ******************************
{
  if (not _masterCell->isUnique()) uniquify();
  _masterCell->slaveAbutmentBox( getCell() );
//_masterCell->_setShuntedPath( Path(getCell()->getShuntedPath(),this) );
  setTransformation( Transformation() );
  setPlacementStatus( Instance::PlacementStatus::PLACED );
}

Instance* Instance::getClone(Cell* cloneCell) const
// ************************************************
{
  Instance* clone = Instance::create( cloneCell
                                    , getName()
                                    , getMasterCell()
                                    , getTransformation()
                                    , getPlacementStatus()
                                    );

  if (not clone->_getSharedPathMap().isEmpty()) {
    cerr << Warning( "Instance::getClone(): While cloning instance %s, SharedPathMap is not empty.\n"
                     "          (Occurrence will still uses the original instance)"
                   , getString(getName()).c_str()
                   ) << endl;
  }

  for( Plug* iplug : getPlugs() ) {
    if (iplug->isConnected()) {
      Plug* clonePlug = clone->getPlug( iplug->getMasterNet() );
      Net*  cloneNet  = cloneCell->getNet( iplug->getNet()->getName() );
      if (cloneNet) {
        clonePlug->setNet( cloneNet );
      } else {
        cerr << Warning( "Instance::getClone(): While cloning instance %s, missing net %s in cloned cell %s."
                       , getString(getName()).c_str()
                       , getString(iplug->getNet()->getName()).c_str()
                       , getString(cloneCell->getName()).c_str()
                       ) << endl;
      }
    }
  }

  return clone;
}

void Instance::_postCreate()
// *************************
{
    _cell->_getInstanceMap()._insert(this);
    _masterCell->_getSlaveInstanceSet()._insert(this);

    for_each_net(externalNet, _masterCell->getExternalNets()) {
        Plug::_create(this, externalNet);
        end_for;
    }

    bool autoMaterialization = not autoMaterializationIsDisabled();
    if ( _placementStatus == PlacementStatus::UNPLACED ) {
      disableAutoMaterialization();
    }

    Inherit::_postCreate();

    if ( autoMaterialization ) enableAutoMaterialization();
}

void Instance::_preDestroy()
// ************************
{
  SharedPathes pathes = _getSharedPathes();
  while ( pathes.getFirst() ) delete pathes.getFirst();

  Inherit::_preDestroy();

  Plugs plugs = getPlugs();
  while ( plugs.getFirst() ) plugs.getFirst()->_destroy();

  _masterCell->_getSlaveInstanceSet()._remove(this);
  _cell->_getInstanceMap()._remove(this);

  if (_masterCell->isUniquified()) _masterCell->destroy();
}

string Instance::_getString() const
// ********************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    s.insert(s.length() - 1, " " + getString(_masterCell->getName()));
    return s;
}

Record* Instance::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Cell", _cell));
        record->add(getSlot("Name", &_name));
        record->add(getSlot("MasterCell", _masterCell));
        record->add(getSlot("Transformation", &_transformation));
        record->add(getSlot("PlacementStatus", &_placementStatus));
        record->add(getSlot("XCenter", DbU::getValueString(getAbutmentBox().getXCenter())));
        record->add(getSlot("YCenter", DbU::getValueString(getAbutmentBox().getYCenter())));
        record->add(getSlot("Plugs", &_plugMap));
        record->add(getSlot("SharedPathes", &_sharedPathMap));
    }
    return record;
}

void Instance::_toJson( JsonWriter* writer ) const
// ***********************************************
{
  Inherit::_toJson( writer );

  jsonWrite( writer, "_name"           , getName()        );
  jsonWrite( writer, "_masterCell"     , _masterCell->getHierarchicalName() );
  jsonWrite( writer, "_transformation" , &_transformation  );
  jsonWrite( writer, "_placementStatus", _placementStatus );
}

void Instance::_toJsonCollections(JsonWriter* writer) const
// ********************************************************
{
  jsonWrite( writer, "+plugMap", getPlugs() );
  Inherit::_toJsonCollections( writer );
}

// ****************************************************************************************************
// Instance::PlugMap implementation
// ****************************************************************************************************

Instance::PlugMap::PlugMap()
// *************************
:    Inherit()
{
}

const Net* Instance::PlugMap::_getKey(Plug* plug) const
// ****************************************************
{
    return plug->getMasterNet();
}

unsigned Instance::PlugMap::_getHashValue(const Net* masterNet) const
// ******************************************************************
{
  return masterNet->getId() / 8;
}

Plug* Instance::PlugMap::_getNextElement(Plug* plug) const
// *******************************************************
{
    return plug->_getNextOfInstancePlugMap();
}

void Instance::PlugMap::_setNextElement(Plug* plug, Plug* nextPlug) const
// **********************************************************************
{
    plug->_setNextOfInstancePlugMap(nextPlug);
}



// ****************************************************************************************************
// Instance::SharedPathMap implementation
// ****************************************************************************************************

Instance::SharedPathMap::SharedPathMap()
// *************************************
:    Inherit()
{
}

const SharedPath* Instance::SharedPathMap::_getKey(SharedPath* sharedPath) const
// *****************************************************************************
{
    return sharedPath->getTailSharedPath();
}

unsigned Instance::SharedPathMap::_getHashValue(const SharedPath* tailSharedPath) const
// ************************************************************************************
{
  return (tailSharedPath) ? (tailSharedPath->getHash()/8) : 0;
}

SharedPath* Instance::SharedPathMap::_getNextElement(SharedPath* sharedPath) const
// *******************************************************************************
{
    return sharedPath->_getNextOfInstanceSharedPathMap();
}

void Instance::SharedPathMap::_setNextElement(SharedPath* sharedPath, SharedPath* nextSharedPath) const
// ****************************************************************************************************
{
    sharedPath->_setNextOfInstanceSharedPathMap(nextSharedPath);
};

// ****************************************************************************************************
// Instance::PlacementStatus implementation
// ****************************************************************************************************

Instance::PlacementStatus::PlacementStatus(const Code& code)
// *********************************************************
:    _code(code)
{ }

Instance::PlacementStatus::PlacementStatus(const PlacementStatus& placementstatus)
// *******************************************************************************
:    _code(placementstatus._code)
{ }

Instance::PlacementStatus::PlacementStatus(string s)
// *************************************************
:    _code(UNPLACED)
{
  if      (s == "UNPLACED") _code = UNPLACED;
  else if (s == "PLACED"  ) _code = PLACED;
  else if (s == "FIXED"   ) _code = FIXED;
}

Instance::PlacementStatus& Instance::PlacementStatus::operator=(const PlacementStatus& placementstatus)
// ****************************************************************************************************
{
    _code = placementstatus._code;
    return *this;
}

string Instance::PlacementStatus::_getString() const
// *************************************************
{
  return getString(&_code);
}

Record* Instance::PlacementStatus::_getRecord() const
// ********************************************
{
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", &_code));
    return record;
}


// ****************************************************************************************************
// JsonInstance implementation
// ****************************************************************************************************

Initializer<JsonInstance>  jsonInstanceInit ( 0 );

void  JsonInstance::initialize()
// *****************************
{ JsonTypes::registerType( new JsonInstance (JsonWriter::RegisterMode) ); }

JsonInstance::JsonInstance(unsigned long flags)
// ********************************************
  : JsonEntity(flags)
{
  add( "_name"           , typeid(string)          );
  add( "_masterCell"     , typeid(string)          );
  add( "_transformation" , typeid(Transformation*) );
  add( "_placementStatus", typeid(string)          );
  add( "+plugMap"        , typeid(JsonArray)       );
}

string JsonInstance::getTypeName() const
// *************************************
{ return "Instance"; }

JsonInstance* JsonInstance::clone(unsigned long flags) const
// *********************************************************
{ return new JsonInstance ( flags ); }

void JsonInstance::toData(JsonStack& stack)
// ****************************************
{
  check( stack, "JsonInstance::toData" );
  presetId( stack );

  Instance* instance = Instance::create
    ( get<Cell* >(stack,".Cell")
    , get<string>(stack,"_name")
    , DataBase::getDB()->getCell( get<string>(stack,"_masterCell"), DataBase::NoFlags )
    , get<Transformation>(stack,"_transformation") 
    , Instance::PlacementStatus(get<string>(stack,"_placementStatus") )
    );
  
  update( stack, instance );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
