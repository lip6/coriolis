
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


# include <QKeyEvent>
# include <QMouseEvent>
# include <QWheelEvent>

# include <hurricane/viewer/CellWidget.h>
# include <hurricane/viewer/Command.h>


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Command".


  Command::Command ()
    : _cellWidgets()
    , _active(false)
  { }


  Command::~Command ()
  {
    set<CellWidget*>::iterator iwidget = _cellWidgets.begin();
    for ( ; iwidget != _cellWidgets.end() ; iwidget++ )
      (*iwidget)->unbindCommand ( this );
  }


  bool  Command::wheelEvent ( CellWidget*, QWheelEvent* )
  { return false; }


  bool  Command::keyPressEvent ( CellWidget*, QKeyEvent* )
  { return false; }


  bool  Command::keyReleaseEvent ( CellWidget*, QKeyEvent* )
  { return false; }


  bool  Command::mouseMoveEvent ( CellWidget*, QMouseEvent* )
  { return false; }


  bool  Command::mousePressEvent ( CellWidget*, QMouseEvent* )
  { return false; }


  bool  Command::mouseReleaseEvent ( CellWidget*, QMouseEvent* )
  { return false; }


  void  Command::draw ( CellWidget* )
  { }


} // End of Hurricane namespace.
