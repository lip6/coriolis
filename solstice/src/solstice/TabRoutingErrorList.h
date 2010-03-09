

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
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TabRoutingErrorList.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include  <hurricane/viewer/ControllerWidget.h>
#include  <solstice/RoutingErrorListWidget.h>

namespace Solstice {


  using Hurricane::ControllerTab;

// -------------------------------------------------------------------
// Class  :  "Hurricane::TabRoutingErrorList".


  class TabRoutingErrorList : public ControllerTab {
      Q_OBJECT;

    public:
                             TabRoutingErrorList         ( QWidget* parent=NULL );
      inline  RoutingErrorListWidget* getRoutingErrorListBrowser  ();
      inline  QCheckBox*     getSyncRoutingErrorList     ();
      inline  QCheckBox*     getSyncSelection   ();
      virtual void           cellPreModificate  ();
      virtual void           cellPostModificate ();
    public slots:           
      virtual void           setCell            ( Cell* );
      virtual void           setCellWidget      ( CellWidget* );
      virtual void           setSyncRoutingErrorList     ( bool );
      virtual void           setSyncSelection   ( bool );
    protected:
      RoutingErrorListWidget* _routingErrorListBrowser;
      QCheckBox*     _syncRoutingErrorList;
      QCheckBox*     _syncSelection;
      bool           _cwCumulativeSelection;
  };


  inline RoutingErrorListWidget* TabRoutingErrorList::getRoutingErrorListBrowser () { return _routingErrorListBrowser; }
  inline QCheckBox*     TabRoutingErrorList::getSyncRoutingErrorList    () { return _syncRoutingErrorList; }
  inline QCheckBox*     TabRoutingErrorList::getSyncSelection  () { return _syncSelection; }

} // End of Solstice namespace.
