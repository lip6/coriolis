// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K n i k  -  G l o  b a l   R o u t e r                   |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GraphicKnikEngine.cpp"                  |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <hurricane/DebugSession.h>
#include <hurricane/Warning.h>
#include <hurricane/Go.h>
#include <hurricane/Cell.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <crlcore/AllianceFramework.h>
#include <knik/Vertex.h>
#include <knik/Edge.h>
#include <knik/GraphicKnikEngine.h>


namespace Knik {


  using namespace std;

  using Hurricane::Warning;
  using Hurricane::Graphics;
  using CRL::AllianceFramework;
    

// -------------------------------------------------------------------
// Class  :  "CRL::GraphicKnikEngine".


  size_t             GraphicKnikEngine::_references = 0;
  GraphicKnikEngine* GraphicKnikEngine::_singleton  = NULL;


  void GraphicKnikEngine::initKnikEdges ( CellWidget* widget ) 
  {
    widget->getDrawingPlanes().setPen ( Qt::NoPen );
  }


  void GraphicKnikEngine::drawKnikEdges ( CellWidget*           widget
                                        , const Go*             go
                                        , const BasicLayer*     basicLayer
                                        , const Box&            box
                                        , const Transformation& transformation
                                        ) 
  {
    const Edge* edge = dynamic_cast<const Edge*>(go);
    if ( edge ) {
      unsigned int occupancy = 255;
      if ( edge->getRealOccupancy() < edge->getCapacity() )
        occupancy = (unsigned int)( 255.0 * ( (float)edge->getRealOccupancy() / (float)edge->getCapacity() ) );

      QPainter& painter = widget->getPainter();
      if ( edge->getRealOccupancy() > edge->getCapacity() ) {
        QColor color (Qt::cyan);
        painter.setPen (DisplayStyle::darken(color,widget->getDarkening()));
      }
      painter.setBrush ( Graphics::getColorScale(ColorScale::Fire).getBrush(occupancy,widget->getDarkening()) );
      painter.drawRect ( widget->dbuToDisplayRect(edge->getBoundingBox(), false) );
      painter.setPen(Qt::NoPen);

    // affichage des infos de l'arete
      if ( edge->hasInfo() ) {
        QColor color (Qt::blue);
        painter.setPen (DisplayStyle::darken(color,widget->getDarkening()));
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        painter.setFont(font);
      //QString text = QString("%1%2%3").arg(edge->getConnexID()).arg(",").arg(edge->getConstCost());
        QString text = QString("%1").arg(edge->getConnexID());
      //Point fromPos = edge->getFrom()->getPosition();
      //Point   toPos = edge->getTo()->getPosition();
      //painter.drawText ( widget->dbuToDisplayPoint ( (fromPos.getX()+toPos.getX())/2+DbU::lambda(0.5), (fromPos.getY()+toPos.getY())/2+DbU::lambda(0.5) ), text );

        if ( edge->isVertical() ) {
          painter.save();
          Box bbox = edge->getBoundingBox();
          painter.translate (widget->dbuToDisplayPoint(bbox.getXMin(), bbox.getYMin()));
          painter.rotate(-90);
          painter.drawText (QRect(0,0,widget->dbuToDisplayLength(bbox.getHeight()),widget->dbuToDisplayLength(bbox.getWidth())), text, QTextOption(Qt::AlignCenter));
        //painter.drawText (0, 0, text);
          painter.restore();
        } else
          painter.drawText ( widget->dbuToDisplayRect ( edge->getBoundingBox(),false ), text, QTextOption (Qt::AlignCenter) );
        painter.setPen(Qt::NoPen);
      }
    }
  }


  void GraphicKnikEngine::initKnikVertex ( CellWidget* widget ) 
  {
  //widget->getDrawingPlanes().setPen ( Qt::NoPen );
  //widget->getDrawingPlanes().setBrush ( color.darker(widget->getDarkening()) );
  }


  void GraphicKnikEngine::drawKnikVertex ( CellWidget*           widget
                                         , const Go*             go
                                         , const BasicLayer*     basicLayer
                                         , const Box&            box
                                         , const Transformation& transformation
                                         ) 
  {
    const Vertex* vertex = dynamic_cast<const Vertex*>(go);
    if ( vertex ) {
      QPainter& painter = widget->getPainter();
      QColor color = Qt::darkRed;
      QPen pen (DisplayStyle::darken(color,widget->getDarkening()));
      pen.setWidth(2);
      widget->setPen(pen);
      painter.drawEllipse ( widget->dbuToDisplayRect ( vertex->getBoundingBox(), false ) );

      if ( vertex->hasInfo() ) {
        QColor color (Qt::blue);
        painter.setPen (DisplayStyle::darken(color,widget->getDarkening()));
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        painter.setFont(font);
      //QString text = QString("%1%2%3").arg(edge->getConnexID()).arg(",").arg(edge->getConstCost());
        QString text = QString("%1 / %2").arg(vertex->getConnexID()).arg(vertex->getDistance());
      //Point center = vertex->getPosition();
        Box textBox = Box(vertex->getXMin(), vertex->getYMin(), vertex->getXMax(), vertex->getYMax());
        painter.drawText ( widget->dbuToDisplayRect ( textBox,false ), text, QTextOption (Qt::AlignCenter) );
        painter.setPen(Qt::NoPen);
      }
    }
  }


