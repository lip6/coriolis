
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/Tracks.h"                               |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACKS_H
#define  KATANA_TRACKS_H

#include <string>
#include "hurricane/Collection.h"
#include "hurricane/Interval.h"


namespace Katana {

  using  std::string;
  using  Hurricane::_TName;
  using  Hurricane::Locator;
  using  Hurricane::Collection;
  using  Hurricane::GenericLocator;
  using  Hurricane::GenericCollection;
  using  Hurricane::GenericFilter;
  using  Hurricane::Interval;

  class Track;
  class RoutingPlane;

  typedef GenericCollection<Track*> Tracks;
  typedef GenericLocator<Track*>    TrackLocator;
  typedef GenericFilter<Track*>     TrackFilter;


// -------------------------------------------------------------------
// Class  :  "Tracks_Range".

  class Tracks_Range: public Collection<Track*> {

    public:
    // Locator Sub-Class.
      class Locator : public Hurricane::Locator<Track*> {
        public:
                                              Locator    ( const RoutingPlane* routingPlane
                                                         , const Interval&     constraints );
                                              Locator    ( const Locator& );
          virtual Hurricane::Locator<Track*>* getClone   () const;
          virtual bool                        isValid    () const;
          virtual Track*                      getElement () const;
          virtual void                        progress   ();
          virtual string                      _getString () const;

        protected:
          const Interval  _constraints;
          Track*          _track;
      };

    public:
    // Tracks_Range Methods.
      static  Tracks                      get          ( RoutingPlane* routingPlane
                                                       , Interval&     constraints );
                                          Tracks_Range ( const RoutingPlane* routingPlane
                                                       , const Interval&     constraints );
                                          Tracks_Range ( const Tracks_Range& );
      virtual Collection<Track*>*         getClone     () const;
	  virtual Hurricane::Locator<Track*>* getLocator   () const;
      virtual string                      _getString   () const;

    protected:
    // Tracks_Range Attributes.
      const RoutingPlane*  _routingPlane;
      const Interval       _constraints;
  };


// -------------------------------------------------------------------
// Class  :  "Tracks_Spiral".

  class Tracks_Spiral : public Collection<Track*> {

    public:
    // Locator Sub-Class.
      class Locator : public Hurricane::Locator<Track*> {
        public:
                                              Locator    ( const RoutingPlane* routingPlane
                                                         , const Interval&     optimal
                                                         , const Interval&     constraints );
                                              Locator    ( const Locator& );
          virtual Hurricane::Locator<Track*>* getClone   () const;
          virtual bool                        isValid    () const;
          virtual Track*                      getElement () const;
          virtual void                        progress   ();
                  bool                        InOptimal  () const;
          virtual string                      _getString () const;

        protected:
          const Interval  _optimal;
          const Interval  _constraints;
          Track*          _minTrack;
          Track*          _maxTrack;
          bool            _onMin;
          bool            _inMinOptimal;
          bool            _inMaxOptimal;
      };

    public:
    // Tracks_Spiral Methods.
      static  Tracks                      get           ( RoutingPlane* routingPlane
                                                        , Interval&     optimal
                                                        , Interval&     constraints );
                                          Tracks_Spiral ( const RoutingPlane* routingPlane
                                                        , const Interval&     optimal
                                                        , const Interval&     constraints );
                                          Tracks_Spiral ( const Tracks_Spiral& );
      virtual Collection<Track*>*         getClone      () const;
	  virtual Hurricane::Locator<Track*>* getLocator    () const;
      virtual string                      _getString    () const;

    protected:
    // Tracks_Spiral Attributes.
      const RoutingPlane*  _routingPlane;
      const Interval       _optimal;
      const Interval       _constraints;
  };


}  // Anabatic namespace.


#endif  // KATANA_TRACKS_H
