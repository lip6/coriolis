
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
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
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CFG_CONFIGURATION__
#define  __CFG_CONFIGURATION__

#include  <string>
#include  <map>
#include  <set>
#include  <iostream>
#include  "vlsisapd/configuration/Parameter.h"
#include  "vlsisapd/configuration/LayoutDescription.h"


namespace Cfg {

  class ConfigurationWidget;
  class ConfigurationDialog;


  class Configuration {
    public:
      enum Flags   { DriveValues=0x1, DriveLayout=0x2 };
      enum LogType { LogRestart=0, LogNeedExist, LogTypeSize };
    public:
      static Configuration*           get                 ();
    public:
    // Methods.
             ConfigurationWidget*     buildWidget         ( unsigned int flags );
             ConfigurationDialog*     buildDialog    ();
      inline const std::map<const std::string,Parameter*>&
                                      getParameters       () const;
      inline const std::set<std::string>&                 
                                      getLogs             ( unsigned int type ) const;
      inline unsigned int             getFlags            () const;
      inline const LayoutDescription& getLayout           () const;
      inline LayoutDescription&       getLayout           ();
             Parameter*               getParameter        ( const std::string& id
                                                          , Parameter::Type    type=Parameter::Unknown ) const;
             Parameter*               addParameter        ( const std::string& id
                                                          , Parameter::Type    type
                                                          , const std::string& value );
      inline void                     setFlags            ( unsigned int mask );
      inline bool                     hasLogs             () const;
             void                     addLog              ( unsigned int type, const std::string& id );
             void                     removeLog           ( unsigned int type, const std::string& id );
      inline void                     clearLogs           ();
             void                     print               ( std::ostream& ) const;
             bool                     readFromFile        ( const std::string& );
             bool                     writeToFile         ( const std::string&, unsigned int flags, const std::string& tabs="" ) const;
             void                     writeToStream       ( std::ostream&, unsigned int flags, const std::string& tabs="" ) const;
    private:
    // Attributes.
      static Configuration*                   _singleton;
      std::map<const std::string,Parameter*>  _parameters;
      LayoutDescription                       _layout;
      unsigned int                            _flags;
      std::vector< std::set<std::string> >    _logSets;
    private:
      Configuration ();
  };


// Inline Methods.
  inline const std::map<const std::string,Parameter*>& Configuration::getParameters () const
  { return _parameters; }

  inline const std::set<std::string>& Configuration::getLogs ( unsigned int type ) const
  { return _logSets[(type<LogTypeSize) ? type : 0]; }

  inline const LayoutDescription& Configuration::getLayout () const { return _layout; }
  inline       LayoutDescription& Configuration::getLayout ()       { return _layout; }
  inline       unsigned int       Configuration::getFlags  () const { return _flags; }
  inline       void               Configuration::setFlags  ( unsigned int mask ) { _flags |= mask; }

  inline bool  Configuration::hasLogs () const
  {
    for ( size_t ilog=0 ; ilog<_logSets.size() ; ++ilog )
      if ( not _logSets[ilog].empty () ) return true;
    return false;
  }

  inline void  Configuration::clearLogs ()
  {
    for ( size_t ilog=0 ; ilog<_logSets.size() ; ++ilog )
      _logSets[ilog].clear ();
  }


  inline Parameter* getParamString ( const std::string& id, const std::string& value="<undefined>" )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::String);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::String, value );
    }
    return parameter;
  }


  inline Parameter* getParamBool ( const std::string& id, bool value=false )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Bool);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Bool, "false" );
      parameter->setBool ( value );
    }
    return parameter;
  }


  inline Parameter* getParamInt ( const std::string& id, int value=0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Int);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Int, "0" );
      parameter->setInt ( value );
    }
    return parameter;
  }


  inline Parameter* getParamEnumerate ( const std::string& id, int value=0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Enumerate);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Enumerate, "0" );
      parameter->setInt ( value );
    }
    return parameter;
  }


  inline Parameter* getParamDouble ( const std::string& id, double value=0.0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Double);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Double, "0.0" );
      parameter->setDouble ( value );
    }
    return parameter;
  }


  inline Parameter* getParamPercentage ( const std::string& id, double value=91.0 )
  {
    Parameter* parameter = Configuration::get()->getParameter(id,Parameter::Percentage);
    if ( parameter == NULL ) {
      parameter = Configuration::get()->addParameter ( id, Parameter::Percentage, "0.91" );
      parameter->setPercentage ( value );
    }
    return parameter;
  }




} // End of Cfg namespace.


#endif  // __CFG_CONFIGURATION__
