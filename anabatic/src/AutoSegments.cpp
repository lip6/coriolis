// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoSegments.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"


namespace Anabatic {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::_TName;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::Hook;
  using Hurricane::RoutingPad;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegmentStack".

  void  AutoSegmentStack::push ( AutoContact* contact, AutoSegment* segment )
  {
    cdebug_log(145,0) << "Stacking " << contact << " + " << segment << endl;
    push_back( make_pair(contact,segment) );
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegments_OnContact".

  AutoSegments_OnContact::Locator::Locator ( AutoSegment* master, Contact* contact )
    : AutoSegmentHL()
    , _master      (master)
    , _element     (NULL)
  {
    _hook = contact->getBodyHook()->getPreviousMasterHook();
    progress();
  }


  AutoSegmentHL* AutoSegments_OnContact::Locator::getClone () const
  { return new Locator(*this); }


  AutoSegment* AutoSegments_OnContact::Locator::getElement () const
  { return _element; }


  bool  AutoSegments_OnContact::Locator::isValid () const
  { return !_hook; }


  void  AutoSegments_OnContact::Locator::progress ()
  {
    cdebug_log(145,0) << "AutoSegments_OnContact::Locator::progress()" << endl;

    while (_hook and not _hook->isMaster()) {
      _hook    = _hook->getNextHook(); 
      _element = NULL;

      if ( _hook->isMaster() ) { _hook = NULL; break; }

      Segment* segment = dynamic_cast<Segment*>( _hook->getComponent() );
      if (segment) _element = Session::lookup( segment );

      if (not _element or (_element == _master)) continue;

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
  { return new AutoSegments_OnContact(*this); }


  AutoSegmentHL* AutoSegments_OnContact::getLocator () const
  { return new Locator(_master,_contact); }


  string  AutoSegments_OnContact::_getString () const
  {
    string s = "<" + _TName("AutoSegments_OnContact") + " "
                   + getString(_master)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegments_OnRoutingPad".

  AutoSegments_OnRoutingPad::Locator::Locator ( RoutingPad* rp, const AutoContactTerminal* contact )
    : AutoSegmentHL()
    , _elements    ({NULL,NULL,NULL,NULL})
    , _index       (0)
  {
    if (rp) {
      for ( Component* component : rp->getSlaveComponents() ) {
        AutoSegment* segment = Session::lookup( dynamic_cast<Segment*>(component) );
        if (not segment) continue;

        size_t offset = 2;
        if (contact and (contact->getSegment() == segment)) offset = 0;

        if (segment->isHorizontal()) _elements[offset  ] = segment;
        else                         _elements[offset+1] = segment;
      }
    }

    while ( (_index < 4) and not _elements[_index] ) ++_index; 
  }


  AutoSegmentHL* AutoSegments_OnRoutingPad::Locator::getClone () const
  { return new Locator(*this); }


  AutoSegment* AutoSegments_OnRoutingPad::Locator::getElement () const
  { return (_index < 4) ? _elements[_index] : NULL; }


  bool  AutoSegments_OnRoutingPad::Locator::isValid () const
  { return (_index < 4); }


  void  AutoSegments_OnRoutingPad::Locator::progress ()
  {
    cdebug_log(145,0) << "AutoSegments_OnRoutingPad::Locator::progress()" << endl;
    ++_index;
    while ( (_index < 4) and not _elements[_index] ) ++_index; 
  }


  string  AutoSegments_OnRoutingPad::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_OnRoutingPad::Locator")
                   + getString(_index)
                   + ">";
    return s;
  }


  AutoSegments_OnRoutingPad::AutoSegments_OnRoutingPad ( const AutoContact* contact )
    : AutoSegmentHC()
    , _routingPad(NULL)
    , _contact   (dynamic_cast<const AutoContactTerminal*>(contact))
  {
    if (_contact)
      _routingPad = dynamic_cast<RoutingPad*>(_contact->getAnchor());
  }


  AutoSegmentHC* AutoSegments_OnRoutingPad::getClone () const
  { return new AutoSegments_OnRoutingPad(*this); }


  AutoSegmentHL* AutoSegments_OnRoutingPad::getLocator () const
  { return new Locator(_routingPad,_contact); }


  string  AutoSegments_OnRoutingPad::_getString () const
  {
    string s = "<" + _TName("AutoSegments_OnRoutingPad") + " "
                   + getString(_routingPad)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Connecteds".

  AutoSegments_Connecteds::Locator::Locator ( AutoSegment* segment, Flags flags )
    : AutoSegmentHL()
    , _stack ()
  {
    cdebug_log(145,0) << "AutoSegments_Connecteds::Locator::Locator()" << endl;

    if (flags & Flags::Source) {
      AutoContact* contact = segment->getAutoSource();
      if (contact) _stack.push( contact, segment );
    }

    if (flags & Flags::Target) {
      AutoContact* contact = segment->getAutoTarget();
      if (contact) _stack.push( contact, segment );
    }
  }


  AutoSegmentHL* AutoSegments_Connecteds::Locator::getClone () const
  { return new Locator(*this); }


  bool  AutoSegments_Connecteds::Locator::isValid () const
  { return not _stack.isEmpty(); }


  void  AutoSegments_Connecteds::Locator::progress ()
  {
    cdebug_log(145,0) << "AutoSegments_Connecteds::Locator::progress()" << endl;

    while (not _stack.isEmpty()) {
      AutoContact* sourceContact = _stack.getAutoContact ();
      AutoSegment* sourceSegment = _stack.getAutoSegment ();

      _stack.pop ();

      AutoContactTerminal* sourceTerminal = dynamic_cast<AutoContactTerminal*>( sourceContact );
      if (sourceTerminal) {
        for ( AutoSegment* currentSegment : sourceTerminal->getRpConnecteds() ) {
          cdebug_log(145,0) << "Looking at: " << currentSegment << endl;
          if (currentSegment == sourceSegment) continue;

          AutoContact* targetContact = currentSegment->getAutoSource();
          if (not targetContact->isTerminal()) _stack.push( targetContact, currentSegment );

          targetContact = currentSegment->getAutoTarget();
          if (not targetContact->isTerminal()) _stack.push( targetContact, currentSegment );
        }
      } else {
        LocatorHelper helper (sourceContact,Flags::WithPerpands);
        for ( ; helper.isValid() ; helper.progress() ) {
          AutoSegment* currentSegment = helper.getSegment();
          cdebug_log(145,0) << "Looking at: " << currentSegment << endl;

          if (currentSegment == sourceSegment) continue;

          AutoContact* targetContact  = currentSegment->getOppositeAnchor( sourceContact );
          if (targetContact) _stack.push( targetContact, currentSegment );
        }
      }
      break;
    }
  }


  string  AutoSegments_Connecteds::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Connecteds::Locator") + ">";
    return s;
  }


  AutoSegmentHC* AutoSegments_Connecteds::getClone () const
  { return new AutoSegments_Connecteds(*this); }


  AutoSegmentHL* AutoSegments_Connecteds::getLocator () const
  { return new Locator(_segment,_flags); }


  AutoSegment* AutoSegments_Connecteds::Locator::getElement () const
  { return _stack.getAutoSegment(); }


  string  AutoSegments_Connecteds::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Connecteds") + " "
                   + getString(_segment)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Aligneds".

  AutoSegments_Aligneds::Locator::Locator ( AutoSegment* segment, Flags flags )
    : AutoSegmentHL()
    , _flags (flags)
    , _master(segment)
    , _stack ()
  {
    if (not _master) return;
    _flags |= (_master->isHorizontal()) ? Flags::Horizontal : Flags::Vertical;

    cdebug_log(145,0) << "AutoSegments_Aligneds::Locator::Locator() - _flags:" << _flags << endl;

    AutoContact* contact = segment->getAutoSource();
    if (contact) _stack.push( contact, segment );

    contact = segment->getAutoTarget();
    if (contact) _stack.push( contact, segment );

    if (not (_flags & Flags::WithSelf)) progress();
  }


  AutoSegmentHL* AutoSegments_Aligneds::Locator::getClone () const
  { return new Locator(*this); }


  bool  AutoSegments_Aligneds::Locator::isValid () const
  { return not _stack.isEmpty(); }


  void  AutoSegments_Aligneds::Locator::progress ()
  {
    cdebug_log(145,0) << "AutoSegments_Aligneds::Locator::progress()" << endl;

    while (not _stack.isEmpty()) {
      AutoContact* sourceContact = _stack.getAutoContact ();
      AutoSegment* sourceSegment = _stack.getAutoSegment ();

      _stack.pop ();

      LocatorHelper helper (sourceContact, _flags);
      for ( ; helper.isValid() ; helper.progress() ) {
        AutoSegment* currentSegment = helper.getSegment();
        cdebug_log(145,0) << "Looking at: " << currentSegment << endl;

        if (currentSegment == sourceSegment) continue;

        if (   (not (_flags & Flags::NoCheckLayer))
           and AutoSegment::areAlignedsAndDiffLayer(currentSegment,_master)) {
          cerr << Error("Aligned segments not in same layer (aligneds locator)\n"
                        "        %s\n"
                        "        %s."
                       ,getString(_master).c_str()
                       ,getString(currentSegment).c_str()) << endl;
          continue;
        }

        AutoContact* targetContact  = currentSegment->getOppositeAnchor( sourceContact );
        if (targetContact) _stack.push( targetContact, currentSegment );
      }

      if (_stack.getAutoSegment() == _master) continue;
      break;
    }
  }


  string  AutoSegments_Aligneds::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Aligneds::Locator") + ">";
    return s;
  }


  AutoSegmentHC* AutoSegments_Aligneds::getClone () const
  { return new AutoSegments_Aligneds(*this); }


  AutoSegmentHL* AutoSegments_Aligneds::getLocator () const
  { return new Locator(_segment,_flags); }


  AutoSegment* AutoSegments_Aligneds::Locator::getElement () const
  { return _stack.getAutoSegment(); }


  string  AutoSegments_Aligneds::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Aligneds") + " "
                   + getString(_segment)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_Perpandiculars".

  AutoSegments_Perpandiculars::Locator::Locator ( AutoSegment* master, Flags flags )
    : AutoSegmentHL()
  //, _flags         (Flags::WithPerpands|Flags::WithDoglegs)
    , _flags         (Flags::WithPerpands|flags)
    , _master        (master)
    , _stack         ()
    , _perpandiculars()
  {
    cdebug_log(145,0) << "AutoSegments_Perpandiculars::Locator::Locator()" << endl;
    cdebug_log(145,0) << "  " << _master << endl;

    if (not _master) return;
    if (_master->isHorizontal()) _flags |= Flags::Horizontal;
    else                         _flags |= Flags::Vertical;

    AutoContact* contact = _master->getAutoSource();
    if ( contact ) _stack.push( contact, _master );

    contact = _master->getAutoTarget();
    if ( contact ) _stack.push( contact, _master );

    progress();
  }


  AutoSegment* AutoSegments_Perpandiculars::Locator::getElement () const
  {
    if (_perpandiculars.empty()) return NULL;
    return _perpandiculars.back();
  }


  void  AutoSegments_Perpandiculars::Locator::progress ()
  {
    cdebug_log(145,1) << "AutoSegments_Perpandiculars::Locator::progress()" << endl;

    if (not _perpandiculars.empty()) _perpandiculars.pop_back();
    if (not _perpandiculars.empty()) return;

    while ( not _stack.isEmpty() ) {
      AutoContact* sourceContact = _stack.getAutoContact();
      AutoSegment* sourceSegment = _stack.getAutoSegment();

      _stack.pop();
      cdebug_log(145,0) << "Iterate over: " << sourceContact << endl;

      LocatorHelper helper (sourceContact, _flags);
      for ( ; helper.isValid() ; helper.progress() ) {
        AutoSegment* currentSegment = helper.getSegment();
        if (currentSegment == sourceSegment) continue;

        cdebug_log(145,0) << "| " << currentSegment << endl;

        if (AutoSegment::areAligneds(currentSegment,_master)) {
          AutoContact* targetContact = currentSegment->getOppositeAnchor( sourceContact );

          if (targetContact) {
            if (_master->getLayer() != currentSegment->getLayer()) {
              continue;
            }
            cdebug_log(145,0) << "Stacking target. " << endl;
            _stack.push( targetContact, currentSegment );

            // if (  (_master->isHorizontal() and sourceContact->isHTee())
            //    or (_master->isVertical  () and sourceContact->isVTee()) ) {
            //   if (AutoSegment::areAlignedsAndDiffLayer(currentSegment,_master)) {
            //     cerr << Error("Aligned segments not in same layer (perpandicular locator)\n"
            //                   "        %s\n"
            //                   "        %s."
            //                  ,getString(_master).c_str()
            //                  ,getString(currentSegment).c_str()) << endl;
            //     continue;
            //   }

            //   cdebug_log(145,0) << "Stacking target. " << endl;
            //   _stack.push( targetContact, currentSegment );
            // }
          } else {
            cdebug_log(145,0) << "No opposite anchor to: " << sourceContact << endl;
          }
        } else {
          if ( (_flags & Flags::WithDoglegs) and currentSegment->isLocal() and sourceContact->isTurn() ) {
            AutoContact* targetContact = currentSegment->getOppositeAnchor( sourceContact );
            if (targetContact->isTurn()) {
              AutoSegment* targetGlobal = targetContact->getPerpandicular( currentSegment );
              if (targetGlobal->isGlobal() and (_master->getLayer() == targetGlobal->getLayer())) {
                cdebug_log(145,0) << "Global aligned though dogleg:" << targetGlobal << endl;
                Interval masterConstraints;
                Interval targetConstraints;
                _master     ->getConstraints( masterConstraints );
                targetGlobal->getConstraints( targetConstraints );
                if (targetConstraints.intersect(masterConstraints)) {
                  cdebug_log(145,0) << "Stacking dogleg global. " << endl;
                  _stack.push( targetContact, currentSegment );
                  continue;
                }
              }
            }
          }
          _perpandiculars.push_back( currentSegment );
        }
      }

      if (_stack.isEmpty()) break;
      if (_stack.getAutoSegment() == _master) continue;
      if (not _perpandiculars.empty()) break;
    }

    cdebug_tabw(145,-1);
  }


  AutoSegmentHL* AutoSegments_Perpandiculars::Locator::getClone () const
  { return new Locator(*this); }


  bool  AutoSegments_Perpandiculars::Locator::isValid () const
  { return not _perpandiculars.empty(); }


  AutoSegmentHC* AutoSegments_Perpandiculars::getClone () const
  { return new AutoSegments_Perpandiculars(*this); }


  AutoSegmentHL* AutoSegments_Perpandiculars::getLocator () const
  { return new Locator(_master,_flags); }


  string  AutoSegments_Perpandiculars::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Perpandiculars::Locator") + ">";
    return s;
  }


  string  AutoSegments_Perpandiculars::_getString () const
  {
    string s = "<" + _TName("AutoSegments_Perpandiculars") + " "
                   + getString(_master)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_AnchorOnGCell".

  AutoSegments_AnchorOnGCell::Locator::Locator ( GCell* fcell, Flags flags )
    : AutoSegmentHL()
    , _flags      (flags)
    , _itContact  (fcell->getContacts().begin())
    , _itEnd      (fcell->getContacts().end())
    , _hookLocator(NULL)
    , _element    (NULL)
  { progress(); }


  AutoSegments_AnchorOnGCell::Locator::~Locator ()
  { if (_hookLocator) delete _hookLocator; }


  AutoSegment* AutoSegments_AnchorOnGCell::Locator::getElement () const
  { return _element; }


  AutoSegmentHL* AutoSegments_AnchorOnGCell::Locator::getClone () const
  { return new Locator(*this); }


  bool  AutoSegments_AnchorOnGCell::Locator::isValid () const
  { return _element != NULL; }


  void  AutoSegments_AnchorOnGCell::Locator::progress ()
  {
    cdebug_log(145,1) << "AutoSegments_AnchorOnGCell::Locator::progress()" << endl;

    while ( true ) {
      if (_hookLocator == NULL) {
        if (_itContact == _itEnd) {
          cdebug_log(145,0) << "No more AutoContacts" << endl;
          cdebug_tabw(145,-1);
          return;
        }

        cdebug_log(145,0) << *_itContact << endl;

        _hookLocator = (*_itContact)->getBodyHook()->getSlaveHooks().getLocator();
        _itContact++;
      } else {
        _hookLocator->progress();
      }

      while ( _hookLocator->isValid() ) {
        cdebug_log(145,0) << _hookLocator->getElement() << endl;
        Hook* hook = dynamic_cast<Segment::SourceHook*>(_hookLocator->getElement());
        if (hook) {
          if (  ((_flags & Flags::Source) and (dynamic_cast<Segment::SourceHook*>(hook)))
             or ((_flags & Flags::Target) and (dynamic_cast<Segment::TargetHook*>(hook))) ) {
            _element = Session::lookup( static_cast<Segment*>(hook->getComponent()) );

            if (_element->isHorizontal()) {
              if (_flags & Flags::Horizontal) { cdebug_tabw(145,-1); return; }
            } else
              if (_flags & Flags::Vertical) { cdebug_tabw(145,-1); return; }
          }
        }
        _hookLocator->progress();
      }
      _hookLocator = NULL;
      _element     = NULL;
    }

    cdebug_tabw(145,-1);
  }


  string  AutoSegments_AnchorOnGCell::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchorOnGCell::Locator") + ">";
    return s;
  }


  AutoSegmentHC* AutoSegments_AnchorOnGCell::getClone () const
  { return new AutoSegments_AnchorOnGCell(*this); }


  AutoSegmentHL* AutoSegments_AnchorOnGCell::getLocator () const
  { return new Locator(_fcell,_flags); }


  string  AutoSegments_AnchorOnGCell::_getString () const
  {
    string s = "<" + _TName("AutoSegments_AnchorOnGCell") + " "
                   + getString(_fcell)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoSegments_CachedOnContact".

  AutoSegments_CachedOnContact::Locator::Locator ( AutoContact* sourceContact, Flags direction )
    : AutoSegmentHL()
    , _helper(new LocatorHelper(sourceContact,direction))
  { }


  AutoSegments_CachedOnContact::Locator::~Locator ()
  { delete _helper; }


  AutoSegment* AutoSegments_CachedOnContact::Locator::getElement () const
  { return _helper->getSegment(); }


  AutoSegmentHL* AutoSegments_CachedOnContact::Locator::getClone () const
  { return new Locator(*this); }

 
  bool  AutoSegments_CachedOnContact::Locator::isValid () const
  { return _helper->isValid(); }


  void  AutoSegments_CachedOnContact::Locator::progress ()
  {
    cdebug_log(145,0) << "AutoSegments_CachedOnContact::Locator::progress()" << endl;
    _helper->progress();
  }


  AutoSegmentHL* AutoSegments_CachedOnContact::getLocator () const
  { return new Locator(_sourceContact,_direction); }


  AutoSegmentHC* AutoSegments_CachedOnContact::getClone () const
  { return new AutoSegments_CachedOnContact(*this); }


  string  AutoSegments_CachedOnContact::Locator::_getString () const
  {
    string s = "<" + _TName("AutoSegments_CachedOnContact::Locator") + ">";
    return s;
  }


  string  AutoSegments_CachedOnContact::_getString () const
  {
    string s = "<" + _TName("AutoSegments_CachedOnContact") + " "
                   + getString(_sourceContact)
                   + ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_IsAccountable".

  AutoSegmentHF* AutoSegments_IsAccountable::getClone   () const
  { return new AutoSegments_IsAccountable(); }


  bool  AutoSegments_IsAccountable::accept ( AutoSegment* segment ) const
  { return segment->isCanonical(); }


  string  AutoSegments_IsAccountable::_getString () const
  { return "<AutoSegments_IsAccountable>"; }


// -------------------------------------------------------------------
// Class  :  "AutoSegments_InDirection".


  AutoSegmentHF* AutoSegments_InDirection::getClone () const
  { return new AutoSegments_InDirection(_direction); }


  bool  AutoSegments_InDirection::accept ( AutoSegment* segment ) const
  {
    return    ( segment->isHorizontal() and (_direction & Flags::Horizontal) )
           or ( segment->isVertical  () and (_direction & Flags::Vertical  ) );
  }


  string  AutoSegments_InDirection::_getString () const
  { return "<AutoSegments_InDirection>"; }


} // Anabatic namespace.
