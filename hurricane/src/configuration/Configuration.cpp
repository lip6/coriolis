// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Configuration.cpp"                      |
// +-----------------------------------------------------------------+


#include <cstring>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/configuration/ConfigurationWidget.h"
#include "hurricane/configuration/ConfigurationDialog.h"
#include "hurricane/configuration/ParameterWidget.h"


namespace Cfg {

  using std::string;
  using std::map;
  using std::ostream;
  using std::ofstream;
  using std::setw;
  using std::cerr;
  using std::endl;
  using std::vector;
  using std::set;


  Configuration* Configuration::_singleton = NULL;


  Configuration* Configuration::get ()
  {
    if ( _singleton == NULL ) _singleton = new Configuration ();
    return _singleton;
  }


  Parameter::Priority  Configuration::pushDefaultPriority ( Parameter::Priority priority )
  { return Parameter::pushDefaultPriority(priority); }


  Parameter::Priority  Configuration::popDefaultPriority ()
  { return Parameter::popDefaultPriority(); }


  Parameter::Priority  Configuration::getDefaultPriority ()
  { return Parameter::getDefaultPriority(); }


  Configuration::Configuration ()
    : _parameters()
    , _layout    (this)
    , _flags     (0)
    , _logSets   ()
  { }


  ConfigurationWidget* Configuration::buildWidget ( unsigned int flags )
  { return _layout.buildWidget(flags); }


  ConfigurationDialog* Configuration::buildDialog()
  { return new ConfigurationDialog(); }


  Parameter* Configuration::getParameter ( string name, Parameter::Type type ) const
  {
    map<const string,Parameter*>::const_iterator iparameter = _parameters.find(name);
    if ( iparameter == _parameters.end() ) return NULL;

    if ( type != Parameter::Unknown ) {
      Parameter::Type t1 = (*iparameter).second->getType();
      Parameter::Type t2 = type;

      if ( t1 > t2 ) std::swap ( t1, t2 );

      if (         (t1 != t2)
         and not ( (t1 == Parameter::Double) and (t2 == Parameter::Percentage) )
         and not ( (t1 == Parameter::Int   ) and (t2 == Parameter::Enumerate ) ) )
        cerr << "[ERROR] Accessing " << Parameter::typeToString((*iparameter).second->getType())
             << " parameter <" << (*iparameter).second->getId()
             << "> as " << Parameter::typeToString(type)<< " (type mismatch)." << endl;
    }

    return (*iparameter).second;
  }


  Parameter* Configuration::addParameter ( string              id
                                         , Parameter::Type     type
                                         , string              value
                                         , Parameter::Priority priority )
  {
    Parameter* p = getParameter ( id );
    if ( p != NULL ) {
    // Update values here.
      return p;
    }

    p = new Parameter ( id, type, value, priority );
    _parameters.insert ( make_pair(id,p) );

    return p;
  }


  const set<Configuration::LogEntry>& Configuration::getLogs ( unsigned int mask ) const
  {
    static set<LogEntry> _failsafe;

    map< unsigned int, set<LogEntry> >::const_iterator ilog = _logSets.find(mask);
    if ( ilog != _logSets.end() ) return (*ilog).second;

    return _failsafe;
  }

  void  Configuration::addLog ( unsigned int mask, string id )
  {
    map< unsigned int, set<LogEntry> >::iterator ilog = _logSets.find(mask);
    if ( ilog == _logSets.end() ) {
      _logSets.insert ( make_pair(mask,set<LogEntry>()) );
      ilog = _logSets.find(mask);
    }
    (*ilog).second.insert ( LogEntry(id) );
  }


  void  Configuration::removeLog ( unsigned int mask, string id )
  {
    map< unsigned int, set<LogEntry> >::iterator ilog = _logSets.find(mask);
    if ( ilog != _logSets.end() ) (*ilog).second.erase ( id );
  }


  void  Configuration::print ( ostream& out ) const
  {
  }


  bool  Configuration::writeToFile ( std::string fileName, unsigned int flags, string tabs ) const
  { return false; }


  void  Configuration::writeToStream ( ostream& out, unsigned int flags, string tabs ) const
  { }


  bool  Configuration::readFromFile ( std::string fileName )
  { return false; }


}  // Cfg namespace.
