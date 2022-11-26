// -*- mode: C++; explicit-buffer-name: "Configuration.h<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2016-2022, All Rights Reserved
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


#pragma  once
#include <string>
#include <vector>

#include "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
  class RoutingPad;
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
  using  Hurricane::RoutingPad;
  using  Hurricane::Cell;
  using  CRL::CellGauge;
  using  CRL::RoutingGauge;
  using  CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Configuration".

  class Configuration {
    public:
      static const size_t  ndepth = (size_t)-1;
    public:
    // Constructor & Destructor.
                                 Configuration        ( const CellGauge* cg=NULL, const RoutingGauge* rg=NULL );
                                 Configuration        ( const Configuration& );
      virtual                   ~Configuration        ();
      virtual Configuration*     clone                () const;
    // Methods.
      inline  bool               isGLayer             ( const Layer* ) const;
              bool               isGMetal             ( const Layer* ) const;
              bool               isGContact           ( const Layer* ) const;
              bool               isTwoMetals          () const;
              bool               isHybrid             () const;
              bool               isHV                 () const;
              bool               isVH                 () const;
      inline  std::string        getNetBuilderStyle   () const;
      inline  StyleFlags         getRoutingStyle      () const;
              const Layer*       getGContactLayer     () const;
              const Layer*       getGHorizontalLayer  () const;
              const Layer*       getGVerticalLayer    () const;
      inline  size_t             getGVerticalDepth    () const;
      inline  DbU::Unit          getGVerticalPitch    () const;
      inline  size_t             getGHorizontalDepth  () const;
      inline  DbU::Unit          getGHorizontalPitch  () const;
      inline  size_t             getDVerticalDepth    () const;
      inline  const Layer*       getDVerticalLayer    () const;
      inline  DbU::Unit          getDVerticalWidth    () const;
      inline  DbU::Unit          getDPVerticalWidth   () const;
      inline  DbU::Unit          getDVerticalPitch    () const;
      inline  DbU::Unit          getDVerticalOffset   () const;
      inline  size_t             getDHorizontalDepth  () const;
      inline  const Layer*       getDHorizontalLayer  () const;
      inline  DbU::Unit          getDHorizontalWidth  () const;
      inline  DbU::Unit          getDPHorizontalWidth () const;
      inline  DbU::Unit          getDHorizontalPitch  () const;
      inline  DbU::Unit          getDHorizontalOffset () const;
      inline  size_t             getDContactDepth     () const;
      inline  const Layer*       getDContactLayer     () const;
      inline  DbU::Unit          getDContactWidth     () const;
      inline  DbU::Unit          getDContactPitch     () const;
              size_t             getDepth             () const;
              size_t             getAllowedDepth      () const;
              size_t             getLayerDepth        ( const Layer* ) const;
              CellGauge*         getCellGauge         () const;
              RoutingGauge*      getRoutingGauge      () const;
              RoutingLayerGauge* getLayerGauge        ( size_t depth ) const;
              const Layer*       getRoutingLayer      ( size_t depth ) const;
              Layer*             getContactLayer      ( size_t depth ) const;
              DbU::Unit          getSliceHeight       () const;
              DbU::Unit          getSliceStep         () const;
              DbU::Unit          getPitch             ( size_t depth, Flags flags ) const;
              DbU::Unit          getOffset            ( size_t depth ) const;
              DbU::Unit          getWireWidth         ( size_t depth ) const;
              DbU::Unit          getPWireWidth        ( size_t depth ) const;
              DbU::Unit          getExtensionCap      ( size_t depth ) const;
              Flags              getDirection         ( size_t depth ) const;
              DbU::Unit          getPitch             ( const Layer*, Flags flags ) const;
              DbU::Unit          getOffset            ( const Layer* ) const;
              DbU::Unit          getWireWidth         ( const Layer* ) const;
              DbU::Unit          getPWireWidth        ( const Layer* ) const;
              DbU::Unit          getExtensionCap      ( const Layer* ) const;
              Flags              getDirection         ( const Layer* ) const;
              float              getSaturateRatio     () const;
              size_t             getSaturateRp        () const;
      inline  std::string        getDiodeName         () const;
      inline  DbU::Unit          getAntennaGateMaxWL  () const;
      inline  DbU::Unit          getAntennaDiodeMaxWL () const;
              DbU::Unit          getGlobalThreshold   () const;
              void               setAllowedDepth      ( size_t );
              void               setSaturateRatio     ( float );
              void               setSaturateRp        ( size_t );
              void               setGlobalThreshold   ( DbU::Unit );
              DbU::Unit          getEdgeLength        () const;
              DbU::Unit          getEdgeWidth         () const;
              float              getEdgeCostH         () const;
              float              getEdgeCostK         () const;
              float              getEdgeHInc          () const;
              float              getEdgeHScaling      () const;
              int                getGlobalIterations  () const;
              DbU::Unit          isOnRoutingGrid      ( RoutingPad* ) const;
              bool               selectRpComponent    ( RoutingPad* ) const;
      inline  void               setRoutingStyle      ( StyleFlags );
      inline  void               resetRoutingStyle    ( StyleFlags );
      virtual void               print                ( Cell* ) const;
      virtual Record*            _getRecord           () const;
      virtual string             _getString           () const;
      virtual string             _getTypeName         () const;
    protected:
    // Attributes.
      const Layer*            _gmetalh;
      const Layer*            _gmetalv;
      const Layer*            _gcontact;
      size_t                  _gdepthv;
      size_t                  _gdepthh;
      size_t                  _ddepthv;
      size_t                  _ddepthh;
      size_t                  _ddepthc;
      std::string             _netBuilderStyle;
      StyleFlags              _routingStyle;
      CellGauge*              _cg;
      RoutingGauge*           _rg;
      std::vector<DbU::Unit>  _extensionCaps;
      float                   _saturateRatio;
      size_t                  _saturateRp;
      DbU::Unit               _globalThreshold;
      size_t                  _allowedDepth;
      DbU::Unit               _edgeLength;
      DbU::Unit               _edgeWidth;
      float                   _edgeCostH;
      float                   _edgeCostK;
      float                   _edgeHInc;
      float                   _edgeHScaling;
      int                     _globalIterations;
      std::string             _diodeName;
      DbU::Unit               _antennaGateMaxWL;
      DbU::Unit               _antennaDiodeMaxWL;
    private:
      Configuration& operator=           ( const Configuration& ) = delete;
      void           _setTopRoutingLayer ( Name name );
  };


  inline  std::string  Configuration::getNetBuilderStyle   () const { return _netBuilderStyle; }
  inline  StyleFlags   Configuration::getRoutingStyle      () const { return _routingStyle; }
  inline  bool         Configuration::isGLayer             ( const Layer* layer ) const { return isGMetal(layer) or isGContact(layer); }
  inline  size_t       Configuration::getGHorizontalDepth  () const { return _gdepthh; }
  inline  size_t       Configuration::getGVerticalDepth    () const { return _gdepthv; }
  inline  DbU::Unit    Configuration::getGHorizontalPitch  () const { return getPitch( getGHorizontalDepth(), Flags::NoFlags ); }
  inline  DbU::Unit    Configuration::getGVerticalPitch    () const { return getPitch( getGVerticalDepth  (), Flags::NoFlags ); }
  inline  size_t       Configuration::getDVerticalDepth    () const { return _ddepthv; }
  inline  const Layer* Configuration::getDVerticalLayer    () const { return getRoutingLayer( getDVerticalDepth() ); }
  inline  DbU::Unit    Configuration::getDVerticalWidth    () const { return getWireWidth   ( getDVerticalDepth() ); }
  inline  DbU::Unit    Configuration::getDPVerticalWidth   () const { return getPWireWidth  ( getDVerticalDepth() ); }
  inline  DbU::Unit    Configuration::getDVerticalPitch    () const { return getPitch       ( getDVerticalDepth(), Flags::NoFlags ); }
  inline  DbU::Unit    Configuration::getDVerticalOffset   () const { return getOffset      ( getDVerticalDepth() ); }
  inline  size_t       Configuration::getDHorizontalDepth  () const { return _ddepthh; }
  inline  const Layer* Configuration::getDHorizontalLayer  () const { return getRoutingLayer( getDHorizontalDepth() ); }
  inline  DbU::Unit    Configuration::getDHorizontalWidth  () const { return getWireWidth   ( getDHorizontalDepth() ); }
  inline  DbU::Unit    Configuration::getDPHorizontalWidth () const { return getPWireWidth  ( getDHorizontalDepth() ); }
  inline  DbU::Unit    Configuration::getDHorizontalPitch  () const { return getPitch       ( getDHorizontalDepth(), Flags::NoFlags ); }
  inline  DbU::Unit    Configuration::getDHorizontalOffset () const { return getOffset      ( getDHorizontalDepth() ); }
  inline  size_t       Configuration::getDContactDepth     () const { return _ddepthc; }
  inline  const Layer* Configuration::getDContactLayer     () const { return getContactLayer( getDContactDepth() ); }
  inline  DbU::Unit    Configuration::getDContactWidth     () const { return getWireWidth   ( getDContactDepth() ); }
  inline  DbU::Unit    Configuration::getDContactPitch     () const { return getPitch       ( getDContactDepth(), Flags::NoFlags ); }
  inline  std::string  Configuration::getDiodeName         () const { return _diodeName; }
  inline  DbU::Unit    Configuration::getAntennaGateMaxWL  () const { return _antennaGateMaxWL; }
  inline  DbU::Unit    Configuration::getAntennaDiodeMaxWL () const { return _antennaDiodeMaxWL; }
  inline  void         Configuration::setRoutingStyle      ( StyleFlags flags ) { _routingStyle  =  flags; }
  inline  void         Configuration::resetRoutingStyle    ( StyleFlags flags ) { _routingStyle &= ~flags; }


} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Configuration);
