// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2010, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BloatProfile.cpp"                       |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "hurricane/DebugSession.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/Path.h"
#include "hurricane/Query.h"
#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "anabatic/GCell.h"
#include "etesian/BloatProperty.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DbU;
  using Hurricane::Entity;
  using Hurricane::Instance;
  using Hurricane::Path;
  using Hurricane::Transformation;
  using Hurricane::Occurrence;
  using Etesian::BloatState;
  using Etesian::BloatExtension;
  using Anabatic::Edge;
  using namespace Katana;

  class Slices;


// -------------------------------------------------------------------
// Class  :  "FlatInstance".

  class FlatInstance {
    public:
      static DbU::Unit   getXFromOccurrence ( Occurrence );
      static DbU::Unit   getYFromOccurrence ( Occurrence );
    public:
      inline             FlatInstance       ( Occurrence );
      inline DbU::Unit   getX               () const;
      inline Occurrence  getOccurrence      () const;
    private:
      DbU::Unit   _x;
      Occurrence  _instanceOcc;
  };


  DbU::Unit  FlatInstance::getXFromOccurrence ( Occurrence o )
  {
    Instance*      instance = dynamic_cast<Instance*>( o.getEntity() );
    Transformation transf   = instance->getTransformation();
    o.getPath().getTransformation().applyOn( transf );
    Box ab = instance->getMasterCell()->getAbutmentBox();
    transf.applyOn( ab );
    return ab.getXMin();
  }


  DbU::Unit  FlatInstance::getYFromOccurrence ( Occurrence o )
  {
    Instance*      instance = dynamic_cast<Instance*>( o.getEntity() );
    Transformation transf   = instance->getTransformation();

  //cerr << "Inst transf:" << transf << endl;
  //cerr << "Path transf:" << o.getPath().getTransformation() << endl;
    
    o.getPath().getTransformation().applyOn( transf );
    Box ab = instance->getMasterCell()->getAbutmentBox();
    transf.applyOn( ab );
    return ab.getYMin();
  }



  inline  FlatInstance::FlatInstance ( Occurrence o )
    : _x(getXFromOccurrence(o))
    , _instanceOcc(o)
  { }


  inline DbU::Unit   FlatInstance::getX          () const { return _x; }
  inline Occurrence  FlatInstance::getOccurrence () const { return _instanceOcc; }


  bool  operator< ( const FlatInstance& lhs, const FlatInstance& rhs )
  { return lhs.getX() < rhs.getX(); }


// -------------------------------------------------------------------
// Class  :  "Slice".

  class Slice {
    public:
      inline            Slice                 ( Slices*, GCell* );
      inline bool       useStaticBloatProfile () const;
      inline uint32_t   getBloatOverloadAdd   () const;
      inline DbU::Unit  getY                  () const;
      inline void       add                   ( Occurrence );
      inline void       sort                  ();
             void       tagOverloadeds        ( size_t& count, size_t& newCount );
    private:
      Slices*               _owner;
      GCell*                _left;
      vector<FlatInstance>  _instances;
  };


  inline Slice::Slice ( Slices* owner, GCell* left )
    : _owner    (owner)
    , _left     (left)
    , _instances()
  { }


  inline DbU::Unit  Slice::getY () const         { return _left->getYMin(); }
  inline void       Slice::add  ( Occurrence o ) { _instances.push_back( o ); }
  inline void       Slice::sort ()               { std::sort( _instances.begin(), _instances.end() ); }


