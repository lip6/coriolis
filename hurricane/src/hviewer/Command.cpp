
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
// |  C++ Module  :       "./Command.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


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


  void  Command::wheelEvent        ( QWheelEvent* ) { }
  void  Command::keyPressEvent     ( QKeyEvent*   ) { }
  void  Command::keyReleaseEvent   ( QKeyEvent*   ) { }
  void  Command::mouseMoveEvent    ( QMouseEvent* ) { }
  void  Command::mousePressEvent   ( QMouseEvent* ) { }
  void  Command::mouseReleaseEvent ( QMouseEvent* ) { }
  void  Command::draw              ()               { }


} // End of Hurricane namespace.
