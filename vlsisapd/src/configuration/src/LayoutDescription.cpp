// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LayoutDescription.cpp"                  |
// +-----------------------------------------------------------------+


#include "vlsisapd/configuration/LayoutDescription.h"
#include "vlsisapd/configuration/Configuration.h"
#include "vlsisapd/configuration/ParameterWidget.h"
#include "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;
  using std::vector;
  using std::set;
  using std::map;
  using std::make_pair;
  using std::ostream;


// -------------------------------------------------------------------
// Class  :  "Cfg::WidgetDescription".

  string  WidgetDescription::typeToString ( WidgetDescription::Type type )
  {
    switch ( type ) {
      case Title:     return "Title";
      case Section:   return "Section";
      case Rule:      return "Rule";
      case Parameter: return "Parameter";
    }
    return "Rule";
  }


  WidgetDescription::Type  WidgetDescription::stringToType ( const string& s )
  {
    if (s == "Title"    ) return Title;
    if (s == "Section"  ) return Section;
    if (s == "Parameter") return Parameter;
    return Rule;
  }


// -------------------------------------------------------------------
// Class  :  "Cfg::TabDescription".

  void  TabDescription::addWidget ( WidgetDescription* widget )
  {
    _widgets.push_back(widget);
    _layout->addWidgetLookup(widget);
  }


// -------------------------------------------------------------------
// Class  :  "Cfg::LayoutDescription".

  size_t  LayoutDescription::_timestamp = 0;


  WidgetDescription* LayoutDescription::getWidget ( const string& id )
  {
    map<const string,WidgetDescription*>::iterator iwid = _widgets.find(id);
    if ( iwid != _widgets.end() ) return (*iwid).second;

    return NULL;
  }


  void  LayoutDescription::addWidgetLookup ( WidgetDescription* widget )
  {
    _widgets.insert ( make_pair(widget->getId(),widget) );
  }


  TabDescription* LayoutDescription::getTab ( const string& tabName, const string& id )
  {
    for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
      if ( _tabs[itab]->getName() == tabName ) return _tabs[itab];
    }

    addTab ( new TabDescription(this,tabName,id) );
    return getBackTab();
  }
  

  void  LayoutDescription::addRule ( const string& tabName )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::rule() );
    ++_timestamp;
  }


  void  LayoutDescription::addTitle ( const string& tabName, const string& title )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::title(title) );
    ++_timestamp;
  }


  void  LayoutDescription::addSection ( const string& tabName, const string& section, int column )
  {
    TabDescription* tab = getTab ( tabName );
    tab->addWidget ( WidgetDescription::section(section,column) );
    ++_timestamp;
  }


  void  LayoutDescription::addParameter ( const string& tabName
                                        , const string& id
                                        , const string& label
                                        , int           column
                                        , int           span
                                        , unsigned int  flags )
  {
    TabDescription*    tab    = getTab ( tabName );
    WidgetDescription* widget = WidgetDescription::parameter(id,label,column,span,flags);

    tab->addWidget ( widget );
    ++_timestamp;
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
            if ( parameter ) {
              // cerr << "WidgetDescription::Parameter: " << (void*)widgets[iwidget]
              //      << " " << widgets[iwidget]->getId()
              //      << " " << (void*)parameter << endl;
              cw->addParameter ( _tabs[itab]->getName()
                               , parameter
                               , widgets[iwidget]->getLabel()
                               , widgets[iwidget]->getColumn()
                               , widgets[iwidget]->getSpan()
                               , widgets[iwidget]->getFlags() );
            } else {
              cerr << "[ERROR] LayoutDescription::buildWidget(): reference to non-existent parameter id=\""
                   << widgets[iwidget]->getId() << "\"." << endl;
            }
            break;
        }
      }
    }

    cw->syncSlaves ();

    return cw;
  }


  void  LayoutDescription::writeToStream ( ostream& out, const string& tabs ) const
  {
    set<string> tabset;
    Configuration::_tokenize ( tabset, tabs );

    out << "  <layout>" << endl;

    for ( size_t itab=0 ; itab<_tabs.size() ; ++itab ) {
      if ( not tabset.empty() ) {
        set<string>::iterator isavetab = tabset.begin();
        for ( ; isavetab != tabset.end() ; ++isavetab ) {
          if ( _tabs[itab]->getId().compare(0,(*isavetab).size(),*isavetab) == 0 ) {
            break;
          }
        }
        if ( isavetab == tabset.end() ) continue;
      }

      out << "    <tab name=\"" << _tabs[itab]->getName()
          <<         "\" id=\"" << _tabs[itab]->getId()
          <<         "\">" << endl;

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
