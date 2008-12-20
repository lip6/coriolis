
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
// |  C++ Header  :       "./MoveCommand.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_MOVE_COMMAND_H__
#define  __HURRICANE_MOVE_COMMAND_H__

#include  <QPoint>

#include  "hurricane/viewer/Command.h"


namespace Hurricane {


  class MoveCommand : public Command {
    public:
                                MoveCommand          ();
      virtual                  ~MoveCommand          ();
      virtual bool              keyPressEvent        ( CellWidget*, QKeyEvent* );
      virtual bool              keyReleaseEvent      ( CellWidget*, QKeyEvent* );
      virtual bool              mouseMoveEvent       ( CellWidget*, QMouseEvent* );
      virtual bool              mousePressEvent      ( CellWidget*, QMouseEvent* );
      virtual bool              mouseReleaseEvent    ( CellWidget*, QMouseEvent* );
    protected:
              bool              _active;
              bool              _firstEvent;
              QPoint            _lastPosition;
    private:
                                MoveCommand          ( const MoveCommand& );
              MoveCommand&      operator=            ( const MoveCommand& );
  };


}


#endif
