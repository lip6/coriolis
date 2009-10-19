
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


  string RulerCommand::_name = "RulerCommand";


  RulerCommand::RulerCommand ()
    : Command()
    , _ruler ()
  { }


  RulerCommand::~RulerCommand ()
  { }


  const string& RulerCommand::getName () const
  { return _name; }


  void  RulerCommand::mouseMoveEvent ( QMouseEvent* event )
  {
    if ( !isActive() ) return;

    _ruler->setExtremity ( _cellWidget->_onSnapGrid(_cellWidget->screenToDbuPoint(event->pos())) );
    _cellWidget->update ();
  }


  void  RulerCommand::mousePressEvent ( QMouseEvent* event )
  {
    if (   (event->modifiers() &  Qt::ShiftModifier )
       and (event->button()    == Qt::LeftButton    ) ) {
      setActive ( true );
      _ruler.reset ( new Ruler
                   (_cellWidget->_onSnapGrid(_cellWidget->screenToDbuPoint(_cellWidget->getMousePosition())) ) );
      return;
    }

    if ( isActive() ) {
      if ( event->button() != Qt::RightButton )
        _cellWidget->addRuler ( _ruler );

      setActive ( false );
      _ruler.reset ();
    }
  }


  void  RulerCommand::keyPressEvent ( QKeyEvent* event )
  {
    if ( !isActive() ) return;

    if ( event->key() == Qt::Key_Escape ) {
      setActive ( false );
      _ruler.reset ();
    }
  }


  void  RulerCommand::draw ()
  {
    if ( !isActive() ) return;
    _cellWidget->drawRuler ( _ruler );
  }


} // End of Hurricane namespace.
