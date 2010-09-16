
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
#include  "vlsisapd/configuration/ParameterWidget.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;
  using std::vector;
  using std::ostream;


  TabDescription* LayoutDescription::getTab ( const string& tabName )
  {
    for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
      if ( _tabs[itab]->getName() == tabName ) return _tabs[itab];
    }

    addTab ( new TabDescription(tabName) );
    return getBackTab();
  }
  

  void  LayoutDescription::addRule ( const string& tabName )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::rule() );
  }


  void  LayoutDescription::addTitle ( const string& tabName, const string& title )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::title(title) );
  }


  void  LayoutDescription::addSection ( const string& tabName, const string& section, int column )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::section(section,column) );
  }


  void  LayoutDescription::addParameter ( const string& tabName
                                        , const string& id
                                        , const string& label
                                        , int           column
                                        , int           span
                                        , unsigned int  flags )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::parameter(id,label,column,span,flags) );
  }
  

  ConfigurationWidget* LayoutDescription::buildWidget ( unsigned int flags )
  {
    ConfigurationWidget* cw = new ConfigurationWidget ( flags );

    for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
      const vector<WidgetDescription*> widgets = _tabs[itab]->getWidgets();

      for ( size_t iwidget=0 ; iwidget<widgets.size() ; ++iwidget ) {
        switch ( widgets[iwidget]->getType() ) {
          case WidgetDescription::Rule:
            cw->addRuler ( _tabs[itab]->getName() );
            break;
          case WidgetDescription::Title:
            cw->addTitle ( _tabs[itab]->getName()
                         , widgets[iwidget]->getLabel() );
            break;
          case WidgetDescription::Section:
            cw->addSection ( _tabs[itab]->getName()
                           , widgets[iwidget]->getLabel()
                           , widgets[iwidget]->getColumn() );
            break;
          case WidgetDescription::Parameter:
            Parameter* parameter = _configuration->getParameter ( widgets[iwidget]->getId() );
            cw->addParameter ( _tabs[itab]->getName()
                             , parameter
                             , widgets[iwidget]->getLabel()
                             , widgets[iwidget]->getColumn()
                             , widgets[iwidget]->getSpan()
                             , widgets[iwidget]->getFlags() );
            break;
        }
      }
    }

    cw->syncSlaves ();

    return cw;
  }


  void  LayoutDescription::writeToStream ( ostream& out ) const
  {
    out << "  <layout>" << endl;

    for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
      out << "    <tab name=\"" << _tabs[itab]->getName() << "\">" << endl;

      const vector<WidgetDescription*>& widgets = _tabs[itab]->getWidgets();
      for ( size_t iwidget=0 ; iwidget<widgets.size() ; ++iwidget ) {
        WidgetDescription* widget = widgets[iwidget];

        out << "      <widget ";

        switch ( widget->getType() ) {
          case WidgetDescription::Rule:
            out << "type=\"rule\"";
            break;
          case WidgetDescription::Title:
            out << "type=\"title\" label=\"" << toXml(widget->getLabel()) << "\"";
            break;
          case WidgetDescription::Section:
            out <<    "type=\"section\""
                <<  " label=\"" << widget->getLabel() << "\""
                << " column=\"" << widget->getColumn() << "\"";
            break;
          case WidgetDescription::Parameter:
            out <<     " id=\"" << widget->getId() << "\""
                <<  " label=\"" << widget->getLabel() << "\""
                << " column=\"" << widget->getColumn() << "\"";

            if ( widget->getSpan() != 1 ) out << " span=\"" << widget->getSpan() << "\"";
            if ( widget->getFlags() & ParameterWidget::UseSpinBox ) out << " spinbox=\"true\"";
            if ( widget->getFlags() & ParameterWidget::IsFileName ) out << " filename=\"true\"";
            if ( widget->getFlags() & ParameterWidget::IsPathName ) out << " pathname=\"true\"";
            break;
        }

        out << "/>" << endl;
      }

      out << "    </tab>" << endl;
    }

    out << "  </layout>" << endl;
  }


}  // End of Cfg namespace.
