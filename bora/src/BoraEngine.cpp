// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BoraEngine.cpp"                              |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <sstream>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DebugSession.h" 
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/HApplication.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/viewer/Script.h"
#include "hurricane/UpdateSession.h" 
#include "hurricane/analog/AnalogCellExtension.h"
#include "hurricane/analog/LayoutGenerator.h"
#include "hurricane/analog/TransistorFamily.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/GdsDriver.h"
#include "anabatic/Dijkstra.h"
#include "katana/KatanaEngine.h"
#include "bora/SlicingPlotWidget.h"
#include "bora/SlicingDataWidget.h"
#include "bora/AnalogDistance.h"
#include "bora/BoraEngine.h"
#include "bora/PyBoraEngine.h"


namespace Bora {

  using namespace std;
  using Hurricane::dbo_ptr;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Box;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;
  using Analog::AnalogCellExtension;
  using Analog::Device;
  using CRL::System;
  using CRL::GdsDriver;


// -------------------------------------------------------------------
// Class  :  "BoraEngine".

  Name  BoraEngine::_toolName = "Bora";
  bool  first = false;


  const Name& BoraEngine::staticGetName ()
  { return _toolName; }


  BoraEngine* BoraEngine::get ( const Cell* cell )
  {
    return static_cast<BoraEngine*>( ToolEngine::get(cell,staticGetName()) );
  }


  BoraEngine::BoraEngine ( Cell* cell )
    : Super  (cell)
    , _viewer(NULL)
  { }


  void  BoraEngine::_postCreate ()
  {
    Super::_postCreate();
    _runBoraInit();
  }


  BoraEngine* BoraEngine::create ( Cell* cell )
  {
    cdebug_log(539,0) << "BoraEngine* BoraEngine::create( Cell* cell )" << endl;
    BoraEngine* bora = new BoraEngine( cell );

    bora->_postCreate();
    return bora;
  }


  void  BoraEngine::_runBoraInit ()
  {
    Utilities::Path pythonSitePackages = System::getPath("pythonSitePackages");
    Utilities::Path confFile           = "coriolis/bora/initHook.py";
    Utilities::Path systemConfFile     = pythonSitePackages / confFile;

    if (systemConfFile.exists()) {
    //Isobar::Script::addPath( systemConfDir.toString() );

      dbo_ptr<Isobar::Script> script = Isobar::Script::create( confFile.toPyModPath() );
      script->addKwArgument( "bora"    , (PyObject*)PyBoraEngine_Link(this) );
      script->runFunction  ( "boraHook", getCell() );

    //Isobar::Script::removePath( systemConfDir.toString() );
    } else {
      cerr << Warning( "Bora system configuration file:\n  <%s> not found."
                     , systemConfFile.toString().c_str() ) << endl;
    }
  }


  BoraEngine::~BoraEngine ()
  { }


  void  BoraEngine::_preDestroy ()
  {
    cdebug.log(539,1) << "BoraEngine::_preDestroy()" << endl;

    cmess1 << "  o  Deleting ToolEngine<" << getName() << "> from Cell <"
           << getCell()->getName() << ">" << endl;

    cdebug.tabw(539,-1);

    Super::_preDestroy();
  }


  const Name& BoraEngine::getName () const
  { return _toolName; }


  void  BoraEngine::setViewer ( Hurricane::CellViewer* viewer )
  { _viewer = viewer; }


