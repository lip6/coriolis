
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
//#include "hurricane/viewer/NetlistModel.h"
//#include "hurricane/viewer/NetlistWidget.h"
#include "hurricane/viewer/HNetlistModel.h"
#include "hurricane/viewer/HNetlist.h"


namespace Hurricane {



  NetlistWidget::NetlistWidget ( QWidget* parent )
    : QWidget(parent)
    , _cell(NULL)
    , _baseModel(NULL)
    , _sortModel(NULL)
    , _view(NULL)
    , _rowHeight(20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _baseModel = new NetlistModel ( this );

    _sortModel = new QSortFilterProxyModel ( this );
    _sortModel->setSourceModel       ( _baseModel );
    _sortModel->setDynamicSortFilter ( true );
    _sortModel->setFilterKeyColumn   ( 0 );

    _view = new QTableView(this);
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
            , this                   , SLOT(textFilterChanged())
            );                       
    connect ( _view                  , SIGNAL(activated(const QModelIndex&))
            , this                   , SLOT(selectNet(const QModelIndex&))
            );
//  connect ( _view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&))
//          , this                   , SLOT(selectCurrent   (const QModelIndex&,const QModelIndex&)) );
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


  void  NetlistWidget::selectNet ( const QModelIndex& index )
  {
    if ( !index.isValid() ) return;

    const Net* net = _baseModel->getNet ( _sortModel->mapToSource(index).row() );

    if ( net )
      emit netSelected ( net );
  }


  void  NetlistWidget::selectCurrent ( const QModelIndex& current, const QModelIndex& )
  {
    selectNet ( current );
  }


  void  NetlistWidget::keyPressEvent ( QKeyEvent* event )
  {
    if      ( event->key() == Qt::Key_Asterisk ) { selectNet(_view->currentIndex()); }
    else if ( event->key() == Qt::Key_Plus     ) { goTo( 1); selectNet(_view->currentIndex()); }
    else if ( event->key() == Qt::Key_Minus    ) { goTo(-1); selectNet(_view->currentIndex()); }
    else {
      event->ignore();
    }
  }


  void  NetlistWidget::textFilterChanged ()
  {
    _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
    forceRowHeight ();
  }


} // End of Hurricane namespace.
