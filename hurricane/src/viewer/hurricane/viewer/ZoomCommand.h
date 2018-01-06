
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
// |  C++ Header  :       "./ZoomCommand.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_ZOOM_COMMAND_H__
#define  __HURRICANE_ZOOM_COMMAND_H__

#include  <QPoint>

#include  "hurricane/viewer/AreaCommand.h"


namespace Hurricane {


  class ZoomCommand : public AreaCommand {
    public:
                            ZoomCommand       ();
      virtual              ~ZoomCommand       ();
      virtual const string& getName           () const;
      virtual Type          getType           () const;
      virtual void          wheelEvent        ( QWheelEvent* );
      virtual void          keyPressEvent     ( QKeyEvent* );
      virtual void          mousePressEvent   ( QMouseEvent* );
      virtual void          mouseReleaseEvent ( QMouseEvent* );
    private:              
                            ZoomCommand       ( const ZoomCommand& );
              ZoomCommand&  operator=         ( const ZoomCommand& );
    private:              
      static string  _name;
  };


}


#endif