// -------------------------------------------------------------------
// Class  :  "Slices".

  class Slices {
    public:
                           Slices         ( KatanaEngine* );
                          ~Slices         ();
      inline KatanaEngine* getKatana      () const;
      inline void          add            ( Occurrence );
      inline void          sort           ();
      inline void          tagOverloadeds ();
    private:
      KatanaEngine*   _katana;
      Box             _cellAb;
      DbU::Unit       _sliceHeight;
      vector<Slice*>  _slices;
  };


  Slices::Slices ( KatanaEngine* katana )
    : _katana     (katana)
    , _cellAb     (_katana->getCell()->getAbutmentBox())
    , _sliceHeight(_katana->getConfiguration()->getSliceHeight())
    , _slices     ()
  {
    GCell* left     = _katana->getSouthWestGCell();
    size_t slicesNb = _cellAb.getHeight() / _sliceHeight;

    for ( size_t islice=0 ; islice<slicesNb ; ++islice ) {
      if (not left)
        throw Error( "Slices::Slices(): No GCell under slice [%d] @%s in \"%s\"."
                   , islice, DbU::getValueString( (DbU::Unit)islice * _sliceHeight).c_str()
                   , getString(_katana->getCell()->getName()).c_str()
                   );

      if (not left->isMatrix())
        throw Error( "Slices::Slices(): Non-matrix GCell under slice [%d] @%s in \"%s\".\n"
                     "        (%s)"
                   , islice, DbU::getValueString( (DbU::Unit)islice * _sliceHeight).c_str()
                   , getString(_katana->getCell()->getName()).c_str()
                   , getString(left).c_str()
                   );

      if (left->getYMin() != (DbU::Unit)islice * _sliceHeight + _cellAb.getYMin())
        throw Error( "Slices::Slices(): Misaligned GCell under slice [%d] @%s in \"%s\".\n"
                     "        (%s)"
                   , islice, DbU::getValueString( (DbU::Unit)islice * _sliceHeight).c_str()
                   , getString(_katana->getCell()->getName()).c_str()
                   , getString(left).c_str()
                   );

      _slices.push_back( new Slice( this, left ) );
      left = left->getNorth();
    }
  }


  Slices::~Slices ()
  { for ( Slice* slice : _slices ) delete slice; }


  inline KatanaEngine* Slices::getKatana () const
  { return _katana; }


  inline void  Slices::sort ()
  { for ( Slice* slice : _slices ) slice->sort(); }


  inline void  Slices::add ( Occurrence o )
  {
    DbU::Unit y      = FlatInstance::getYFromOccurrence( o );
    size_t    islice = (y - _cellAb.getYMin()) / _sliceHeight;

    if (islice >= _slices.size()) {
      throw Error( "Slices::add(): Out of range instance occurrence placed at Y %s (slice:%u).\n"
                   "           (%s)"
                 , DbU::getValueString(y).c_str()
                 , islice
                 , o.getCompactString().c_str()
                 );
    }

    _slices[islice]->add( o );
  }
  
  
  inline void  Slices::tagOverloadeds ()
  {
    size_t count    = 0;
    size_t newCount = 0;
    for ( Slice* slice : _slices ) slice->tagOverloadeds( count, newCount );

    cmess2 << Dots::asUInt  ("     - Bloated cells"          ,count   ) << endl;
    cmess2 << Dots::asUInt  ("     - Bloated cells, new"     ,newCount) << endl;
  }


  inline bool  Slice::useStaticBloatProfile () const
  { return _owner->getKatana()->useStaticBloatProfile(); }


  inline uint32_t  Slice::getBloatOverloadAdd () const
  { return _owner->getKatana()->getBloatOverloadAdd(); }


  void  Slice::tagOverloadeds ( size_t& count, size_t& newCount )
  {
    GCell*  gcell  = _left;
    size_t  iLeft  = 0;
    size_t  iRight = 0;

    while ( gcell ) {
    //cerr << "> Under:"  << gcell << endl;

      uint32_t  overload = 0;
      for ( Edge* edge : gcell->getEdges( Flags::NorthSide|Flags::EastSide ) ) {
        if (edge->getRealOccupancy() > edge->getCapacity()) {
          overload = std::max( overload, edge->getRealOccupancy() - edge->getCapacity() );

          // if (edge->getRealOccupancy() > edge->getRawCapacity()) {
          //   overload += 4;
          // }
        }
      }

      iLeft = iRight;
      while ( iRight < _instances.size() ) {
        if (_instances[iRight].getX() >= gcell->getXMax()) break;
        ++iRight;
      }

      if (iLeft >= _instances.size()) break;

      if (overload) {
        overload += getBloatOverloadAdd();

        for ( size_t i=iLeft ; i<iRight ; ++i ) {
          BloatState* state = BloatExtension::get( _instances[i].getOccurrence() );
          if (not state) {
            if ( (_owner->getKatana()->getPassNumber() > 0) and useStaticBloatProfile() )
              continue;

            state = BloatExtension::create( _instances[i].getOccurrence(), overload );
            ++newCount;

          //cerr << "| Bloat: " << overload << " " << _instances[i].getOccurrence() << endl;
          } else {
            state->setTracksCount( state->getTracksCount() + overload );
          }
          ++count;
        }
      }

      gcell = gcell->getEast();
    }

    
  //   GCell* gcell     = _left;
  //   Edge*  eastEdge  = _left->getEastEdge();
  //   Edge*  northEdge = _left->getNorthEdge();
  //   bool   bloated   = false;

  // //cerr << "+ Slice @" << DbU::getValueString(getY()) << endl;

  //   for ( FlatInstance& fi : _instances ) {
  //   //cerr << "| @" << DbU::getValueString(fi.getX()) << " " << fi.getOccurrence() << endl;

  //     if (fi.getX() >= gcell->getXMax()) {
  //       for ( gcell = gcell->getEast() ; gcell and (fi.getX() > gcell->getXMax())
  //           ; gcell = gcell->getEast() ) {
  //       //cerr << "| Skip " << gcell << endl;
  //       }
  //     //cerr << "| Advance to " << gcell << endl;
  //       if (not gcell) break;
        
  //       bloated   = false;
  //       eastEdge  = gcell->getEastEdge();
  //       northEdge = gcell->getNorthEdge();
  //     }

  //     unsigned int overload = 0;

  //     if (eastEdge) {
  //       if (eastEdge->getRealOccupancy() > eastEdge->getCapacity()) {
  //         overload = eastEdge->getRealOccupancy() - eastEdge->getCapacity();
  //       }
  //       // else if (eastEdge->getHistoricCost() > 3.0) {
  //       //   overload = 4;
  //       // }
  //     }

  //     if (northEdge) {
  //       if (northEdge->getRealOccupancy() > northEdge->getCapacity()) {
  //         overload = std::max( overload, northEdge->getRealOccupancy() - northEdge->getCapacity() );
  //       }
  //       // else if (northEdge->getHistoricCost() > 3.0) {
  //       //   overload = 4;
  //       // }
  //     }

  //     if (overload and not bloated) {
  //       bloated   = true;
  //       overload += 8;
        
  //       BloatState* state = BloatExtension::get( fi.getOccurrence() );
  //       if (not state) {
  //         state = BloatExtension::create( fi.getOccurrence(), overload );
  //       //cerr << "> Bloat: " << fi.getOccurrence() << endl;
  //       //cerr << "> Under:"  << gcell << endl;
  //         ++newCount;
  //       } else {
  //         state->setTracksCount( state->getTracksCount() + overload );
  //       }
  //       ++count;
  //     }
  //   }
  } 


} // Anonymous namespace.


namespace Katana {

  using CRL::CatalogExtension;


  void  KatanaEngine::_buildBloatProfile ()
  {
    cmess1 << "  o  Build bloat profile on \"" << getCell()->getName() << "\"." << endl;
    
    Slices slices ( this );

    for ( Occurrence occurrence : getCell()->getLeafInstanceOccurrences() )
    {
      Instance* instance     = static_cast<Instance*>(occurrence.getEntity());
      Cell*     masterCell   = instance->getMasterCell();

      if (CatalogExtension::isFeed(masterCell)) continue;

      slices.add( occurrence );
    }

    slices.sort();
    slices.tagOverloadeds();
  } 


}  // Katana namespace.
