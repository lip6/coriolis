
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
// |  C++ Module  :       "./MoveCommand.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/MoveCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "MoveCommand".


  MoveCommand::MoveCommand ()
    : Command()
    , _active(false)
    , _lastPosition()
  { }


  MoveCommand::~MoveCommand ()
  { }


  bool  MoveCommand::keyPressEvent ( CellWidget* widget, QKeyEvent* event )
  { 
    switch ( event->key() ) {
      case Qt::Key_Up:    widget->goUp    (); return true;
      case Qt::Key_Down:  widget->goDown  (); return true;
      case Qt::Key_Left:  widget->goLeft  (); return true;
      case Qt::Key_Right: widget->goRight (); return true;
    }
    return false;
  }


  bool  MoveCommand::mouseMoveEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !_active ) return false;

    int  dx = event->x() - _lastPosition.x();
    dx <<= 1;
    if ( dx > 0 ) widget->goLeft  (  dx );
    if ( dx < 0 ) widget->goRight ( -dx );

    int dy = event->y() - _lastPosition.y();
    dy <<= 1;
    if ( dy > 0 ) widget->goUp   (  dy );
    if ( dy < 0 ) widget->goDown ( -dy );

    _lastPosition = event->pos();

    return true;
  }


  bool  MoveCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( _active ) return true;

    if ( ( event->button() == Qt::LeftButton ) && !event->modifiers() ) {
      _active       = true;
      _lastPosition = event->pos();
      widget->pushCursor ( Qt::ClosedHandCursor );
    }

    return _active;
  }


  bool  MoveCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !_active ) return false;

    _active = false;
    widget->popCursor ();

    return false;
  }


} // End of Hurricane namespace.
