
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
// |  C++ Header  :  "./katana/TrackElements.h"                      |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACKELEMENTS_H
#define  KATANA_TRACKELEMENTS_H

#include  "anabatic/AutoSegments.h"


namespace Katana {

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

  using Anabatic::AutoSegments_Perpandiculars;

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
// Class  :  "TrackElements_Perpandiculars".

  class TrackElements_Perpandiculars : public TrackElementHC {

    public:
    // Sub-Class: Locator.
      class Locator : public TrackElementHL {
        public:
                                  Locator    ( TrackElement* segment, Flags flags );
          inline                  Locator    ( const Locator& );
          virtual TrackElement*   getElement () const;
          virtual TrackElementHL* getClone   () const;
          virtual bool            isValid    () const;
          virtual void            progress   ();
          virtual string          _getString () const;
        protected:
          AutoSegments_Perpandiculars::Locator  _locator;
          TrackElement*                         _element;
      };

    public:
    // TrackElements_Perpandiculars Methods.
      inline                  TrackElements_Perpandiculars ( TrackElement* segment, Flags flags=Flags::NoFlags );
      inline                  TrackElements_Perpandiculars ( const TrackElements_Perpandiculars& );
      virtual TrackElementHC* getClone                     () const;
	  virtual TrackElementHL* getLocator                   () const;
      virtual string         _getString                    () const;

    protected:
    // TrackElements_Perpandiculars Attributes.
      Flags          _flags;
      TrackElement*  _segment;
  };


  inline TrackElements_Perpandiculars::Locator::Locator ( const Locator& locator )
    : TrackElementHL()
    , _locator      (locator._locator)
    , _element      (NULL)
  { }


  inline TrackElements_Perpandiculars::TrackElements_Perpandiculars ( TrackElement* segment, Flags flags )
    : TrackElementHC()
    , _flags        (flags)
    , _segment      (segment)
  { }


  inline TrackElements_Perpandiculars::TrackElements_Perpandiculars ( const TrackElements_Perpandiculars& tracksegments )
    : TrackElementHC()
    , _flags        (tracksegments._flags)
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


}  // Katana namespace.


#endif  // KATANA_TRACKELEMENTS_H
