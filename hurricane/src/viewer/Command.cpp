// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Command.cpp"                            |
// +-----------------------------------------------------------------+


#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/Command.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Command".


  Command::Command ()
    : _cellWidget()
    , _active    (false)
  { }


  Command::~Command ()
  {
    if ( _cellWidget ) _cellWidget->unbindCommand ( this );
  }


  Command::Type  Command::getType () const
  { return Normal; }


  void  Command::setActive ( bool state )
  {
    _active = state;
    _cellWidget->setActiveCommand ( (state and (getType() != AlwaysActive)) ? this : NULL );
  }


  void  Command::setCellWidget     ( CellWidget* widget ) { _cellWidget=widget; }
  void  Command::wheelEvent        ( QWheelEvent* ) { }
  void  Command::keyPressEvent     ( QKeyEvent*   ) { }
  void  Command::keyReleaseEvent   ( QKeyEvent*   ) { }
  void  Command::mouseMoveEvent    ( QMouseEvent* ) { }
  void  Command::mousePressEvent   ( QMouseEvent* ) { }
  void  Command::mouseReleaseEvent ( QMouseEvent* ) { }
  void  Command::draw              ()               { }
  void  Command::reset             ()               { }


} // End of Hurricane namespace.
