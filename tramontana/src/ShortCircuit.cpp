
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


  string  ShortCircuit::_getString () const
  {
    string s = "<Short " + getString(_occurrenceA) + "+" + getString(_occurrenceB) + ">";
    return s;
  }


} // Tramontana namespace.


