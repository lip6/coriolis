// -*- mode: C++; explicit-buffer-name: "Configuration.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/Configuration.h"                    |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_CONFIGURATION_H
#define  ANABATIC_CONFIGURATION_H

#include <string>
#include <vector>

#include "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}

namespace CRL {
  class CellGauge;
  class RoutingGauge;
  class RoutingLayerGauge;
}

#include "anabatic/Constants.h"


namespace Anabatic {


  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Name;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;
  using  CRL::CellGauge;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Configuration" (decorator).

  class Configuration {
    public:
    // Constructor & Destructor.
      virtual                   ~Configuration      ();
      virtual Configuration*     clone              () const = 0;
    // Methods.                                     
      virtual bool               isGMetal           ( const Layer* ) const = 0;
      virtual bool               isGContact         ( const Layer* ) const = 0;
      virtual size_t             getDepth           () const = 0;
      virtual size_t             getAllowedDepth    () const = 0;
      virtual size_t             getLayerDepth      ( const Layer* ) const = 0;
      virtual CellGauge*         getCellGauge       () const = 0;
      virtual RoutingGauge*      getRoutingGauge    () const = 0;
      virtual RoutingLayerGauge* getLayerGauge      ( size_t depth ) const = 0;
      virtual const Layer*       getRoutingLayer    ( size_t depth ) const = 0;
      virtual Layer*             getContactLayer    ( size_t depth ) const = 0;
      virtual DbU::Unit          getSliceHeight     () const = 0;
      virtual DbU::Unit          getSliceStep       () const = 0;
      virtual DbU::Unit          getPitch           ( size_t depth, Flags flags ) const = 0;
      virtual DbU::Unit          getOffset          ( size_t depth ) const = 0;
      virtual DbU::Unit          getWireWidth       ( size_t depth ) const = 0;
      virtual DbU::Unit          getExtensionCap    ( size_t depth ) const = 0;
      virtual Flags              getDirection       ( size_t depth ) const = 0;
      virtual DbU::Unit          getPitch           ( const Layer*, Flags flags ) const = 0;
      virtual DbU::Unit          getOffset          ( const Layer* ) const = 0;
      virtual DbU::Unit          getWireWidth       ( const Layer* ) const = 0;
      virtual DbU::Unit          getExtensionCap    ( const Layer* ) const = 0;
      virtual Flags              getDirection       ( const Layer* ) const = 0;
      virtual void               setAllowedDepth    ( size_t ) = 0;
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
// Class  :  "Anabatic::ConfigurationConcrete".

  class ConfigurationConcrete : public Configuration {
      friend class Configuration;
    public:
    // Constructor & Destructor.
                                     ConfigurationConcrete ( const CellGauge* cg=NULL, const RoutingGauge* rg=NULL );
      virtual                       ~ConfigurationConcrete ();
      virtual ConfigurationConcrete* clone                 () const;
    // Methods.
      virtual bool                   isGMetal              ( const Layer* ) const;
      virtual bool                   isGContact            ( const Layer* ) const;
      virtual size_t                 getDepth              () const;
      virtual size_t                 getAllowedDepth       () const;
      virtual size_t                 getLayerDepth         ( const Layer* ) const;
      virtual CellGauge*             getCellGauge          () const;
      virtual RoutingGauge*          getRoutingGauge       () const;
      virtual RoutingLayerGauge*     getLayerGauge         ( size_t depth ) const;
      virtual const Layer*           getRoutingLayer       ( size_t depth ) const;
      virtual Layer*                 getContactLayer       ( size_t depth ) const;
      virtual DbU::Unit              getSliceHeight        () const;
      virtual DbU::Unit              getSliceStep          () const;
      virtual DbU::Unit              getPitch              ( size_t depth, Flags flags ) const;
      virtual DbU::Unit              getOffset             ( size_t depth ) const;
      virtual DbU::Unit              getWireWidth          ( size_t depth ) const;
      virtual DbU::Unit              getExtensionCap       ( size_t depth ) const;
      virtual Flags                  getDirection          ( size_t depth ) const;
      virtual DbU::Unit              getPitch              ( const Layer*, Flags flags ) const;
      virtual DbU::Unit              getOffset             ( const Layer* ) const;
      virtual DbU::Unit              getWireWidth          ( const Layer* ) const;
      virtual DbU::Unit              getExtensionCap       ( const Layer* ) const;
      virtual Flags                  getDirection          ( const Layer* ) const;
      virtual void                   setAllowedDepth       ( size_t );
      virtual void                   print                 ( Cell* ) const;
      virtual Record*                _getRecord            () const;
      virtual string                 _getString            () const;
      virtual string                 _getTypeName          () const;
    protected:
    // Attributes.
      const Layer*            _gmetalh;
      const Layer*            _gmetalv;
      const Layer*            _gcontact;
      CellGauge*              _cg;
      RoutingGauge*           _rg;
      std::vector<DbU::Unit>  _extensionCaps;
      size_t                  _allowedDepth;
    private:
                             ConfigurationConcrete ( const ConfigurationConcrete& );
      ConfigurationConcrete& operator=             ( const ConfigurationConcrete& );
      void                   _setTopRoutingLayer   ( Name name );
  };


} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Configuration);
INSPECTOR_P_SUPPORT(Anabatic::ConfigurationConcrete);


#endif  // ANABATIC_CONFIGURATION_H
