
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
// |  C++ Header  :       "./RoutingErrorListWidget.cpp"             |
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


#include <hurricane/Commons.h>

#include <crlcore/Utilities.h>


#include <solstice/RoutingError.h>
#include <hurricane/viewer/Graphics.h>
#include <solstice/RoutingErrorListModel.h>
#include <solstice/RoutingErrorListWidget.h>




namespace Solstice {


  RoutingErrorListWidget::RoutingErrorListWidget ( QWidget* parent )
    : QWidget       (parent)
    , _cellWidget   (NULL)
    , _cell         (NULL)
    , _baseModel    (new RoutingErrorListModel(this))
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



    QAction* fitErrorAction = new QAction  ( tr("&Fit to Error"), this );
    fitErrorAction->setShortcut   ( QKeySequence(tr("CTRL+F")) );
    fitErrorAction->setStatusTip  ( tr("Fit the view to the Routing Error bounding box") );
    addAction ( fitErrorAction );

    connect ( _filterPatternLineEdit , SIGNAL(textChanged(const QString &))
            , this                   , SLOT  (textFilterChanged()) );                       
    connect ( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
            , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
    connect ( _baseModel, SIGNAL(layoutChanged()), this, SLOT(forceRowHeight     ()) );

    connect ( fitErrorAction , SIGNAL(triggered    ()), this, SLOT(fitToError      ()) );
    

    resize(300, 300);
  }

  void  RoutingErrorListWidget::fitToError ()
  {
    const RoutingError* routingError = _baseModel->getRoutingError ( _sortModel->mapToSource(_view->currentIndex()).row() );

    if ( routingError )
      {
	_cellWidget->reframe(routingError->getBoundingBox().getInflated(DbU::lambda(30)),true);
      }
  }

  void  RoutingErrorListWidget::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  RoutingErrorListWidget::goTo ( int delta )
  {
    if ( delta == 0 ) return;

    QModelIndex newIndex = _sortModel->index ( _view->currentIndex().row()+delta, 0, QModelIndex() );

    if ( newIndex.isValid() )
      _view->selectRow ( newIndex.row() );
  }


  void  RoutingErrorListWidget::updateSelecteds ()
  {
    _forceReselect = true;

    QItemSelection  dummy;
    updateSelecteds ( dummy, dummy );
  }


  void  RoutingErrorListWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    if ( _cellWidget ) _cellWidget->openRefreshSession ();

    _selecteds.resetAccesses ();

    const RoutingError* routingError;
    QModelIndexList iList = _view->selectionModel()->selectedRows();
    for ( int i=0 ; i<iList.size() ; i++ ) {

      routingError = _baseModel->getRoutingError ( _sortModel->mapToSource(iList[i]).row() );
      if ( routingError ) {

        _selecteds.insert ( routingError );
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
      case 1:  break;
      case 64:
	for(set<Occurrence>::iterator i = isel->getRoutingError()->getAllOccurrences()->begin();i != isel->getRoutingError()->getAllOccurrences()->end();i++)
	  {
	    emit occurrenceSelected   ( *i );
	  }
	break;
      case 0:
	for(set<Occurrence>::iterator i = isel->getRoutingError()->getAllOccurrences()->begin();i != isel->getRoutingError()->getAllOccurrences()->end();i++)
	  {
	    emit occurrenceUnselected   ( *i );
	  }
	remove = isel;
	++isel;
	_selecteds.erase   ( remove );
	continue;
      default:
	cerr << Bug("RoutingErrorListWidget::updateSelecteds(): invalid code %d"
		    ,isel->getAccesses()) << endl;
      }
      ++isel;
    }
    
    if ( _cellWidget ) _cellWidget->closeRefreshSession ();
    _cellWidget->refresh();
  }


  void  RoutingErrorListWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight  ();
  }



} // End of Hurricane namespace.
