
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
// |  C++ Header  :       "./Command.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_COMMAND_H__
#define  __HURRICANE_COMMAND_H__


#include  <set>

using namespace std;


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


namespace Hurricane {


  class CellWidget;


  class Command {
    public:
                                Command              ();
      virtual                  ~Command              ();
      inline  bool              isActive             ();
      inline  void              setActive            ( bool state );
      virtual bool              wheelEvent           ( CellWidget*, QWheelEvent* );
      virtual bool              keyPressEvent        ( CellWidget*, QKeyEvent* );
      virtual bool              keyReleaseEvent      ( CellWidget*, QKeyEvent* );
      virtual bool              mouseMoveEvent       ( CellWidget*, QMouseEvent* );
      virtual bool              mousePressEvent      ( CellWidget*, QMouseEvent* );
      virtual bool              mouseReleaseEvent    ( CellWidget*, QMouseEvent* );
      virtual void              draw                 ( CellWidget* );
      inline  set<CellWidget*>& getCellWidgets       ();
    private:
              set<CellWidget*>  _cellWidgets;
              bool              _active;
    private:
                                Command              ( const Command& );
              Command&          operator=            ( const Command& );
  };


// Inline Functions.
  inline set<CellWidget*>& Command::getCellWidgets () { return _cellWidgets; }
  inline bool              Command::isActive       () { return _active; }
  inline void              Command::setActive      ( bool state ) { _active = state; }


}



#endif

