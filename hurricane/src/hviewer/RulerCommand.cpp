
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Module  :       "./RulerCommand.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/RulerCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "RulerCommand".


  RulerCommand::RulerCommand ()
    : Command ()
    , _ruler  ()
  { }


  RulerCommand::~RulerCommand ()
  { }


  bool  RulerCommand::keyPressEvent ( CellWidget* widget, QKeyEvent* event )
  { 
    return false;
  }


  bool  RulerCommand::keyReleaseEvent ( CellWidget* widget, QKeyEvent* event )
  { 
    return false;
  }


  bool  RulerCommand::mouseMoveEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( !isActive() ) return false;

    _ruler->setExtremity ( widget->_onSnapGrid(widget->screenToDbuPoint(event->pos())) );
    widget->update ();

    return true;
  }


  bool  RulerCommand::mousePressEvent ( CellWidget* widget, QMouseEvent* event )
  {
    if ( event->modifiers() & Qt::ShiftModifier ) {
      if ( event->button() == Qt::LeftButton ) {
        if ( !isActive() ) {
          setActive ( true );
          _ruler.reset ( new Ruler
                       (widget->_onSnapGrid(widget->screenToDbuPoint(widget->getMousePosition())) ) );
          return true;
        } else {
          widget->addRuler ( _ruler );
          setActive ( false );
          _ruler.reset ();
        }
      } else if ( event->button() == Qt::RightButton ){
        if ( isActive() ) {
          setActive ( false );
          _ruler.reset ();
        }
      }
    }
    return isActive();
  }


  bool  RulerCommand::mouseReleaseEvent ( CellWidget* widget, QMouseEvent* event )
  { return false; }


  void  RulerCommand::draw ( CellWidget* widget )
  {
    if ( !isActive() ) return;
    widget->drawRuler ( _ruler );
  }


} // End of Hurricane namespace.
