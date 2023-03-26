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
// |  C++ Module   :  "./Tile.cpp"                                   |
// +-----------------------------------------------------------------+


#include <iomanip>
#include "hurricane/utilities/Path.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Timer.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "crlcore/Utilities.h"
#include "tramontana/Tile.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::vector;
  using std::make_pair;
  using Hurricane::dbo_ptr;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Box;
  using Hurricane::Layer;
  using Hurricane::Entity;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Tramontana::Tile".


  uint32_t       Tile::_time = 0;
  vector<Tile*>  Tile::_allocateds;


  Tile::Tile ( Occurrence occurrence, const BasicLayer* layer, const Box& boundingBox )
    : _occurrence   (occurrence) 
    , _layer        (layer)
    , _boundingBox  (boundingBox)
    , _equipotential(nullptr)
    , _flags        (0)
    , _parent       (nullptr)
    , _rank         (0)
    , _timeStamp    (0)
  {
    _allocateds.push_back( this );
  }


  Tile* Tile::create ( Occurrence occurrence, const BasicLayer* layer )
  {
    Component* component = dynamic_cast<Component*>( occurrence.getEntity() );
    if (not component) {
      cerr << Error( "Tile::create(): Must be build over an occurrence of *Component*.\n"
                     "        (%s)"
                   , getString(occurrence).c_str()
                   ) << endl;
      return nullptr;
    }
    if (not component->getLayer()->contains(layer)) {
      cerr << Error( "Tile::create(): Component layer does not contains \"%s\".\n"
                     "        (%s)"
                   , getString(layer->getName()).c_str()
                   , getString(occurrence).c_str()
                   ) << endl;
      return nullptr;
    }

    Box bb = component->getBoundingBox( layer );
    occurrence.getPath().getTransformation().applyOn( bb );
    Tile* tile = new Tile ( occurrence, layer, bb );

    return tile;
  }


  void  Tile::destroy ()
  {
    cdebug_log(160,1) << "Tile::destroy() " << this << endl;
    cdebug_tabw(160,-1);
  }


  Tile::~Tile ()
  { }


  Tile* Tile::getRoot ( uint32_t flags )
  {
    if (not getParent() and (not (flags & MergeEqui))) return this;
    
    Tile* root = this;
    while ( root->getParent() ) {
      if (flags & MergeEqui) {
        if (not root->getParent()->getEquipotential() and root->getEquipotential())
          root->getParent()->setEquipotential( root->getEquipotential() );
      }
      root = root->getParent();
    }

    if (flags & Compress) {
      Tile* current = this;
      while ( current != root ) {
        Tile* curParent = current->getParent();
        current->setParent( root );
        current = curParent;
      }
    }

    if (flags & MergeEqui) {
      Equipotential* rootEqui = root->getEquipotential();
      if (not rootEqui) {
        rootEqui = root->newEquipotential();
      }

      Tile* current = this;
      while ( current ) {
        if (current->isUpToDate()) break;
        if (current->getEquipotential()) {
          if (current->getEquipotential() != rootEqui) {
            rootEqui->merge( current->getEquipotential() );
          }
        } else {
          rootEqui->add( current->getOccurrence() );
        }
        current->syncTime();
        current = current->getParent();
      }
    }

    return root;
  }


  Tile* Tile::merge ( Tile* other )
  {
    Tile* root1 = getRoot();
    Tile* root2 = other->getRoot();
    if (root1 and (root1 == root2)) return root1;

    if (root1->getRank() < root2->getRank())
      std::swap( root1, root2 );
    if (root1->getRank() == root2->getRank())
      root1->incRank();
    root2->setParent( root1 );
  // Fuse root2 into root1 here!

    return root1;
  }


  Equipotential* Tile::newEquipotential ()
  {
    if (_equipotential) {
      cerr << Error( "Tile::newEquipotential(): Equipotential already created (ignoring).\n"
                     "        (on: %s)"
                   , getString(this).c_str()
                   ) << endl;
      return _equipotential;
    }

    _equipotential = Equipotential::create( _occurrence.getOwnerCell() );
    _equipotential->add( _occurrence );
    return _equipotential;
  }


  string  Tile::_getTypeName () const
  { return "Tramontana::Tile"; }


  string  Tile::_getString () const
  {
    ostringstream  os;
    os << "<Tile " << _boundingBox << " " << _layer->getName() << " " << _occurrence << ">";
    return os.str();
  }


  Record* Tile::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record) {
      record->add( getSlot( "_occurrence" , &_occurrence  ) );
      record->add( getSlot( "_layer"      ,  _layer       ) );
      record->add( getSlot( "_boundingBox", &_boundingBox ) );
      record->add( getSlot( "_flags"      , &_flags       ) );
    }
    return record;
  }


} // Tramontana namespace.
