
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
// |  C++ Module  :       "./ConfigurationWidget.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <map>
#include <QApplication>

#include "vlsisapd/configuration/Configuration.h"
#include "vlsisapd/configuration/ParameterWidget.h"
#include "vlsisapd/configuration/ConfTabWidget.h"
#include "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

  using std::max;
  using std::cerr;
  using std::endl;
  using std::string;
  using std::map;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfigurationWidget".


  ConfigurationWidget::ConfigurationWidget ( QWidget* parent )
    : QTabWidget(parent)
    , _boldFont (QApplication::font())
  {
    _boldFont.setBold ( true );

    setAttribute ( Qt::WA_QuitOnClose, false );
  }


  QFont& ConfigurationWidget::getBoldFont ()
  { return _boldFont; }


  void  ConfigurationWidget::addRuler ( const string& tabName )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addRuler ();
  }


  void  ConfigurationWidget::addTitle (  const string& tabName, const string& title )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addTitle ( title );
  }


  void  ConfigurationWidget::addSection (  const string& tabName, const string& section, int column )
  {
    ConfTabWidget* tab = findOrCreate ( tabName );
    tab->addSection ( section, column );
  }


  ParameterWidget* ConfigurationWidget::addParameter ( const string&      tabName
                                                     , Parameter*         parameter
                                                     , const std::string& label
                                                     , int                column
                                                     , int                flags )
  {
    ParameterWidget* pw = find(parameter);
    if ( pw != NULL ) {
      cerr << "[ERROR] Parameter <" << parameter->getId() << "> already added." << endl;
      return pw;
    }

    ConfTabWidget* tab = findOrCreate ( tabName );
    return tab->addParameter ( parameter, label, column, flags );
  }


  ParameterWidget* ConfigurationWidget::find ( Parameter* parameter ) const
  {
    if ( parameter == NULL ) return NULL;
    return find ( parameter->getId() );
  }


  ParameterWidget* ConfigurationWidget::find ( const string& id ) const
  { return findChild<ParameterWidget*>(id.c_str()); }


  ConfTabWidget* ConfigurationWidget::findOrCreate ( const string& tabName )
  {
    ConfTabWidget* tab = findChild<ConfTabWidget*>(tabName.c_str());
    if ( tab != NULL ) return tab;

    tab = new ConfTabWidget ( tabName );
    addTab ( tab, tabName.c_str() );

    return tab;
  }


  void  ConfigurationWidget::syncSlaves ()
  {
    const map<const string,Parameter*>& parameters = Configuration::get()->getParameters ();
    map<const string,Parameter*>::const_iterator iparam = parameters.begin();

    for ( ; iparam != parameters.end() ; ++iparam ) {
      if ( (*iparam).second->getSlaves().empty() ) continue;

      ParameterWidget* pw = find ( (*iparam).first );
      if ( pw == NULL ) continue;

      pw->enableSlaves ( pw->getParameter()->asBool() );
    }
  }


}  // End of Cfg namespace.
