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
// |  C++ Module   :  "./EquipotentialComponents.cpp"                |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "tramontana/EquipotentialComponents.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Path;


// -------------------------------------------------------------------
// Class  :  "Tramontana::EquipotentialComponents".

  EquipotentialComponents::EquipotentialComponents ()
    : Super()
    , _equipotential(nullptr)
  { }


  EquipotentialComponents::EquipotentialComponents ( const Equipotential* equi )
    : Super()
    , _equipotential(equi)
  { }


  EquipotentialComponents::EquipotentialComponents ( const EquipotentialComponents& other )
    : Super()
    , _equipotential(other._equipotential)
  { }


  EquipotentialComponents& EquipotentialComponents::operator= ( const EquipotentialComponents& other )
  {
    _equipotential = other._equipotential;
    return *this;
  }


  Collection<Occurrence>* EquipotentialComponents::getClone () const
  { return new EquipotentialComponents( *this ); }


  Locator<Occurrence>* EquipotentialComponents::getLocator () const
  { return new Locator ( _equipotential ); }


  string  EquipotentialComponents::_getString () const
  {
    string s = "<EquipotentialComponents ";
    if (_equipotential) {
      s += " " + getString( _equipotential );
    } else {
      s += " NULL";
    }
    s += ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Tramontana::EquipotentialComponents::Locator".


  EquipotentialComponents::Locator::Locator ()
    : Super()
    , _equipotential     (nullptr)
    , _state             (Constructed)
    , _componentsIterator()
    , _netsIterator      ()
    , _childsIterator    ()
    , _childCompsLocator (nullptr)
    , _componentsLocator (nullptr)
  { }


  EquipotentialComponents::Locator::Locator ( const Equipotential* equi )
    : Super()
    , _equipotential     (equi)
    , _state             (Constructed)
    , _componentsIterator(equi->getComponents().end())
    , _netsIterator      (equi->getNets().end())
    , _childsIterator    (equi->getChilds().end())
    , _childCompsLocator (nullptr)
    , _componentsLocator (nullptr)
  {
    progress();
  }


  EquipotentialComponents::Locator::Locator ( const Locator& other )
    : Super()
    , _equipotential     (other._equipotential)
    , _state             (other._state)
    , _componentsIterator(other._componentsIterator)
    , _netsIterator      (other._netsIterator)
    , _childsIterator    (other._childsIterator)
    , _childCompsLocator (nullptr)
    , _componentsLocator (nullptr)
  {
    if (other._childCompsLocator) _childCompsLocator = other._childCompsLocator->getClone();
    if (other._componentsLocator) _componentsLocator = other._componentsLocator->getClone();
  }


  EquipotentialComponents::Locator& EquipotentialComponents::Locator::operator= ( const Locator& other )
  {
    _equipotential     = other._equipotential;
    _state             = other._state;
    _componentsIterator= other._componentsIterator;
    _netsIterator      = other._netsIterator;
    _childsIterator    = other._childsIterator;
    _componentsLocator = (other._componentsLocator) ? other._componentsLocator->getClone() : nullptr;
    _childCompsLocator = (other._childCompsLocator) ? other._childCompsLocator->getClone() : nullptr;
    return *this;
  }


  Occurrence  EquipotentialComponents::Locator::getElement () const
  {
    if (not _equipotential or (_state >= Finished)) return Occurrence();
    switch ( _state ) {
      case InComponents: return (*_componentsIterator);
      case InNets:       return Occurrence( _componentsLocator->getElement() );
      case InChildEquis: {
        Path compPath = (*_childsIterator).getPath();
        Path tailPath = _childCompsLocator->getElement().getPath();
        while ( not tailPath.isEmpty() ) {
          compPath = Path( compPath, tailPath.getHeadInstance() );
          tailPath = tailPath.getTailPath();
        }
        return Occurrence( _childCompsLocator->getElement().getEntity(), compPath );
      }
      default:
        break;
    }
    return Occurrence();
  }


  Locator<Occurrence>* EquipotentialComponents::Locator::getClone () const
  { return new Locator( *this ); }


  bool  EquipotentialComponents::Locator::isValid () const
  { return (_equipotential) and (_state < Finished); }


  void  EquipotentialComponents::Locator::progress ()
  {
    while ( isValid() ) {
      switch ( _state ) {
        case Constructed: {
          _state              = InComponents;
          _componentsIterator = _equipotential->getComponents().begin();
          if (_componentsIterator != _equipotential->getComponents().end()) return;
        }
        case InComponents: {
          if (_componentsIterator != _equipotential->getComponents().end()) {
            ++_componentsIterator;
            if (_componentsIterator != _equipotential->getComponents().end()) return;
          }
          _state        = InNets;
          _netsIterator = _equipotential->getNets().begin();
        }
        case InNets: {
          if (_netsIterator != _equipotential->getNets().end()) {
            if (not _componentsLocator) {
              _componentsLocator = (*_netsIterator)->getComponents().getLocator()->getClone();
              if (_componentsLocator->isValid()) return;
            } else {
              _componentsLocator->progress();
              if (_componentsLocator->isValid()) return;
            }

            _componentsLocator = nullptr;
            ++_netsIterator;
            if (_netsIterator != _equipotential->getNets().end())
              continue;
          }
          _state          = InChildEquis;
          _childsIterator = _equipotential->getChilds().begin();
        }
        case InChildEquis: {
          if (_childsIterator != _equipotential->getChilds().end()) {
            if (not _childCompsLocator) {
              Equipotential* child = dynamic_cast<Equipotential*>( (*_childsIterator).getEntity() );
              _childCompsLocator = child->getFlatComponents().getLocator()->getClone();
              if (_childCompsLocator->isValid()) return;
            } else {
              _childCompsLocator->progress();
              if (_childCompsLocator->isValid()) return;
            }

            _childCompsLocator = nullptr;
            ++_childsIterator;
            if (_childsIterator != _equipotential->getChilds().end())
              continue;
          }
          _state = Finished;
        }
        case Finished:
          break;
      }
    }
  }


  string EquipotentialComponents::Locator::_getString () const
  {
    string s = "<EquipotentialComponents::Locator";
    if (_equipotential) {
      s += " " + getString(_equipotential);
    } else {
      s += " NULL";
    }
    s += ">";
    return s;
  }


} // Tramontana namespace.
