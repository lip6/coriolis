// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SlicingWidget.cpp"                           |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QVBoxLayout>
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellViewer.h"
#include "bora/SlicingPlotWidget.h"
#include "bora/SlicingDataWidget.h"
#include "bora/SlicingWidget.h"


namespace Bora {

  using std::cerr;
  using std::endl;
  using Hurricane::Graphics;


// -------------------------------------------------------------------
// Class  :  "Bora::SlicingTab".


  SlicingTab::SlicingTab ( QWidget* parent )
    : QWidget(parent)
    , _viewer(NULL)
  { }

  void  SlicingTab::setViewer          ( CellViewer* viewer ) { _viewer = viewer; }
  void  SlicingTab::cellPreModificate  () { }
  void  SlicingTab::cellPostModificate () { }


// -------------------------------------------------------------------
// Class  :  "Bora::TabPlot".


  TabPlot::TabPlot ( QWidget* parent )
    : SlicingTab(parent)
    , _plot   (new SlicingPlotWidget())
  {
    _plot->setObjectName( "STree.tabPlot.plot" );

    QVBoxLayout* wLayout = new QVBoxLayout();
    wLayout->setContentsMargins( 0, 0, 0, 0 );
    wLayout->addWidget( _plot );
    setLayout( wLayout );
  }


  void  TabPlot::setViewer ( CellViewer* viewer )
  {
    if (getViewer() != viewer) {
      SlicingTab::setViewer( viewer );
      _plot->setViewer( viewer );
    }
  }


  void TabPlot::setDatas()
  {
    _plot->setDatas();
  }


// -------------------------------------------------------------------
// Class  :  "Bora::TabDatas".


  TabDatas::TabDatas ( QWidget* parent )
    : SlicingTab(parent)
    , _datas  (new SlicingDataWidget())
  {
    _datas->setObjectName( "STree.tabDatas.plot" );

    QVBoxLayout* wLayout  = new QVBoxLayout();
    wLayout->setContentsMargins( 0, 0, 0, 0 );
    wLayout->addWidget( _datas );
    setLayout( wLayout );
  }


  void TabDatas::setViewer ( CellViewer* viewer )
  {
    if (getViewer() != viewer) {
      SlicingTab::setViewer( viewer );
      _datas->setViewer( viewer );
    }
  }


  void TabDatas::updateContents()
  {
    _datas->updateContents();
  }


// -------------------------------------------------------------------
// Class  :  "Bora::SlicingWidget".


  SlicingWidget::SlicingWidget ( QWidget* parent )
    : QTabWidget(parent)
    , _cell     (NULL)
    , _viewer   (NULL)
    , _tabPlot  (new TabPlot())
    , _tabDatas (new TabDatas())
  {
    setObjectName ( "Slicing" );
    setAttribute  ( Qt::WA_QuitOnClose, false );
    setWindowTitle( tr("SlicingTree Viewer") );

    _tabPlot ->setObjectName( "slicing.plot" );
    _tabDatas->setObjectName( "slicing.datas" );

    addTab( _tabPlot , "SlicingTree's Pareto" );
    addTab( _tabDatas, "SlicingTree's possible dimensions" );

    QAction* toggleShow = new QAction ( tr("SlicingTree Viewer"), this );
    toggleShow->setObjectName( "slicing.action.hideShow" );
    toggleShow->setShortcut  ( QKeySequence(tr("CTRL+J")) );
    addAction( toggleShow );

    connect(  toggleShow          , SIGNAL( triggered          ())             , this                 , SLOT  ( toggleShow         ())              );
    connect( _tabPlot->getPlot  (), SIGNAL( updatePlacement    (BoxSet*))      , this                 , SIGNAL( updatePlacement    (BoxSet*))       );
    connect( _tabDatas->getDatas(), SIGNAL( updatePlacement    (BoxSet*))      , this                 , SIGNAL( updatePlacement    (BoxSet*))       );
    connect( _tabDatas->getDatas(), SIGNAL( updateSelectedPoint(double,double)), _tabPlot->getPlot  (), SLOT  ( updateSelectedPoint(double,double)) );
    connect( _tabPlot->getPlot  (), SIGNAL( updateSelectedRow  (double,double)), _tabDatas->getDatas(), SLOT  ( updateSelectedRow  (double,double)) );

    resize( Graphics::isHighDpi() ? QSize(1300,1300) : QSize(870,670) );
  }


  void  SlicingWidget::toggleShow ()
  { 
    setVisible(not isVisible()); 
  }


  void  SlicingWidget::setViewer ( CellViewer* viewer )
  {
    _viewer = viewer;
    if (_viewer) {
      for ( int i=0 ; i<count() ; ++i )
        (static_cast<SlicingTab*>(widget(i)))->setViewer( _viewer );

      connect( _viewer->getCellWidget(), SIGNAL(cellChanged        (Cell*)), this, SLOT(cellChanged       (Cell*)));
      connect( _viewer->getCellWidget(), SIGNAL(cellPreModificated ()     ), this, SLOT(cellPreModificate ()     ));
      connect( _viewer->getCellWidget(), SIGNAL(cellPostModificated()     ), this, SLOT(cellPostModificate()     ));
    }
  }


  void  SlicingWidget::setCell ( Cell* cell )
  {
    _cell = cell;
  }


  void  SlicingWidget::cellChanged ( Cell* cell )
  {
    cerr << "[WARNING] SlicingWidget::cellChanged() does nothing yet." << endl;
  }


  void  SlicingWidget::cellPreModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<SlicingTab*>(widget(i)))->cellPreModificate();
  }


  void  SlicingWidget::cellPostModificate ()
  {
    for ( int i=0 ; i<count() ; ++i )
      (static_cast<SlicingTab*>(widget(i)))->cellPostModificate();
  }


  void SlicingWidget::updateContents()
  {
    _tabPlot->setDatas();
    _tabDatas->updateContents();
  }


}  // Bora namespace.
