
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
// |  C++ Header  :       "./AreaCommand.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_AREA_COMMAND__
#define  __HURRICANE_AREA_COMMAND__

#include  <QPoint>

#include  "hurricane/viewer/Command.h"


namespace Hurricane {


  class AreaCommand : public Command {
    public:
                           AreaCommand          ();
      virtual             ~AreaCommand          ();
      inline  void         setStartPoint        ( const QPoint& start );
      inline  void         setStopPoint         ( const QPoint& stop );
      inline  void         setDrawingEnabled    ( bool state );
      inline  void         setDrawingThreshold  ( int );
      virtual void         draw                 ();
      virtual void         drawCorner           ( bool bottomLeft );
      virtual void         mouseMoveEvent       ( QMouseEvent* event );
      inline  bool         isDrawingEnabled     () const;
      inline  int          getDrawingThreshold  () const;
    protected:
              QPoint       _startPoint;
              QPoint       _stopPoint;
              QPoint       _cornerPoints[3];
              int          _drawingThreshold;
              bool         _drawingEnabled;
    private:
                           AreaCommand          ( const AreaCommand& );
              AreaCommand& operator=            ( const AreaCommand& );
  };


  inline void  AreaCommand::setStartPoint ( const QPoint& start )
  { _startPoint = start; _stopPoint = start; }


  inline void  AreaCommand::setStopPoint ( const QPoint& stop )
  { _stopPoint  = stop; }


  inline void  AreaCommand::setDrawingEnabled ( bool state )
  { _drawingEnabled  = state; }


  inline void  AreaCommand::setDrawingThreshold ( int threshold )
  { _drawingThreshold  = threshold; }


  inline bool  AreaCommand::isDrawingEnabled () const
  { return _drawingEnabled; }


  inline int  AreaCommand::getDrawingThreshold () const
  { return _drawingThreshold; }


}


#endif
