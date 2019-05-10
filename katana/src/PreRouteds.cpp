// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BuildPreRouteds.cpp"                    |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "katana/RoutingPlane.h"
#include "katana/TrackFixedSegment.h"
#include "katana/Track.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::Warning;
  using Hurricane::Error;


  void  KatanaEngine::setFixedPreRouted ()
  {
    for ( size_t depth=0 ; depth<_routingPlanes.size() ; ++depth ) {
      RoutingPlane* rp = _routingPlanes[depth];
      if (rp->getLayerGauge()->getType() == Constant::PinOnly ) continue;
      if (rp->getLayerGauge()->getDepth() > getConfiguration()->getAllowedDepth() ) continue;

      size_t tracksSize = rp->getTracksSize();
      for ( size_t itrack=0 ; itrack<tracksSize ; ++itrack ) {
        Track* track = rp->getTrackByIndex( itrack );

        for ( size_t ielement=0 ; ielement<track->getSize() ; ++ielement ) {
          TrackElement* element = track->getSegment( ielement );
         
          if (element->getNet() == NULL) continue;
          element->setRouted();
        }
      }
    }
  }


} // Katana namespace.
