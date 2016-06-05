// -*- mode: C++; explicit-buffer-name: "Configuration.h<smurf>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   S m u r f  -  A   D e m o   T o o l E n g i n e               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./smurf/Configuration.h"                       |
// +-----------------------------------------------------------------+


#ifndef  SMURF_CONFIGURATION_H
#define  SMURF_CONFIGURATION_H

#include  <string>

#include  "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}

#include  "crlcore/RoutingGauge.h"
namespace CRL {
  class RoutingLayerGauge;
}


namespace Smurf {


  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Smurf::Configuration" (decorator).


  class Configuration {
    public:
    // Constructor & Destructor.
      virtual                   ~Configuration      ();
      virtual Configuration*     clone              () const = 0;
    // Methods.                                     
      virtual RoutingGauge*      getRoutingGauge    () const = 0;
      virtual void               print              ( Cell* ) const = 0;
      virtual Record*            _getRecord         () const = 0;
      virtual string             _getString         () const = 0;
      virtual string             _getTypeName       () const = 0;
    protected:
                                 Configuration      ();
    private:
                                 Configuration      ( const Configuration& );
              Configuration&     operator=          ( const Configuration& );
    private:
      static  Configuration*     _default;
  };


// -------------------------------------------------------------------
// Class  :  "Smurf::ConfigurationConcrete".


  class ConfigurationConcrete : public Configuration {
      friend class Configuration;
    public:
    // Constructor & Destructor.
                                     ConfigurationConcrete ( const RoutingGauge* rg=NULL );
      virtual                       ~ConfigurationConcrete ();
      virtual ConfigurationConcrete* clone                 () const;
    // Methods.
      virtual RoutingGauge*          getRoutingGauge       () const;
      virtual void                   print                 ( Cell* ) const;
      virtual Record*                _getRecord            () const;
      virtual string                 _getString            () const;
      virtual string                 _getTypeName          () const;
    protected:
    // Attributes.
      RoutingGauge* _rg;
    private:
                             ConfigurationConcrete ( const ConfigurationConcrete& );
      ConfigurationConcrete& operator=             ( const ConfigurationConcrete& );
  };


} // Smurf namespace.


INSPECTOR_P_SUPPORT(Smurf::Configuration);
INSPECTOR_P_SUPPORT(Smurf::ConfigurationConcrete);


#endif  // SMURF_CONFIGURATION_H
