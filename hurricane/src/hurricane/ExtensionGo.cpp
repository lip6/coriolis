
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
// |  C++ Module  :       "./ExtensionGo.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Cell.h"
#include  "hurricane/ExtensionGo.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::ExtensionGo".


  ExtensionGo::ExtensionGo ( Cell* cell )
    : Go()
    , _cell(cell)
  { }


  void  ExtensionGo::materialize ()
  {
    if ( !isMaterialized() ) {
      if ( _cell ) {
        ExtensionSlice* slice = _cell->getExtensionSlice ( getName() );
        if ( !slice ) slice = ExtensionSlice::_create ( _cell, getName() );
        QuadTree* quadTree = slice->_getQuadTree ();
        quadTree->insert ( this );
        _cell->_fit ( quadTree->getBoundingBox() );
      } else {
        cerr << Warning("%s not inserted into QuadTree.",getString(this).c_str()) << endl;
      }
    }
  }


  void ExtensionGo::unmaterialize ()
  {
    if ( isMaterialized() ) {
      ExtensionSlice* slice = _cell->getExtensionSlice ( getName() );
      if ( slice ) {
        _cell->_unfit ( getBoundingBox() );
        slice->_getQuadTree()->remove ( this );
        if ( slice->isEmpty() ) slice->_destroy ();
      }
    }
  }


  Cell* ExtensionGo::getCell () const
  {
    return _cell;
  }


  string  ExtensionGo::_getTypeName () const
  {
    return "ExtensionGo";
  }


  string ExtensionGo::_getString () const
  {
    string s = Go::_getString();
    s.insert ( s.length() - 1, " " + getString(getName()) );
    return s;
  }


  Record* ExtensionGo::_getRecord () const
  {
    return Go::_getRecord();
  }


} // End of Hurricane namespace.
