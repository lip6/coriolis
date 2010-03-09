
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./AutoSegments.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_AUTOSEGMENTS_H__
#define  __KATABATIC_AUTOSEGMENTS_H__

#include  <string>
#include  <list>
#include  <vector>
#include  <map>
#include  "hurricane/Collection.h"
#include  "hurricane/DbU.h"
#include  "hurricane/Box.h"

namespace Hurricane {
  class Hook;
  class Component;
  class Contact;
  class Segment;
  class Net;
}


namespace Katabatic {


  using std::string;
  using std::pair;
  using std::list;
  using std::vector;
  using std::map;

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

  class AutoContact;
  class AutoSegment;
  class GCell;


// -------------------------------------------------------------------
// Collections.


  typedef Hurricane::Filter<AutoSegment*>     AutoSegmentHF;
  typedef Hurricane::Locator<AutoSegment*>    AutoSegmentHL;
  typedef Hurricane::Collection<AutoSegment*> AutoSegmentHC;
  typedef GenericCollection<AutoSegment*>     AutoSegments;
  typedef GenericLocator<AutoSegment*>        AutoSegmentLocator;
  typedef GenericFilter<AutoSegment*>         AutoSegmentFilter;
  typedef map<Segment*,AutoSegment*>          AutoSegmentLut;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegmentStack".


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
// Class  :  "Katabatic::AutoSegments_OnContact".


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
// Class  :  "AutoSegments_Collapsed".


