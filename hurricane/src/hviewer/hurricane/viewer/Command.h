
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


#include  <string>
#include  <map>


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


namespace Hurricane {

  using std::map;
  using std::string;

  class CellWidget;


  class Command {
    public:
      enum  Type  { Normal=1, AlwaysActive=2 };
    public:
                            Command           ();
      virtual              ~Command           ();
      virtual const string& getName           () const = 0;
      virtual Type          getType           () const;
      inline  bool          isActive          () const;
              void          setActive         ( bool state );
      inline  void          setCellWidget     ( CellWidget* );
      virtual void          wheelEvent        ( QWheelEvent* );
      virtual void          keyPressEvent     ( QKeyEvent* );
      virtual void          keyReleaseEvent   ( QKeyEvent* );
      virtual void          mouseMoveEvent    ( QMouseEvent* );
      virtual void          mousePressEvent   ( QMouseEvent* );
      virtual void          mouseReleaseEvent ( QMouseEvent* );
      virtual void          draw              ();
    private:
                            Command           ( const Command& );
              Command&      operator=         ( const Command& );
    protected:
      CellWidget* _cellWidget;
      bool        _active;
  };


// Inline Functions.
  bool  Command::isActive      () const { return _active; }
  void  Command::setCellWidget ( CellWidget* widget ) { _cellWidget=widget; }


}



#endif

