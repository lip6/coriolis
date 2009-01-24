
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
    : Command      ()
    , _active      (false)
    , _firstEvent  (true)
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
      case Qt::Key_Space:
        if ( !_active ) {
          _active       = true;
          _firstEvent   = true;
        //_lastPosition = widget->getMousePosition();
          widget->pushCursor ( Qt::ClosedHandCursor );
          return true;
        }
    }
    return false;
  }


  bool  MoveCommand::keyReleaseEvent ( CellWidget* widget, QKeyEvent* event )
  { 
    switch ( event->key() ) {
      case Qt::Key_Space:
        if ( _active && !event->isAutoRepeat() ) {
          _active = false;
          widget->popCursor ();
          return true;
        }
        break;
    }
    return false;
  }


  bool  MoveCommand::mouseMoveEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !_active ) return false;

    QPoint eventPosition = event->pos();
    if ( _firstEvent ) { _firstEvent = false; _lastPosition = eventPosition; }

    int dx = eventPosition.x() - _lastPosition.x();
    int dy = eventPosition.y() - _lastPosition.y();
    int rx = dx%2;
    int ry = dy%2;
    _lastPosition = QPoint ( eventPosition.x() - rx, eventPosition.y() - ry );
    dx -= rx;
    dy -= ry;

    if ( dx > 0 ) widget->goLeft  (  dx );
    if ( dx < 0 ) widget->goRight ( -dx );
    if ( dy > 0 ) widget->goUp    (  dy );
    if ( dy < 0 ) widget->goDown  ( -dy );

    return true;
  }


  bool  MoveCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    return _active;
  }


  bool  MoveCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  {
    return _active;
  }


} // End of Hurricane namespace.