  class AutoSegments_Collapsed : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
          inline                 Locator    ( AutoSegment* segment , bool withPerpand );
          inline                 Locator    ( const Locator &locator );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          bool              _withPerpand;
          AutoSegment*      _master;
          AutoSegmentStack  _stack;
      };

    public:
    // AutoSegments_Collapsed Methods.
                             AutoSegments_Collapsed ( AutoSegment*, bool withPerpand=false );
                             AutoSegments_Collapsed ( const AutoSegments_Collapsed& );
      virtual AutoSegmentHC* getClone               () const;
	  virtual AutoSegmentHL* getLocator             () const;
      virtual string         _getString             () const;

    protected:
    // AutoSegments_Collapsed Attributes.
      bool          _withPerpand;
      AutoSegment*  _segment;
  };


  inline AutoSegments_Collapsed::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _withPerpand(locator._withPerpand)
    , _master(locator._master)
    , _stack(locator._stack)
  { }


  inline AutoSegments_Collapsed::AutoSegments_Collapsed ( AutoSegment* segment, bool withPerpand )
    : AutoSegmentHC()
    , _withPerpand(withPerpand)
    , _segment(segment)
  { }


  inline AutoSegments_Collapsed::AutoSegments_Collapsed ( const AutoSegments_Collapsed& autosegments )
    : AutoSegmentHC()
    , _withPerpand(autosegments._withPerpand)
    , _segment(autosegments._segment)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_CollapsedPerpandicular".


  class AutoSegments_CollapsedPerpandicular : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( AutoSegment* segment );
          inline                 Locator    ( const Locator& );
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          AutoSegment*         _master;
          AutoSegmentStack     _stack;
          vector<AutoSegment*> _perpandiculars;
      };

    public:
    // AutoSegments_CollapsedPerpandicular Methods.
      inline                 AutoSegments_CollapsedPerpandicular ( AutoSegment* segment );
      inline                 AutoSegments_CollapsedPerpandicular ( const AutoSegments_CollapsedPerpandicular& );
      virtual AutoSegmentHC* getClone                            () const;
	  virtual AutoSegmentHL* getLocator                          () const;
      virtual string         _getString                          () const;

    protected:
    // AutoSegments_CollapsedPerpandicular Attributes.
      AutoSegment*  _segment;
  };


  inline AutoSegments_CollapsedPerpandicular::Locator::Locator ( const Locator& locator )
    : AutoSegmentHL()
    , _master(locator._master)
    , _stack(locator._stack)
    , _perpandiculars()
  { }


  inline AutoSegments_CollapsedPerpandicular::AutoSegments_CollapsedPerpandicular
  ( AutoSegment* segment )
    : AutoSegmentHC()
    , _segment(segment)
  { }


  inline AutoSegments_CollapsedPerpandicular::AutoSegments_CollapsedPerpandicular
  ( const AutoSegments_CollapsedPerpandicular& autosegments )
    : AutoSegmentHC()
    , _segment(autosegments._segment)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_AnchorOnGCell".


  class AutoSegments_AnchorOnGCell : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( GCell* fcell, bool sourceAnchor, unsigned int direction );
          inline                 Locator    ( const Locator& );
          virtual               ~Locator    ();
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          bool                                  _sourceAnchor;
          unsigned int                          _direction;
          vector<AutoContact*>::const_iterator  _itContact;
          vector<AutoContact*>::const_iterator  _itEnd;
          Hurricane::Locator<Hook*>*            _hookLocator;
          AutoSegment*                          _element;
      };

    public:
    // AutoSegments_CollapsedPerpandicular Methods.
      inline                 AutoSegments_AnchorOnGCell ( GCell* fcell, bool sourceAnchor, unsigned int direction );
      inline                 AutoSegments_AnchorOnGCell ( const AutoSegments_AnchorOnGCell& );
      virtual AutoSegmentHC* getClone                   () const;
	  virtual AutoSegmentHL* getLocator                 () const;
      virtual string         _getString                 () const;

    public:
    // AutoSegments_CollapsedPerpandicular Attributes.
      GCell*        _fcell;
      unsigned int  _direction;
      bool          _sourceAnchor;
  };


  inline AutoSegments_AnchorOnGCell::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _sourceAnchor(locator._sourceAnchor)
    , _direction(locator._direction)
    , _itContact(locator._itContact)
    , _itEnd(locator._itEnd)
    , _hookLocator(locator._hookLocator->getClone())
    , _element(locator._element)
  { }


  inline AutoSegments_AnchorOnGCell::AutoSegments_AnchorOnGCell
  ( GCell* fcell, bool sourceAnchor, unsigned int direction )
    : AutoSegmentHC()
    , _fcell(fcell)
    , _direction(direction)
    , _sourceAnchor(sourceAnchor)
  { }


  inline AutoSegments_AnchorOnGCell::AutoSegments_AnchorOnGCell
  ( const AutoSegments_AnchorOnGCell& autosegments )
    : AutoSegmentHC()
    , _fcell(autosegments._fcell)
    , _direction(autosegments._direction)
    , _sourceAnchor(autosegments._sourceAnchor)
  { }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_AnchoredBySource".


  class AutoSegments_AnchoredBySource : public AutoSegmentHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoSegmentHL {
        public:
                                 Locator    ( AutoContact* sourceAnchor, unsigned int direction );
          inline                 Locator    ( const Locator& );
          virtual               ~Locator    ();
          virtual AutoSegment*   getElement () const;
          virtual AutoSegmentHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          unsigned int                      _direction;
          Hurricane::Locator<AutoContact*>* _contactLocator;
          Hurricane::Locator<Hook*>*        _hookLocator;
          AutoSegment*                      _element;
      };

    // Constructors.
    public:
    // AutoSegments_AnchoredBySource Methods.
      inline                 AutoSegments_AnchoredBySource ( AutoContact* sourceContact, unsigned int direction );
      inline                 AutoSegments_AnchoredBySource ( const AutoSegments_AnchoredBySource& );
      virtual AutoSegmentHC* getClone                      () const;
	  virtual AutoSegmentHL* getLocator                    () const;
      virtual string         _getString                    () const;

    protected:
    // AutoSegments_AnchoredBySource Attributes.
      unsigned int  _direction;
      AutoContact*  _sourceContact;

  };


  inline  AutoSegments_AnchoredBySource::Locator::Locator ( const Locator &locator )
    : AutoSegmentHL()
    , _direction(locator._direction)
    , _contactLocator(locator._contactLocator->getClone())
    , _hookLocator(locator._hookLocator->getClone())
    , _element(locator._element)
  { }


  inline  AutoSegments_AnchoredBySource::AutoSegments_AnchoredBySource
  ( AutoContact* sourceContact, unsigned int direction )
    : AutoSegmentHC()
    , _direction(direction)
    , _sourceContact(sourceContact)
  { }


  inline  AutoSegments_AnchoredBySource::AutoSegments_AnchoredBySource
  ( const AutoSegments_AnchoredBySource& autosegments )
    : AutoSegmentHC()
    , _direction(autosegments._direction)
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
      inline                 AutoSegments_InDirection ( unsigned int direction );
      inline                 AutoSegments_InDirection ( const AutoSegments_InDirection& );
      virtual AutoSegmentHF* getClone                 () const;
      virtual bool           accept                   ( AutoSegment* segment ) const;
      virtual string         _getString               () const;
    protected:
      unsigned int  _direction;
  };


  inline  AutoSegments_InDirection::AutoSegments_InDirection ( unsigned int direction )
    : AutoSegmentHF()
    , _direction(_direction)
  {}


  inline  AutoSegments_InDirection::AutoSegments_InDirection ( const AutoSegments_InDirection& filter )
    : AutoSegmentHF()
    , _direction(filter._direction)
  {}


} // End of Katabatic namespace.


# endif
