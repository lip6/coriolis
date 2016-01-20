// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RulerCommand.cpp"                       |
// +-----------------------------------------------------------------+


# include <QMouseEvent>
# include <QKeyEvent>

# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/RulerCommand.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "RulerCommand".


  string RulerCommand::_name = "RulerCommand";


  RulerCommand::RulerCommand ()
    : Command ()
    , _ruler  ()
    , _drawing(false)
  { }


  RulerCommand::~RulerCommand ()
  { }


  const string& RulerCommand::getName () const
  { return _name; }


  void  RulerCommand::mouseMoveEvent ( QMouseEvent* event )
  {
    if ( not isActive() or not _drawing ) return;

    _ruler->setExtremity ( _cellWidget->_onCursorGrid(_cellWidget->screenToDbuPoint(event->pos())) );
    _cellWidget->update ();
  }


  void  RulerCommand::mousePressEvent ( QMouseEvent* event )
  {
    if ( not isActive() ) return;

    if ( event->button() == Qt::LeftButton ) {
      if ( not _drawing ) {
        _drawing = true;
        _ruler.reset ( new Ruler
                     (_cellWidget->_onCursorGrid(_cellWidget->screenToDbuPoint(_cellWidget->getMousePosition())) ) );
      } else {
        setActive ( false );
        _drawing = false;
        _cellWidget->addRuler ( _ruler );
        _ruler.reset ();
      }
    }
  }


  void  RulerCommand::keyPressEvent ( QKeyEvent* event )
  {
    if ( isActive() and (event->key() == Qt::Key_Escape) ) {
      setActive ( false );
      _drawing = false;
      _ruler.reset ();
      return;
    }

    if ( event->key() != Qt::Key_K ) return;

    if ( event->modifiers() & Qt::ShiftModifier ) {
      _cellWidget->clearRulers ();
      return;
    }

    setActive ( true );
  }


  void  RulerCommand::draw ()
  {
    if ( not _drawing ) return;
    _cellWidget->drawRuler ( _ruler );
  }


} // End of Hurricane namespace.
