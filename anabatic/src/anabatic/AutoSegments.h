// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AutoSegments.h"                     |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOSEGMENTS_H
#define  ANABATIC_AUTOSEGMENTS_H

#include <string>
#include <list>
#include <vector>
#include <array>
#include <map>
#include "hurricane/Collection.h"
#include "hurricane/DbU.h"
#include "hurricane/Box.h"

namespace Hurricane {
  class Hook;
  class Component;
  class Contact;
  class Segment;
  class RoutingPad;
  class Net;
}


namespace Anabatic {

  using std::string;
  using std::pair;
  using std::list;
  using std::vector;
  using std::map;

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Hook;
  using Hurricane::Entity;
  using Hurricane::Component;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::RoutingPad;
  using Hurricane::Net;
  using Hurricane::Filter;
  using Hurricane::Locator;
  using Hurricane::Collection;
  using Hurricane::GenericFilter;
  using Hurricane::GenericLocator;
  using Hurricane::GenericCollection;

  class LocatorHelper;
  class AutoContact;
  class AutoContactTerminal;
  class AutoSegment;
  class GCell;


// -------------------------------------------------------------------
// Collections.

  typedef Hurricane::Filter<AutoSegment*>                 AutoSegmentHF;
  typedef Hurricane::Locator<AutoSegment*>                AutoSegmentHL;
  typedef Hurricane::Collection<AutoSegment*>             AutoSegmentHC;
  typedef GenericCollection<AutoSegment*>                 AutoSegments;
  typedef GenericLocator<AutoSegment*>                    AutoSegmentLocator;
  typedef GenericFilter<AutoSegment*>                     AutoSegmentFilter;
  typedef map<Segment*,AutoSegment*,Entity::CompareById>  AutoSegmentLut;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegmentStack".

  class AutoSegmentStack : protected list<pair<AutoContact*,AutoSegment*> > {
    public:
      inline bool         isEmpty        () const;
      inline size_t       getSize        () const;
             void         push           ( AutoContact*, AutoSegment* );
      inline void         pop            ();
      inline AutoContact* getAutoContact () const;
      inline AutoSegment* getAutoSegment () const;
  };


  inline bool         AutoSegmentStack::isEmpty        () const { return empty(); };
  inline size_t       AutoSegmentStack::getSize        () const { return size(); };
  inline void         AutoSegmentStack::pop            () { if ( !empty() ) pop_back(); };
  inline AutoContact* AutoSegmentStack::getAutoContact () const { return empty() ? NULL : back().first; };
  inline AutoSegment* AutoSegmentStack::getAutoSegment () const { return empty() ? NULL : back().second; };


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegments_OnContact".

