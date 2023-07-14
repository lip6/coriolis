
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ConfTabWidget.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <QWidget>
class QFont;
class QGridLayout;

#include "hurricane/configuration/ConfigurationWidget.h"


namespace Cfg {

  class ParameterWidget;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfTabWidget".


  class ConfTabWidget : public QWidget {
      Q_OBJECT;
    public:
                           ConfTabWidget          ( ConfigurationWidget* parent, const std::string& name="<noname>" );
      virtual             ~ConfTabWidget          ();
    public:
      ConfigurationWidget* getConfigurationWidget ();
      QFont&               getParentBoldFont      ();
      void                 addRuler               ();
      void                 addTitle               ( const std::string& title );
      void                 addSection             ( const std::string& section, int column=0 );
      ParameterWidget*     addParameter           ( Parameter*, const std::string& label, int column=0, int span=1, int flags=0 );
    signals:                                     
      void                 updateParameters       ();
    private:                                     
      int                  _getMaxRowCount        ();
      int                  _alignMaxRowCount      ();
    private:
      QGridLayout*                   _gridLayout;
      int                            _columns;
      int                            _span;
      int*                           _rowsCount;
      ConfigurationWidget*           _confWidget;
      std::vector<ParameterWidget*>  _parameters;
  };


} // Cfg namespace.
