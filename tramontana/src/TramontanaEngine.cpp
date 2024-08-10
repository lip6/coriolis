// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./TramontanaEngine.cpp"                       |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Timer.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/isobar/Script.h"
#include "crlcore/Measures.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "tramontana/ShortCircuit.h"
#include "tramontana/SweepLine.h"
#include "tramontana/TramontanaEngine.h"


namespace Tramontana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::vector;
  using std::make_pair;
  using Hurricane::Tabulation;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using CRL::addMeasure;
  using CRL::Measures;
  using CRL::MeasuresSet;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TramontanaEngine".

  Name  TramontanaEngine::_toolName = "Tramontana";


  const Name& TramontanaEngine::staticGetName ()
  { return _toolName; }


  TramontanaEngine* TramontanaEngine::get ( const Cell* cell )
  { return static_cast<TramontanaEngine*>(ToolEngine::get(cell,staticGetName())); }


  TramontanaEngine::TramontanaEngine ( Cell* cell, uint32_t depth )
    : Super          (cell, (depth==0))
    , _configuration (new Configuration())
    , _viewer        (NULL)
    , _depth         (depth)
    , _flags         (0)
    , _equipotentials()
    , _openNets      ()
    , _shortedNets   ()
    , _powerNets     ()
    , _groundNets    ()
  { }


  void  TramontanaEngine::_postCreate ()
  {
    Super::_postCreate();
  }


  TramontanaEngine* TramontanaEngine::create ( Cell* cell, uint32_t depth )
  {
    TramontanaEngine* tramontana = new TramontanaEngine ( cell, depth );

    tramontana->_postCreate();

    return tramontana;
  }


  void  TramontanaEngine::_preDestroy ()
  {
    UpdateSession::open();
    cdebug_log(160,1) << "TramontanaEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << _cell->getName() << ">" << endl;
    _flags |= DestroyStage;

    ShortCircuit::removeShortingEquis( getCell() );
    for ( Equipotential* equi : _equipotentials )
      equi->destroy();
    Super::_preDestroy();

    cdebug_tabw(160,-1);
    UpdateSession::close();
  }


  TramontanaEngine::~TramontanaEngine ()
  { }


  const Name& TramontanaEngine::getName () const
  { return _toolName; }


  bool  TramontanaEngine::getSuccessState () const
  {
    bool  success = _openNets.empty() and _shortedNets.empty();
    if (not doMergeSupplies()) {
      success = success and (_powerNets.size() < 2) and (_groundNets.size() < 2);
    }
    return success;
  }


  void  TramontanaEngine::extract ( bool isTopLevel )
  {
    if (getDepth() == 0) {
      if (cmess2.enabled())
        cmess1 << "  o  Extracting " << getCell() << endl;
      if (isTopLevel) startMeasures();
    }

    cdebug_log(160,0) << "EXTRACTING " << getCell() << endl;
    for ( Instance* instance : getCell()->getInstances() ) {
      Cell*             master    = instance->getMasterCell();
      TramontanaEngine* extractor = TramontanaEngine::get( master );
      if (not extractor) {
        extractor = TramontanaEngine::create( master, getDepth()+1 );
        extractor->extract( false );
        extractor->printSummary();
      }
    }
    _extract();

    if ((getDepth() == 0) and isTopLevel) {
      printSummary();
      stopMeasures();
      printMeasures();
    }
  }


  void  TramontanaEngine::_extract ()
  {
    SweepLine sweepLine ( this );
    sweepLine.run();
    consolidate();
  //showEquipotentials();
  }


  void  TramontanaEngine::showEquipotentials () const
  {
    cerr << "Equipotentials:" << endl;
    for ( Equipotential* equi : _equipotentials ) {
      equi->show();
    }
  }


  void  TramontanaEngine::printSummary () const
  {
    if (not cmess2.enabled()) {
      string reportCell = "  o  Extracting " + getString(getCell());
      string status     = "nocheck";
      if (getCell()->isExtractConsistent()) {
        status = "success";
        if (not getSuccessState())
          status = "failed";
      }
      cmess1 << Dots::asString( reportCell, status ) << endl;
      return;
    }

    if (not getCell()->isExtractConsistent()) {
      cmess2 << "     o  No LVS summary for \"" << getCell()->getName() << "\" (inconsistent)." << endl;
      return;
    }

    ostringstream shortMessage;
    ostringstream openMessage;
    
    for ( Equipotential* equi : _shortedNets ) {
      const Equipotential::NetMap& netMap = equi->getNets(); 
      shortMessage << "        - Short circuit between " << netMap.size() << " nets:\n";
      for ( auto netData : netMap ) {
        if (netData.first->isFused()) continue;
        shortMessage << "          | \"" << netData.first->getName() << "\".\n";
      }
      shortMessage << "          + Shorted components:\n";
      for ( const ShortCircuit* shortCircuit : equi->getShortCircuits() ) {
        if (shortCircuit->isTopLevel()) {
          shortMessage << "            > T A:" << shortCircuit->getComponentA() << "\n"
                       << "            | T B:" << shortCircuit->getComponentB() << "\n";
        } else if (shortCircuit->isAcrossLevels()) {
          shortMessage << "            > A A:" << shortCircuit->getEquiA() << "\n"
                       << "            | A B:" << shortCircuit->getComponentB() << "\n";
        } else if (shortCircuit->isDeepShort()) {
          shortMessage << "            > D A:" << shortCircuit->getEquiA() << "\n"
                       << "            | D B:" << shortCircuit->getEquiB() << "\n";
        }
        shortMessage <<  "            | Shorting @" << shortCircuit->getShortingBox() << "\n";
      }
    }

    const ShortCircuit::ShortingEquis& shortingEquis = ShortCircuit::getShortingEquis( getCell() ); 
    if (not shortingEquis.empty()) {
      shortMessage << "        - Trans-hierarchical shorts:\n";
      for ( auto equiDatas : shortingEquis ) {
        shortMessage << "          | \"" << equiDatas.first->getName() << "\" in cell \""
                     << equiDatas.first->getCell()->getName() << "\" causes "
                     << equiDatas.second << " short circuits.\n";
      }
    }

    for ( auto& netEquis : _openNets ) {
      openMessage << "        - Open on " << netEquis.first << ":\n";
      for ( Equipotential* equi : netEquis.second ) {
        openMessage << "          | " << equi << ".\n";
      }
    }

    cmess2 << "     o  LVS summary for \"" << getCell()->getName() << "\"." << endl;
    cmess2 << Dots::asUInt  ("        - Power nets"    , _powerNets  .size()) << endl;
    cmess2 << Dots::asUInt  ("        - Ground nets"   , _groundNets .size()) << endl;
    cmess2 << Dots::asUInt  ("        - Short circuits", _shortedNets.size()) << endl;
    cmess2 << Dots::asUInt  ("        - Open circuits" , _openNets   .size()) << endl;
    if (_shortedNets.size()) cmess2 << shortMessage.str() << endl;
    if (_openNets   .size()) cmess2 <<  openMessage.str() << endl;
  }
  

  void  TramontanaEngine::consolidate ()
  {
  //cerr << "Tramontana::consolidate()" << endl;
    for ( Equipotential* equi : _equipotentials )
      equi->consolidate();

    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply() or net->isFused() or net->isBlockage()) continue;
      if (net->getProperty(EquipotentialRelation::staticGetName())) continue;
      _openNets.insert( make_pair( net, EquipotentialSet() ));
    }

    for ( Equipotential* equi : _equipotentials ) {
      if (equi->isBuried()) continue;

      const Equipotential::NetMap& netMap = equi->getNets(); 
      if (equi->isSupply()) {
        if (equi->isPower ()) _powerNets .push_back( equi );
        if (equi->isGround()) _groundNets.push_back( equi );

        if ( netMap.size() == 0) continue;
        if ((netMap.size() == 1) and not equi->hasFused()) continue;
        if ((netMap.size() == 2) and     equi->hasFused()) continue;
        _shortedNets.insert( equi );
        continue;
      }

      if (equi->hasOpens()) {
        auto iopenNet       = _openNets.end();
        Net* missingOpenNet = nullptr;
        for ( auto& netData : netMap ) {
          if (netData.first->isFused()) continue;
          iopenNet = _openNets.find( netData.first );
          if (iopenNet == _openNets.end()) {
            if (not missingOpenNet) missingOpenNet = netData.first;
          } else
            break;
        }
        if (iopenNet == _openNets.end()) {
          if (missingOpenNet)
            iopenNet = _openNets.insert( make_pair( missingOpenNet, EquipotentialSet() )).first;
          else {
            cerr << Error( "Tramontana::consolidate(): Unable to lookup open net for\n"
                           "        %s."
                         , getString(equi).c_str() ) << endl;
            continue;
          }
        }
        iopenNet->second.insert( equi );
      }

      if (equi->hasShorts()) {
        _shortedNets.insert( equi );
      }
    }
  }


  void   TramontanaEngine::add ( Equipotential* equi )
  {
    _equipotentials.insert( equi );
  }


  void   TramontanaEngine::remove ( Equipotential* equi )
  {
    _equipotentials.erase( equi );
  }
  

  string  TramontanaEngine::_getTypeName () const
  { return "Tramontana::TramontanaEngine"; }


  string  TramontanaEngine::_getString () const
  {
    ostringstream  os;
    os << "<TramontanaEngine " << _cell->getName () << ">";
    return os.str();
  }


  Record* TramontanaEngine::_getRecord () const
  {
    Record* record = Super::_getRecord ();
                                     
    if (record) {
      record->add( getSlot( "_equipotentials", &_equipotentials ) );
    }
    return record;
  }


} // Tramontana namespace.
