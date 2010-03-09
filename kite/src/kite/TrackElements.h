
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./TrackElements.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_TRACKELEMENTS_H__
#define  __KITE_TRACKELEMENTS_H__

#include  "katabatic/AutoSegments.h"


namespace Kite {


  using std::string;
  using std::set;

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Hook;
  using Hurricane::Component;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::Net;
  using Hurricane::Filter;
  using Hurricane::Locator;
  using Hurricane::Collection;
  using Hurricane::GenericFilter;
  using Hurricane::GenericLocator;
  using Hurricane::GenericCollection;

  using Katabatic::AutoSegments_CollapsedPerpandicular;

  class TrackElement;


// -------------------------------------------------------------------
// Collections.


  typedef Hurricane::Filter<TrackElement*>     TrackElementHF;
  typedef Hurricane::Locator<TrackElement*>    TrackElementHL;
  typedef Hurricane::Collection<TrackElement*> TrackElementHC;
  typedef GenericCollection<TrackElement*>     TrackElements;
  typedef GenericLocator<TrackElement*>        TrackElementLocator;
  typedef GenericFilter<TrackElement*>         TrackElementFilter;


// -------------------------------------------------------------------
// Class  :  "TrackElements_CollapsedPerpandicular".


  class TrackElements_CollapsedPerpandicular : public TrackElementHC {

    public:
    // Sub-Class: Locator.
      class Locator : public TrackElementHL {
        public:
                                  Locator    ( TrackElement* segment );
          inline                  Locator    ( const Locator& );
          virtual TrackElement*   getElement () const;
          virtual TrackElementHL* getClone   () const;
          virtual bool            isValid    () const;
          virtual void            progress   ();
          virtual string          _getString () const;
        protected:
          AutoSegments_CollapsedPerpandicular::Locator  _locator;
          TrackElement*                                 _element;
      };

    public:
    // TrackElements_CollapsedPerpandicular Methods.
      inline                  TrackElements_CollapsedPerpandicular ( TrackElement* segment );
      inline                  TrackElements_CollapsedPerpandicular ( const TrackElements_CollapsedPerpandicular& );
      virtual TrackElementHC* getClone                            () const;
	  virtual TrackElementHL* getLocator                          () const;
      virtual string         _getString                           () const;

    protected:
    // TrackElements_CollapsedPerpandicular Attributes.
      TrackElement*  _segment;
  };


  inline TrackElements_CollapsedPerpandicular::Locator::Locator ( const Locator& locator )
    : TrackElementHL()
    , _locator      (locator._locator)
    , _element      (NULL)
  { }


  inline TrackElements_CollapsedPerpandicular::TrackElements_CollapsedPerpandicular
  ( TrackElement* segment )
    : TrackElementHC()
    , _segment      (segment)
  { }


  inline TrackElements_CollapsedPerpandicular::TrackElements_CollapsedPerpandicular
  ( const TrackElements_CollapsedPerpandicular& tracksegments )
    : TrackElementHC()
    , _segment      (tracksegments._segment)
  { }


// -------------------------------------------------------------------
// Class  :  "TrackElements_UniqCanonical".


  class TrackElements_UniqCanonical : public TrackElementHF {
    public:
      inline                  TrackElements_UniqCanonical ( set<TrackElement*>& );
      inline                  TrackElements_UniqCanonical ( const TrackElements_UniqCanonical& );
      virtual TrackElementHF* getClone                    () const;
      virtual bool            accept                      ( TrackElement* segment ) const;
      virtual string          _getString                  () const;
    private:
      set<TrackElement*>& _canonicals;
  };


  inline  TrackElements_UniqCanonical::TrackElements_UniqCanonical ( set<TrackElement*>& canonicals )
    : TrackElementHF()
    , _canonicals(canonicals)
  {}


  inline  TrackElements_UniqCanonical::TrackElements_UniqCanonical ( const TrackElements_UniqCanonical& filter )
    : TrackElementHF()
    , _canonicals(filter._canonicals)
  {}


} // End of Kite namespace.


#endif  // __KITE_TRACKELEMENTS__
