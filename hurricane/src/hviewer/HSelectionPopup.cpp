
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Module  :       "./HSelectionPopup.cpp"                    |
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
#include "hurricane/viewer/HSelectionPopupModel.h"
#include "hurricane/viewer/HSelectionPopup.h"


namespace Hurricane {



  HSelectionPopup::HSelectionPopup ( QWidget* parent )
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

    _model = new HSelectionPopupModel ( this );

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


  void  HSelectionPopup::popup ()
  {
    show ();
    grabMouse ();
  }


  void  HSelectionPopup::forceRowHeight ()
  {
    for (  int rows=_model->rowCount()-1; rows >= 0 ; rows-- )
      _view->setRowHeight ( rows, _rowHeight );
  }


  void  HSelectionPopup::keyPressEvent ( QKeyEvent* event )
  {
    event->ignore();
  }



  void  HSelectionPopup::mouseMoveEvent ( QMouseEvent* event )
  {
    QModelIndex index = _view->indexAt ( event->pos() );
    if ( index.isValid() )
      _view->selectionModel()->select ( index, QItemSelectionModel::ClearAndSelect );
    else
      _view->selectionModel()->clearSelection ();
  }



  void  HSelectionPopup::mouseReleaseEvent ( QMouseEvent* event )
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


  void  HSelectionPopup::add ( Occurrence occurrence, bool showChange )
  {
    _model->add ( occurrence, showChange );
  }


  void  HSelectionPopup::clear ()
  {
    _model->clear ();
  }


  void  HSelectionPopup::updateLayout ()
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
