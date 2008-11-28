
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Module  :       "./NetlistWidget.cpp"                      |
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

#include "hurricane/Commons.h"
#include "hurricane/Net.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/NetlistModel.h"
#include "hurricane/viewer/NetlistWidget.h"


namespace {


  using namespace Hurricane;


} // End of anonymous namespace.


namespace Hurricane {


  NetlistWidget::NetlistWidget ( QWidget* parent )
    : QWidget   (parent)
    , _cell     (NULL)
    , _baseModel(new NetlistModel(this))
    , _sortModel(new QSortFilterProxyModel(this))
    , _view     (new QTableView(this))
    , _rowHeight(20)
    , _selecteds()
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

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

    connect ( _filterPatternLineEdit , SIGNAL(textChanged(const QString &))
            , this                   , SLOT  (textFilterChanged()) );                       
    connect ( _view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&))
            , this                   , SLOT  (updateSelecteds (const QItemSelection&,const QItemSelection&)) );
    connect ( _baseModel , SIGNAL(layoutChanged()), this, SLOT(forceRowHeight()) );

    resize(300, 300);
  }


  void  NetlistWidget::forceRowHeight ()
  {
    for (  int rows=_sortModel->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  NetlistWidget::goTo ( int delta )
  {
    if ( delta == 0 ) return;

    QModelIndex newIndex = _sortModel->index ( _view->currentIndex().row()+delta, 0, QModelIndex() );

    if ( newIndex.isValid() )
      _view->selectRow ( newIndex.row() );
  }


  void  NetlistWidget::updateSelecteds ( const QItemSelection& , const QItemSelection& )
  {
    const Net* net;

    _selecteds.resetAccesses ();

    QModelIndexList iList = _view->selectionModel()->selectedRows();
    for ( int i=0 ; i<iList.size() ; i++ ) {
      net = _baseModel->getNet ( _sortModel->mapToSource(iList[i]).row() );
      if ( net )
        _selecteds.insert ( net );
    }

    SelectedNetSet::iterator remove;
    SelectedNetSet::iterator isel  = _selecteds.begin ();
    while ( isel != _selecteds.end() ) {
      switch ( isel->getAccesses() ) {
        case 1:  break;
        case 64:
          emit netSelected ( isel->getNet() );
          break;
        case 0:
          emit netUnselected ( isel->getNet() );
          remove = isel;
          ++isel;
          _selecteds.erase ( remove );
          continue;
        default:
          cerr << Bug("NetlistWidget::updateSelecteds(): invalid code %d"
                     ,isel->getAccesses()) << endl;
      }
      ++isel;
    }
  }


  void  NetlistWidget::keyPressEvent ( QKeyEvent* event )
  {
  }


  void  NetlistWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight ();
  }


} // End of Hurricane namespace.
