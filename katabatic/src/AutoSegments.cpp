
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
// |  C++ Module  :       "./AutoSegments.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"


namespace Katabatic {


  using namespace std;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::_TName;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::Hook;
  using Hurricane::Contact;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegmentStack".


  void  AutoSegmentStack::push ( AutoContact* contact, AutoSegment* segment )
  {
    ltrace(80) << "Stacking " << contact << " + " << segment << endl;

    push_back(make_pair(contact,segment));
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegments_OnContact".


  AutoSegments_OnContact::Locator::Locator ( AutoSegment* master, Contact* contact )
    : AutoSegmentHL()
    , _master(master)
    , _element(NULL)
  {
    _hook = contact->getBodyHook()->getPreviousMasterHook();
    progress ();
  }


  AutoSegmentHL* AutoSegments_OnContact::Locator::getClone () const
  {
    return new Locator(*this);
  }


  AutoSegment* AutoSegments_OnContact::Locator::getElement () const
  {
    return _element;
  }


  bool  AutoSegments_OnContact::Locator::isValid () const
  {
    return !_hook;
  }


  void  AutoSegments_OnContact::Locator::progress ()
  {
    ltrace(80) << "AutoSegments_OnContact::Locator::progress()" << endl;

    while ( _hook && !_hook->isMaster() ) {
      _hook    = _hook->getNextHook(); 
      _element = NULL;

      if ( _hook->isMaster() ) { _hook = NULL; break; }

      Segment* segment = dynamic_cast<Segment*>( _hook->getComponent() );
      if ( segment ) _element = Session::lookup ( segment );

      if ( !_element || (_element == _master) ) continue;

      break;
    }
  }


  string  AutoSegments_OnContact::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_OnContact::Locator")
                   + getString(_element)
                   + ">";
    return s;
  }


  AutoSegmentHC* AutoSegments_OnContact::getClone () const
  {
    return new AutoSegments_OnContact(*this);
  }


  AutoSegmentHL* AutoSegments_OnContact::getLocator () const
  {
    return new Locator(_master,_contact);
  }


  string  AutoSegments_OnContact::_getString () const
  {
    string s = "<" + _TName("AutoSegments_OnContact") + " "
                   + getString(_master)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Collapsed".


  AutoSegments_Collapsed::Locator::Locator ( AutoSegment* segment, bool withPerpand )
    : AutoSegmentHL()
    , _withPerpand(withPerpand)
    , _master(segment)
    , _stack()
  {
    if ( not _master ) return;

    AutoContact* contact = segment->getAutoSource();
    if ( contact ) _stack.push ( contact, segment );

    contact = segment->getAutoTarget();
    if ( contact ) _stack.push ( contact, segment );

    progress ();
  }


  AutoSegmentHL* AutoSegments_Collapsed::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  AutoSegments_Collapsed::Locator::isValid () const
  {
    return !_stack.isEmpty();
  }


  void  AutoSegments_Collapsed::Locator::progress ()
  {
    ltrace(80) << "AutoSegments_Collapsed::Locator::progress()" << endl;

    while ( !_stack.isEmpty() ) {
      AutoContact* sourceContact = _stack.getAutoContact ();
      AutoSegment* sourceSegment = _stack.getAutoSegment ();

      _stack.pop ();

      forEach ( Component*, component, sourceContact->getSlaveComponents() ) {
        if ( *component == sourceSegment->getSegment() ) continue;

        Segment* segment = dynamic_cast<Segment*>(*component);
        if ( !segment ) continue;

        AutoSegment* currentSegment = Session::lookup ( segment );
        if ( !currentSegment ) {
          cerr << Error("Can't lookup <AutoSegment> for %s.",getString(segment).c_str()) << endl;
          continue;
        }

        unsigned int  state = AutoSegment::getPerpandicularState ( sourceContact
                                                                 , sourceSegment
                                                                 , currentSegment
                                                                 , _master
                                                                 );
        if ( state & (AutoSegment::PerpandicularIndirect
                     |AutoSegment::ParallelOrExpanded
                     |AutoSegment::ParallelAndLayerChange ) ) {
          ltrace(98) << "Reject: " << currentSegment << endl;
          continue;
        }

        AutoContact* targetContact = currentSegment->getOppositeAnchor ( sourceContact );
        if ( targetContact ) _stack.push ( targetContact, currentSegment );
      }

      if ( _stack.isEmpty() ) break;
      if ( _stack.getAutoSegment() == _master ) continue;
      if ( AutoSegment::areAligneds(_stack.getAutoSegment(),_master) || _withPerpand ) break;
    }
  }


  string  AutoSegments_Collapsed::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Collapsed::Locator")
      + ">";

    return s;
  }


