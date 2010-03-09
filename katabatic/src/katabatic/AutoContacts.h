
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
// |  C++ Header  :       "./AutoContacts.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KATABATIC_AUTOCONTACTS__
#define  __KATABATIC_AUTOCONTACTS__

#include  <string>
#include  <map>
#include  <list>

#include  "hurricane/Collection.h"

namespace Hurricane {
  class Contact;
  class Segment;
}


namespace Katabatic {


  using std::map;
  using std::list;
  using std::pair;
  using std::string;
  using Hurricane::Locator;
  using Hurricane::Collection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericCollection;
  using Hurricane::GenericFilter;
  using Hurricane::Contact;
  using Hurricane::Segment;

  class AutoContact;
  class AutoSegment;


// -------------------------------------------------------------------
// Collections.


  typedef Hurricane::Locator<AutoContact*>     AutoContactHL;
  typedef Hurricane::Collection<AutoContact*>  AutoContactHC;
  typedef GenericCollection<AutoContact*>      AutoContacts;
  typedef GenericLocator<AutoContact*>         AutoContactLocator;
  typedef GenericFilter<AutoContact*>          AutoContactFilter;
  typedef map<Contact*,AutoContact*>           AutoContactLut;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactStack".
  

  class AutoContactStack : protected list<pair<AutoContact*,Segment*> > {
    public:
      inline bool         isEmpty        () const;
      inline size_t       getSize        () const;
             void         push           ( AutoContact*, Segment* );
      inline void         pop            ();
      inline AutoContact* getAutoContact () const;
      inline Segment*     getSegment     () const;
  };


  inline bool         AutoContactStack::isEmpty        () const { return empty(); }
  inline size_t       AutoContactStack::getSize        () const { return size(); }
  inline void         AutoContactStack::pop            () { if ( !empty() ) pop_back(); }
  inline AutoContact* AutoContactStack::getAutoContact () const { return empty() ? NULL : back().first; }
  inline Segment*     AutoContactStack::getSegment     () const { return empty() ? NULL : back().second; }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContacts_Collapsed".


  class AutoContacts_Collapsed : public AutoContactHC {

    public:
    // Sub-Class: Locator.
      class Locator : public AutoContactHL {
        public:
          inline                 Locator    ( AutoContact*, unsigned int  direction );
          inline                 Locator    ( const Locator& );
          virtual AutoContact*   getElement () const;
          virtual AutoContactHL* getClone   () const;
          virtual bool           isValid    () const;
          virtual void           progress   ();
          virtual string         _getString () const;
        protected:
          unsigned int      _direction;
          AutoContactStack  _stack;
      };

    public:
    // AutoContacts_Collapsed Methods.
      inline                 AutoContacts_Collapsed ( AutoContact*, unsigned int direction );
      inline                 AutoContacts_Collapsed ( const AutoContacts_Collapsed& );
      virtual AutoContactHC* getClone               () const;
      virtual AutoContactHL* getLocator             () const;
      virtual string         _getString             () const;

    public:
    // AutoContacts_Collapsed Attributes.
      AutoContact*  _contact;
      unsigned int  _direction;
  };


  AutoContacts_Collapsed::Locator::Locator ( AutoContact*  contact
                                           , unsigned int  direction )
    : AutoContactHL()
    , _stack()
  {
    _stack.push(contact,NULL);
  }


  AutoContacts_Collapsed::Locator::Locator ( const Locator &locator )
    : AutoContactHL()
    , _stack(locator._stack)
  { }


  AutoContacts_Collapsed::AutoContacts_Collapsed ( AutoContact* contact, unsigned int direction )
    : AutoContactHC()
    , _contact(contact)
    , _direction(direction)
  { }


  AutoContacts_Collapsed::AutoContacts_Collapsed ( const AutoContacts_Collapsed& autocontacts )
    : AutoContactHC()
    , _contact(autocontacts._contact)
    , _direction(autocontacts._direction)
  { }


} // End of Katabatic namespace.


#endif  // __KATABATIC_AUTOCONTACTS__
