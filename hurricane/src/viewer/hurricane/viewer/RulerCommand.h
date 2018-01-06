// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RulerCommand.h"                         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_RULER_COMMAND_H
#define  HURRICANE_RULER_COMMAND_H

#include  <QPoint>

#include  "hurricane/viewer/Command.h"
#include  "hurricane/viewer/Ruler.h"


namespace Hurricane {


  class RulerCommand : public Command {
    public:
                            RulerCommand    ();
      virtual              ~RulerCommand    ();
      virtual const string& getName         () const;
      virtual void          mouseMoveEvent  ( QMouseEvent* );
      virtual void          mousePressEvent ( QMouseEvent* );
      virtual void          keyPressEvent   ( QKeyEvent* );
      virtual void          draw            ();
    private:
                            RulerCommand    ( const RulerCommand& );
              RulerCommand& operator=       ( const RulerCommand& );
    private:
      static string           _name;
      std::shared_ptr<Ruler>  _ruler;
      bool                    _drawing;
  };


}


#endif
