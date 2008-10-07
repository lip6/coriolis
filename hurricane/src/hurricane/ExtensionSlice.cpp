
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ExtensionSlice.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/ExtensionSlice.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ExtensionSlice".


  unsigned int  ExtensionSlice::_masks = 0;


  ExtensionSlice::ExtensionSlice ( Cell* cell, const Name& name, ExtensionSlice::Mask mask )
    : _cell(cell)
    , _name(name)
    , _mask(mask)
    , _quadTree()
  {
    _cell->_insertSlice ( this );
  }


  ExtensionSlice::~ExtensionSlice ()
  {
    _cell->_removeSlice ( this );
  }


  ExtensionSlice* ExtensionSlice::_create ( Cell* cell, const Name& name )
  {
    if ( !cell )
      throw Error ( "ExtensionSlice::_create(): Cannot create, NULL cell argument." );

    if ( name.isEmpty() )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create, empty name argument."
                  , getString(cell->getName()).c_str()
                  );

    if ( cell->getExtensionSlice(name) )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create, %s already exists."
                  , getString(cell->getName()).c_str()
                  , getString(name).c_str()
                  );

    if ( _masks >= sizeof(unsigned long long) )
      throw Error ( "ExtensionSlice::_create():\n"
                    "  In %s, cannot create %s, no more free masks."
                  , getString(cell->getName()).c_str()
                  , getString(name).c_str()
                  );

    ExtensionSlice* slice = new ExtensionSlice(cell,name,(1<<++_masks));

    return slice;
  }


  void  ExtensionSlice::_destroy ()
  {
    delete this;
  }


  string  ExtensionSlice::_getTypeName () const
  {
    return _TName("ExtensionSlice");
  }


  string ExtensionSlice::_getString () const
  {
    string s = "<" + _getTypeName();
    s += " " + getString(_name);
    s += ">";
    return s;
  }


  Record* ExtensionSlice::_getRecord () const
  {
    Record* record = new Record(getString(this));
    if (record) {
      record->add(getSlot("Cell"    , _cell    ));
      record->add(getSlot("Name"    , _name    ));
      record->add(getSlot("Mask"    , _mask    ));
      record->add(getSlot("QuadTree",&_quadTree));
    }
    return record;
  }


} // End of Hurricane namespace.
