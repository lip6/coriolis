// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universié 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Dag.cpp"                                     |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DataBase.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/Measures.h"
#include "crlcore/Histogram.h"
#include "foehn/FoehnEngine.h"
#include "foehn/DagProperty.h"
#include "foehn/Dag.h"


namespace Foehn {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using Hurricane::Plug;
  using CRL::addMeasure;
  using CRL::getMeasure;
  using CRL::Histogram;


// -------------------------------------------------------------------
// Class  :  "Foehn::Dag".

  
  Dag::Dag ( FoehnEngine* foehn, string label )
    : _foehn        (foehn)
    , _configuration(foehn->getConfiguration())
    , _label        (label)
    , _dorder       ()
    , _inputs       ()
    , _reacheds     ()
  {  }


  Dag::~Dag ()
  {
    for ( Entity* entity : _dorder ) {
      DagProperty* property = DagExtension::get( entity );
      if (property) property->decref();
    }
  } 


  Cell* Dag::getCell () const
  { return _foehn->getCell(); }


  void  Dag::addDStart ( Instance* instance )
  {
    DagProperty* prop = DagExtension::get( instance );
    if (not prop) {
      prop = DagProperty::create( instance );
    }
    prop->setMinDepth( 0 );
    prop->setMaxDepth( 0 );
  //addToDOrder( instance );
    _reacheds.push_back( instance );
  }


  void  Dag::addDStart ( Net* net )
  {
    cdebug_log(130,0) << "FoehnEngine::addDStart() " << net << endl;
    DagProperty* prop = DagExtension::get( net );
    if (not prop) {
      prop = DagProperty::create( net );
    }
    prop->setMinDepth( 0 );
    prop->setMaxDepth( 0 );
    _inputs.push_back( net );
  }


  void  Dag::addToDOrder ( Instance* instance )
  {
    cdebug_log(130,1) << "FoehnEngine::addToDOrder() " << instance << endl;
    _dorder.push_back( instance );

    Net*         driver     = nullptr;
    DagProperty* driverProp = nullptr;
    DagProperty* instProp   = DagExtension::get( instance );
    if (not instProp)
      instProp = DagProperty::create( instance );

    for ( Plug* plug : instance->getPlugs() ) {
      if (not plug->getNet()) continue;
      if (plug->getMasterNet()->getDirection() & Net::Direction::DirOut) {
        driver = plug->getNet();
        driverProp = DagExtension::get( driver );
        if (not driverProp) {
          driverProp = DagProperty::create( driver );
        }
      }
    }

    if (not driver) {
      cerr << Warning( "FoehnEngine::addToDOrder(): No driver found on %s."
                     , getString(instance).c_str() ) << endl;
      cdebug_tabw(130,-1);
      return;
    }
    cdebug_log(130,0) << "driver " << driver << endl;
    driverProp->setMinDepth( instProp->getMinDepth() );
    driverProp->setDriver  ( instance );
    _dorder.push_back( driver );
    cdebug_tabw(130,-1);
  }


  void  Dag::_dpropagateOn ( Net* net )
  {
    DagProperty* netProp = DagExtension::get( net );
    cdebug_log(130,1) << "_dpropagateOn() @" << netProp->getMinDepth() << " " << net << endl;
    for ( Plug* plug : net->getPlugs() ) {
      if (plug->getMasterNet()->getDirection() & Net::Direction::DirOut) continue;
      if (plug->getInstance()) {
        _dpropagateOn( plug->getInstance() );
      }
    }
    cdebug_tabw(130,-1);
  }


  void  Dag::_dpropagateOn ( Instance* instance )
  {
    if (isDff(instance->getMasterCell()->getName())) return;

    cdebug_log(130,1) << "_dpropagateOn() " << instance << endl;
    DagProperty* prop = DagExtension::get( instance );
    if (prop) {
      cdebug_log(130,0) << "Skip, already reached " << endl;
      cdebug_tabw(130,-1);
      return;
    }

    int32_t  depth = 0;
    for ( Plug* plug : instance->getPlugs() ) {
      if (isIgnoredPlug(plug)) continue;
      if (plug->getMasterNet()->getDirection() & Net::Direction::DirIn) {
        DagProperty* netProp = DagExtension::get( plug->getNet() );
        if (not netProp) {
          cdebug_log(130,0) << "Reject: not reached " << plug->getNet() << endl;
          cdebug_tabw(130,-1);
          return;
        }
        depth = std::max( depth, netProp->getMinDepth() );
      }
    }

    if (not prop)
      prop = DagProperty::create( instance );
    prop->setMinDepth( depth+1 );
    cdebug_log(130,0) << "Reached @" << (depth+1) << endl;
  //addToDOrder( instance );
    _reacheds.push_back( instance );
    cdebug_tabw(130,-1);
  }


  void  Dag::dpropagate ()
  {
  //DebugSession::open( 130, 141 );
    cdebug_log(130,1) << "Dag::dpropagate()" << endl;
    while ( not _reacheds.empty() or not _inputs.empty() ) {
      size_t istart = _dorder.size();
      for ( Net* net : _inputs )
        _dorder.push_back( net );
      _inputs.clear();
      for ( Instance* instance : _reacheds )
        addToDOrder( instance );
      _reacheds.clear();
      cdebug_log(130,0) << "_dorder.size()=" << _dorder.size() << " istart=" << istart << endl;
      for ( ; istart<_dorder.size() ; ++istart ) {
        cdebug_log(130,0) << istart << " " << _dorder[istart] << endl;
        Net* net = dynamic_cast<Net*>( _dorder[istart] );
        if (not net) continue;
        _dpropagateOn( net );
      }
    }
    cdebug_tabw(130,-1);
  //DebugSession::close();
  }


  void  Dag::resetDepths ()
  {
    for ( Entity* entity : _dorder ) {
      DagProperty* property = DagExtension::get( entity );
      property->setMinDepth( -1 );
      property->setMaxDepth( -1 );
    }
  }

  
  string  Dag::_getTypeName () const
  { return "Dag"; }


  string  Dag::_getString () const
  {
    ostringstream os;
    os << "<" << _getTypeName() << " \"" << _label << "\" " << getCell()->getName() << ">";
    return os.str();
  }


  Record* Dag::_getRecord () const
  {
    Record* record = new Record( _getString() );
    record->add( getSlot( "_foehn"        , &_foehn         ));
    record->add( getSlot( "_configuration", &_configuration ));
    record->add( getSlot( "_label"        , &_label         ));
    record->add( getSlot( "_dorder"       , &_dorder        ));
    record->add( getSlot( "_inputs"       , &_inputs        ));
    return record;
  }


}  // Foehn namespace.
