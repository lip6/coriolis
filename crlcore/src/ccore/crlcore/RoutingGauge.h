// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/RoutingGauge.h"                      |
// +-----------------------------------------------------------------+


#ifndef CRL_ROUTING_GAUGE_H
#define CRL_ROUTING_GAUGE_H


class QXmlSteamReader;

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

  using  std::string;
  using  std::vector;
  using  Hurricane::Name;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "RoutingGauge".
 
  class RoutingGauge {

    public:
    // Constants.
      static  const size_t        nlayerdepth;
    // Constructors & Destructors.
      static  RoutingGauge*       create            ( const char* name );
      virtual void                destroy           ();
    // Accessors.                                   
              RoutingGauge*       getClone          () const;
      inline  const Name          getName           () const;
      inline  Technology*         getTechnology     () const;
      inline  size_t              getDepth          () const;
              RoutingLayerGauge*  getLayerGauge     ( const Layer* ) const;
              size_t              getViaDepth       ( const Layer* ) const;
              size_t              getLayerDepth     ( const Layer* ) const;
              unsigned int        getLayerType      ( const Layer* ) const;
              unsigned int        getLayerDirection ( const Layer* ) const;
              RoutingLayerGauge*  getLayerGauge     ( size_t depth ) const;
      inline  unsigned int        getLayerDirection ( size_t depth ) const;
      inline  unsigned int        getLayerType      ( size_t depth ) const;
      inline  DbU::Unit           getLayerPitch     ( size_t depth ) const;
      inline  DbU::Unit           getLayerOffset    ( size_t depth ) const;
      inline  DbU::Unit           getLayerWireWidth ( size_t depth ) const;
      inline  DbU::Unit           getViaWidth       ( size_t depth ) const;
              const Layer*        getRoutingLayer   ( size_t depth ) const;
              Layer*              getContactLayer   ( size_t depth ) const;
              const vector<RoutingLayerGauge*>&     
                                  getLayerGauges    () const;
    // Methods.
              void                addLayerGauge     ( RoutingLayerGauge* layerGauge );
              void                checkConnexity    () const;
    // Hurricane Managment.                         
      virtual Record*             _getRecord        ( Record* record=NULL ) const;
      virtual string              _getString        () const;
      virtual string              _getTypeName      () const;

    protected:
    // Internal - Attributes.
      Name                        _name;
      vector<RoutingLayerGauge*>  _layerGauges;
      vector<Layer*>              _viaLayers;
      Technology*                 _technology;

    // Internal - Constructors & Destructors.
                                  RoutingGauge ( const char* name );
                                  RoutingGauge ( const RoutingGauge& );
      virtual                    ~RoutingGauge ();
      virtual void                _preDestroy  ();
              RoutingGauge&       operator=    ( const RoutingGauge& );
  };


  inline const Name    RoutingGauge::getName           () const { return _name; }
  inline size_t        RoutingGauge::getDepth          () const { return _layerGauges.size(); }
  inline Technology*   RoutingGauge::getTechnology     () const { return _technology; }
  inline unsigned int  RoutingGauge::getLayerType      ( size_t depth ) const { return getLayerGauge(depth)->getType(); }
  inline unsigned int  RoutingGauge::getLayerDirection ( size_t depth ) const { return getLayerGauge(depth)->getDirection(); }
  inline DbU::Unit     RoutingGauge::getLayerPitch     ( size_t depth ) const { return getLayerGauge(depth)->getPitch(); }
  inline DbU::Unit     RoutingGauge::getLayerOffset    ( size_t depth ) const { return getLayerGauge(depth)->getOffset(); }
  inline DbU::Unit     RoutingGauge::getLayerWireWidth ( size_t depth ) const { return getLayerGauge(depth)->getWireWidth(); }
  inline DbU::Unit     RoutingGauge::getViaWidth       ( size_t depth ) const { return getLayerGauge(depth)->getViaWidth(); }


} // CRL namespace.

INSPECTOR_P_SUPPORT(CRL::RoutingGauge);

#endif
