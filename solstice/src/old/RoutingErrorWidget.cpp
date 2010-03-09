
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
// |                  H U R R I C A N E                              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingErrorWidget.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFontMetrics>
#include  <QLabel>
#include  <QLineEdit>
#include  <QHeaderView>
#include  <QKeyEvent>
#include  <QGroupBox>
#include  <QVBoxLayout>
#include  <QAction>

#include "hurricane/Commons.h"
#include "hurricane/Net.h"

#include "hurricane/viewer/Graphics.h"
#include "solstice/RoutingErrorModel.h"
#include "solstice/RoutingErrorWidget.h"


namespace {


  using namespace Hurricane;


} // End of anonymous namespace.


namespace Solstice {


  RoutingErrorWidget::RoutingErrorWidget ( QWidget* parent )
    : QWidget       (parent)
    , _cellWidget   (NULL)
    , _cell         (NULL)
    , _baseModel    (new RoutingErrorModel(this))
    , _sortModel    (new QSortFilterProxyModel(this))
    , _view         (new QTableView(this))
    , _rowHeight    (20)
    , _selecteds    ()
    , _forceReselect(false)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy ( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _sortModel->setSourceModel       ( _baseModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 0 );

    _view->setShowGrid(false);
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSortingEnabled(true);
    _view->setModel ( _sortModel );
    _view->horizontalHeader()->setStretchLastSection ( true );

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );

    _filterPatternLineEdit = new QLineEdit(this);
    QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
    filterPatternLabel->setBuddy(_filterPatternLineEdit);

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget(_view                 , 1, 0, 1, 2);
    gLayout->addWidget(filterPatternLabel    , 2, 0);
    gLayout->addWidget(_filterPatternLineEdit, 2, 1);

    setLayout ( gLayout );

    QAction* fitAction = new QAction  ( tr("&Fit to Error"), this );
    fitAction->setShortcut   ( QKeySequence(tr("CTRL+F")) );
    fitAction->setStatusTip  ( tr("Fit the view to the Error's bounding box") );
    addAction ( fitAction );

    connect ( _filterPatternLineEdit , SIGNAL(textChanged(const QString &))
            , this                   , SLOT  (textFilterChanged()) );                       
    connect ( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
            , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
    connect ( _baseModel, SIGNAL(layoutChanged()), this, SLOT(forceRowHeight    ()) );
    connect ( fitAction , SIGNAL(triggered    ()), this, SLOT(fitToRoutingError ()) );

    resize(300, 300);
  }


  void  RoutingErrorWidget::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  RoutingErrorWidget::goTo ( int delta )
  {
    if ( delta == 0 ) return;

    QModelIndex newIndex = _sortModel->index ( _view->currentIndex().row()+delta, 0, QModelIndex() );

    if ( newIndex.isValid() )
      _view->selectRow ( newIndex.row() );
  }


  void  RoutingErrorWidget::updateSelecteds ()
  {
    _forceReselect = true;

    QItemSelection  dummy;
    updateSelecteds ( dummy, dummy );
  }


  void  RoutingErrorWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    if ( _cellWidget ) _cellWidget->openRefreshSession ();
    
      _selecteds.resetAccesses ();
    
      const RoutingError* error;
      QModelIndexList iList = _view->selectionModel()->selectedRows();
      for ( int i=0 ; i<iList.size() ; i++ ) {
	cout << "yuvhchgc" <<flush;
        error = _baseModel->getRoutingError ( _sortModel->mapToSource(iList[i]).row() );
	cout << error->getName();
        if ( error ) {
	  _selecteds.insert ( error );
	} 
      }
      
      if ( _forceReselect ) {
	_selecteds.forceInserteds();
	_forceReselect = false;
      }
  
    SelectedRoutingErrorSet::iterator remove;
    SelectedRoutingErrorSet::iterator isel  = _selecteds.begin ();
    while ( isel != _selecteds.end() ) {
      switch ( isel->getAccesses() ) {
      case 1:  	
	emit routingErrorSelected ( isel->getRoutingError() );
	cout << "EMIT NONE" << flush; break;
      case 64:
	cout << "EMIT SELECT" << flush;
	emit routingErrorSelected ( isel->getRoutingError() );
	break;
      case 0:
	cout << "EMIT UNSELECT" << flush;
	emit routingErrorUnselected ( isel->getRoutingError() );
	remove = isel;
	++isel;
	_selecteds.erase ( remove );
	continue;
      default:
	cerr << Bug("RoutingErrorWidget::updateSelecteds(): invalid code %d"
		    ,isel->getAccesses()) << endl;
      }
      ++isel;
    }
    
    if ( _cellWidget ) _cellWidget->closeRefreshSession ();
  }
  
  
  void  RoutingErrorWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight  ();
  }


  void  RoutingErrorWidget::fitToRoutingError ()
  {
    const RoutingError* routingError = _baseModel->getRoutingError ( _sortModel->mapToSource(_view->currentIndex()).row() );
    if ( routingError ) emit routingErrorFitted ( routingError );
  }



  void  RoutingErrorWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( this, 0, _cellWidget, 0 );
    }
 
    _cellWidget = cw;
    if ( _cellWidget ) {
      setCell ( _cellWidget->getCell() );
      /*TODO*/
      //      connect ( this, SIGNAL(routingErrorFitted(const RoutingError*)), _cellWidget, SLOT(fitToNet (const Net*)) );
    } else

      setCell ( NULL );
  }
 
 
   void  RoutingErrorWidget::setCell ( Cell* cell )
   {
     _cell = cell;
     _view->selectionModel()->clear ();
     _baseModel->setCell ( cell );
      
     string windowTitle = "RoutingError" + getString(cell);
     setWindowTitle ( tr(windowTitle.c_str()) );
 
     int rows = _sortModel->rowCount ();
     for ( rows-- ; rows >= 0 ; rows-- )
       _view->setRowHeight ( rows, _rowHeight );
     _view->selectRow ( 0 );
     _view->resizeColumnToContents ( 0 );
   }






} // End of Solstice namespace.
