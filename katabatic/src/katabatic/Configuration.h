// -*- mode: C++; explicit-buffer-name: "Configuration.h<katabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/Configuration.h"                   |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_CONFIGURATION_H
#define  KATABATIC_CONFIGURATION_H

#include  <string>
#include  <vector>

#include  "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}

namespace CRL {
  class CellGauge;
  class RoutingGauge;
  class RoutingLayerGauge;
}


namespace Katabatic {


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
// Class  :  "Katabatic::Configuration" (decorator).


  class Configuration {
    public:
      enum Flag { NoFlags=0x0000, PitchAbove=0x0001, PitchBelow=0x0002 };
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
      virtual DbU::Unit          getPitch           ( size_t depth, unsigned int flags ) const = 0;
      virtual DbU::Unit          getOffset          ( size_t depth ) const = 0;
      virtual DbU::Unit          getWireWidth       ( size_t depth ) const = 0;
      virtual DbU::Unit          getExtensionCap    ( size_t depth ) const = 0;
      virtual unsigned int       getDirection       ( size_t depth ) const = 0;
      virtual DbU::Unit          getPitch           ( const Layer*, unsigned int flags ) const = 0;
      virtual DbU::Unit          getOffset          ( const Layer* ) const = 0;
      virtual DbU::Unit          getWireWidth       ( const Layer* ) const = 0;
      virtual DbU::Unit          getExtensionCap    ( const Layer* ) const = 0;
      virtual unsigned int       getDirection       ( const Layer* ) const = 0;
      virtual float              getSaturateRatio   () const = 0;
      virtual size_t             getSaturateRp      () const = 0;
      virtual DbU::Unit          getGlobalThreshold () const = 0;
      virtual size_t             getHEdgeCapacity   () const = 0;
      virtual size_t             getVEdgeCapacity   () const = 0;
      virtual void               setAllowedDepth    ( size_t ) = 0;
      virtual void               setSaturateRatio   ( float ) = 0;
      virtual void               setSaturateRp      ( size_t ) = 0;
      virtual void               setGlobalThreshold ( DbU::Unit ) = 0;
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
// Class  :  "Katabatic::ConfigurationConcrete".


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
      virtual DbU::Unit              getPitch              ( size_t depth, unsigned int flags ) const;
      virtual DbU::Unit              getOffset             ( size_t depth ) const;
      virtual DbU::Unit              getWireWidth          ( size_t depth ) const;
      virtual DbU::Unit              getExtensionCap       ( size_t depth ) const;
      virtual unsigned int           getDirection          ( size_t depth ) const;
      virtual DbU::Unit              getPitch              ( const Layer*, unsigned int flags ) const;
      virtual DbU::Unit              getOffset             ( const Layer* ) const;
      virtual DbU::Unit              getWireWidth          ( const Layer* ) const;
      virtual DbU::Unit              getExtensionCap       ( const Layer* ) const;
      virtual unsigned int           getDirection          ( const Layer* ) const;
      virtual float                  getSaturateRatio      () const;
      virtual size_t                 getSaturateRp         () const;
      virtual DbU::Unit              getGlobalThreshold    () const;
      virtual size_t                 getHEdgeCapacity      () const;
      virtual size_t                 getVEdgeCapacity      () const;
      virtual void                   setAllowedDepth       ( size_t );
      virtual void                   setSaturateRatio      ( float );
      virtual void                   setSaturateRp         ( size_t );
      virtual void                   setGlobalThreshold    ( DbU::Unit );
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
      float                   _saturateRatio;
      size_t                  _saturateRp;
      DbU::Unit               _globalThreshold;
      size_t                  _allowedDepth;
      size_t                  _hEdgeCapacity;
      size_t                  _vEdgeCapacity;
    private:
                             ConfigurationConcrete ( const ConfigurationConcrete& );
      ConfigurationConcrete& operator=             ( const ConfigurationConcrete& );
      void                   _setTopRoutingLayer   ( Name name );
  };


} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::Configuration);
INSPECTOR_P_SUPPORT(Katabatic::ConfigurationConcrete);


#endif  // KATABATIC_CONFIGURATION_H
