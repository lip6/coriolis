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
// Class  :  "Anabatic::Configuration".

  class Configuration {
    public:
    // Constructor & Destructor.
                                 Configuration       ( const CellGauge* cg=NULL, const RoutingGauge* rg=NULL );
                                 Configuration       ( const Configuration& );
      virtual                   ~Configuration       ();
      virtual Configuration*     clone               () const;
    // Methods.
              bool               isGMetal            ( const Layer* ) const;
              bool               isGContact          ( const Layer* ) const;
              const Layer*       getGContactLayer    () const;
              const Layer*       getGHorizontalLayer () const;
              const Layer*       getGVerticalLayer   () const;
              size_t             getDepth            () const;
              size_t             getAllowedDepth     () const;
              size_t             getLayerDepth       ( const Layer* ) const;
              CellGauge*         getCellGauge        () const;
              RoutingGauge*      getRoutingGauge     () const;
              RoutingLayerGauge* getLayerGauge       ( size_t depth ) const;
              const Layer*       getRoutingLayer     ( size_t depth ) const;
              Layer*             getContactLayer     ( size_t depth ) const;
              DbU::Unit          getSliceHeight      () const;
              DbU::Unit          getSliceStep        () const;
              DbU::Unit          getPitch            ( size_t depth, Flags flags ) const;
              DbU::Unit          getOffset           ( size_t depth ) const;
              DbU::Unit          getWireWidth        ( size_t depth ) const;
              DbU::Unit          getExtensionCap     ( size_t depth ) const;
              Flags              getDirection        ( size_t depth ) const;
              DbU::Unit          getPitch            ( const Layer*, Flags flags ) const;
              DbU::Unit          getOffset           ( const Layer* ) const;
              DbU::Unit          getWireWidth        ( const Layer* ) const;
              DbU::Unit          getExtensionCap     ( const Layer* ) const;
              Flags              getDirection        ( const Layer* ) const;
              float              getSaturateRatio    () const;
              size_t             getSaturateRp       () const;
              DbU::Unit          getGlobalThreshold  () const;
              void               setAllowedDepth     ( size_t );
              void               setSaturateRatio    ( float );
              void               setSaturateRp       ( size_t );
              void               setGlobalThreshold  ( DbU::Unit );
              DbU::Unit          getEdgeLength       () const;
              DbU::Unit          getEdgeWidth        () const;
              float              getEdgeCostH        () const;
              float              getEdgeCostK        () const;
              float              getEdgeHInc         () const;
      virtual void               print               ( Cell* ) const;
      virtual Record*            _getRecord          () const;
      virtual string             _getString          () const;
      virtual string             _getTypeName        () const;
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
      DbU::Unit               _edgeLength;
      DbU::Unit               _edgeWidth;
      float                   _edgeCostH;
      float                   _edgeCostK;
      float                   _edgeHInc;
    private:
      Configuration& operator=           ( const Configuration& ) = delete;
      void           _setTopRoutingLayer ( Name name );
  };


} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::Configuration);

#endif  // ANABATIC_CONFIGURATION_H
