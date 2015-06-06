// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellsWidget.cpp"                             |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QAction>
#include <QModelIndex>
#include "hurricane/viewer/Graphics.h"
#include "crlcore/CellsModel.h"
#include "crlcore/CellsWidget.h"
#include "crlcore/LibraryManager.h"


namespace CRL {

  using Hurricane::Graphics;


  CellsWidget::CellsWidget ( QWidget* parent )
    : QWidget        (parent)
    , _baseModel     (new CellsModel(this))
    , _view          (new QTableView(this))
    , _rowHeight     (20)
    , _selected      ()
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

    _view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionMode       ( QAbstractItemView::SingleSelection );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _baseModel );
  //_view->resizeColumnToContents ( 1 );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setResizeMode        ( 0, QHeaderView::Stretch );
    horizontalHeader->setResizeMode        ( 1, QHeaderView::ResizeToContents );
  //horizontalHeader->setStretchLastSection( false );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QLabel* title = new QLabel( "Cells" );
    title->setFont( Graphics::getFixedFont(QFont::Bold,false,false,+1) );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget( title, 0, Qt::AlignHCenter );
    vLayout->addWidget( _view );
    setLayout( vLayout );

    connect( _view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&))
           , this                   , SLOT  (selectCurrent (const QModelIndex&,const QModelIndex&)) );
  }


  void  CellsWidget::setLibrary ( const AllianceLibrary* library )
  {
    _baseModel->setLibrary( library );
    _view->resizeColumnToContents ( 0 );
    _view->selectRow( 0 );

    emit selectCurrent( _view->currentIndex(), _view->currentIndex() );
  }


  void  CellsWidget::updateSelected ()
  { if (_selected.isValid()) _baseModel->emitDataChanged(_selected); }


  void  CellsWidget::selectCurrent ( const QModelIndex& index, const QModelIndex& )
  {
    if (index.isValid()) {
      _selected = index;
      emit selectedCell( _baseModel->getCellDatas(index.row()) );
    }
  }


}  // CRL namespace.
