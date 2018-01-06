
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
// |  C++ Header  :       "./SelectCommand.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECT_COMMAND_H__
#define  __HURRICANE_SELECT_COMMAND_H__

#include  <set>

#include  <QObject>
#include  <QPoint>

class QAction;

#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/AreaCommand.h"


using namespace std;


namespace Hurricane {


  class Cell;
  class Selector;
  class SelectionPopup;


  class SelectCommand : public QObject, public AreaCommand {
      Q_OBJECT;

    public:
      enum SelectMode { AllMode=0, NetMode=1, NoAnonNetMode=2 };
    public:
      static  const string&  getStaticName     ();
                             SelectCommand     ();
      virtual               ~SelectCommand     ();
      virtual const string&  getName           () const;
      inline  unsigned int   getSelectMode     () const;
      inline  void           setSelectMode     ( unsigned int );
      virtual void           keyPressEvent     ( QKeyEvent* );
      virtual void           mousePressEvent   ( QMouseEvent* );
      virtual void           mouseReleaseEvent ( QMouseEvent* );
              void           bindToAction      ( QAction* );
    signals:
              void           selectionToggled  ( Occurrence );
    private:
                             SelectCommand     ( const SelectCommand& );
              SelectCommand& operator=         ( const SelectCommand& );
    private:
      static string           _name;
             SelectionPopup*  _selectionPopup;
             unsigned int     _selectMode;
  };


  inline unsigned int  SelectCommand::getSelectMode () const { return _selectMode; }
  inline void          SelectCommand::setSelectMode ( unsigned int mode ) { _selectMode = mode%3; }


} // End of Hurricane namespace.


#endif
