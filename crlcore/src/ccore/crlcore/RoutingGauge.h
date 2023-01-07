// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/RoutingGauge.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <vector>
#include "hurricane/Name.h"
#include "hurricane/Slot.h"

namespace Hurricane {
  class Layer;
  class Technology;
}

#include "crlcore/RoutingLayerGauge.h"

namespace CRL {

  using std::string;
  using std::vector;
  using Hurricane::JsonObject;
  using Hurricane::JsonStack;
  using Hurricane::Initializer;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::Layer;
  using Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "RoutingGauge".
 
  class RoutingGauge {

    public:
    // Constants.
      static  const size_t        nlayerdepth;
    // Constructors & Destructors.
      static  RoutingGauge*       create                ( const char* name );
      virtual void                destroy               ();
    // Predicates.                                       
      inline  bool                isSymbolic            () const;
      inline  bool                isTwoMetals           () const;
      inline  bool                isSuperPitched        () const;
      inline  bool                isHV                  () const;
      inline  bool                isVH                  () const;
      inline  bool                hasPowerSupply        () const;
              bool                hasLayer              ( const Layer* ) const;
    // Accessors.                                       
              RoutingGauge*       getClone              () const;
      inline  const Name          getName               () const;
      inline  Technology*         getTechnology         () const;
      inline  size_t              getDepth              () const;
      inline  size_t              getUsableLayers       () const;
      inline  size_t              getFirstRoutingLayer  () const;
      inline  size_t              toRoutingDepth        ( size_t depth ) const;
      inline  DbU::Unit           getHorizontalPitch    () const;
      inline  DbU::Unit           getVerticalPitch      () const;
              RoutingLayerGauge*  getFirstRoutingGauge  () const;
              RoutingLayerGauge*  getHorizontalGauge    () const;
              RoutingLayerGauge*  getVerticalGauge      () const;
              RoutingLayerGauge*  getPowerSupplyGauge   () const;
              RoutingLayerGauge*  getLayerGauge         ( const Layer* ) const;
              size_t              getViaDepth           ( const Layer* ) const;
              size_t              getLayerDepth         ( const Layer* ) const;
              unsigned int        getLayerType          ( const Layer* ) const;
              unsigned int        getLayerDirection     ( const Layer* ) const;
              DbU::Unit           getPitch              ( const Layer* ) const;
              DbU::Unit           getOffset             ( const Layer* ) const;
              DbU::Unit           getWireWidth          ( const Layer* ) const;
              DbU::Unit           getPWireWidth         ( const Layer* ) const;
              DbU::Unit           getViaWidth           ( const Layer* ) const;
              RoutingLayerGauge*  getLayerGauge         ( size_t depth ) const;
              const Layer*        getRoutingLayer       ( size_t depth ) const;
              Layer*              getContactLayer       ( size_t depth ) const;
      inline  unsigned int        getLayerDirection     ( size_t depth ) const;
      inline  unsigned int        getLayerType          ( size_t depth ) const;
      inline  DbU::Unit           getLayerPitch         ( size_t depth ) const;
      inline  DbU::Unit           getLayerOffset        ( size_t depth ) const;
      inline  DbU::Unit           getLayerWireWidth     ( size_t depth ) const;
      inline  DbU::Unit           getLayerPWireWidth    ( size_t depth ) const;
      inline  DbU::Unit           getViaWidth           ( size_t depth ) const;
              const vector<RoutingLayerGauge*>&
                                  getLayerGauges        () const;
    // Methods.                                         
              void                addLayerGauge         ( RoutingLayerGauge* layerGauge );
              void                checkConnexity        () const;
      inline  void                setSymbolic           ( bool );
    // Hurricane Managment.                             
              void                toJson                ( JsonWriter* ) const;
      virtual Record*             _getRecord            ( Record* record=NULL ) const;
      virtual string              _getString            () const;
      virtual string              _getTypeName          () const;

    protected:
    // Internal - Attributes.
      Name                        _name;
      vector<RoutingLayerGauge*>  _layerGauges;
      vector<Layer*>              _viaLayers;
      Technology*                 _technology;
      bool                        _isSymbolic;
      bool                        _isSuperPitched;
      size_t                      _firstRoutingLayer;
      size_t                      _usableLayers;

    // Internal - Constructors & Destructors.
                                  RoutingGauge ( const char* name );
                                  RoutingGauge ( const RoutingGauge& );
      virtual                    ~RoutingGauge ();
      virtual void                _preDestroy  ();
              RoutingGauge&       operator=    ( const RoutingGauge& );
  };


  inline bool               RoutingGauge::isSymbolic            () const { return _isSymbolic; }
  inline bool               RoutingGauge::isSuperPitched        () const { return _isSuperPitched; }
  inline bool               RoutingGauge::isTwoMetals           () const { return (_usableLayers < 3); }
  inline bool               RoutingGauge::isHV                  () const { return getFirstRoutingGauge()->isHorizontal(); }
  inline bool               RoutingGauge::isVH                  () const { return getFirstRoutingGauge()->isVertical(); }
  inline bool               RoutingGauge::hasPowerSupply        () const { return (getPowerSupplyGauge() != NULL); }
  inline const Name         RoutingGauge::getName               () const { return _name; }
  inline Technology*        RoutingGauge::getTechnology         () const { return _technology; }
  inline size_t             RoutingGauge::getFirstRoutingLayer  () const { return _firstRoutingLayer; }
  inline size_t             RoutingGauge::getUsableLayers       () const { return _usableLayers; }
  inline size_t             RoutingGauge::toRoutingDepth        ( size_t depth ) const { return _firstRoutingLayer+depth; }
  inline DbU::Unit          RoutingGauge::getHorizontalPitch    () const { return getHorizontalGauge()->getPitch(); }
  inline DbU::Unit          RoutingGauge::getVerticalPitch      () const { return getVerticalGauge  ()->getPitch(); }
  inline size_t             RoutingGauge::getDepth              () const { return _layerGauges.size(); }
  inline DbU::Unit          RoutingGauge::getViaWidth           ( size_t depth ) const { return getLayerGauge(depth)->getViaWidth(); }
  inline unsigned int       RoutingGauge::getLayerType          ( size_t depth ) const { return getLayerGauge(depth)->getType(); }
  inline unsigned int       RoutingGauge::getLayerDirection     ( size_t depth ) const { return getLayerGauge(depth)->getDirection(); }
  inline DbU::Unit          RoutingGauge::getLayerPitch         ( size_t depth ) const { return getLayerGauge(depth)->getPitch(); }
  inline DbU::Unit          RoutingGauge::getLayerOffset        ( size_t depth ) const { return getLayerGauge(depth)->getOffset(); }
  inline DbU::Unit          RoutingGauge::getLayerWireWidth     ( size_t depth ) const { return getLayerGauge(depth)->getWireWidth(); }
  inline DbU::Unit          RoutingGauge::getLayerPWireWidth    ( size_t depth ) const { return getLayerGauge(depth)->getPWireWidth(); }
  inline void               RoutingGauge::setSymbolic           ( bool state ) { _isSymbolic=state; }


// -------------------------------------------------------------------
// Class  :  "JsonRoutingGauge".

  class JsonRoutingGauge : public JsonObject {
    public:
      static  void              initialize       ();
                                JsonRoutingGauge ( unsigned long flags );
      virtual string            getTypeName      () const;
      virtual JsonRoutingGauge* clone            ( unsigned long flags ) const;
      virtual void              toData           ( JsonStack& );
  };


} // CRL namespace.

INSPECTOR_P_SUPPORT(CRL::RoutingGauge);