  AutoSegmentHC* AutoSegments_Collapsed::getClone () const
  {
    return new AutoSegments_Collapsed(*this);
  }


  AutoSegmentHL* AutoSegments_Collapsed::getLocator () const
  {
    return new Locator(_segment,_withPerpand);
  }


  AutoSegment* AutoSegments_Collapsed::Locator::getElement () const
  {
    return _stack.getAutoSegment();
  }


  string  AutoSegments_Collapsed::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Collapsed") + " "
                   + getString(_segment)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_CollapsedPerpandicular".


  AutoSegments_CollapsedPerpandicular::Locator::Locator ( AutoSegment* segment )
    : AutoSegmentHL()
    , _master(segment)
    , _stack()
    , _perpandiculars()
  {
    ltrace(80) << "AutoSegments_CollapsedPerpandicular::Locator::Locator()" << endl;
    ltrace(80) << "  " << _master << endl;

    if ( not _master ) return;

    AutoContact* contact = segment->getAutoSource();
    if ( contact ) _stack.push ( contact, segment );

    contact = segment->getAutoTarget();
    if ( contact ) _stack.push ( contact, segment );

    progress ();
  }


  AutoSegment* AutoSegments_CollapsedPerpandicular::Locator::getElement () const
  {
    if ( _perpandiculars.empty() ) return NULL;
    return _perpandiculars.back ();
  }


  void  AutoSegments_CollapsedPerpandicular::Locator::progress ()
  {
    ltrace(80) << "AutoSegments_CollapsedPerpandicular::Locator::progress()" << endl;

    if ( !_perpandiculars.empty() ) _perpandiculars.pop_back ();
    if ( !_perpandiculars.empty() ) return;

    while ( !_stack.isEmpty() ) {
      AutoContact* sourceContact = _stack.getAutoContact ();
      AutoSegment* sourceSegment = _stack.getAutoSegment ();

      _stack.pop ();

      forEach ( Component*, component, sourceContact->getSlaveComponents() ) {
        if ( *component == sourceSegment->getSegment() ) continue;

        Segment* segment = dynamic_cast<Segment*>(*component);
        if ( !segment ) continue;

        AutoSegment* currentSegment = Session::lookup ( segment );
        if ( !currentSegment ) {
          cerr << Error("Can't lookup <AutoSegment> for %s.",getString(segment).c_str()) << endl;
          continue;
        }

        ltrace(99) << "  Try Perpandicular: " << currentSegment << endl;
        unsigned int state = AutoSegment::getPerpandicularState ( sourceContact
                                                                , sourceSegment
                                                                , currentSegment
                                                                , _master
                                                                );

        if ( state & AutoSegment::PerpandicularAny ) {
          _perpandiculars.push_back ( currentSegment );
          ltrace(99) << "Stacked Perpandicular: " << currentSegment << endl;
        }
        if ( state & (AutoSegment::PerpandicularIndirect
                     |AutoSegment::ParallelOrExpanded
                     |AutoSegment::ParallelAndLayerChange ) )
          continue;

        ltrace(99) << "Stacked Opposite of: " << currentSegment << endl;

        AutoContact* targetContact = currentSegment->getOppositeAnchor ( sourceContact );
        if ( targetContact ) _stack.push ( targetContact, currentSegment );
      }

      if ( _stack.isEmpty() ) break;
      if ( _stack.getAutoSegment() == _master ) continue;
      if ( !_perpandiculars.empty() ) break;
    }
  }