  void  BoraEngine::updateSlicingTree()
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( getCell() );
    if (slicingtree) {
      cmess1 << "  o  Updating the SlicingTree." << endl;

      startMeasures();

      slicingtree->updateGlobalSize();

      stopMeasures();
      printMeasures();
    } else {
      cerr << Error( "BoraEngine::updateSlicingTree(): "
                     "Cannot update, the SlicingTree needs to be created first." ) << endl;
    }
  }


  void  BoraEngine::updatePlacement ( DbU::Unit width, DbU::Unit height )
  {
    cdebug_log(536,1) << "BoraEngine::updatePlacement(DbU::Unit,DbU::Unit)" << endl;

    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( getCell() );
    if (slicingtree){
      BoxSet* bs = slicingtree->getPairHW( width, height );
      if (bs) updatePlacement( bs );
    } else
      cerr << "[Error]: The slicing tree needs to be created first." << endl;

    cdebug_tabw(536,-1);
  }


  void  BoraEngine::updatePlacement ( size_t index )
  {
    cdebug_log(536,1) << "BoraEngine::updatePlacement(int)" << endl;

    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( getCell() );
    if (slicingtree) {
      cmess2 << Dots::asInt( "  o  Selecting analog placement (index)", index ) << endl;

      BoxSet* bs = slicingtree->getPairHW( index );
      if (bs) updatePlacement( bs );
    } else
      cerr << Error( "BoraEngine::updatePlacement(): The slicing tree needs to be created first." ) << endl;

    cdebug_tabw(536,-1);
  }


  void  BoraEngine::updatePlacement ( BoxSet* boxSet )
  {
    cdebug_log(536,1) << "BoraEngine::updatePlacement(BoxSet*)" << endl;

  //DebugSession::addToTrace( getCell(), "vin+" );

    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( getCell() );
    if (slicingtree) { 
      NodeSets nodeSets = slicingtree->getNodeSets();
      if (not nodeSets.empty()) {
        Cell* cell = SlicingNode::getCell();
        Katana::KatanaEngine* katana = Katana::KatanaEngine::get( cell );
        if (katana) katana->destroy();

        slicingtree->_setGlobalSize( boxSet ); 

        if (_viewer) emitCellPreModificated ();
        UpdateSession::open();
        slicingtree->clearGlobalRouting();
        slicingtree->place();

        CRL::RoutingGauge* rg      = slicingtree->getRoutingGauge();
        DbU::Unit          hpitch  = rg->getHorizontalPitch();
        DbU::Unit          vpitch  = rg->getVerticalPitch();
        
        slicingtree->expandRoutingChannel( hpitch*2, vpitch*2 );
        slicingtree->replace();
        slicingtree->updateSymNetAxis();
        UpdateSession::close();

        katana = Katana::KatanaEngine::create(cell);
        katana->openSession();
        Anabatic::GCell* gcell = katana->getSouthWestGCell();
        slicingtree->setKatana( katana );
        slicingtree->setGCell( gcell );
        slicingtree->clearRoutingPads();
        slicingtree->flattenDigitalNets();
        slicingtree->createRoutingPads();
        slicingtree->resetWireOccupation();
        Katana::Session::close();
        katana->openSession();

        Anabatic::Dijkstra* dijkstra = new Anabatic::Dijkstra( katana );
        AnalogDistance distance = AnalogDistance( cell, hpitch, vpitch );
        dijkstra->setDistance      ( distance );
        dijkstra->setSearchAreaHalo( std::max(hpitch,vpitch) );

        for ( Net* net : cell->getNets() ) {
          distance.setNet( net );
          dijkstra->load( net );
          NetRoutingState* state = NetRoutingExtension::get( net );
          if ( not (state and state->isSymSlave()) ) {
            slicingtree->setVertexRestriction( net, katana );
            dijkstra->run();
            slicingtree->updateWireOccupation( dijkstra );
          }
        }
        katana->setState( Anabatic::EngineState::EngineGlobalLoaded );

        slicingtree->expandRoutingChannel();
        slicingtree->replace();
        slicingtree->updateSymNetAxis();
        
        katana->updateMatrix();
        katana->analogInit();
        Katana::Session::close(); 
        
        if (_viewer) {
          getCellWidget()->fitToContents();
          emitCellPostModificated();
        }
      } else
        cerr << Error( "BoraEngine::updatePlacement(): The slicing tree can not be placed.\n"
                       "        The slicing tree needs to be updated first." ) << endl;
    } else
      cerr << Error( "BoraEngine::updatePlacement(): The slicing tree can not be set.\n"
                     "        The slicing tree needs to be created first." ) << endl;

    cdebug_tabw(536,-1);
  }


  void  BoraEngine::setToleranceRatio ( DbU::Unit ratioh, DbU::Unit ratiow )
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) {
      slicingtree->setToleranceRatioH( ratioh );
      slicingtree->setToleranceRatioW( ratiow );
      resetPlacement();
    } else
      cerr << Error( "BoraEngine::setToleranceRatio(): The slicing tree's tolerance ratio cannot be set.\n"
                     "        The slicing tree needs to be created first." ) << endl;
  }


  void  BoraEngine::setToleranceBand ( DbU::Unit bandh, DbU::Unit bandw )
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) {
      slicingtree->setToleranceBandH( bandh );
      slicingtree->setToleranceBandW( bandw );
      resetPlacement();
    } else
      cerr << Error( "BoraEngine::setToleranceBand(): The slicing tree's tolerance ratio can not be set.\n"
                     "        The slicing tree needs to be created first." ) << endl;
  }


  void  BoraEngine::showPareto ()
  { }


  void BoraEngine::resetPlacement ()
  {
    emitCellPreModificated ();
    DbU::Unit y = 0;
    for ( Instance* instance : getCell()->getInstances() ) {
      UpdateSession::open();
      instance->setTransformation ( Transformation( 0, y ) );
      instance->setPlacementStatus( Instance::PlacementStatus::PLACED );
      UpdateSession::close();

      Cell* model = instance->getMasterCell();   
      if (model) {
        Device* device = dynamic_cast<Device*>( model );
        if (device) y += device->getAbutmentBox().getHeight();
      }
    }
    getCellWidget()->fitToContents();
    emitCellPostModificated();
  }


  DbU::Unit  BoraEngine::getToleranceRatioH () const
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) return slicingtree->getToleranceRatioH();

    cerr << Error( "BoraEngine::getToleranceRatioH(): The slicing tree needs to be created first." ) << endl;
    return 0;
  }


  DbU::Unit  BoraEngine::getToleranceRatioW () const
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) return slicingtree->getToleranceRatioW();

    cerr << Error( "BoraEngine::getToleranceRatioW(): The slicing tree needs to be created first." ) << endl;
    return 0;
  }


  DbU::Unit  BoraEngine::getToleranceBandH () const
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) return slicingtree->getToleranceBandH();

    cerr << Error( "BoraEngine::getToleranceBandH(): The slicing tree needs to be created first." ) << endl;
    return 0;
  }


  DbU::Unit  BoraEngine::getToleranceBandW () const
  {
    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (slicingtree) return slicingtree->getToleranceBandW();

    cerr << Error( "BoraEngine::getToleranceBandW(): The slicing tree needs to be created first." ) << endl;
    return 0;
  }


} // Bora namespace.
