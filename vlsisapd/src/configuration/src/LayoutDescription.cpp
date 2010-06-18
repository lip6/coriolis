
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
// |  C++ Module  :       "./LayoutDescription.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "vlsisapd/configuration/LayoutDescription.h"
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

    using std::cerr;
    using std::endl;
    using std::string;
    using std::vector;


    ConfigurationWidget* LayoutDescription::buildWidget ()
    {
      ConfigurationWidget* cw = new ConfigurationWidget ();

      for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
        const vector<WidgetDescription*> widgets = _tabs[itab]->getWidgets();

        for ( size_t iwidget=0 ; iwidget<widgets.size() ; ++iwidget ) {
          switch ( widgets[iwidget]->getType() ) {
            case WidgetDescription::Rule:
              cw->addRuler ( _tabs[itab]->getName() );
              break;
            case WidgetDescription::Title:
              cw->addTitle ( _tabs[itab]->getName()
                           , widgets[iwidget]->getTitle() );
              break;
            case WidgetDescription::Section:
              cw->addSection ( _tabs[itab]->getName()
                             , widgets[iwidget]->getTitle()
                             , widgets[iwidget]->getColumn() );
              break;
            case WidgetDescription::Parameter:
              Parameter* parameter = _configuration->getParameter ( widgets[iwidget]->getId() );
              cw->addParameter ( _tabs[itab]->getName()
                               , parameter
                               , widgets[iwidget]->getLabel()
                               , widgets[iwidget]->getColumn()
                               , widgets[iwidget]->getFlags() );
              break;
          }
        }
      }

      cw->syncSlaves ();

      return cw;
    }


}  // End of Cfg namespace.
