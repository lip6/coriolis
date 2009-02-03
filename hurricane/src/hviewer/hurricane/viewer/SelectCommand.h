
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
                               SelectCommand        ();
      virtual                 ~SelectCommand        ();
      virtual bool             mousePressEvent      ( CellWidget*, QMouseEvent* );
      virtual bool             mouseReleaseEvent    ( CellWidget*, QMouseEvent* );
              void             bindToAction         ( QAction* action );
    signals:
              void             selectionToggled     ( Occurrence occurrence );
    private:
              SelectionPopup*  _selectionPopup;
    private:
                               SelectCommand        ( const SelectCommand& );
              SelectCommand&   operator=            ( const SelectCommand& );
  };


} // End of Hurricane namespace.


#endif
