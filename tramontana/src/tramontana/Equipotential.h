// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
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
// |  C++ Header   :  "./tramontana/Equipotential.h"                 |
// +-----------------------------------------------------------------+


#pragma  once
#include <iostream>
#include <set>
#include "hurricane/Net.h"
#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Occurrences.h"
#include "tramontana/EquipotentialRelation.h"


namespace Tramontana {

  using Hurricane::Record;
  using Hurricane::Box;
  using Hurricane::DbU;
  using Hurricane::DBo;
  using Hurricane::Entity;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::NetSet;
  using Hurricane::Cell;
  using Hurricane::Component;
  using Hurricane::OccurrenceSet;
  using Hurricane::Occurrence;
  using Hurricane::Occurrences;


  class NetCompareByName {
    public:
      bool operator() ( const Net* lhs, const Net* rhs ) const;
  };


  class OccNetCompareByName {
    public:
      bool operator() ( const Occurrence& lhs, const Occurrence& rhs ) const;
  };


// -------------------------------------------------------------------
// Class  :  "Tramontana::ShortCircuit".

  class ShortCircuit {
    public:
      inline ShortCircuit ( Net*, Net*, Component* );
    private:
      Net*       _netA;
      Net*       _netB;
      Component* _shortCircuit;
  };

  
  inline ShortCircuit::ShortCircuit ( Net* a, Net* b, Component* shortCircuit )
    : _netA        (a)
    , _netB        (b)
    , _shortCircuit(shortCircuit)
  { }


// -------------------------------------------------------------------
// Class  :  "Tramontana::Equipotential".

  class Equipotential : public Entity {
    public:
      typedef  Entity  Super;
      typedef  std::map< Net*, std::pair<uint32_t,uint32_t>, NetCompareByName >  NetMap;
    public:
      static        Equipotential* get               ( Component* );
      static        Equipotential* get               ( Occurrence );
      static        Occurrence     getChildEqui      ( Occurrence );
    public:
      static        Equipotential* create            ( Cell* );
      inline        bool           isEmpty           () const;
      inline        bool           isBuried          () const;
      virtual       Cell*          getCell           () const;
      virtual       Box            getBoundingBox    () const;
      inline        std::string    getName           () const;
                    std::string    getFlagsAsString  () const;
      inline        Net::Type      getType           () const;
      inline        Net::Direction getDirection      () const;
                    void           show              () const;
      inline        bool           hasComponent      ( Component* ) const;
                    void           add               ( Occurrence, const Box& boundingBox=Box() );
                    void           merge             ( Equipotential* );
      inline        void           add               ( ShortCircuit* );
                    void           consolidate       ();
                    void           clear             ();
      inline  const OccurrenceSet& getComponents     () const;
      inline  const OccurrenceSet& getChilds         () const;
      inline  const NetMap&        getNets           () const;
                    Occurrences    getFlatComponents () const;
      inline  const std::vector<ShortCircuit*>&
                                   getShortCircuits  () const;
                    Record*        _getRecord        () const;
                    std::string    _getString        () const;
                    std::string    _getTypeName      () const;
    protected:                                       
      virtual       void           _postCreate       ();
      virtual       void           _preDestroy       ();
    private:                                         
                                   Equipotential     ( Cell* );
                                  ~Equipotential     ();
    private:                                         
                                   Equipotential     ( const Equipotential& ) = delete;
                    Equipotential& operator=         ( const Equipotential& ) = delete;
    private:
      Cell*                       _owner;
      Box                         _boundingBox;
      NetMap                      _nets;
      OccurrenceSet               _components;
      OccurrenceSet               _childs;
      std::string                 _name;
      Net::Type                   _type;
      Net::Direction              _direction;
      uint32_t                    _netCount;
      bool                        _isBuried;
      bool                        _isExternal;
      bool                        _isGlobal;
      bool                        _isAutomatic;
      bool                        _hasFused;
      std::vector<ShortCircuit*>  _shortCircuits;
  };


  
  inline       bool            Equipotential::isEmpty          () const { return _components.empty() and _childs.empty(); }
  inline       bool            Equipotential::isBuried         () const { return _isBuried; }
  inline const OccurrenceSet&  Equipotential::getComponents    () const { return _components; }
  inline const OccurrenceSet&  Equipotential::getChilds        () const { return _childs; }
  inline const Equipotential::NetMap&
                               Equipotential::getNets          () const { return _nets; }
  inline       std::string     Equipotential::getName          () const { return _name; }
  inline       Net::Type       Equipotential::getType          () const { return _type; }
  inline       Net::Direction  Equipotential::getDirection     () const { return _direction; }
  inline const std::vector<ShortCircuit*>&
                               Equipotential::getShortCircuits () const { return _shortCircuits; }
  inline       void            Equipotential::add              ( ShortCircuit* s ) { _shortCircuits.push_back( s ); }

  inline bool  Equipotential::hasComponent ( Component* component ) const
  {
    if (component->getCell() != getCell()) return false;
    EquipotentialRelation* relation = dynamic_cast<EquipotentialRelation*>(
                                        component->getNet()->getProperty( EquipotentialRelation::staticGetName() ));
    if (not relation) {
      relation = dynamic_cast<EquipotentialRelation*>(
                   component->getProperty( EquipotentialRelation::staticGetName() ));
    }
    if (not relation) return false;
    return (relation->getMasterOwner() == this);
  }


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::Equipotential);
