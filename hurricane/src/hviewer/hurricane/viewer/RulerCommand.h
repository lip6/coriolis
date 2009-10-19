
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
// |  C++ Header  :       "./RulerCommand.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_RULER_COMMAND__
#define  __HURRICANE_RULER_COMMAND__

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
    protected:
      static string                _name;
      std::tr1::shared_ptr<Ruler>  _ruler;
  };


}


#endif
