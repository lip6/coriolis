
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
// |  C++ Module  :       "./AutoContacts.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <utility>

#include "hurricane/Segment.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"

#include "katabatic/Session.h"
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"


namespace Katabatic {


  using namespace std;
  using Hurricane::Hook;
  using Hurricane::Error;
  using Hurricane::Segment;
  using Hurricane::ForEachIterator;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContacts_Collapsed".


  void  AutoContactStack::push ( AutoContact* contact, Segment* segment )
  {
    ltrace(80) << "Stacking " << (void*)contact->getContact() << ":" << contact
               << " + " << (void*)segment << ":" << segment << endl;
  
    push_back(make_pair(contact,segment));
  }


  AutoContact* AutoContacts_Collapsed::Locator::getElement () const
  { 
    return _stack.getAutoContact ();
  }


  void  AutoContacts_Collapsed::Locator::progress ()
  {
    AutoContact* sourceContact = _stack.getAutoContact ();
    Segment*     sourceSegment = _stack.getSegment ();

    _stack.pop ();

    forEach ( Hook*, ihook, sourceContact->getBodyHook()->getHooks() ) {
      Segment* segment = NULL;

      if ( _direction & Constant::Horizontal ) segment = dynamic_cast<Horizontal*>(ihook->getComponent());
      else                                     segment = dynamic_cast<Vertical*  >(ihook->getComponent());

      if ( segment && (segment != sourceSegment) ) {
        AutoSegment* autoSegment = Session::lookup ( segment );

        if ( !autoSegment ) {
          cerr << Error("Can't lookup <AutoSegment> for %s.",getString(segment).c_str()) << endl;
          continue;
        }
        if ( !autoSegment->isCollapsed() ) continue;

        Hook*        opposite      = segment->getOppositeHook ( *ihook );
        AutoContact* targetContact = dynamic_cast<AutoContact*>(opposite->getMasterHook()->getComponent());

        if ( targetContact ) _stack.push ( targetContact, segment );
      }
    }
  }


  Hurricane::Locator<AutoContact*>* AutoContacts_Collapsed::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  AutoContacts_Collapsed::Locator::isValid () const
  {
    return _stack.isEmpty();
  }


  string  AutoContacts_Collapsed::Locator::_getString () const
  {
    string s = "<" + _TName("AutoContacts_Collapsed::Locator")
                   + getString(_stack.getAutoContact())
                   + ">";
    return s;
  }


  Collection<AutoContact*>* AutoContacts_Collapsed::getClone () const
  {
    return new AutoContacts_Collapsed(*this);
  }


  Hurricane::Locator<AutoContact*>* AutoContacts_Collapsed::getLocator () const
  {
    return new Locator(_contact,_direction);
  }


  string  AutoContacts_Collapsed::_getString () const
  {
    string s = "<" + _TName("AutoContacts_Collapsed") + " "
                   + getString(_contact) + " "
                   + getString(_direction)
                   + ">";
    return s;
  }


} // End of Katabatic namespace.
