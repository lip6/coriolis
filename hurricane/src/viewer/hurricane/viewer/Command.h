// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/Command.h"                  |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_COMMAND_H
#define  HURRICANE_COMMAND_H

#include <string>
#include <map>

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
      virtual void          setCellWidget     ( CellWidget* );
      virtual void          wheelEvent        ( QWheelEvent* );
      virtual void          keyPressEvent     ( QKeyEvent* );
      virtual void          keyReleaseEvent   ( QKeyEvent* );
      virtual void          mouseMoveEvent    ( QMouseEvent* );
      virtual void          mousePressEvent   ( QMouseEvent* );
      virtual void          mouseReleaseEvent ( QMouseEvent* );
      virtual void          draw              ();
      virtual void          reset             ();
    private:
                            Command           ( const Command& );
              Command&      operator=         ( const Command& );
    protected:
      CellWidget* _cellWidget;
      bool        _active;
  };


// Inline Functions.
  bool  Command::isActive      () const { return _active; }


}



#endif

