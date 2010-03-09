

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
// |  C++ Module  :       "./solstice/TabRoutingError.h"             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __SOLSTICE_TAB_ROUTINGERROR__
#define  __SOLSTICE_TAB_ROUTINGERROR__


#include  <QTabWidget>

#include  <hurricane/viewer/ControllerWidget.h>

class QCheckBox;
class QComboBox;

namespace Hurricane {
  class Cell;
  class Occurrence;
}


namespace Solstice {

  using Hurricane::Cell;
  using Hurricane::CellWidget;
  using Hurricane::ControllerTab;

  class RoutingError;
  class RoutingErrorListWidget;

  class TabRoutingError : public ControllerTab {
      Q_OBJECT;

    public:
                                              TabRoutingError         ( QWidget* parent=NULL );
      inline             RoutingErrorListWidget*  getRoutingErrorBrowser  ();
      inline             QCheckBox*           getSyncRoutingError     ();
      inline             QCheckBox*           getSyncSelection        ();
      virtual            void                 cellPreModificate       ();
      virtual            void                 cellPostModificate      ();
    public slots:                 	      	          
      virtual            void                 setCell                 ( Cell* );
      virtual            void                 setCellWidget           ( CellWidget* );
      virtual            void                 setSyncRoutingError     ( bool );
      virtual            void                 setSyncSelection        ( bool );
                         void                 select                  ( const RoutingError* );
                         void                 unselect                ( const RoutingError* );
    protected:
                        RoutingErrorListWidget*  _routingErrorBrowser;
                        QCheckBox*           _syncRoutingError;
                        QCheckBox*           _syncSelection;
                        bool                 _cwCumulativeSelection;
  };			         
			         
			         
  inline RoutingErrorListWidget*      TabRoutingError::getRoutingErrorBrowser () { return _routingErrorBrowser; }
  inline QCheckBox*               TabRoutingError::getSyncRoutingError    () { return _syncRoutingError; }
  inline QCheckBox*               TabRoutingError::getSyncSelection       () { return _syncSelection; }


}//end of namespace solstice

#endif //TAB_ROUTINGERROR
