// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./NetAlias.cpp"                                |
// +-----------------------------------------------------------------+


#include "hurricane/NetAlias.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"


namespace Hurricane {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "Hurricane::NetAliasHook".

  NetAliasHook::NetAliasHook ()
    : _next(this)
  { }
  

  NetAliasHook::~NetAliasHook ()
  { if (isAttached()) detach(); }

  
  bool  NetAliasHook::isAttached () const
  { return (_next != this); }


  NetAliasName* NetAliasHook::find ( const Name& name ) const
  {
    const NetAliasHook* current = this;
    do {
      if ( not isMaster() and (current->getName() == name) )
        return const_cast<NetAliasName*>( dynamic_cast<const NetAliasName*>(current) );

      current = current->_next;
    } while ( current != this );

    return NULL;
  }


  NetAliasHook* NetAliasHook::getNext () const
  { return _next; }
  
  
  NetAliasHook* NetAliasHook::getPrevious () const
  {
    NetAliasHook* current = _next;
    while ( current->_next != this )
      current = current->_next;
    return current;
  }
  
  
  void  NetAliasHook::attach ( NetAliasHook* otherRing )
  {
    NetAliasHook* otherRingPrev = otherRing->getPrevious();
  
    otherRingPrev->_next = _next;
    _next                = otherRing;
  }
  
  
  void  NetAliasHook::detach ()
  {
    if (not isAttached()) return;
  
    NetAliasHook* prev = getPrevious();
  
    prev->_next = _next;
    _next       = this;
  }
  
  
  void  NetAliasHook::detachAll ()
  {
    NetAliasHook* current = _next;
    _next = this;
  
    while ( current != this )
    {
      NetAliasHook* next = current->_next;
      current->_next = current;
      current = next;
    }
  }


  Record* NetAliasHook::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot("_next", _next) );
    return record;
  }
  
  
// -------------------------------------------------------------------
// Class  :  "Hurricane::NetMainName".

  ptrdiff_t  NetMainName::_offset = 0;


  NetMainName::NetMainName ( Net* owner )
    : NetAliasHook()
  {
    if (not _offset) _offset = (ptrdiff_t)this - (ptrdiff_t)owner;
  }


  NetMainName::~NetMainName ()
  { clear(); }


  void  NetMainName::clear ()
  {
    vector<NetAliasName*> slaves;
    for ( NetAliasHook* current=getNext() ; current->getNext()!=this ; current=current->getNext() ) {
      slaves.push_back( dynamic_cast<NetAliasName*>(current) );
    }
    detachAll();

    AliasNameSet& slaveAliases = getNet()->getCell()->_getNetAliasSet();
    for ( auto slave : slaves ) {
      slaveAliases.erase( slave );
      delete slave;
    }
  }


  bool    NetMainName::isMaster   () const { return true; }
  bool    NetMainName::isSlave    () const { return false; }
  Name    NetMainName::getName    () const { return getNet()->getName(); }
  Net*    NetMainName::getNet     () const { return (Net*)((ptrdiff_t)this - _offset); }
  string  NetMainName::_getString () const { return "<NetMainName " + getString(getName()) + ">"; }
  

// -------------------------------------------------------------------
// Class  :  "Hurricane::NetAliasName".

  NetAliasName::NetAliasName ( Name name )
    : NetAliasHook()
    , _name       (name)
  { }


  NetAliasName::~NetAliasName ()
  {
    if (isAttached()) {
      cerr << Error("NetAliasName::~NetAliasName() still attached, prepare to crash.") << endl;
    }
  }


  bool    NetAliasName::isMaster () const { return false; }
  bool    NetAliasName::isSlave  () const { return true; }
  Name    NetAliasName::getName  () const { return _name; }


  Net* NetAliasName::getNet () const
  {
    NetAliasHook* current = this->getNext();
    while (current != this) {
      if (current->isMaster()) return current->getNet();
      current = current->getNext();
    }
    return NULL;
  }


  string  NetAliasName::_getString () const
  { return "<NetAliasName " + getString(getName()) + " <> " + getString(getNet()->getName()) + ">"; } 


  Record* NetAliasName::_getRecord () const
  {
    Record* record = NetAliasHook::_getRecord();
    if (record) {
      record->add ( getSlot("_name", &_name) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::AliasList" (Collection).

  AliasList::Locator::Locator ( const Net* net )
    : Hurricane::Locator<NetAliasHook*>()
    , _hook                            (net->getMainName()->getNext())
  { }


  Locator<NetAliasHook*>* AliasList::Locator::getClone () const
  { return new Locator(*this); }


  NetAliasHook* AliasList::Locator::getElement () const
  { return isValid() ?  _hook : NULL; }


  bool  AliasList::Locator::isValid () const
  { return (_hook and not _hook->isMaster()); }


  void  AliasList::Locator::progress ()
  { if (not _hook->isMaster()) _hook = _hook->getNext(); }


  string  AliasList::Locator::_getString () const
  {
    string s = "<" + _TName("AliasList::Locator")
                   + getString(getElement())
                   + ">";
    return s;
  }


  Collection<NetAliasHook*>* AliasList::getClone () const
  { return new AliasList(*this); }


  Locator<NetAliasHook*>* AliasList::getLocator () const
  { return new Locator(_net); }


  string  AliasList::_getString () const
  {
    string s = "<" + _TName("NetAliasHook") + " "
                   + getString(_net->getName())
                   + ">";
    return s;
  }


}  // Hurricane namespace.
