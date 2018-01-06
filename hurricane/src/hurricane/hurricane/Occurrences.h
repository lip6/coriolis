// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
// |  Author      :                       Rémy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Occurrences.cpp"                             |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_OCCURENCES
#define HURRICANE_OCCURENCES

#include "hurricane/Collection.h"
#include "hurricane/Occurrence.h"

namespace Hurricane {

  class Instance;

  typedef GenericCollection<Occurrence> Occurrences;
  typedef GenericLocator<Occurrence> OccurrenceLocator;
  typedef GenericFilter<Occurrence> OccurrenceFilter;


// -------------------------------------------------------------------
// Class  :  "Occurrence_Contains".

  class Occurrence_Contains : public OccurrenceFilter {
    public:
      inline                    Occurrence_Contains ( const Instance* );
      inline                    Occurrence_Contains ( const Occurrence_Contains& );
      virtual OccurrenceFilter* getClone            () const;
      virtual bool              accept              ( Occurrence ) const;
      virtual string            _getString          () const;
    protected:
      const Instance* _instance;
  };


  Occurrence_Contains::Occurrence_Contains ( const Instance* instance )
    : OccurrenceFilter()
    , _instance(instance)
  { }

  Occurrence_Contains::Occurrence_Contains ( const Occurrence_Contains& other )
    : OccurrenceFilter()
    , _instance(other._instance)
  { }


#define for_each_occurrence(occurrence, occurrences)       \
{                                                          \
    OccurrenceLocator _locator = occurrences.getLocator(); \
    while (_locator.isValid()) {                           \
        Occurrence occurrence = _locator.getElement();     \
        _locator.progress();


} // Hurricane namespace.

#endif // HURRICANE_OCCURENCES
