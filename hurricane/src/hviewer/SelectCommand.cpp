
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
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SelectCommand.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>
# include <QAction>

# include "hurricane/Cell.h"

# include "hurricane/viewer/CellWidget.h"
# include "hurricane/viewer/SelectCommand.h"
# include "hurricane/viewer/HSelectionPopup.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "SelectCommand".


  SelectCommand::SelectCommand ()
    : AreaCommand()
    , _selectAction(NULL)
    , _selectionPopup(NULL)
  {
    _selectionPopup = new HSelectionPopup ();

    connect ( _selectionPopup, SIGNAL(occurrenceSelected(Occurrence,bool))
            , this           , SIGNAL(selectionToggled  (Occurrence,bool)) );
  }


  SelectCommand::~SelectCommand ()
  {
    delete _selectionPopup;
  }


  void  SelectCommand::bindToAction ( QAction* action )
  {
    _selectAction = action;
  }


  bool  SelectCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( isActive() ) return true;

    if ( event->button() == Qt::RightButton ) {
      if ( !event->modifiers() ) {
        setActive         ( true );
        setStartPoint     ( event->pos() );
        setDrawingEnabled ( true );
      } else if ( event->modifiers() == Qt::ControlModifier ) {
        QRect selectArea ( event->pos() - QPoint(2,2), QSize(4,4) );
        forEach ( Occurrence, ioccurrence
                , widget->getCell()->getOccurrencesUnder(widget->screenToDbuBox(selectArea)) ) {
          _selectionPopup->add ( *ioccurrence );
        }
        _selectionPopup->updateLayout ();
        _selectionPopup->move ( event->globalPos() );
        _selectionPopup->popup ();
      }
    }

    return isActive();
  }


  bool  SelectCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !isActive() ) return false;

    setActive ( false );
    setDrawingEnabled ( false );

    QRect selectArea;
    if ( _startPoint == _stopPoint )
      selectArea = QRect ( _startPoint - QPoint(2,2), QSize(4,4) );
    else
      selectArea = QRect ( _startPoint, _stopPoint );
      
  //widget->unselectAll ();
    widget->selectOccurrencesUnder ( widget->screenToDbuBox(selectArea) );

    if ( _selectAction ) {
      if ( !_selectAction->isChecked() )
        _selectAction->setChecked ( true );
      else
        widget->redraw ();
    } else {
      widget->setShowSelection ( true );
      widget->redraw ();
    }
    
    return false;
  }



} // End of Hurricane namespace.
