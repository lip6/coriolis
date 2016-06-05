// ****************************************************************************************************
// File: ./DataBase.cpp
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

#include "hurricane/Initializer.h"
#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/SharedPath.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"


namespace {

  using namespace std;
  using namespace Hurricane;


  class CellDepths {
    public:
      typedef map<Cell* const,int>      CellMap;
      typedef multimap<int,Cell* const> DepthMap;
    public:
                             CellDepths ( Library* );
      inline const DepthMap& getDepths  () const;
    private:
             void            _gatherCells   ( Library* );
             void            _computeDepths ();
             int             _computeDepth  ( pair<Cell* const,int>& );
    private:
      CellMap   _cellMap;
      DepthMap  _depthMap;
  };


  inline const CellDepths::DepthMap& CellDepths::getDepths () const { return _depthMap; }


  CellDepths::CellDepths ( Library* library )
    : _cellMap ()
    , _depthMap()
  {
    _gatherCells  ( library );
    _computeDepths();
  }


  void  CellDepths::_gatherCells ( Library* library )
  {
    for ( Cell* cell : library->getCells() ) _cellMap.insert( make_pair(cell,-1) );

    for ( Library* childLibrary : library->getLibraries() )
      _gatherCells( childLibrary );
  }


  int  CellDepths::_computeDepth ( pair<Cell* const,int>& cellDepth )
  {
    if (cellDepth.second != -1) return cellDepth.second;

    int depth = 0;

    if (not cellDepth.first->isLeaf()) {
      for ( Instance* instance : cellDepth.first->getInstances() ) {
        Cell* masterCell = instance->getMasterCell();
        pair<Cell* const,int>& masterDepth = *(_cellMap.find( masterCell ));
        depth = std::max( depth, _computeDepth(masterDepth)+1 );
      }
    }

    cellDepth.second = depth;
    return cellDepth.second;
  }


  void  CellDepths::_computeDepths ()
  {
    _depthMap.clear();

    for ( auto cellDepth : _cellMap ) {
      _computeDepth( cellDepth );
      _depthMap.insert( make_pair(cellDepth.second,cellDepth.first) );
    }
  }


} // Anonymous namespace.

namespace Hurricane {



// ****************************************************************************************************
// DataBase implementation
// ****************************************************************************************************

DataBase* DataBase::_db = NULL;


DataBase::DataBase()
// *****************
:    Inherit(),
    _technology(NULL),
    _rootLibrary(NULL)
{
    if (_db)
        throw Error("Can't create " + _TName("DataBase") + " : already exists");
}

DataBase* DataBase::create()
// *************************
{
    DataBase* dataBase = new DataBase();

    dataBase->_postCreate();

    return dataBase;
}

void DataBase::_postCreate()
// *************************
{
  Init::runOnce();
  Inherit::_postCreate();

  _db = this;
}

void DataBase::_preDestroy()
// ************************
{
    UpdateSession::open();
    Inherit::_preDestroy();

    if (_rootLibrary) _rootLibrary->destroy();
    if (_technology) _technology->destroy();
    UpdateSession::close();

    _db = NULL;
}

string DataBase::_getString() const
// ********************************
{
    return Inherit::_getString();
}

Record* DataBase::_getRecord() const
// *********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("_technology"    , _technology        ));
        record->add(getSlot("_rootLibrary"   , _rootLibrary       ));
        record->add(getSlot("DbU::precision" , DbU::getPrecision()));
        record->add(getSlot("DbU::resolution", DbU::db(1)         ));
      //record->add(getSlot("GridStep", getValueString(getGridStep())));
    }
    return record;
}

DataBase* DataBase::getDB()
// ************************
{
    return _db;
}

