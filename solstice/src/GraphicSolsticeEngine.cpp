
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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicSolsticeEngine.cpp"              |
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
#include <hurricane/viewer/PaletteWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/viewer/ControllerWidget.h>

#include <crlcore/AllianceFramework.h>
#include <crlcore/GraphicToolEngine.h>

#include <solstice/GraphicSolsticeEngine.h>
#include <solstice/SolsticeEngine.h>

#include <solstice/TabRoutingErrorList.h>


namespace Solstice {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Graphics;
  using CRL::AllianceFramework;


// -------------------------------------------------------------------
// Class  :  "CRL::GraphicSolsticeEngine".


  size_t                 GraphicSolsticeEngine::_references = 0;
  GraphicSolsticeEngine* GraphicSolsticeEngine::_singleton  = NULL;


  SolsticeEngine* GraphicSolsticeEngine::createEngine ( ) 
  {
    Cell* cell = getCell ();
    
    SolsticeEngine* solstice = SolsticeEngine::get ( cell );
    if ( !solstice ) {
      solstice = SolsticeEngine::create ( cell );
    }
    return solstice;
  }
  



  void  GraphicSolsticeEngine::initSolsticeRoutingError ( CellWidget* widget )
  {
    // QPainter& painter = widget->getPainter();

  
  }


  void  GraphicSolsticeEngine::drawSolsticeRoutingError ( CellWidget*           widget
							  , const Go*             go
							  , const BasicLayer*     basicLayer
							  , const Box&            box
							  , const Transformation& transformation
							  )
  {
    const RoutingError* error = static_cast<const RoutingError*>(go);
    QPainter& painter = widget->getPainter();
    QColor c = QColor(Qt::red);
    c.setAlpha(100);
    c = DisplayStyle::darken(c,widget->getDarkening());
    painter.setPen  (QPen(c,15));
    painter.setBrush(c);
    painter.drawRect ( widget->dbuToScreenRect(error->getBoundingBox()) );
  }
  






  GraphicSolsticeEngine* GraphicSolsticeEngine::grab ()
  {
    if ( !_references ) 
      _singleton = new GraphicSolsticeEngine ();
    _references++;  
    return _singleton;
  }
  

  const Name& GraphicSolsticeEngine::getName () const
  { return SolsticeEngine::getStaticName(); }




  Cell* GraphicSolsticeEngine::getCell ()
  {
    if ( !_viewer ) {
      throw Error ( "<b>Solstice:</b> GraphicSolsticeEngine not bound to any Viewer." );
      return NULL;
    }

    if ( !_viewer->getCell() ) {
      throw Error ( "<b>Solstice:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }



  size_t GraphicSolsticeEngine::release ()
  {
    _references--;
    if ( !_references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }



  void GraphicSolsticeEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );
    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.extract");
    if ( !prMenu ) {
      throw("Solstice doit être chargé avant Equinox (c'est plus convenable , Non ??");
    }



    QAction* gTestAction = _viewer->findChild<QAction*>("viewer.menuBar.extract.solstice.test");
    if ( gTestAction )
      cerr << Warning("GraphicSolsticeEngine::addToMenu() - Solstice test already hooked in.") << endl;
    else {
      gTestAction = new QAction  ( tr("Solstice - Testing"), _viewer );
      gTestAction->setObjectName ( "viewer.menuBar.extract.test" );
      gTestAction->setStatusTip  ( tr("Run the <b>Solstice</b> Test") );
      gTestAction->setVisible    ( true );
      prMenu->addAction ( gTestAction );

      connect ( gTestAction, SIGNAL(triggered()), this, SLOT(test()) );
    }

    // fin du sous menu

    connect ( this, SIGNAL(cellPreModificated ()), _viewer->getCellWidget(), SLOT(cellPreModificate ()) );
    connect ( this, SIGNAL(cellPostModificated()), _viewer->getCellWidget(), SLOT(cellPostModificate()) );
  }



  void GraphicSolsticeEngine::test ()
  {
    SolsticeEngine* solstice = createEngine ( );
    if ( !solstice ) return;

  //emit cellPreModificated ();

      solstice->compare ();

    //emit cellPostModificated ();
    _viewer->dumpObjectTree();
    QTabWidget* ctrlTab = _viewer->getControllerWidget();
    if (ctrlTab) {
      ctrlTab->addTab ( _tabRoutingErrorList       , "RoutingError"     );
      (static_cast<ControllerTab*>((_tabRoutingErrorList)))->setCellWidget ( _viewer->getCellWidget() );

    PaletteWidget* palette = ctrlTab->findChild<PaletteWidget*>("controller.tabPalette.palette");
    if (palette)
      {
	palette->setItemVisible(RoutingError::_extensionName,true);
      } else {
      throw Warning("l'onglet controller.tabPalette.palette n'existe pas");
    }
      
    }else {
      throw Warning("Le panel des erreurs de s'affichera pas");
    }

  }


  GraphicSolsticeEngine::GraphicSolsticeEngine ()
    : GraphicTool        ()
    , _viewer            (NULL)
    , _tabRoutingErrorList   (new TabRoutingErrorList())
  { 
    _tabRoutingErrorList      ->setObjectName ( "controller.tabRoutingErrorList"    );
    addDrawGo ( RoutingError::_extensionName   , initSolsticeRoutingError   , drawSolsticeRoutingError    );

  }


  GraphicSolsticeEngine::~GraphicSolsticeEngine()
  { 
    // destruction des outils graphiques
    //removeDrawGo ( RoutingError::_extensionName );
    //_tabRoutingErrorList->destroy();

    // destruction de solstice
    cout << "get cell" <<endl;
    Cell* cell = getCell ();
    cout << "get cell (bis)" <<endl;
    SolsticeEngine* solstice = SolsticeEngine::get ( cell );
    cout << "destroy" <<endl;
    if (solstice)
      solstice->destroy();


  }


}  // End of Solstice namespace.
