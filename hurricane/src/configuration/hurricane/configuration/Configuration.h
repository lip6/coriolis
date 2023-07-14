// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU/LIP6 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <map>
#include <set>
#include <iostream>
#include "hurricane/configuration/Parameter.h"
#include "hurricane/configuration/LayoutDescription.h"


namespace Cfg {

  class ConfigurationWidget;
  class ConfigurationDialog;


  class Configuration {
    public:
      enum Flags   { DriveValues=0x1, DriveLayout=0x2 };
      enum LogType { LogRestart=0x1, LogNeedExist=0x2, AllLogs=0xFFFF };
    public:
      class LogEntry {
        public:
          inline               LogEntry ( std::string id );
          inline  std::string  getId    () const;
          inline  std::string  getValid () const;
          inline  void         restore  () const;
        private:
          std::string  _id;
          std::string  _valid;
      };

    public:
      static Configuration*            get                 ();
      static Parameter::Priority       pushDefaultPriority ( Parameter::Priority );
      static Parameter::Priority       popDefaultPriority  ();
      static Parameter::Priority       getDefaultPriority  ();
      static void                      _tokenize           ( std::set<std::string>& tokens, std::string line );
    public:
    // Methods.
             ConfigurationWidget*      buildWidget         ( unsigned int flags );
             ConfigurationDialog*      buildDialog         ();
      inline const std::map<std::string,Parameter*>&
                                       getParameters       () const;
             const std::set<LogEntry>& getLogs             ( unsigned int ilog ) const;
      inline unsigned int              getFlags            () const;
      inline const LayoutDescription&  getLayout           () const;
      inline LayoutDescription&        getLayout           ();
             Parameter*                getParameter        ( std::string     id
                                                           , Parameter::Type type=Parameter::Unknown ) const;
             Parameter*                addParameter        ( std::string     id
                                                           , Parameter::Type type
                                                           , std::string     value
                                                           , Parameter::Priority priority=Parameter::UseDefault );
      inline void                      setFlags            ( unsigned int mask );
      inline bool                      hasLogs             ( unsigned int mask ) const;
             void                      addLog              ( unsigned int mask, std::string id );
             void                      removeLog           ( unsigned int mask, std::string id );
      inline void                      restoreFromLogs     ( unsigned int mask );
      inline void                      clearLogs           ( unsigned int mask );
             void                      print               ( std::ostream& ) const;
             bool                      readFromFile        ( std::string );
             bool                      writeToFile         ( std::string, unsigned int flags, std::string tabs="" ) const;
             void                      writeToStream       ( std::ostream&, unsigned int flags, std::string tabs="" ) const;
    private:
    // Attributes.
      static Configuration*                         _singleton;
      std::map<std::string,Parameter*>              _parameters;
      LayoutDescription                             _layout;
      unsigned int                                  _flags;
      std::map< unsigned int, std::set<LogEntry> >  _logSets;
    private:
      Configuration ();
  };


// Inline Methods.
  inline const std::map<std::string,Parameter*>& Configuration::getParameters () const
  { return _parameters; }

  inline const LayoutDescription& Configuration::getLayout () const { return _layout; }
  inline       LayoutDescription& Configuration::getLayout ()       { return _layout; }
  inline       unsigned int       Configuration::getFlags  () const { return _flags; }
  inline       void               Configuration::setFlags  ( unsigned int mask ) { _flags |= mask; }


  inline bool  Configuration::hasLogs ( unsigned int mask ) const
  {
    std::map< unsigned int, std::set<LogEntry> >::const_iterator ilog = _logSets.begin();
    for ( ; ilog != _logSets.end() ; ++ilog )
      if ( (mask & (*ilog).first) and not (*ilog).second.empty () ) return true;
    return false;
  }


  inline void  Configuration::restoreFromLogs ( unsigned int mask )
  {
    std::map< unsigned int, std::set<LogEntry> >::iterator ilog = _logSets.begin();
    for ( ; ilog != _logSets.end() ; ++ilog ) {
      if ( mask & (*ilog).first ) {
        std::set<LogEntry>::iterator ientry = (*ilog).second.begin();
        for ( ; ientry != (*ilog).second.end() ; ++ientry ) {
        //std::cerr << "Restoring " << (*ientry).getId() << " -> " << (*ientry).getValid() << std::endl;
          (*ientry).restore ();
        }
      }
    }
  }


  inline void  Configuration::clearLogs ( unsigned int mask )
  {
    std::map< unsigned int, std::set<LogEntry> >::iterator ilog = _logSets.begin();
    for ( ; ilog != _logSets.end() ; ++ilog ) {
      if ( mask & (*ilog).first ) (*ilog).second.clear();
    }
  }


  inline bool  hasParameter ( std::string id )
  {
    return (Configuration::get()->getParameter(id,Parameter::Unknown) != NULL);
  }


  inline Parameter* getParamString ( std::string id, std::string value="<undefined>" )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::String);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::String, value );
    }
    return parameter;
  }


  inline Parameter* getParamBool ( std::string id, bool value=false )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Bool);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Bool, "false" );
      parameter->setBool ( value );
    }
    return parameter;
  }


  inline Parameter* getParamInt ( std::string id, int value=0 )
  {
  //std::cerr << "getParamInt() " << id << " value:" << value << std::endl;

    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Int);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Int, "0" );
      parameter->setInt ( value );
    }
    return parameter;
  }


  inline Parameter* getParamEnumerate ( std::string id, int value=0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Enumerate);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Enumerate, "0" );
      parameter->setInt ( value );
    }
    return parameter;
  }


  inline Parameter* getParamDouble ( std::string id, double value=0.0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Double);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Double, "0.0" );
      parameter->setDouble ( value );
    }
    return parameter;
  }


  inline Parameter* getParamPercentage ( std::string id, double value=91.0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Percentage);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Percentage, "0.91" );
      parameter->setPercentage ( value );
    }
    return parameter;
  }


  inline Configuration::LogEntry::LogEntry ( std::string id )
    : _id   (id)
    , _valid("")
  {
    Parameter* parameter = Configuration::get()->getParameter(id);
    if ( parameter != NULL ) _valid = parameter->asString();
  }


  inline std::string  Configuration::LogEntry::getId     () const { return _id; }
  inline std::string  Configuration::LogEntry::getValid  () const { return _valid; }


  inline void  Configuration::LogEntry::restore () const
  {
    Parameter* parameter = Configuration::get()->getParameter(_id);
    if ( parameter != NULL ) parameter->setString ( _valid
                                                  , Configuration::getDefaultPriority()
                                                  , (Parameter::AllRequirements | Parameter::FromString)
                                                  & ~Parameter::TypeCheck
                                                  );
  }

  inline bool  operator< ( const Configuration::LogEntry& lhs, const Configuration::LogEntry& rhs )
  { return lhs.getId() < rhs.getId(); }


} // Cfg namespace.