Library* DataBase::getLibrary(string rpath, unsigned int flags)
// ************************************************************
{
  Library* parent  = getRootLibrary();
  if ( not parent and (not (flags & CreateLib)) ) return NULL;

  char   separator = SharedPath::getNameSeparator();
  Name   childName;
  size_t dot       = rpath.find( separator );
  if (dot != string::npos) {
    childName = rpath.substr( 0, dot );
    rpath     = rpath.substr( dot+1 );
  } else {
    childName = rpath;
    rpath.clear();
  }

  if (not parent) {
    parent = Library::create( this, childName );
    if (flags & WarnCreateLib) {
      cerr << Warning( "DataBase::getLibrary(): Creating Root library \"%s\"."
                     , getString(childName).c_str()
                     ) << endl;
    }
  }
  if (childName != parent->getName())
    return NULL;

  while ( (dot != string::npos) and parent ) {
    dot = rpath.find( separator );
    if (dot != string::npos) {
      childName = rpath.substr( 0, dot );
      rpath     = rpath.substr( dot+1 );
    } else {
      childName = rpath;
      rpath.clear();
    }

    Library* child = parent->getLibrary( childName );
    if ( not child and (flags & CreateLib) ) {
      child = Library::create( parent, childName );
      if (flags & WarnCreateLib) {
        cerr << Warning( "DataBase::getLibrary(): Creating library \"%s\" (parent:\"%s\")."
                       , getString(childName).c_str()
                       , getString(parent->getName()).c_str()
                       ) << endl;
      }
    }
    parent = child;
  }
  return parent;
}

Cell* DataBase::getCell(string rpath, unsigned int flags)
// ******************************************************
{
  char     separator = SharedPath::getNameSeparator();
  size_t   dot       = rpath.rfind( separator );
  string   cellName  = rpath.substr(dot+1);
  Library* library   = getLibrary( rpath.substr(0,dot), flags );
  Cell*    cell      = NULL;

  if (library)
    cell = library->getCell( rpath.substr(dot+1) );

  if (not cell and _cellLoader) return _cellLoader( rpath );

  return cell;
  
}

void DataBase::_toJson(JsonWriter* w) const
// ****************************************
{
  Inherit::_toJson( w );

  jsonWrite( w, "_precision"       , DbU::getPrecision() );
  jsonWrite( w, "_gridsPerLambda"  , DbU::getGridsPerLambda() );
  jsonWrite( w, "_physicalsPerGrid", DbU::getPhysicalsPerGrid() );
  jsonWrite( w, "_technology"      , _technology  );
  jsonWrite( w, "_rootLibrary"     , _rootLibrary );

  w->key( "+cellsOrderedByDepth" );
  w->startArray();
  CellDepths cells = CellDepths( _rootLibrary );
  for ( auto depthCell : cells.getDepths() ) {
    depthCell.second->toJson( w );
  }
  w->endArray();
}


// ****************************************************************************************************
// JsonDataBase implementation
// ****************************************************************************************************


Initializer<JsonDataBase>  jsonDataBaseInit ( 0 );


JsonDataBase::JsonDataBase(unsigned long flags)
// ********************************************
  : JsonDBo(flags)
{
  add( "_precision"       , typeid(int64_t    ) );
  add( "_gridsPerLambda"  , typeid(double     ) );
  add( "_physicalsPerGrid", typeid(double     ) );
  add( "_technology"      , typeid(Technology*) );
  add( "_rootLibrary"     , typeid(Library*   ) );
}

string JsonDataBase::getTypeName() const
// *********************************
{ return "DataBase"; }

void  JsonDataBase::initialize()
// *****************************
{ JsonTypes::registerType( new JsonDataBase (JsonWriter::RegisterMode) ); }

JsonDataBase* JsonDataBase::clone(unsigned long flags) const
// *************************************************
{ return new JsonDataBase ( flags ); }

void JsonDataBase::toData(JsonStack& stack)
// ***************************************
{
  check( stack, "JsonDataBase::toData" );

  DataBase* db = DataBase::getDB();
  DbU::setPrecision       ( get<int64_t>(stack,"_precision"       ), DbU::NoTechnoUpdate );
  DbU::setGridsPerLambda  ( get<double >(stack,"_gridsPerLambda"  ), DbU::NoTechnoUpdate );
  DbU::setPhysicalsPerGrid( get<double >(stack,"_physicalsPerGrid"), DbU::Unity );

  update( stack, db );
}


} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
