// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      C y c l o p  -  S i m p l e   V i e w e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Cyclop.cpp"                             |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QMenu>
#include "hurricane/viewer/CellWidget.h"
#include "OpenCellDialog.h"
#include "DemoGo.h"
#include "Cyclop.h"


namespace CRL {

// -------------------------------------------------------------------
// Class  :  "Cyclop".


  Cyclop::Cyclop ( QWidget* parent )
    : CellViewer(parent)
    , _banner ( "Cyclop"
              , "2.0b"
              , "Coriolis Simple Viewer"
              , "2006"
              , "Jean-Paul Chaput"
              , ""
              )
    , _stressDisplayAction(NULL)
  { }


  Cyclop::~Cyclop ()
  { }


  Cyclop* Cyclop::create ( QWidget* parent )
  {
    Cyclop* cyclop = new Cyclop ( parent );

    cyclop->_postCreate ();

    return cyclop;
  }


  void  Cyclop::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  void  Cyclop::_postCreate ()
  {
    setApplicationName ( tr("cyclop") );

    QAction* openAction = findChild<QAction*>("viewer.menuBar.file.openCell");
    if ( openAction ) {
      connect ( openAction, SIGNAL(triggered()), this, SLOT(openCell()) );
    }

    QMenu* debugMenu = createDebugMenu ();
    _stressDisplayAction = new QAction  ( tr("Stress Display"), this );
    _stressDisplayAction->setObjectName ( "viewer.menuBar.debug.stressDisplay" );
    _stressDisplayAction->setStatusTip  ( tr("Intensive use of display redrawing") );
    connect ( _stressDisplayAction, SIGNAL(triggered()), this, SLOT(stressDisplay()) );

  //debugMenu->addAction ( _stressDisplayAction );

    getCellWidget()->addDrawExtensionGo ( DemoGo::staticGetName()
                                        , DemoGo::initDrawDemoGo
                                        , DemoGo::drawDemoGo
                                        );
  }


  void  Cyclop::_preDestroy ()
  { }


  Cell* Cyclop::getCellFromDb ( const char* name )
  {
    return AllianceFramework::get()->getCell ( name, Catalog::State::Views );
  }


  void  Cyclop::openCell ()
  {
    Cyclop*  viewer   = this;
    QString  cellName;
    bool     newViewer;

    if ( OpenCellDialog::runDialog ( this, cellName, newViewer ) ) {
      Cell* cell = getCellFromDb ( cellName.toStdString().c_str() );
      if ( cell ) {
        if ( newViewer ) {
          viewer = Cyclop::create ();
          viewer->show ();
        }
        viewer->setCell ( cell );
      } else
        cerr << "[ERROR] Cell not found: " << cellName.toStdString() << endl;
    }
  }


  void  Cyclop::stressDisplay ()
  {
    cerr << "Starting Stress Display Test." << endl;
    for ( int i=0 ; i<1000 ; i++ ) {
      if ( !(i % 100) ) cerr << "iteration: " << i << endl;
      getCellWidget()->goUp   ( 150 );
      getCellWidget()->goDown ( 150 );
    }
    close ();
  }


} // End of CRL namespace.
