
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./ShortCircuit.cpp"                           |
// +-----------------------------------------------------------------+


#include "tramontana/ShortCircuit.h"


namespace Tramontana {

  using std::string;


// -------------------------------------------------------------------
// Class  :  "Tramontana::ShortCircuit".

  ShortCircuit::ShortsByCells  ShortCircuit::_shortsByCells;


  ShortCircuit::ShortCircuit ( Occurrence occA, Occurrence occB )
    : _occurrenceA(occA)
    , _occurrenceB(occB)
    , _overlap    (nullptr)
  {
    if (       occB.getPath().isEmpty()
       and not occA.getPath().isEmpty()) {
      _occurrenceA = occB;
      _occurrenceB = occA;
    }

    Cell* cell = occA.getOwnerCell();
    if (_shortsByCells.find(cell) == _shortsByCells.end())
      _shortsByCells.insert( make_pair( cell, ShortingEquis() ) );
    ShortingEquis& shortingEquis = _shortsByCells.find( cell )->second;
    
    if (not isTopLevelA()) {
      Equipotential* equi = dynamic_cast<Equipotential*>( getEquiA().getEntity() );
      auto iequi = shortingEquis.find( equi );
      if (iequi == shortingEquis.end())
        shortingEquis[ equi ] = 1;
      else
        iequi->second++;
    }

    if (not isTopLevelB()) {
      Equipotential* equi = dynamic_cast<Equipotential*>( getEquiB().getEntity() );
      auto iequi = shortingEquis.find( equi );
      if (iequi == shortingEquis.end())
        shortingEquis[ equi ] = 1;
      else
        iequi->second++;
    }
    
    Box shortingBox = getBoundingBoxA().getIntersection( getBoundingBoxB() );
    if (shortingBox.isEmpty()) {
      std::cerr << Warning( "ShortCircuit::Shortcircuit(): Empty short ciruit area.\n"
                            "           On: %s"
                          , getString(this).c_str()) << std::endl;
      shortingBox = getBoundingBoxA().merge( getBoundingBoxB() );
    }
    _overlap = DRCError::create( _occurrenceA.getOwnerCell(), "Short", shortingBox );
  }


  string  ShortCircuit::_getString () const
  {
    string s = "<Short " + getString(_occurrenceA) + "+" + getString(_occurrenceB) + ">";
    return s;
  }


} // Tramontana namespace.