  class AutoSegments_OnContact : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( AutoSegment* master, Contact* contact );
          inline                 Locator    ( const Locator& );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          AutoSegment* _master;
          Hook*        _hook;
          AutoSegment* _element;
      };

    public:
    // AutoSegments_OnContact Methods.
      inline                 AutoSegments_OnContact ( AutoSegment* master, Contact* contact );
      inline                 AutoSegments_OnContact ( const AutoSegments_OnContact& );
      virtual AutoSegmentHC* getClone               () const;
	  virtual AutoSegmentHL* getLocator             () const;
      virtual string         _getString             () const;

    protected:
    // AutoSegments_OnContact Attributes.
      AutoSegment*  _master;
      Contact*      _contact;
  };

  
  inline AutoSegments_OnContact::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _master(locator._master)
    , _hook(locator._hook)
    , _element(locator._element)
  { }


  inline AutoSegments_OnContact::AutoSegments_OnContact ( AutoSegment* master, Contact* contact )
    : AutoSegmentHC()
    , _master(master)
    , _contact(contact)
  { }


  inline AutoSegments_OnContact::AutoSegments_OnContact ( const AutoSegments_OnContact& segments )
    : AutoSegmentHC()
    , _master(segments._master)
    , _contact(segments._contact)
  { }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegments_OnRoutingPad".

  class AutoSegments_OnRoutingPad : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( RoutingPad* rp, const AutoContactTerminal* contact );
          inline                 Locator    ( const Locator& );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          std::array<AutoSegment*,4> _elements;
          size_t                     _index;
      };

    public:
    // AutoSegments_OnRoutingPad Methods.
                             AutoSegments_OnRoutingPad ( const AutoContact* contact );
      inline                 AutoSegments_OnRoutingPad ( const AutoSegments_OnRoutingPad& );
      virtual AutoSegmentHC* getClone                  () const;
	  virtual AutoSegmentHL* getLocator                () const;
      virtual string         _getString                () const;

    protected:
    // AutoSegments_OnRoutingPad Attributes.
            RoutingPad*          _routingPad;
      const AutoContactTerminal* _contact;
  };

  
  inline AutoSegments_OnRoutingPad::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _elements(locator._elements)
    , _index   (locator._index)
  { }


  inline AutoSegments_OnRoutingPad::AutoSegments_OnRoutingPad ( const AutoSegments_OnRoutingPad& segments )
    : AutoSegmentHC()
    , _routingPad(segments._routingPad)
    , _contact   (segments._contact)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Aligneds".

  class AutoSegments_Aligneds : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
          inline                 Locator    ( AutoSegment* segment , Flags flags );
          inline                 Locator    ( const Locator &locator );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          Flags             _flags;
          AutoSegment*      _master;
          AutoSegmentStack  _stack;
      };

    public:
    // AutoSegments_Aligneds Methods.
                             AutoSegments_Aligneds ( AutoSegment*, Flags flags );
                             AutoSegments_Aligneds ( const AutoSegments_Aligneds& );
      virtual AutoSegmentHC* getClone               () const;
	  virtual AutoSegmentHL* getLocator             () const;
      virtual string         _getString             () const;

    protected:
    // AutoSegments_Aligneds Attributes.
      Flags         _flags;
      AutoSegment*  _segment;
  };


  inline AutoSegments_Aligneds::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _flags (locator._flags)
    , _master(locator._master)
    , _stack (locator._stack)
  { }


  inline AutoSegments_Aligneds::AutoSegments_Aligneds ( AutoSegment* segment, Flags flags )
    : AutoSegmentHC()
    , _flags  (flags)
    , _segment(segment)
  { }


  inline AutoSegments_Aligneds::AutoSegments_Aligneds ( const AutoSegments_Aligneds& autosegments )
    : AutoSegmentHC()
    , _flags  (autosegments._flags)
    , _segment(autosegments._segment)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Connecteds".

  class AutoSegments_Connecteds : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
          inline                 Locator    ( AutoSegment* segment, Flags flags );
          inline                 Locator    ( const Locator &locator );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          AutoSegmentStack  _stack;
      };

    public:
    // AutoSegments_Connecteds Methods.
                             AutoSegments_Connecteds ( AutoSegment*, Flags flags );
                             AutoSegments_Connecteds ( const AutoSegments_Connecteds& );
      virtual AutoSegmentHC* getClone                () const;
	  virtual AutoSegmentHL* getLocator              () const;
      virtual string         _getString              () const;

    protected:
    // AutoSegments_Connecteds Attributes.
      Flags         _flags;
      AutoSegment*  _segment;
  };


  inline AutoSegments_Connecteds::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _stack (locator._stack)
  { }


  inline AutoSegments_Connecteds::AutoSegments_Connecteds ( AutoSegment* segment, Flags flags )
    : AutoSegmentHC()
    , _flags  (flags)
    , _segment(segment)
  { }


  inline AutoSegments_Connecteds::AutoSegments_Connecteds ( const AutoSegments_Connecteds& autosegments )
    : AutoSegmentHC()
    , _flags  (autosegments._flags)
    , _segment(autosegments._segment)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Perpandiculars".

  class AutoSegments_Perpandiculars : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( AutoSegment* master, Flags flags );
          inline                 Locator    ( const Locator& );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          Flags                _flags;
          AutoSegment*         _master;
          AutoSegmentStack     _stack;
          vector<AutoSegment*> _perpandiculars;
      };

    public:
    // AutoSegments_Perpandiculars Methods.
      inline                 AutoSegments_Perpandiculars ( AutoSegment*, Flags flags=Flags::NoFlags );
      inline                 AutoSegments_Perpandiculars ( const AutoSegments_Perpandiculars& );
      virtual AutoSegmentHC* getClone                    () const;
	  virtual AutoSegmentHL* getLocator                  () const;
      virtual string         _getString                  () const;

    protected:
    // AutoSegments_Perpandiculars Attributes.
      Flags         _flags;
      AutoSegment*  _master;
  };


  inline AutoSegments_Perpandiculars::Locator::Locator ( const Locator& locator )
    : AutoSegmentHL()
    , _flags         (locator._flags)
    , _master        (locator._master)
    , _stack         (locator._stack)
    , _perpandiculars()
  { }


  inline AutoSegments_Perpandiculars::AutoSegments_Perpandiculars
  ( AutoSegment* master, Flags flags )
    : AutoSegmentHC()
    , _flags (flags)
    , _master(master)
  { }


  inline AutoSegments_Perpandiculars::AutoSegments_Perpandiculars
  ( const AutoSegments_Perpandiculars& autosegments )
    : AutoSegmentHC()
    , _flags (autosegments._flags)
    , _master(autosegments._master)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_AnchorOnGCell".

  class AutoSegments_AnchorOnGCell : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( GCell* fcell, Flags flags );
          inline                 Locator    ( const Locator& );
          virtual               ~Locator    ();
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          Flags                                 _flags;
          vector<AutoContact*>::const_iterator  _itContact;
          vector<AutoContact*>::const_iterator  _itEnd;
          Hurricane::Locator<Hook*>*            _hookLocator;
          AutoSegment*                          _element;
      };

    public:
    // AutoSegments_Perpandiculars Methods.
      inline                 AutoSegments_AnchorOnGCell ( GCell* fcell, Flags flags );
      inline                 AutoSegments_AnchorOnGCell ( const AutoSegments_AnchorOnGCell& );
      virtual AutoSegmentHC* getClone                   () const;
	  virtual AutoSegmentHL* getLocator                 () const;
      virtual string         _getString                 () const;

    public:
    // AutoSegments_Perpandiculars Attributes.
      GCell* _fcell;
      Flags  _flags;
  };


  inline AutoSegments_AnchorOnGCell::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _flags       (locator._flags)
    , _itContact   (locator._itContact)
    , _itEnd       (locator._itEnd)
    , _hookLocator (locator._hookLocator->getClone())
    , _element     (locator._element)
  { }


  inline AutoSegments_AnchorOnGCell::AutoSegments_AnchorOnGCell ( GCell* fcell, Flags flags )
    : AutoSegmentHC()
    , _fcell(fcell)
    , _flags(flags)
  { }


  inline AutoSegments_AnchorOnGCell::AutoSegments_AnchorOnGCell
  ( const AutoSegments_AnchorOnGCell& autosegments )
    : AutoSegmentHC()
    , _fcell(autosegments._fcell)
    , _flags(autosegments._flags)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_CachedOnContact".

  class AutoSegments_CachedOnContact : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( AutoContact* sourceAnchor, Flags direction );
          inline                 Locator    ( const Locator& );
          virtual               ~Locator    ();
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          LocatorHelper* _helper;
      };

    // Constructors.
    public:
    // AutoSegments_CachedOnContact Methods.
      inline                 AutoSegments_CachedOnContact ( AutoContact* sourceContact
                                                          , Flags direction=Flags::Horizontal|Flags::Vertical );
      inline                 AutoSegments_CachedOnContact ( const AutoSegments_CachedOnContact& );
      virtual AutoSegmentHC* getClone                     () const;
	  virtual AutoSegmentHL* getLocator                   () const;
      virtual string         _getString                   () const;

    protected:
    // AutoSegments_CachedOnContact Attributes.
      Flags         _direction;
      AutoContact*  _sourceContact;

  };


  inline  AutoSegments_CachedOnContact::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _helper(locator._helper)
  { }


  inline  AutoSegments_CachedOnContact::AutoSegments_CachedOnContact
  ( AutoContact* sourceContact, Flags direction )
    : AutoSegmentHC()
    , _direction    (direction)
    , _sourceContact(sourceContact)
  {
    if (_direction & Flags::Vertical) _direction |= Flags::WithPerpands;
  }


  inline  AutoSegments_CachedOnContact::AutoSegments_CachedOnContact
  ( const AutoSegments_CachedOnContact& autosegments )
    : AutoSegmentHC()
    , _direction    (autosegments._direction)
    , _sourceContact(autosegments._sourceContact)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_IsAccountable".

  class AutoSegments_IsAccountable : public AutoSegmentHF {
    public:
      virtual AutoSegmentHF* getClone   () const;
      virtual bool           accept     ( AutoSegment* ) const;
      virtual string         _getString () const;
  };


// -------------------------------------------------------------------
// Class  :  "AutoSegments_InDirection".

  class AutoSegments_InDirection : public AutoSegmentHF {
    public:
      inline                 AutoSegments_InDirection ( Flags direction );
      inline                 AutoSegments_InDirection ( const AutoSegments_InDirection& );
      virtual AutoSegmentHF* getClone                 () const;
      virtual bool           accept                   ( AutoSegment* segment ) const;
      virtual string         _getString               () const;
    protected:
      Flags  _direction;
  };


  inline  AutoSegments_InDirection::AutoSegments_InDirection ( Flags direction )
    : AutoSegmentHF()
    , _direction(direction)
  {}


  inline  AutoSegments_InDirection::AutoSegments_InDirection ( const AutoSegments_InDirection& filter )
    : AutoSegmentHF()
    , _direction(filter._direction)
  {}


} // End of Anabatic namespace.


# endif
