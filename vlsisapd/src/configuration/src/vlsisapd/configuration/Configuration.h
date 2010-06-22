
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
#include  <iostream>
#include  "vlsisapd/configuration/Parameter.h"
#include  "vlsisapd/configuration/LayoutDescription.h"


namespace Cfg {

  class ConfigurationWidget;


  class Configuration {
    public:
      static Configuration*           get            ();
    public:
    // Methods.
             ConfigurationWidget*     buildWidget    ();
      inline const std::map<const std::string,Parameter*>&
                                      getParameters  () const;
      inline const LayoutDescription& getLayout      () const;
      inline LayoutDescription&       getLayout      ();
             Parameter*               getParameter   ( const std::string& id
                                                     , Parameter::Type    type=Parameter::Unknown ) const;
             Parameter*               addParameter   ( const std::string& id
                                                     , Parameter::Type    type
                                                     , const std::string& value );
             void                     print          ( std::ostream& ) const;
             void                     readFromFile   ( const std::string& );
             void                     writeToStream  ( std::ostream& ) const;
    private:
    // Attributes.
      static Configuration*                   _singleton;
      std::map<const std::string,Parameter*>  _parameters;
      LayoutDescription                       _layout;
    private:
      Configuration ();
  };


// Inline Methods.
  inline const std::map<const std::string,Parameter*>& Configuration::getParameters () const
  { return _parameters; }

  inline const LayoutDescription& Configuration::getLayout () const { return _layout; }
  inline       LayoutDescription& Configuration::getLayout ()       { return _layout; }


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
