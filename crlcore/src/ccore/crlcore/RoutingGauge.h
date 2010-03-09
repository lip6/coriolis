
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RoutingGauge.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __CRL_ROUTING_GAUGE_H__
#define __CRL_ROUTING_GAUGE_H__


class QXmlSteamReader;

#include  <string>
#include  <vector>
#include  "hurricane/Name.h"
#include  "hurricane/Slot.h"

namespace Hurricane {
  class Layer;
  class Technology;
}


namespace CRL {


  using  std::string;
  using  std::vector;

  using  Hurricane::Name;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::Technology;

  class RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "RoutingGauge".
 
  class RoutingGauge {

    public:
    // Constructors & Destructors.
      static  RoutingGauge*       create            ( const char* name );
      virtual void                destroy           ();
    // Accessors.                                   
              RoutingGauge*       getClone          () const;
      inline  const Name          getName           () const;
      inline  Technology*         getTechnology     () const;
      inline  size_t              getDepth          () const;
              size_t              getLayerDepth     ( const Layer* ) const;
              RoutingLayerGauge*  getLayerGauge     ( const Layer* ) const;
              unsigned int        getLayerDirection ( const Layer* ) const;
              RoutingLayerGauge*  getLayerGauge     ( size_t depth ) const;
              unsigned int        getLayerDirection ( size_t depth ) const;
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


} // End of CRL namespace.


#endif