  KnikEngine* GraphicKnikEngine::createEngine ( unsigned int mode ) 
  {
    Cell* cell = getCell ();

    KnikEngine* knik = KnikEngine::get ( cell );
    if ( !knik ) {
      if ( cell->getRubbers().getFirst() == NULL )
        cell->flattenNets ( ((mode==BuildSolution)?Cell::BuildRings:0) );
      knik = KnikEngine::create ( cell
                                , _congestion
                                , _preCongestion
                                , _benchMode
                                , _useSegments
                                , _edgeCost
                                );
      if ( mode == LoadSolution )
        knik->createRoutingGraph ();
    }
  //else
  //  cerr << Warning("%s already has a Knik engine.",getString(cell).c_str()) << endl;

    return knik;
  }


  void GraphicKnikEngine::flattenNets () 
  {
    Cell* cell = getCell ();

  //emit cellPreModificated();

    cell->flattenNets ( Cell::BuildRings );

  //emit cellPostModificated();
  }


  void GraphicKnikEngine::run ()
  {
    KnikEngine* knik = createEngine ( BuildSolution );
    if ( !knik ) return;

  //emit cellPreModificated ();
    
    map<Name,Net*> excludedNets;
    knik->run ( excludedNets );

  //emit cellPostModificated ();
  }


  void GraphicKnikEngine::route ()
  {
    KnikEngine* knik = createEngine ( BuildSolution );
    if ( !knik ) return;

  //emit cellPreModificated ();

    map<Name,Net*> excludedNets;
    knik->Route ( excludedNets );

  //emit cellPostModificated ();
  }


  bool GraphicKnikEngine::analyse ()
  {
    Cell* cell = getCell ();

  //emit cellPreModificated();

    bool        done = false;
    KnikEngine* knik = KnikEngine::get ( cell );
    if ( knik ) {
      done = knik->analyseRouting();
    }

  //emit cellPostModificated();
    return done;
  }


  void GraphicKnikEngine::unroute () 
  {
    Cell* cell = getCell ();

  //emit cellPreModificated();

    KnikEngine* knik = KnikEngine::get ( cell );
    if ( knik ) knik->unrouteOvSegments();

  //emit cellPostModificated();
  }


  void GraphicKnikEngine::reroute () 
  {
    Cell* cell = getCell ();

  //emit cellPreModificated();

    KnikEngine* knik = KnikEngine::get ( cell );
    if ( knik ) knik->reroute();

  //emit cellPostModificated();
  }


  void GraphicKnikEngine::saveSolution () 
  {
    Cell* cell = getCell ();

  //emit cellPreModificated();

    KnikEngine* knik = KnikEngine::get ( cell );
    if ( knik ) knik->saveSolution();

  //emit cellPostModificated();
  }


  void GraphicKnikEngine::loadSolution () 
  {
    KnikEngine* knik = createEngine ( LoadSolution );
    if ( !knik ) return;

  //emit cellPreModificated();

    knik->loadSolution();

  //emit cellPostModificated();
  }