  AutoSegmentHL* AutoSegments_CollapsedPerpandicular::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  AutoSegments_CollapsedPerpandicular::Locator::isValid () const
  {
    return !_perpandiculars.empty();
  }


  AutoSegmentHC* AutoSegments_CollapsedPerpandicular::getClone () const
  {
    return new AutoSegments_CollapsedPerpandicular(*this);
  }


  AutoSegmentHL* AutoSegments_CollapsedPerpandicular::getLocator () const
  {
    return new Locator(_segment);
  }


  string  AutoSegments_CollapsedPerpandicular::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_CollapsedPerpandicular::Locator")
                   + ">";
    return s;
  }


  string  AutoSegments_CollapsedPerpandicular::_getString () const
  {
    string s = "<" + _TName("AutoSegments_CollapsedPerpandicular") + " "
                   + getString(_segment)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_AnchorOnGCell".


  AutoSegments_AnchorOnGCell::Locator::Locator ( GCell* fcell, bool sourceAnchor, unsigned int direction )
    : AutoSegmentHL()
    , _sourceAnchor(sourceAnchor)
    , _direction(direction)
    , _itContact(fcell->getContacts()->begin())
    , _itEnd(fcell->getContacts()->end())
    , _hookLocator(NULL)
    , _element(NULL)
  {
    progress ();
  }


  AutoSegments_AnchorOnGCell::Locator::~Locator ()
  {
    if ( _hookLocator ) delete _hookLocator;
  }


  AutoSegment* AutoSegments_AnchorOnGCell::Locator::getElement () const
  {
    return _element;
  }


  AutoSegmentHL* AutoSegments_AnchorOnGCell::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  AutoSegments_AnchorOnGCell::Locator::isValid () const
  {
    return _element != NULL;
  }


  void  AutoSegments_AnchorOnGCell::Locator::progress ()
  {
    ltrace(80) << "AutoSegments_AnchorOnGCell::Locator::progress()" << endl;
    ltracein(79);

    while ( true ) {
      if ( _hookLocator == NULL ) {
        if ( _itContact == _itEnd ) {
          ltrace(79) << "No more AutoContacts" << endl;
          ltraceout(79);
          return;
        }

        ltrace(79) << *_itContact << endl;

        _hookLocator = (*_itContact)->getBodyHook()->getSlaveHooks().getLocator();
        _itContact++;
      } else {
        _hookLocator->progress ();
      }

      while ( _hookLocator->isValid() ) {
        ltrace(79) << _hookLocator->getElement() << endl;
        Hook* hook = dynamic_cast<Segment::SourceHook*>(_hookLocator->getElement());
        if ( hook ) {
          _element = Session::lookup ( static_cast<Segment*>(hook->getComponent()) );
          if ( _element->isHorizontal() ) {
            if ( _direction & Constant::Horizontal ) { ltraceout(79); return; }
          } else
            if ( _direction & Constant::Vertical   ) { ltraceout(79); return; }
        }
        _hookLocator->progress();
      }
      _hookLocator = NULL;
      _element     = NULL;
    }

    ltraceout(79);
  }


  string  AutoSegments_AnchorOnGCell::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchorOnGCell::Locator")
                   + ">";
    return s;
  }


  AutoSegmentHC* AutoSegments_AnchorOnGCell::getClone () const
  {
    return new AutoSegments_AnchorOnGCell(*this);
  }


  AutoSegmentHL* AutoSegments_AnchorOnGCell::getLocator () const
  {
    return new Locator(_fcell,_sourceAnchor,_direction);
  }


  string  AutoSegments_AnchorOnGCell::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchorOnGCell") + " "
                   + getString(_fcell)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoSegments_AnchoredBySource".


