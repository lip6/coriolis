
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
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


  string MoveCommand::_name = "MoveCommand";


  MoveCommand::MoveCommand ()
    : Command      ()
    , _firstEvent  (true)
    , _lastPosition()
  { }


  MoveCommand::~MoveCommand ()
  { }


  const string& MoveCommand::getName () const
  { return _name; }


  Command::Type  MoveCommand::getType () const
  { return AlwaysActive; }


  void  MoveCommand::keyPressEvent ( QKeyEvent* event )
  { 
    if ( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) )
      return;

    switch ( event->key() ) {
      case Qt::Key_Up:    _cellWidget->goUp    (); return;
      case Qt::Key_Down:  _cellWidget->goDown  (); return;
      case Qt::Key_Left:  _cellWidget->goLeft  (); return;
      case Qt::Key_Right: _cellWidget->goRight (); return;
      case Qt::Key_Space:
        if ( !isActive() ) {
          setActive ( true );
          _firstEvent = true;
        //_lastPosition = _cellWidget->getMousePosition();
          _cellWidget->pushCursor ( Qt::ClosedHandCursor );
          return;
        }
    }
  }


  void  MoveCommand::keyReleaseEvent ( QKeyEvent* event )
  { 
    switch ( event->key() ) {
      case Qt::Key_Space:
        if ( isActive() && !event->isAutoRepeat() ) {
          setActive ( false );
          _cellWidget->popCursor ();
          return;
        }
        break;
    }
  }


  void  MoveCommand::mouseMoveEvent ( QMouseEvent* event )
  {
    if ( !isActive() ) return;

    QPoint eventPosition = event->pos();
    if ( _firstEvent ) { _firstEvent = false; _lastPosition = eventPosition; }

    int dx = eventPosition.x() - _lastPosition.x();
    int dy = eventPosition.y() - _lastPosition.y();
    int rx = dx%2;
    int ry = dy%2;
    _lastPosition = QPoint ( eventPosition.x() - rx, eventPosition.y() - ry );
    dx -= rx;
    dy -= ry;

    if ( dx > 0 ) _cellWidget->goLeft  (  dx );
    if ( dx < 0 ) _cellWidget->goRight ( -dx );
    if ( dy > 0 ) _cellWidget->goUp    (  dy );
    if ( dy < 0 ) _cellWidget->goDown  ( -dy );
  }


} // End of Hurricane namespace.
