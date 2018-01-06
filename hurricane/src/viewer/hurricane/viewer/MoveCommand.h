
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
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
      virtual              ~MoveCommand          ();
      virtual const string& getName              () const;
      virtual Type          getType              () const;
      virtual void          keyPressEvent        ( QKeyEvent* );
      virtual void          keyReleaseEvent      ( QKeyEvent* );
      virtual void          mouseMoveEvent       ( QMouseEvent* );
    private:
                            MoveCommand          ( const MoveCommand& );
              MoveCommand&  operator=            ( const MoveCommand& );
    private:
      static string  _name;
             bool    _firstEvent;
             QPoint  _lastPosition;
  };


}


#endif
