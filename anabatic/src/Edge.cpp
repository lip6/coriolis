// -*- mode: C++; explicit-buffer-name: "Edge.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Edge.cpp"                                    |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/Segment.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "anabatic/Edge.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using Anabatic::NetData;
  using Anabatic::AnabaticEngine;


  class SortSegmentByLength {
    public:
      inline       SortSegmentByLength ( AnabaticEngine* );
      inline bool  operator()          ( const Segment*, const Segment* );
    private:
      AnabaticEngine* _anabatic;
  };


  inline  SortSegmentByLength::SortSegmentByLength ( AnabaticEngine* anabatic )
    : _anabatic(anabatic)
  { }


  inline bool  SortSegmentByLength::operator() ( const Segment* lhs, const Segment* rhs )
  {
    NetData* lhsNData = _anabatic->getNetData( lhs->getNet() );
    NetData* rhsNData = _anabatic->getNetData( rhs->getNet() );
    if (lhsNData->isGlobalFixed() and not  rhsNData->isGlobalFixed()) return true;
    if (rhsNData->isGlobalFixed() and not  lhsNData->isGlobalFixed()) return false;

    DbU::Unit delta = rhs->getLength() - lhs->getLength();
    if (delta > 0) return true;
    if (delta < 0) return false;
    return (lhs->getId() < rhs->getId());
  }

  
}  // Anonymous namespace.


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;
  using Hurricane::RoutingPad;


  Name       Edge::_extensionName = "Anabatic::Edge";
  DbU::Unit  Edge::unity          = 1;


  Edge::Edge ( GCell* source, GCell* target, Flags flags )
    : Super(source->getCell())
    , _flags            (flags|Flags::Invalidated)
    , _capacities       (NULL)
    , _reservedCapacity (0)
    , _realOccupancy    (0)
    , _estimateOccupancy(0.0)
    , _historicCost     (0.0)
    , _source           (source)
    , _target           (target)
    , _axis             (0)
    , _segments         ()
  { }


  void  Edge::_postCreate ()
  {
    Super::_postCreate();

    if (_flags.isset(Flags::Horizontal)) {
      _axis = std::max( _source->getYMin(), _target->getYMin() );
      _source->_add( this, Flags::EastSide );
      _target->_add( this, Flags::WestSide );
    } else {
      _axis = std::max( _source->getXMin(), _target->getXMin() );
      _source->_add( this, Flags::NorthSide );
      _target->_add( this, Flags::SouthSide );
    }
  }


  Edge* Edge::create ( GCell* source, GCell* target, Flags flags )
  {
    if (not source) throw Error( "Edge::create(): NULL source argument." );
    if (not target) throw Error( "Edge::create(): NULL target argument." );
    if (source == target)
      throw Error("Edge::create(): Source & target are the same (%s).", getString(source).c_str() );

    if (not (flags.intersect(Flags::Horizontal|Flags::Vertical))) {
      Box border = GCell::getBorder( source, target );

      if (border.isEmpty())
        throw Error( "Edge::create(): source & target GCells are *not* contiguous.\n"
                     "        source:%s\n"
                     "        target:%s"
                   , getString(source).c_str()
                   , getString(target).c_str()
                   );

      if (border.getYMin() == border.getYMax()) flags |= Flags::Horizontal;
      else                                      flags |= Flags::Vertical;
    }

    Edge* edge = new Edge ( source, target, flags );
    edge->_postCreate();

    cdebug_log(110,1) << "Edge::create(): " << /*(void*)edge << ":" <<*/ edge << endl;  
    cdebug_log(110,0) << "source:" << /*(void*)source << ":" <<*/ edge->getSource() << endl;
    cdebug_log(110,0) << "target:" << /*(void*)target << ":" <<*/ edge->getTarget() << endl;
    cdebug_tabw(110,-1);
    return edge;
  }


  Edge::~Edge ()
  { }


  void  Edge::_preDestroy ()
  {
    _source->getAnabatic()->_unrefCapacity( _capacities );
    _source->_remove( this, _flags|Flags::Source );
    _target->_remove( this, _flags|Flags::Target );

    Super::_preDestroy();
  }


  void  Edge::destroy ()
  {
    _preDestroy();
    delete this;
  }


  AnabaticEngine* Edge::getAnabatic () const
  { return (_source) ? _source->getAnabatic() : NULL; }


  DbU::Unit  Edge::getAxisMin () const
  {
    if (_flags.isset(Flags::Vertical))
      return std::max( _source->getXMin(), _target->getXMin() );
    return std::max( _source->getYMin(), _target->getYMin() );
  }


  GCell* Edge::getOpposite ( const GCell* from ) const
  {
    if (from == _source) return _target;
    if (from == _target) return _source;

    cerr << Error( "Edge::getOpposite(): On %s,\n"
                   "        \"from\" CGell id:%u is neither source nor target (S-id:%u, T-id:%u)."
                 , getString(this).c_str()
                 , from->getId()
                 , _source->getId()
                 , _target->getId()
                 ) << endl;
    return NULL;
  }


  Interval  Edge::getSide () const
  {
    Interval side;

    if (_flags.isset(Flags::Vertical))
      side = Interval( std::max(_source->getXMin(),_target->getXMin())
                     , std::min(_source->getXMax(),_target->getXMax()) );
    else
      side = Interval( std::max(_source->getYMin(),_target->getYMin())
                     , std::min(_source->getYMax(),_target->getYMax()) );

    return side;
  }


  DbU::Unit  Edge::getDistance () const
  {
    Point     sourceCenter = getSource()->getBoundingBox().getCenter();
    Point     targetCenter = getTarget()->getBoundingBox().getCenter();
    DbU::Unit dx           = targetCenter.getX() - sourceCenter.getX();
    DbU::Unit dy           = targetCenter.getY() - sourceCenter.getY();

    if (dx < 0) dx = -dx;
  //if (dx)     dx += DbU::fromLambda( 0.1 );

    return dx + ((dy > 0) ? dy : -dy);
  }


  void  Edge::incRealOccupancy ( int delta )
  {
    unsigned int occupancy = 0;
    if ((int)_realOccupancy + delta > 0) occupancy = _realOccupancy + delta;
    if ((_realOccupancy <= getCapacity()) and (occupancy >  getCapacity())) getAnabatic()->addOv   ( this );
    if ((_realOccupancy >  getCapacity()) and (occupancy <= getCapacity())) getAnabatic()->removeOv( this );
    _realOccupancy = occupancy;
  }


  void  Edge::incRealOccupancy2 ( int value )
  {
    _realOccupancy += value;
  }


  Segment* Edge::getSegment ( const Net* owner ) const
  {
    for ( Segment* segment : _segments ) {
      if (segment->getNet() == owner) return segment;
    }
    return NULL;
  }


  bool  Edge::isEnding ( Segment* segment ) const
  {
    cdebug_log(112,0) << "Edge::isEnding() " << this << endl;
    cdebug_log(112,0) << "| Sbb " << _source->getBoundingBox() << endl;
    cdebug_log(112,0) << "| Tbb " << _target->getBoundingBox() << endl;
    cdebug_log(112,0) << "| " << segment << endl;
                                                        
    if (Session::getRoutingGauge()->isTwoMetals()) {
      Contact* contact = dynamic_cast<Contact*>( segment->getSource() );
      cdebug_log(112,0) << "| source" << contact << endl;
      if (contact) {
        if (  (_source->isStdCellRow() and _source->getBoundingBox().contains(contact->getCenter()))
           or (_target->isStdCellRow() and _target->getBoundingBox().contains(contact->getCenter())) )
          return true;
      }

      contact = dynamic_cast<Contact*>( segment->getTarget() );
      cdebug_log(112,0) << "| target" << contact << endl;
      if (contact) {
        if (  (_source->isStdCellRow() and _source->getBoundingBox().contains(contact->getCenter()))
           or (_target->isStdCellRow() and _target->getBoundingBox().contains(contact->getCenter())) )
          return true;
      }
    } else {
    // This part is not used (yet). We don't call isEnding() when running
    // in over-the-cell mode.
      Contact* contact = dynamic_cast<Contact*>( segment->getSource() );
      cdebug_log(112,0) << "| source" << contact << endl;
      if (contact) {
        for ( Hook* hook : contact->getBodyHook()->getHooks() ) {
          if (dynamic_cast<RoutingPad*>(hook->getComponent())) {
            if (  _source->getBoundingBox().contains(contact->getCenter())
               or _target->getBoundingBox().contains(contact->getCenter()) )
              return true;
            break;
          }
        }
      }

      contact = dynamic_cast<Contact*>( segment->getTarget() );
      cdebug_log(112,0) << "| target" << contact << endl;
      if (contact) {
        for ( Hook* hook : contact->getBodyHook()->getHooks() ) {
          if (dynamic_cast<RoutingPad*>(hook->getComponent())) {
            if (  _source->getBoundingBox().contains(contact->getCenter())
               or _target->getBoundingBox().contains(contact->getCenter()) )
              return true;
            break;
          }
        }
      }
    }
    return false;
  }


  void  Edge::add ( Segment* segment )
  {
    _segments.push_back( segment );
    Horizontal* h = dynamic_cast<Horizontal*>(segment);
    Vertical*   v = dynamic_cast<Vertical*>(segment);
    DbU::Unit pitch = 0;
    if (h) pitch = Session::getGHorizontalPitch();
    if (v) pitch = Session::getGVerticalPitch();

    int deltaOccupancy = 0;
    
    if (not Session::getRoutingGauge()->isTwoMetals()) deltaOccupancy = segment->getWidth()/pitch;
    else {
    // In channel routing, do not increase edge occupancy on terminals,
    // because the capacity has already been decreased in annotedGlobalGraph (Katana).
      if (not isEnding(segment)) deltaOccupancy = segment->getWidth()/pitch;
    }

    incRealOccupancy( deltaOccupancy ); 
  }


  void  Edge::remove ( Segment* segment )
  {
    for ( size_t i=0 ; i<_segments.size() ; ++i ) {
      if (_segments[i] == segment) {
        cdebug_log(110,0) << "On " << this << endl;
        cdebug_log(110,0) << "| remove:" << segment << endl;

        std::swap( _segments[i], _segments[_segments.size()-1] );
        _segments.pop_back();

        Horizontal* h = dynamic_cast<Horizontal*>(segment);
        Vertical*   v = dynamic_cast<Vertical*>(segment);
        DbU::Unit pitch = 0;
        if (h) pitch = Session::getGHorizontalPitch();
        if (v) pitch = Session::getGVerticalPitch();

        int deltaOccupancy = 0;
    
        if (not Session::getRoutingGauge()->isTwoMetals()) deltaOccupancy = segment->getWidth()/pitch;
        else {
        // In channel routing, do not increase edge occupancy on terminals,
        // because the capacity has already been decreased in annotedGlobalGraph (Katana).
          if (not isEnding(segment)) deltaOccupancy = segment->getWidth()/pitch;
        }

        incRealOccupancy( -deltaOccupancy );
        return;
      }
    }

    cerr << Error( "On %s,\n        segment doesn't belong: %s"
                 , getString(this).c_str()
                 , getString(segment).c_str() ) << endl;
  }


  void  Edge::replace ( Segment* orig, Segment* repl )
  {
    for ( size_t i=0 ; i<_segments.size() ; ++i ) {
      if (_segments[i] == orig) {
        _segments[i] = repl;
        return;
      }
    }
  }


  size_t  Edge::ripup ()
  {
    AnabaticEngine* anabatic = getAnabatic();
    size_t          netCount = 0;

    sort( _segments.begin(), _segments.end(), SortSegmentByLength(anabatic) );

    if (Session::getRoutingGauge()->isTwoMetals()) {
      for ( size_t i=0 ; i<_segments.size() ; ) {
        if (not isEnding(_segments[i])) {
          NetData* netData = anabatic->getNetData( _segments[i]->getNet() );
          if (netData->isGlobalFixed ()) break;
          if (netData->isGlobalRouted()) ++netCount;
          anabatic->ripup( _segments[i], Flags::Propagate );
          continue;
        }
        ++i;
      }
    } else {
      size_t truncate = (size_t)( (getCapacity()*2) / 3 );
      while ( _segments.size() > truncate ) {
        NetData* netData = anabatic->getNetData( _segments[truncate]->getNet() );
        if (netData->isGlobalFixed ()) break;
        if (netData->isGlobalRouted()) ++netCount;
        anabatic->ripup( _segments[truncate], Flags::Propagate );
      }
      
    // DbU::Unit  globalThreshold = Session::getSliceHeight()*3;
    // for ( size_t i=0 ; i<_segments.size() ; ) {
    //   if (_segments[i]->getLength() >= globalThreshold) {
    //     NetData* netData = anabatic->getNetData( _segments[i]->getNet() );
    //     if (netData->isGlobalRouted()) ++netCount;
    //     anabatic->ripup( _segments[i], Flags::Propagate );
    //   } else {
    //     ++i;
    //   }
    // }
    }

    return netCount;
  }


  size_t  Edge::ripupAll ()
  {
    AnabaticEngine* anabatic = getAnabatic();
    size_t          netCount = 0;

    sort( _segments.begin(), _segments.end(), SortSegmentByLength(anabatic) );
    for ( size_t i=0 ; i<_segments.size() ; ) {
      if ((not _segments[i]) or isEnding(_segments[i])) {
	++i; continue;
      }
      NetData* netData = anabatic->getNetData( _segments[i]->getNet() );
      if (netData->isGlobalFixed ()) continue;
      if (netData->isGlobalRouted()) ++netCount;
      anabatic->ripup( _segments[i], Flags::Propagate );
    }
    return netCount;
  }


  void  Edge::_setSource ( GCell* source )
  {
    if (source == _target)
      throw Error("Edge::_setSource(): Source & target are the same (%s).", getString(source).c_str() );
    
    invalidate( false );
    _source=source;
  }


  void  Edge::_setTarget ( GCell* target )
  {
    if (_source == target)
      throw Error("Edge::_setTarget(): Source & target are the same (%s).", getString(target).c_str() );
    
    invalidate( false );
    _target=target;
  }


  void  Edge::invalidate ( bool )
  {
    cdebug_log(110,1) << "Edge::invalidate() " << this << endl;

    _flags |= Flags::Invalidated;
    Super::invalidate( false );

    cdebug_tabw(110,-1);
  }


  void  Edge::materialize ()
  {
    cdebug_log(110,1) << "Edge::materialize() " << this << endl;

    Flags    flags = _flags;
    Interval side  = getSide();
    _axis = side.getCenter();

    if      (getSource()->isStdCellRow() and getTarget()->isStdCellRow()) flags |= Flags::NullCapacity;
    else if (getSource()->isChannelRow() and getTarget()->isChannelRow()) flags |= Flags::InfiniteCapacity;

    _capacities = getAnabatic()->_createCapacity( _flags, side );

    _flags.reset( Flags::Invalidated );
    cdebug_log(110,0) << "Edge::materialize() " << this << endl;

    Super::materialize();

    cdebug_tabw(110,-1);
  }


  const Name& Edge::getName () const
  { return _extensionName; }


  Box  Edge::getBoundingBox () const
  {
    static DbU::Unit halfThickness = getAnabatic()->getConfiguration()->getEdgeWidth () / 2;
    static DbU::Unit halfLength    = getAnabatic()->getConfiguration()->getEdgeLength() / 2;

    Box bb;

    if (_flags.isset(Flags::Horizontal))
      bb = Box( _target->getXMin() - halfLength, _axis - halfThickness
              , _target->getXMin() + halfLength, _axis + halfThickness
              );
    else
      bb = Box( _axis - halfThickness, _target->getYMin() - halfLength
              , _axis + halfThickness, _target->getYMin() + halfLength
              );

    return bb;
  }


  bool  Edge::isMaxCapacity ( Net* net ) const 
  {
    unsigned int wpitch = 0;
    if (net) {
      cdebug_log(112,0) << "_capacity:" << getCapacity() << endl;

      Hurricane::NetRoutingState* state = Hurricane::NetRoutingExtension::get( net );
      wpitch = (state) ? state->getWPitch()-1 : 0;
    }
    return (_realOccupancy + wpitch >= getCapacity()); 
  }


  void  Edge::translate ( const DbU::Unit&, const DbU::Unit& )
  {
    cerr << Error( "Edge::translate(): On %s,\n"
                   "        Must never be called on a Edge object (ignored)."
                 , getString(this).c_str()
                 ) << endl;
  }


  string  Edge::_getTypeName () const
  { return getString(_extensionName); }


  string  Edge::_getString () const
  {
    Point center ( getSource()->getCenter() );

    string s = Super::_getString();
    s.insert( s.size()-1, " S:["+DbU::getValueString(center.getX()) );
    s.insert( s.size()-1, " "   +DbU::getValueString(center.getY()) );
    s.insert( s.size()-1, "] "  +DbU::getValueString(_axis) );
    s.insert( s.size()-1, " "   +getString(_realOccupancy) );
    s.insert( s.size()-1, "/"   +getString(getCapacity()) );
    s.insert( s.size()-1, " h:" +getString(_historicCost) );
    s.insert( s.size()-1, " "   +getString(_flags) );
    return s;
  }


  Record* Edge::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_flags"            ,  _flags            ) );
    record->add( getSlot("_capacities"       ,  _capacities       ) );
    record->add( getSlot("_reservedCapacity" ,  _reservedCapacity ) );
    record->add( getSlot("_realOccupancy"    ,  _realOccupancy    ) );
    record->add( getSlot("_estimateOccupancy",  _estimateOccupancy) );
    record->add( getSlot("_source"           ,  _source           ) );
    record->add( getSlot("_target"           ,  _target           ) );
    record->add( DbU::getValueSlot("_axis", &_axis) );
    record->add( getSlot("_segments"         , &_segments         ) );
    return record;
  }


}  // Anabatic namespace.
