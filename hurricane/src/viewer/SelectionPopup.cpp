// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectionPopup.cpp"                     |
// +-----------------------------------------------------------------+


#include <QFontMetrics>
#include <QLabel>
#include <QHeaderView>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>
#include "hurricane/Commons.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/SelectionPopupModel.h"
#include "hurricane/viewer/SelectionPopup.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::SelectionPopup".


  SelectionPopup::SelectionPopup ( QWidget* parent )
    : QWidget(parent)
    , _model(NULL)
    , _view(NULL)
    , _rowHeight(20)
    , _charWidth(15)
  {
    setAttribute    ( Qt::WA_DeleteOnClose );
    setAttribute    ( Qt::WA_QuitOnClose, false );
  //setWindowFlags  ( Qt::Popup );
    setWindowFlags  ( Qt::FramelessWindowHint );
    setWindowOpacity( 0.9 );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;
    _charWidth = QFontMetrics(Graphics::getFixedFont()).averageCharWidth() + 1;
    _model     = new SelectionPopupModel ( this );

    _view = new QTableView ( this );
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

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible( false );

    connect( _model, SIGNAL(layoutChanged()), this, SLOT(forceRowHeight()) );

    resize( Graphics::isHighDpi() ? 1500 : 600, 10 );
  }


  void  SelectionPopup::popup ()
  {
    show ();
    grabMouse ();
  }


  void  SelectionPopup::forceRowHeight ()
  {
    for (  int rows=_model->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  SelectionPopup::keyPressEvent ( QKeyEvent* event )
  {
  //cerr << "SelectionPopup::keyPressEvent()" << endl;
  //QWidget::keyPressEvent ( event );
  }



  void  SelectionPopup::mouseMoveEvent ( QMouseEvent* event )
  {
    QModelIndex index = _view->indexAt ( event->pos() );
    if ( index.isValid() )
      _view->selectionModel()->select ( index, QItemSelectionModel::ClearAndSelect );
    else
      _view->selectionModel()->clearSelection ();
  }



  void  SelectionPopup::mouseReleaseEvent ( QMouseEvent* event )
  {
    releaseMouse ();
    hide ();

    QModelIndex index = _view->indexAt ( event->pos() );
    if ( index.isValid() ) {
      Occurrence occurrence = _model->getOccurrence(index.row());
      if ( occurrence.getEntity() )
        emit selectionToggled ( occurrence );
    }

    clear ();
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
