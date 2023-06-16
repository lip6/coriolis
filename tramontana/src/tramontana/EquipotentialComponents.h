// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
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
// |  C++ Header   :  "./tramontana/EquipotentialComponents.h"       |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <list>
#include <vector>
#include <array>
#include <map>
#include "hurricane/Collection.h"
#include "hurricane/DbU.h"
#include "hurricane/Box.h"
#include "hurricane/Net.h"
#include "hurricane/Occurrence.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using std::string;
  using std::pair;
  using std::list;
  using std::vector;
  using std::map;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Entity;
  using Hurricane::Component;
  using Hurricane::Net;
  using Hurricane::NetSet;
  using Hurricane::Occurrence;
  using Hurricane::OccurrenceSet;
  using Hurricane::Filter;
  using Hurricane::Locator;
  using Hurricane::Collection;
  using Hurricane::GenericFilter;
  using Hurricane::GenericLocator;
  using Hurricane::GenericCollection;

  typedef  Hurricane::Locator<Component*>  ComponentsLocator;
  typedef  Hurricane::Locator<Occurrence>  OccurrencesLocator;


// -------------------------------------------------------------------
// Class  :  "Tramontana::EquipotentialComponents".

  class EquipotentialComponents : public Collection<Occurrence> {
    public:
      typedef Collection<Occurrence> Super;
      static const uint16_t  Constructed  = (1<<0);
      static const uint16_t  InComponents = (1<<1);
      static const uint16_t  InNets       = (1<<2);
      static const uint16_t  InChildEquis = (1<<3);
      static const uint16_t  Finished     = (1<<4);
    public:
      class Locator : public Hurricane::Locator<Occurrence> {
        public:
          typedef Hurricane::Locator<Occurrence> Super;
        public:
                               Locator    ();
                               Locator    ( const Equipotential* );
                               Locator    ( const Locator& );
                  Locator&     operator=  ( const Locator& );
          virtual Occurrence   getElement () const;
          virtual Hurricane::Locator<Occurrence>*
                               getClone   () const;
          virtual bool         isValid    () const;
          virtual void         progress   ();
          virtual std::string  _getString () const;
        private:
          const Equipotential*                   _equipotential;
                uint16_t                         _state;
                OccurrenceSet::iterator          _componentsIterator;
                Equipotential::NetMap::const_iterator  _netsIterator;
                OccurrenceSet::iterator          _childsIterator;
                OccurrencesLocator*              _childCompsLocator;
                ComponentsLocator*               _componentsLocator;
      };

    public:
                                              EquipotentialComponents ();
                                              EquipotentialComponents ( const Equipotential* );
                                              EquipotentialComponents ( const EquipotentialComponents& );
              EquipotentialComponents&        operator=               ( const EquipotentialComponents& );
      virtual Collection<Occurrence>*         getClone                () const;
      virtual Hurricane::Locator<Occurrence>* getLocator              () const;
      virtual std::string                     _getString              () const;
    private:
      const Equipotential* _equipotential;
  };
  


} // Tramontana namespace.
