
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
// |  C++ Module  :       "./SelectionPopup.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QFontMetrics>
#include  <QLabel>
#include  <QHeaderView>
#include  <QKeyEvent>
#include  <QGroupBox>
#include  <QVBoxLayout>

#include "hurricane/Commons.h"

#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/SelectionPopupModel.h"
#include "hurricane/viewer/SelectionPopup.h"


namespace Hurricane {



  SelectionPopup::SelectionPopup ( QWidget* parent )
    : QWidget(parent)
    , _model(NULL)
    , _view(NULL)
    , _rowHeight(20)
  {
    setAttribute     ( Qt::WA_DeleteOnClose );
    setAttribute     ( Qt::WA_QuitOnClose, false );
  //setWindowFlags   ( Qt::Popup );
    setWindowFlags   ( Qt::FramelessWindowHint );
    setWindowOpacity ( 0.7 );

    _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

    _model = new SelectionPopupModel ( this );

    _view = new QTableView ( this );
    _view->setShowGrid ( false );
    _view->setAlternatingRowColors ( true );
    _view->setSelectionBehavior ( QAbstractItemView::SelectRows );
    _view->setModel ( _model );
    _view->horizontalHeader()->setStretchLastSection ( true );
    _view->setWordWrap ( false );
  //_view->setTextElideMode ( Qt::ElideRight );
  //_view->setFixedSize ( QSize(300,100) );
  //_view->setIconSize ( QSize(600,40) );
  //_view->setStyleSheet ( "QTableView { background-color: #555555; }" );

    QHeaderView* horizontalHeader = _view->horizontalHeader ();
    horizontalHeader->setStretchLastSection ( true );
    horizontalHeader->setMinimumSectionSize ( 200 );
    horizontalHeader->setVisible ( false );

    QHeaderView* verticalHeader = _view->verticalHeader ();
    verticalHeader->setVisible ( false );

    connect ( _model, SIGNAL(layoutChanged()), this, SLOT(forceRowHeight()) );

    resize ( 600, 10 );
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
    event->ignore();
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
        emit occurrenceSelected ( occurrence, true );
    }

    clear ();
  }


  void  SelectionPopup::add ( Occurrence occurrence, bool showChange )
  {
    _model->add ( occurrence, showChange );
  }


  void  SelectionPopup::clear ()
  {
    _model->clear ();
  }


  void  SelectionPopup::updateLayout ()
  {
    _model->updateLayout ();

  // This seems a very bad way to set the size of the popup window
  // and underlying QTableView (top-down instead of bottom-up).
    int   rows       = _model->rowCount();
    QSize windowSize = QSize ( 600, _rowHeight*rows + 4 );

    resize ( windowSize );
    _view->resize ( windowSize );
  }


} // End of Hurricane namespace.
