
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/ShortCircuit.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Warning.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using Hurricane::Warning;
  using Hurricane::Record;
  using Hurricane::Box;
  using Hurricane::DBo;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::DRCError;
  using Hurricane::Cell;
  using Hurricane::Component;
  using Hurricane::OccurrenceSet;
  using Hurricane::Occurrence;
  using Hurricane::Occurrences;


// -------------------------------------------------------------------
// Class  :  "Tramontana::ShortCircuit".

  class ShortCircuit {
    public:
      typedef std::map<Equipotential*,uint32_t,DBo::CompareById>    ShortingEquis;
      typedef std::map<const Cell*,ShortingEquis,DBo::CompareById>  ShortsByCells;
    public:
      static inline const ShortingEquis& getShortingEquis    ( const Cell* );
      static inline const void           removeShortingEquis ( const Cell* );
    public:
                            ShortCircuit    ( Occurrence, Occurrence );
      inline               ~ShortCircuit    ();
      inline bool           isTopLevelA     () const;
      inline bool           isTopLevelB     () const;
      inline bool           isTopLevel      () const;
      inline bool           isAcrossLevels  () const;
      inline bool           isDeepShort     () const;
      inline Occurrence     getOccurrenceA  () const;
      inline Occurrence     getOccurrenceB  () const;
      inline Occurrence     getOccurrenceO  () const;
      inline Component*     getComponentA   () const;
      inline Component*     getComponentB   () const;
      inline Occurrence     getEquiA        () const;
      inline Occurrence     getEquiB        () const;
      inline Box            getBoundingBoxA () const;
      inline Box            getBoundingBoxB () const;
      inline Box            getShortingBox  () const;
      inline std::string    getEquiAName    () const;
      inline std::string    getEquiBName    () const;
             std::string    _getString      () const;
    private:
      static ShortsByCells  _shortsByCells;
    private:
      Occurrence _occurrenceA;
      Occurrence _occurrenceB;
      DRCError*  _overlap;
  };

  inline ShortCircuit::~ShortCircuit ()
  { _overlap->destroy(); }

  inline const ShortCircuit::ShortingEquis& ShortCircuit::getShortingEquis ( const Cell* cell )
  {
    static ShortingEquis nullShorts;
    auto ishorts = _shortsByCells.find( cell );
    if (ishorts != _shortsByCells.end())
      return ishorts->second;
    return nullShorts;
  }

  inline const void  ShortCircuit::removeShortingEquis ( const Cell* cell )
  {
    auto ishorts = _shortsByCells.find( cell );
    if (ishorts != _shortsByCells.end())
      _shortsByCells.erase( ishorts );
  }

  inline bool        ShortCircuit::isTopLevelA    () const { return _occurrenceA.getPath().isEmpty(); }
  inline bool        ShortCircuit::isTopLevelB    () const { return _occurrenceB.getPath().isEmpty(); }
  inline bool        ShortCircuit::isTopLevel     () const { return isTopLevelB(); }
  inline bool        ShortCircuit::isAcrossLevels () const { return     isTopLevelA() and not isTopLevelB(); }
  inline bool        ShortCircuit::isDeepShort    () const { return not isTopLevelA() and not isTopLevelB(); }
  inline Occurrence  ShortCircuit::getOccurrenceA () const { return _occurrenceA; }
  inline Occurrence  ShortCircuit::getOccurrenceB () const { return _occurrenceB; }
  inline Occurrence  ShortCircuit::getOccurrenceO () const { return Occurrence( _overlap ); }
  inline Component*  ShortCircuit::getComponentA  () const { return (dynamic_cast<Component*>( _occurrenceA.getEntity() )); }
  inline Component*  ShortCircuit::getComponentB  () const { return (dynamic_cast<Component*>( _occurrenceB.getEntity() )); }
  inline Occurrence  ShortCircuit::getEquiA       () const { return Equipotential::getChildEqui( _occurrenceA ); }
  inline Occurrence  ShortCircuit::getEquiB       () const { return Equipotential::getChildEqui( _occurrenceB ); }
  inline Box         ShortCircuit::getShortingBox () const { return _overlap->getBoundingBox(); }

  inline Box  ShortCircuit::getBoundingBoxA () const
  {
    Box bb = getComponentA()->getBoundingBox();
    _occurrenceA.getPath().getTransformation().applyOn( bb );
    return bb;
  }

  inline Box  ShortCircuit::getBoundingBoxB () const
  {
    Box bb = getComponentB()->getBoundingBox();
    _occurrenceB.getPath().getTransformation().applyOn( bb );
    return bb;
  }

  inline std::string  ShortCircuit::getEquiAName () const
  {
    if (isTopLevelA()) return "A is not an equi";
    Occurrence occEquiA = getEquiA();
    const Equipotential* equiA = static_cast<const Equipotential*>( occEquiA.getEntity() ); 
    std::string s = getString( occEquiA.getPath().getHeadInstance()->getName() ) + ":" + equiA->getName();
    return s;
  }

  inline std::string  ShortCircuit::getEquiBName () const
  {
    if (isTopLevelB()) return "B is not an equi";
    Occurrence occEquiB = getEquiB();
    const Equipotential* equiB = static_cast<const Equipotential*>( occEquiB.getEntity() ); 
    std::string s = getString( occEquiB.getPath().getHeadInstance()->getName() ) + ":" + equiB->getName();
    return s;
  }


}  // Tramontana namespace.


GETSTRING_POINTER_SUPPORT(Tramontana::ShortCircuit);
IOSTREAM_POINTER_SUPPORT(Tramontana::ShortCircuit);