AutoSegments_AnchoredBySource::Locator::Locator ( AutoContact* sourceAnchor, unsigned int direction )
  : AutoSegmentHL()
  , _direction(direction)
  , _hookLocator(NULL)
  , _element(NULL)
{
  _contactLocator = sourceAnchor->getCollapseds(_direction).getLocator(); 
  progress ();
}


  AutoSegments_AnchoredBySource::Locator::~Locator ()
  {
    if ( _hookLocator    ) delete _hookLocator;
    if ( _contactLocator ) delete _contactLocator;
  }


  AutoSegment* AutoSegments_AnchoredBySource::Locator::getElement () const
  {
    return _element;
  }


  AutoSegmentHL* AutoSegments_AnchoredBySource::Locator::getClone () const
  {
    return new Locator(*this);
  }

 
  bool  AutoSegments_AnchoredBySource::Locator::isValid () const
  {
    return _element != NULL;
  }


  void  AutoSegments_AnchoredBySource::Locator::progress ()
  {
    ltrace(80) << "AutoSegments_AnchoredBySource::Locator::progress()" << endl;
    ltracein(79);

    while ( true ) {
      if ( _hookLocator == NULL ) {
        if ( !_contactLocator->isValid() ) {
          ltrace(79) << "No more AutoContacts" << endl;
          ltraceout(79);
          return;
        }

        ltrace(79) << _contactLocator->getElement() << endl;

        _hookLocator = _contactLocator->getElement()->getBodyHook()->getSlaveHooks().getLocator();
        _contactLocator->progress ();
      } else {
        _hookLocator->progress ();
      }

      while ( _hookLocator->isValid() ) {
        ltrace(79) << _hookLocator->getElement() << endl;
        Hook* hook = dynamic_cast<Segment::SourceHook*>(_hookLocator->getElement());
        if ( hook ) {
          _element = Session::lookup ( static_cast<Segment*>(hook->getComponent()) );
          if ( _element->isHorizontal() ) {
            if ( _direction & Constant::Horizontal ) { ltraceout(79); return; }
          } else
            if ( _direction & Constant::Vertical   ) { ltraceout(79); return; }
        }
        _hookLocator->progress();
      }
      _hookLocator = NULL;
      _element     = NULL;
    }

    ltraceout(79);
  }


  AutoSegmentHL* AutoSegments_AnchoredBySource::getLocator () const
  {
    return new Locator(_sourceContact,_direction);
  }


  AutoSegmentHC* AutoSegments_AnchoredBySource::getClone () const
  {
    return new AutoSegments_AnchoredBySource(*this);
  }


  string  AutoSegments_AnchoredBySource::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchoredBySource::Locator")
                   + ">";
    return s;
  }


  string  AutoSegments_AnchoredBySource::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchoredBySource") + " "
                   + getString(_sourceContact)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_IsAccountable".


  AutoSegmentHF* AutoSegments_IsAccountable::getClone   () const
  {
    return new AutoSegments_IsAccountable();
  }


  bool  AutoSegments_IsAccountable::accept ( AutoSegment* segment ) const
  {
    return segment->isCanonical() && !segment->isCollapsed();
  }


  string  AutoSegments_IsAccountable::_getString () const
  {
    return "<AutoSegments_IsAccountable>";
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_InDirection".


  AutoSegmentHF* AutoSegments_InDirection::getClone () const
  {
    return new AutoSegments_InDirection(_direction);
  }


  bool  AutoSegments_InDirection::accept ( AutoSegment* segment ) const
  {
    return    ( segment->isHorizontal() && (_direction & Constant::Horizontal) )
           || ( segment->isVertical  () && (_direction & Constant::Vertical  ) );
  }


  string  AutoSegments_InDirection::_getString () const
  {
    return "<AutoSegments_InDirection>";
  }





// x-----------------------------------------------------------------x
// |                    Functions Definitions                        |
// x-----------------------------------------------------------------x


} // End of Katabatic namespace.
