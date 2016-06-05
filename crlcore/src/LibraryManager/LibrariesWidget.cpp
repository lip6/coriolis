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
// |  C++ Module  :  "./LibrariesWidget.cpp"                         |
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
#include "crlcore/LibrariesModel.h"
#include "crlcore/LibrariesWidget.h"
#include "crlcore/LibraryManager.h"


namespace CRL {

  using Hurricane::Graphics;


  LibrariesWidget::LibrariesWidget ( QWidget* parent )
    : QWidget        (parent)
    , _baseModel     (new LibrariesModel(this))
    , _view          (new QTableView(this))
    , _rowHeight     (20)
  {
    setAttribute ( Qt::WA_DeleteOnClose );
    setAttribute ( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy ( Qt::ActionsContextMenu );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

    _view->setShowGrid            ( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionMode       ( QAbstractItemView::SingleSelection );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _baseModel );
    _view->resizeColumnToContents ( 1 );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setStretchLastSection( true );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QLabel* title = new QLabel( "Libraries" );
    title->setFont( Graphics::getFixedFont(QFont::Bold,false,false,+1) );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget( title, 0, Qt::AlignHCenter );
    vLayout->addWidget( _view  );
    setLayout( vLayout );

    connect( _view->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&))
           , this                   , SLOT  (selectCurrent (const QModelIndex&,const QModelIndex&)) );

    resize( 100, 300 );
  }


  void  LibrariesWidget::update ()
  { _baseModel->update(); }


  void  LibrariesWidget::initSelection ()
  { _view->selectRow( 0 ); }


  void  LibrariesWidget::selectCurrent ( const QModelIndex& index, const QModelIndex& )
  {
    if (index.isValid())
      emit selectedLibrary( _baseModel->getLibrary(index) );
  }




}  // CRL namespace.
