
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ConfTabWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __CFG_CONF_TAB_WIDGET__
#define  __CFG_CONF_TAB_WIDGET__

#include  <QWidget>
class QFont;
class QGridLayout;

#include  "vlsisapd/configuration/ConfigurationWidget.h"

namespace Cfg {

  class ParameterWidget;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfTabWidget".


  class ConfTabWidget : public QWidget {
      Q_OBJECT;
    public:
                       ConfTabWidget     ( const std::string& name="<noname>", QWidget* parent=NULL );
      virtual         ~ConfTabWidget     ();
    public:          
      QFont&           getParentBoldFont ();
      void             addRuler          ();
      void             addTitle          ( const std::string& title );
      void             addSection        ( const std::string& section, int column=0 );
      ParameterWidget* addParameter      ( Parameter*, const std::string& label, int column=0, int flags=0 );
    signals:
      void             updateParameters  ();
    private:         
      int              _getMaxRowCount   ();
      int              _alignMaxRowCount ();
    private:
      QGridLayout*                   _gridLayout;
      int                            _columns;
      int*                           _rowsCount;
      std::vector<ParameterWidget*>  _parameters;
  };


} // End of Cfg namespace.


#endif  // __CFG_CONF_TAB_WIDGET__
