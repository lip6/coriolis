// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2017-2017, All Rights Reserved
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
// |  C++ Module  :  "./Occurrences.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Occurrences.h"
#include "hurricane/Instance.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Occurrence_Contains".

  OccurrenceFilter* Occurrence_Contains::getClone () const
  { return new Occurrence_Contains(_instance); }


  bool  Occurrence_Contains::accept ( Occurrence occurrence ) const
  {
    if (not _instance) return false;

    if (dynamic_cast<Instance*>(occurrence.getEntity()) == _instance) return true;
    for ( Instance* instance : occurrence.getPath().getInstances() ) {
      if (instance == _instance) return true;
    }
    return false;
  }


  string  Occurrence_Contains::_getString () const
  { return "<Occurrence_Contains " + getString(_instance) + " >"; }



}  // Hurricane namespace.
