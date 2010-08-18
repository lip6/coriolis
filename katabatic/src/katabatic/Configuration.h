
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_CONFIGURATION__
#define  __KATABATIC_CONFIGURATION__

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


namespace Katabatic {


  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Configuration" (decorator).


  class Configuration {
    public:
    // Constructor & Destructor.
      virtual                   ~Configuration      ();
      virtual Configuration*     clone              () const = 0;
    // Methods.                                     
      virtual bool               isGMetal           ( const Layer* ) const = 0;
      virtual size_t             getDepth           () const = 0;
      virtual size_t             getAllowedDepth    () const = 0;
      virtual size_t             getLayerDepth      ( const Layer* ) const = 0;
      virtual RoutingGauge*      getRoutingGauge    () const = 0;
      virtual RoutingLayerGauge* getLayerGauge      ( size_t depth ) const = 0;
      virtual const Layer*       getRoutingLayer    ( size_t depth ) const = 0;
      virtual Layer*             getContactLayer    ( size_t depth ) const = 0;
      virtual DbU::Unit          getExtensionCap    () const = 0;
      virtual float              getSaturateRatio   () const = 0;
      virtual size_t             getSaturateRp      () const = 0;
      virtual DbU::Unit          getGlobalThreshold () const = 0;
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
                                     ConfigurationConcrete ( const RoutingGauge* rg=NULL );
      virtual                       ~ConfigurationConcrete ();
      virtual ConfigurationConcrete* clone                 () const;
    // Methods.
      virtual bool                   isGMetal              ( const Layer* ) const;
      virtual size_t                 getDepth              () const;
      virtual size_t                 getAllowedDepth       () const;
      virtual size_t                 getLayerDepth         ( const Layer* ) const;
      virtual RoutingGauge*          getRoutingGauge       () const;
      virtual RoutingLayerGauge*     getLayerGauge         ( size_t depth ) const;
      virtual const Layer*           getRoutingLayer       ( size_t depth ) const;
      virtual Layer*                 getContactLayer       ( size_t depth ) const;
      virtual DbU::Unit              getExtensionCap       () const;
      virtual float                  getSaturateRatio      () const;
      virtual size_t                 getSaturateRp         () const;
      virtual DbU::Unit              getGlobalThreshold    () const;
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
      const Layer*  _gmetalh;
      const Layer*  _gmetalv;
      const Layer*  _gcontact;
      RoutingGauge* _rg;
      DbU::Unit     _extensionCap;
      float         _saturateRatio;
      size_t        _saturateRp;
      DbU::Unit     _globalThreshold;
      size_t        _allowedDepth;
    private:
                             ConfigurationConcrete ( const ConfigurationConcrete& );
      ConfigurationConcrete& operator=             ( const ConfigurationConcrete& );
  };


}  // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::Configuration);
INSPECTOR_P_SUPPORT(Katabatic::ConfigurationConcrete);


#endif  // __KATABATIC_CONFIGURATION__
