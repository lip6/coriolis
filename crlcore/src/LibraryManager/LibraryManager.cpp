// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./LibraryManager.cpp"                          |
// +-----------------------------------------------------------------+


#include <iostream>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <QVBoxLayout>
#include <QSplitter>
#include "hurricane/Error.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/LibraryManager.h"
#include "crlcore/LibrariesWidget.h"
#include "crlcore/CellsWidget.h"
#include "crlcore/CellsModel.h"
#include "crlcore/ViewsWidget.h"
#include "crlcore/ViewsModel.h"
#include "crlcore/CellDatas.h"


namespace CRL {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "FrameworkObserver".

  void  FrameworkObserver::notify ( unsigned int flags )
  {
    LibraryManager* manager = getOwner();
    if (flags & (AllianceFramework::AddedLibrary
                |AllianceFramework::RemovedLibrary)) {
      manager->getLibrariesWidget()->update();
    }
  }


// -------------------------------------------------------------------
// Class  :  "LibraryManager".

  LibraryManager::LibraryManager ( QWidget* parent )
    : QWidget           (parent)
    , _frameworkObserver(this)
    , _librariesWidget  (new LibrariesWidget())
    , _cellsWidget      (new CellsWidget())
    , _viewsWidget      (new ViewsWidget())
    , _cellViewer       (NULL)
    , _libPath          (NULL)
  {
    setObjectName ( "libraryManager" );
    setAttribute  ( Qt::WA_QuitOnClose, false );
    setWindowTitle( tr("Library Manager") );

    QSplitter* splitter = new QSplitter();
    splitter->addWidget( _librariesWidget );
    splitter->addWidget( _cellsWidget     );
    splitter->addWidget( _viewsWidget     );
    splitter->setStretchFactor( 0, 1 );
    splitter->setStretchFactor( 1, 2 );
    splitter->setStretchFactor( 2, 0 );

    QFrame* separator = new QFrame ();
    separator->setFrameShape ( QFrame::HLine );
    separator->setFrameShadow( QFrame::Sunken );

    QLabel* libTitle = new QLabel ();
    libTitle->setTextFormat( Qt::RichText );
    libTitle->setText( "<b>Library Path:</b>" );

    _libPath = new QLabel ();
    _libPath->setFont      ( Graphics::getFixedFont() );
    _libPath->setTextFormat( Qt::RichText );
    _libPath->setText( "&lt;undefined&gt;" );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addWidget( splitter, 1, 0 );
    vLayout->addWidget( separator );
    vLayout->addWidget( libTitle, 0, Qt::AlignLeft );
    vLayout->addWidget( _libPath, 0, Qt::AlignLeft );
    setLayout( vLayout );

    QAction* toggleShow = new QAction ( tr("Library Manager"), this );
    toggleShow->setObjectName( "libraryManager.action.hideShow" );
    toggleShow->setShortcut  ( QKeySequence(tr("CTRL+M")) );
    addAction( toggleShow );

    connect( toggleShow, SIGNAL(triggered()), this, SLOT(toggleShow()) );
    connect( _librariesWidget        , SIGNAL(selectedLibrary(const AllianceLibrary*))
           , _cellsWidget            , SLOT(  setLibrary     (const AllianceLibrary*)) );
    connect( _librariesWidget        , SIGNAL(selectedLibrary(const AllianceLibrary*))
           , this                    , SLOT(  setLibrary     (const AllianceLibrary*)) );
    connect( _cellsWidget            , SIGNAL(selectedCell   (CellDatas*))
           , _viewsWidget            , SLOT(  setCellDatas   (CellDatas*)) );
    connect( _viewsWidget->getModel(), SIGNAL(selectedCell   (Cell*,unsigned int))
           , this                    , SLOT(  openCell       (Cell*,unsigned int)) );
    connect( _viewsWidget->getModel(), SIGNAL(loadedCell     ())
           , _cellsWidget            , SLOT(  updateSelected ()) );
    connect( _viewsWidget->getModel(), SIGNAL(updatedLibrary ())
           , _cellsWidget->getModel(), SLOT(  updateLibrary  ()) );

    _librariesWidget->initSelection();

    AllianceFramework::get()->addObserver( &_frameworkObserver );

    resize( Graphics::toHighDpi(750), Graphics::toHighDpi(550) );
  }


  LibraryManager::~LibraryManager ()
  {
    AllianceFramework::get()->removeObserver( &_frameworkObserver );
  }


  void  LibraryManager::toggleShow ()
  { setVisible( not isVisible() ); }


  void  LibraryManager::setLibrary ( const AllianceLibrary* library )
  {
    string s = "    &lt;undefined&gt;";
    if (library)
      s = "    &lt;" + getString(library->getPath()) + "&gt;";
    _libPath->setText( s.c_str() );
  }


  CellViewer* LibraryManager::openCell ( Cell* cell, unsigned int flags )
  {
    if (not _cellViewer) {
      _cellViewer = new CellViewer ();
      cerr << Error( "LibraryManager::openCell(): Not attached to a CellViewer, using default." ) << endl;
    }

    CellViewer* viewer = _cellViewer;

    if (cell) {
      if (flags & CellLoader::NewViewer) {
        viewer = _cellViewer->vcreate();
        viewer->show();
      }
      viewer->setCell( cell );
    } else
      cerr << Error( "LibraryManager::openCell(): NULL <cell> parameter." ) << endl;

    return viewer;
  }


  void  LibraryManager::updateLibrary ( Cell* )
  { _cellsWidget->updateLibrary(); }


}  // CRL namespace.
