// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2017, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicTutorialEngine.cpp"              |
// +-----------------------------------------------------------------+


#include <boost/bind.hpp>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <hurricane/Warning.h>
#include <hurricane/Error.h>
#include <hurricane/Breakpoint.h>
#include <hurricane/DebugSession.h>
#include <hurricane/Go.h>
#include <hurricane/Net.h>
#include <hurricane/Cell.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/viewer/ControllerWidget.h>
#include <hurricane/viewer/ExceptionWidget.h>
#include <crlcore/Utilities.h>
#include <crlcore/AllianceFramework.h>
#include <tutorial/GraphicTutorialEngine.h>


namespace Tutorial {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Exception;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::Point;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ControllerWidget;
  using Hurricane::ExceptionWidget;
  using CRL::Catalog;
  using CRL::AllianceFramework;


  size_t                 GraphicTutorialEngine::_references = 0;
  GraphicTutorialEngine* GraphicTutorialEngine::_singleton  = NULL;


  TutorialEngine* GraphicTutorialEngine::createEngine ()
  {
    Cell* cell = getCell ();

    TutorialEngine* tutorial = TutorialEngine::get( cell );
    if (not tutorial) {
      tutorial = TutorialEngine::create( cell );
      tutorial->setViewer( _viewer );
    } else
      cerr << Warning( "%s already has a Tutorial engine.", getString(cell).c_str() ) << endl;

    return tutorial;
  }


  TutorialEngine* GraphicTutorialEngine::getForFramework ()
  {
  // Currently, only one framework is avalaible: Alliance.

    TutorialEngine* tutorial = TutorialEngine::get( getCell() );
    if (tutorial) return tutorial;

    tutorial = createEngine();
    if (not tutorial) 
      throw Error( "Failed to create Tutorial engine on %s.", getString(getCell()).c_str() );

    return tutorial;
  }


  void  GraphicTutorialEngine::_runDemoPart1 ()
  {
    TutorialEngine* tutorial = getForFramework();
    Cell* demo = tutorial->runDemoPart1();

    _viewer->setCell( demo );
  }


  void  GraphicTutorialEngine::_runDemoPart2 ()
  {
    TutorialEngine* tutorial = getForFramework();
    tutorial->runDemoPart2();
  }


  void  GraphicTutorialEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("tutorials.runDemoPart1")) {
      cerr << Warning( "GraphicTutorialEngine::addToMenu() - Tutorial tool already hooked in." ) << endl;
      return;
    }

    _viewer->addMenu  ( "tutorials", "Tutorials", CellViewer::TopMenu );
    _viewer->addToMenu( "tutorials.runDemoPart1"
                      , "Tutorial - &Run Demo, part 1"
                      , "Launch the Tutorial::runDemo() method."
                      , std::bind(&GraphicTutorialEngine::_runDemoPart1,this)
                      );
    _viewer->addToMenu( "tutorials.runDemoPart2"
                      , "Tutorial - &Run Demo, part 2"
                      , "Launch the Tutorial::runDemo() method."
                      , std::bind(&GraphicTutorialEngine::_runDemoPart2,this)
                      );
  }


  const Name& GraphicTutorialEngine::getName () const
  { return TutorialEngine::staticGetName(); }


  Cell* GraphicTutorialEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Tutorial:</b> GraphicTutorialEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Tutorial:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicTutorialEngine* GraphicTutorialEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicTutorialEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicTutorialEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicTutorialEngine::GraphicTutorialEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  { }


  GraphicTutorialEngine::~GraphicTutorialEngine ()
  { }


}  // Tutorial namespace.
