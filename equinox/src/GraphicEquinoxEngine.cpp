
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicEquinoxEngine.cpp"               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include <hurricane/Warning.h>
#include <hurricane/Go.h>
#include <hurricane/Error.h>
#include <hurricane/Cell.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/DataBase.h>
#include <hurricane/Technology.h>

#include <crlcore/AllianceFramework.h>
#include <crlcore/GraphicToolEngine.h>
#include <crlcore/ToolEngine.h>

#include <equinox/Tiles.h>
#include <equinox/Tile.h>
#include <equinox/EquinoxEngine.h>
#include <equinox/Strategy.h>
#include <equinox/TileSweepLine.h>
#include <equinox/GraphicEquinoxEngine.h>


namespace Equinox {


  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Graphics;
  using CRL::AllianceFramework;
    

// -------------------------------------------------------------------
// Class  :  "Equinox::GraphicEquinoxEngine".


  size_t                GraphicEquinoxEngine::_references = 0;
  GraphicEquinoxEngine* GraphicEquinoxEngine::_singleton  = NULL;


  EquinoxEngine* GraphicEquinoxEngine::createEngine ( ) 
  {
    Cell* cell = getCell ();

    EquinoxEngine* equinox = EquinoxEngine::get ( cell );
    if ( !equinox ) {
      equinox = EquinoxEngine::create ( cell );
    }
    return equinox;
  }


  GraphicEquinoxEngine* GraphicEquinoxEngine::grab ()
  {
    if ( !_references ) 
      _singleton = new GraphicEquinoxEngine ();
    _references++;

    return _singleton;
  }


  const Name& GraphicEquinoxEngine::getName () const
  { return EquinoxEngine::staticGetName (); }





  Cell* GraphicEquinoxEngine::getCell ()
  {
    if ( !_viewer ) {
      throw Error ( "<b>Equinox:</b> GraphicEquinoxEngine not bound to any Viewer." );
      return NULL;
    }

    if ( !_viewer->getCell() ) {
      throw Error ( "<b>Equinox:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }



  size_t GraphicEquinoxEngine::release ()
  {
    _references--;
    if ( !_references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }







  void GraphicEquinoxEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );
    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.extract");
    if ( !prMenu ) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if ( !menuBar ) {
        cerr << Warning("GraphicEquinoxEngine::addToMenu() - No MenuBar in parent widget.") << endl;
        return;
      }
      prMenu = menuBar->addMenu ( tr("Extract") );
      prMenu->setObjectName ( "viewer.menuBar.extract" );

     
      prMenu->addSeparator ();
    }


    QAction* gRunWAAction = _viewer->findChild<QAction*>("viewer.menuBar.extract.equinox.withalim");
    if ( gRunWAAction )
      cerr << Warning("GraphicEquinoxEngine::addToMenu() - Equinox withalim already hooked in.") << endl;
    else {
      gRunWAAction = new QAction  ( tr("Equinox - With alim"), _viewer );
      gRunWAAction->setObjectName ( "viewer.menuBar.extract.equinox.withalim" );
      gRunWAAction->setStatusTip  ( tr("Run the <b>Equinox</b> withAlim Stategy") );
      gRunWAAction->setVisible    ( true );
      prMenu->addAction ( gRunWAAction );

      connect ( gRunWAAction, SIGNAL(triggered()), this, SLOT(runWithAlim()) );
    }


    QAction* gRunWoAAction = _viewer->findChild<QAction*>("viewer.menuBar.extract.equinox.withoutalim");
    if ( gRunWoAAction )
      cerr << Warning("GraphicEquinoxEngine::addToMenu() - Equinox withoutalim already hooked in.") << endl;
    else {
      gRunWoAAction = new QAction  ( tr("Equinox - Without alim"), _viewer );
      gRunWoAAction->setObjectName ( "viewer.menuBar.extract.equinox.withoutalim" );
      gRunWoAAction->setStatusTip  ( tr("Run the <b>Equinox</b> withoutAlim Stategy") );
      gRunWoAAction->setVisible    ( true );
      prMenu->addAction ( gRunWoAAction );

      connect ( gRunWoAAction, SIGNAL(triggered()), this, SLOT(runWithoutAlim()) );
    }

    
    // fin du sous menu

    connect ( this, SIGNAL(cellPreModificated ()), _viewer->getCellWidget(), SLOT(cellPreModificate ()) );
    connect ( this, SIGNAL(cellPostModificated()), _viewer->getCellWidget(), SLOT(cellPostModificate()) );
  }



  void GraphicEquinoxEngine::runWithAlim ()
  {
    EquinoxEngine* equinox = createEngine ( );
    if ( !equinox ) return;

  //emit cellPreModificated ();

    equinox->withAlimExtract(1);

  //emit cellPostModificated ();
  }


  void GraphicEquinoxEngine::runWithoutAlim ()
  {
    EquinoxEngine* equinox = createEngine ( );
    if ( !equinox ) return;

  //emit cellPreModificated ();

    equinox->withoutAlimExtract(1);

  //emit cellPostModificated ();
  }




  GraphicEquinoxEngine::GraphicEquinoxEngine ()
    : GraphicTool   ()
    , _viewer       (NULL)
  { 
  }


  GraphicEquinoxEngine::~GraphicEquinoxEngine()
  { 
  }
  

}  // End of Equinox namespace.