  void GraphicKnikEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );
    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute");
    QMenu* stepMenu = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute.stepByStep");
    if ( !prMenu ) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if ( !menuBar ) {
        cerr << Warning("GraphicKnikEngine::addToMenu() - No MenuBar in parent widget.") << endl;
        return;
      }
      prMenu = menuBar->addMenu ( tr("P&&R") );
      prMenu->setObjectName ( "viewer.menuBar.placeAndRoute" );

      stepMenu = prMenu->addMenu ( tr("&Step by Step") );
      stepMenu->setObjectName ( "viewer.menuBar.placeAndRoute.stepByStep" );

      prMenu->addSeparator ();
    }

    QAction* gRunAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.globalRoute");
    if ( gRunAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in.") << endl;
    else {
      gRunAction = new QAction  ( tr("Knik - &Global Route"), _viewer );
      gRunAction->setObjectName ( "viewer.menuBar.placeAndRoute.globalRoute" );
      gRunAction->setStatusTip  ( tr("Run the <b>Knik</b> global router") );
      gRunAction->setVisible    ( true );
      prMenu->addAction ( gRunAction );

      connect ( gRunAction, SIGNAL(triggered()), this, SLOT(run()) );
    }

    QAction* gFlattenAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.flattenNets");
    if ( gFlattenAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in.") << endl;
    else {
      gFlattenAction = new QAction  ( tr("Knik - &Prepare nets"), _viewer );
      gFlattenAction->setObjectName ( "viewer.menuBar.placeAndRoute.flattenNets" );
      gFlattenAction->setStatusTip  ( tr("Prepare nets for routing") );
      gFlattenAction->setVisible    ( true );
      stepMenu->addAction ( gFlattenAction );

      connect ( gFlattenAction, SIGNAL(triggered()), this, SLOT(flattenNets()) );
    }

    QAction* gRouteAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.simpleGlobalRoute");
    if ( gRouteAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in.") << endl;
    else {
      gRouteAction = new QAction  ( tr("Knik - &Simple Global Route (no ripup)"), _viewer );
      gRouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.simpleGlobalRoute" );
      gRouteAction->setStatusTip  ( tr("Run the <b>Knik</b> global router") );
      gRouteAction->setVisible    ( true );
      stepMenu->addAction ( gRouteAction );

      connect ( gRouteAction, SIGNAL(triggered()), this, SLOT(route()) );
    }

    QAction* gAnalyseAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.analyse");
    if ( gAnalyseAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in (analyse function).") << endl;
    else {
      gAnalyseAction = new QAction  ( tr("Knik - &Analyse routing"), _viewer );
      gAnalyseAction->setObjectName ( "viewer.menuBar.placeAndRoute.analyse" );
      gAnalyseAction->setStatusTip  ( tr("Analyse routing done by <b>Knik</b>") );
      gAnalyseAction->setVisible    ( true );
      stepMenu->addAction ( gAnalyseAction );

      connect ( gAnalyseAction, SIGNAL(triggered()), this, SLOT(analyse()) );
    }

    QAction* gUnrouteAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.unroute");
    if ( gUnrouteAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in (unroute function).") << endl;
    else {
      gUnrouteAction = new QAction  ( tr("Knik - &Unroute overflowed segments"), _viewer );
      gUnrouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.unroute" );
      gUnrouteAction->setStatusTip  ( tr("Unroute overflowed segments") );
      gUnrouteAction->setVisible    ( true );
      stepMenu->addAction ( gUnrouteAction );

      connect ( gUnrouteAction, SIGNAL(triggered()), this, SLOT(unroute()) );
    }

    QAction* gRerouteAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.reroute");
    if ( gRerouteAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in (reroute function).") << endl;
    else {
      gRerouteAction = new QAction  ( tr("Knik - &Reroute"), _viewer );
      gRerouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.reroute" );
      gRerouteAction->setStatusTip  ( tr("Reroute") );
      gRerouteAction->setVisible    ( true );
      stepMenu->addAction ( gRerouteAction );

      connect ( gRerouteAction, SIGNAL(triggered()), this, SLOT(reroute()) );
    }

    QAction* gSaveAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.saveSolution");
    if ( gSaveAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in (save function).") << endl;
    else {
      gSaveAction = new QAction  ( tr("Knik - &Save global routing"), _viewer );
      gSaveAction->setObjectName ( "viewer.menuBar.placeAndRoute.saveSolution" );
      gSaveAction->setStatusTip  ( tr("Save global routing (.kgr file)") );
      gSaveAction->setVisible    ( true );
      stepMenu->addAction ( gSaveAction );

      connect ( gSaveAction, SIGNAL(triggered()), this, SLOT(saveSolution()) );
    }

    QAction* gLoadAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.loadSolution");
    if ( gLoadAction )
      cerr << Warning("GraphicKnikEngine::addToMenu() - Knik global router already hooked in (load function).") << endl;
    else {
      gLoadAction = new QAction  ( tr("Knik - &Load global routing"), _viewer );
      gLoadAction->setObjectName ( "viewer.menuBar.placeAndRoute.loadSolution" );
      gLoadAction->setStatusTip  ( tr("Load global routing (.kgr file)") );
      gLoadAction->setVisible    ( true );
      stepMenu->addAction ( gLoadAction );

      connect ( gLoadAction, SIGNAL(triggered()), this, SLOT(loadSolution()) );
    }
    // fin du sous menu
  }


  const Name& GraphicKnikEngine::getName () const
  { return KnikEngine::staticGetName (); }


  Cell* GraphicKnikEngine::getCell ()
  {
    if ( !_viewer ) {
      throw Error ( "<b>Knik:</b> GraphicKnikEngine not bound to any Viewer." );
      return NULL;
    }

    if ( !_viewer->getCell() ) {
      throw Error ( "<b>Knik:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicKnikEngine* GraphicKnikEngine::grab ()
  {
    if ( !_references ) 
      _singleton = new GraphicKnikEngine ();
    _references++;

    return _singleton;
  }


  size_t GraphicKnikEngine::release ()
  {
    _references--;
    if ( !_references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicKnikEngine::GraphicKnikEngine ()
    : GraphicTool   ()
    , _viewer       (NULL)
    , _congestion   (1)
    , _preCongestion(2)
    , _benchMode    (false)
    , _useSegments  (true)
    , _edgeCost     (2.5)
  {
    addDrawGo ( "Knik::Edges",  initKnikEdges,  drawKnikEdges );
    addDrawGo ( "Knik::Vertex", initKnikVertex, drawKnikVertex );
  }


  GraphicKnikEngine::~GraphicKnikEngine()
  { }


}  // End of Knik namespace.
