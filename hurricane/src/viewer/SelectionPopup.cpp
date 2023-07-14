// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectionPopup.cpp"                     |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QCursor>
#include <QLabel>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>
#include "hurricane/Commons.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/SelectionPopupModel.h"
#include "hurricane/viewer/SelectionPopup.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectionPopup".


  SelectionPopup::SelectionPopup ( QWidget* parent )
    : QWidget(parent)
    , _cellWidget(NULL)
    , _model(NULL)
    , _view(NULL)
    , _rowHeight(20)
    , _charWidth(15)
  {
    setAttribute    ( Qt::WA_DeleteOnClose, false );
    setAttribute    ( Qt::WA_QuitOnClose  , false );
    setWindowFlags  ( Qt::Popup|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Dialog );
    setWindowOpacity( 0.9 );
    setMouseTracking( true );
    setFocusPolicy  ( Qt::StrongFocus );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 2;
    _charWidth = QFontMetrics( Graphics::getFixedFont() ).averageCharWidth() + 1;
    _model     = new SelectionPopupModel ( this );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

    _view = new QTableView ();
    _view->setShowGrid( false );
    _view->setAlternatingRowColors( true );
    _view->setSelectionBehavior( QAbstractItemView::SelectRows );
    _view->setModel( _model );
    _view->horizontalHeader()->setStretchLastSection( true );
    _view->setWordWrap( false );
  //_view->setTextElideMode ( Qt::ElideRight );
  //_view->setFixedSize ( QSize(300,100) );
  //_view->setIconSize ( QSize(600,40) );
  //_view->setStyleSheet ( "QTableView { background-color: #555555; }" );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setStretchLastSection( true );
    horizontalHeader->setMinimumSectionSize( (Graphics::isHighDpi()) ? 1500 : 200 );
    horizontalHeader->setVisible( false );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    verticalHeader->setResizeMode( QHeaderView::Fixed );
#else
    verticalHeader->setSectionResizeMode( QHeaderView::Fixed );
#endif
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QVBoxLayout* layout = new QVBoxLayout ();
    layout->addWidget( _view );
    setLayout( layout );

    // if (Graphics::isHighDpi()) resize( 1500, 40 );
    // else                       resize(  600, 20 );
  }


  void  SelectionPopup::showPopup ()
  {
    show();
    grabMouse( Qt::PointingHandCursor );
    grabKeyboard();
    setFocus();
  }


  void  SelectionPopup::keyPressEvent ( QKeyEvent* event )
  {
    if (event->key() == Qt::Key_Escape) {
      releaseMouse();
      releaseKeyboard();
      hide();
    } else if (event->key() == Qt::Key_Space) {
      if (_view->selectionModel()->hasSelection()) {
        QModelIndex index      = _view->selectionModel()->selectedRows().first();
        Occurrence  occurrence = _model->getOccurrence(index.row());
        if (occurrence.getEntity()) {
          if (_cellWidget) _cellWidget->setShowSelection( true );
          emit selectionToggled( occurrence );
        }
      }
      event->accept();
    }
  }


  void  SelectionPopup::mouseMoveEvent ( QMouseEvent* event )
  {
    QModelIndex index = _view->indexAt ( event->pos() );
    if ( index.isValid() ) {
      _view->selectionModel()->select ( index, QItemSelectionModel::ClearAndSelect );
    } else {
      _view->selectionModel()->clearSelection ();
    }
  }


  void  SelectionPopup::mousePressEvent ( QMouseEvent* event )
  {
    if (_view->selectionModel()->hasSelection()) {
      QModelIndex index      = _view->selectionModel()->selectedRows().first();
      Occurrence  occurrence = _model->getOccurrence(index.row());
      if (occurrence.getEntity()) {
        if (_cellWidget) _cellWidget->setShowSelection( true );
        emit selectionToggled( occurrence );
      }
    }
    event->accept();
  }


  void  SelectionPopup::loadOccurrences ( Occurrences occurrences, bool showChange )
  { _model->loadOccurrences ( occurrences, showChange ); }


  void  SelectionPopup::clear ()
  { _model->clear (); }


  void  SelectionPopup::updateLayout ()
  {
    _model->updateLayout ();

  // This seems a very bad way to set the size of the popup window
  // and underlying QTableView (top-down instead of bottom-up).
    int   pixelHeight = _model->rowCount()  * _rowHeight + 4;
    int   pixelWidth  = _model->charWidth() * _charWidth;
    QSize windowSize  = QSize( pixelWidth, pixelHeight );

    resize( windowSize );
    _view->resize( windowSize );
  }


  void  SelectionPopup::clearFilter ()
  { if (_model) _model->clearFilter(); }


  void  SelectionPopup::setFilter ( OccurrenceFilter filter )
  { if (_model) _model->setFilter(filter); }


} // End of Hurricane namespace.
