
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetsetWidget.cpp"                       |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QGridLayout>
#include "hurricane/viewer/NetsetWidget.h"


namespace Hurricane {


  NetsetWidget::NetsetWidget ( QWidget* parent )
    : QWidget       (parent)
    , _cellWidget   (nullptr)
    , _baseModel    (new NetlistModel(this))
    , _view         (new QTableView(this))
    , _rowHeight    (20)
    , _selecteds    ()
    , _forceReselect(false)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy ( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _baseModel );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setDefaultAlignment  ( Qt::AlignHCenter );
    horizontalHeader->setMinimumSectionSize( (Graphics::isHighDpi()) ? 150 : 75 );
    horizontalHeader->setStretchLastSection( true );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget( _view                 , 1, 0, 1, 2 );

    setLayout( gLayout );

    QAction* fitAction = new QAction( tr("&Fit to Net"), this );
    fitAction->setShortcut ( QKeySequence(tr("CTRL+F")) );
    fitAction->setStatusTip( tr("Fit the view to the Net's bounding box") );
    addAction( fitAction );

    connect( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
           , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
    connect( fitAction , SIGNAL(triggered    ()), this, SLOT(fitToNet      ()) );

    resize( 300, 300 );
  }


  void  NetsetWidget::setCellWidget ( CellWidget* cw )
  {
    clear();
    if (_cellWidget) {
      disconnect( this, 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if (_cellWidget) {
      connect ( this, SIGNAL(netFitted(const Net*)), _cellWidget, SLOT(fitToNet (const Net*)) );
    }
  }


  void  NetsetWidget::resizeColumnToContents ( int column )
  { _view->resizeColumnToContents( column ); }


  void  NetsetWidget::updateSelecteds ()
  {
    _forceReselect = true;

    QItemSelection  dummy;
    updateSelecteds( dummy, dummy );
  }


  void  NetsetWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    if (_cellWidget) _cellWidget->openRefreshSession ();

    _selecteds.resetAccesses();

    const Net* net;
    QModelIndexList iList = _view->selectionModel()->selectedRows();
    for ( int i=0 ; i<iList.size() ; i++ ) {
      net = _baseModel->getNet ( iList[i].row() );
      if (net)
        _selecteds.insert( net );
    }

    if (_forceReselect) {
      _selecteds.forceInserteds();
      _forceReselect = false;
    }

    SelectedNetSet::iterator isel = _selecteds.begin();
    while (isel != _selecteds.end()) {
      SelectedNetSet::iterator remove = isel++;
      if (remove->getAccesses() == 0) {
        emit netUnselected ( Occurrence(remove->getNet()) );
        _selecteds.erase( remove );
      }
    }
    isel = _selecteds.begin ();
    while (isel != _selecteds.end()) {
      if (isel->getAccesses() == 64)
        emit netSelected ( Occurrence(isel->getNet()) );
      ++isel;
    }

    if ( _cellWidget) _cellWidget->closeRefreshSession();
  }


  void  NetsetWidget::fitToNet ()
  {
    const Net* net = _baseModel->getNet( _view->currentIndex().row() );
    if (net) emit netFitted ( net );
  }


  void  NetsetWidget::clear ()
  { _baseModel->clear(); }


} // Hurricane namespace.
