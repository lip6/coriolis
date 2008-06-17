

// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id: Record.h,v 1.7 2007/07/29 15:24:58 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Record.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Commons.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Record".


  size_t  Record::_allocateds = 0;


  Record::Record ( const string& name )
    : _name(name)
    , _slotList()
  {
    _allocateds++;
  }


  Record::~Record ()
  {
  //cerr << "Record::~Record() - " << _name << ": " << hex << (void*)this << dec << endl;
	while (!_slotList.empty()) {
      Slot* slot = *_slotList.begin();
      _slotList.remove(slot);
      delete slot;
	}
    _allocateds--;
  }


  size_t  Record::getAllocateds ()
  {
    return _allocateds;
  }


  Slot* Record::getSlot ( unsigned no ) const
  {
	SlotList::const_iterator iterator = _slotList.begin();
	while (no-- && (iterator != _slotList.end())) ++iterator;
	return (iterator == _slotList.end()) ? NULL : *iterator;
  }


  void Record::add ( Slot* slot )
  {
    if (!slot) {
      cerr << "[ERROR] Record::add(): Attempt to add NULL Slot." << endl;
      return;
    }
    _slotList.push_back(slot);
  }


} // End of Hurricane namespace.
