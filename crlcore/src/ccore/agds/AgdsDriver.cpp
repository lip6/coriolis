// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         C R L C o r e   -  ASCII/GDS Driver                     |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./agds/AgdsDriver.cpp"                    |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Go.h"
#include "hurricane/Cell.h"
#include "hurricane/Box.h"
#include "hurricane/Transformation.h"
#include "hurricane/Pad.h"
#include "hurricane/Segment.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Query.h"
using namespace Hurricane;

#include "vlsisapd/agds/Library.h"
#include "vlsisapd/agds/Structure.h"
#include "vlsisapd/agds/Rectangle.h"
// Cannot use AGDS namespace : conflicts with Hurricane::Library object

#include "Agds.h"


namespace {


  class AgdsQuery : public Query {
    public:
                    AgdsQuery           ( Cell* );
      inline  void  setStructure        ( AGDS::Structure* );
      virtual bool  hasGoCallback       () const;
      virtual void  goCallback          ( Go* );
      virtual void  extensionGoCallback ( Go* );
      virtual void  masterCellCallback  ();
  private:
      Cell*            _cell;
      AGDS::Structure* _str;
  };


  AgdsQuery::AgdsQuery ( Cell* cell )
    : Query()
    , _cell(cell)
    , _str (NULL)
  {
    Query::setQuery( _cell, _cell->getBoundingBox(), Transformation(), NULL, 0, Query::DoComponents );
  }


  inline void  AgdsQuery::setStructure        ( AGDS::Structure* str ) { _str = str; }
         void  AgdsQuery::extensionGoCallback ( Go* ) {}
         void  AgdsQuery::masterCellCallback  ()      {}
         bool  AgdsQuery::hasGoCallback       () const { return true; }


  void AgdsQuery::goCallback ( Go* go )
  {
    Box b;
    if (dynamic_cast<const Pad*>(go)) {
      const Pad* pad = static_cast<const Pad*>(go);
      b = pad->getBoundingBox();
    }
    else if (dynamic_cast<const Contact*>(go)) {
      const Contact* contact = static_cast<const Contact*>(go);
      b = contact->getBoundingBox();
    }
    else if (dynamic_cast<const Segment*>(go)) {
      const Segment* segment = static_cast<const Segment*>(go);
      b = segment->getBoundingBox();
    }
    else
      return;

    getTransformation().applyOn( b );
    AGDS::Rectangle* rect = new AGDS::Rectangle ( getBasicLayer()->getExtractNumber()
                                                , DbU::getPhysical(b.getXMin(), DbU::Nano)
                                                , DbU::getPhysical(b.getYMin(), DbU::Nano)
                                                , DbU::getPhysical(b.getXMax(), DbU::Nano)
                                                , DbU::getPhysical(b.getYMax(), DbU::Nano));
    _str->addElement( rect );
  }


} // Anonymous namespace.


namespace CRL {


  void agdsDriver ( const string filePath
                  , Cell*        cell
                  , string&      name
                  , string&      lib
                  , double&      uUnits
                  , double&      pUnits
                  )
  {
    name = getString(cell->getName());
    replace(name.begin(), name.end(), ' ', '_');

    lib = getString(cell->getLibrary()->getName());
    replace(lib.begin(), lib.end(), ' ', '_');

    uUnits = 0.001;
    pUnits = 1.0E-9;

    AGDS::Library* gdsLib = new AGDS::Library ( lib );
    gdsLib->setUserUnits( uUnits );
    gdsLib->setPhysUnits( pUnits );
    AgdsQuery agdsQuery ( cell );

    AGDS::Structure* str = new AGDS::Structure ( getString(name) );
    agdsQuery.setStructure( str );

    forEach ( BasicLayer*, basicLayer, DataBase::getDB()->getTechnology()->getBasicLayers() ) {
      agdsQuery.setBasicLayer( *basicLayer );
      agdsQuery.doQuery();
    }

    gdsLib->addStructure( str );
    gdsLib->writeToFile ( filePath );
  }


} // CRL namespace.
