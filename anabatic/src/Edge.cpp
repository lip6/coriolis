// -*- mode: C++; explicit-buffer-name: "Edge.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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
#include "anabatic/Edge.h"
#include "anabatic/GCell.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;


  Name  Edge::_extensionName = "Anabatic::Edge";


  Edge::Edge ( GCell* source, GCell* target, Flags flags )
    : Super(source->getCell())
    , _flags            (flags|Flags::Invalidated)
    , _capacity         (0)
    , _realOccupancy    (0)
    , _estimateOccupancy(0.0)
    , _source           (source)
    , _target           (target)
    , _axis             (0)
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

    cdebug.log(110,1) << "Edge::create(): " << (void*)edge << ":" << edge << endl;  
    cdebug.log(110) << "source:" << (void*)source << ":" << edge->getSource() << endl;
    cdebug.log(110) << "target:" << (void*)target << ":" << edge->getTarget() << endl;
    cdebug.tabw(110,-1);
    return edge;
  }


  Edge::~Edge ()
  { }


  void  Edge::_preDestroy ()
  {
    _source->_remove( this, _flags|Flags::SourceGCell );
    _target->_remove( this, _flags|Flags::TargetGCell );

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


  void  Edge::_setSource ( GCell* source )
  {
    if (source == _target)
      throw Error("Edge::_setSource(): Source & target are the same (%s).", getString(source).c_str() );
    
    invalidate(); _source=source;
  }


  void  Edge::_setTarget ( GCell* target )
  {
    if (_source == target)
      throw Error("Edge::_setTarget(): Source & target are the same (%s).", getString(target).c_str() );
    
    invalidate(); _target=target;
  }


  void  Edge::_revalidate ()
  {
    Interval side = getSide();
    _axis     = side.getCenter();
    _capacity = getAnabatic()->getCapacity( side.inflate(0,-1), _flags );

    _flags.reset( Flags::Invalidated );
    cdebug.log(110) << "Edge::_revalidate() " << this << endl;
  }


  const Name& Edge::getName () const
  { return _extensionName; }


  Box  Edge::getBoundingBox () const
  {
    static DbU::Unit halfThickness = getAnabatic()->getConfiguration()->getEdgeWidth () / 2;
    static DbU::Unit halfLength    = getAnabatic()->getConfiguration()->getEdgeLength() / 2;

    if (_flags.isset(Flags::Horizontal))
      return Box( _target->getXMin() - halfLength, _axis - halfThickness
                , _target->getXMin() + halfLength, _axis + halfThickness
                );

    return Box( _axis - halfThickness, _target->getYMin() - halfLength
              , _axis + halfThickness, _target->getYMin() + halfLength
              );
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
    string s = Super::_getString();
    s.insert( s.size()-1, " "+DbU::getValueString(_axis) );
    s.insert( s.size()-1, " "+getString(_realOccupancy) );
    s.insert( s.size()-1, "/"+getString(_capacity) );
    s.insert( s.size()-1, " "+getString(_flags) );
    return s;
  }


  Record* Edge::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot("_flags"            , _flags            ) );
    record->add( getSlot("_capacity"         , _capacity         ) );
    record->add( getSlot("_realOccupancy"    , _realOccupancy    ) );
    record->add( getSlot("_estimateOccupancy", _estimateOccupancy) );
    record->add( getSlot("_source"           , _source           ) );
    record->add( getSlot("_target"           , _target           ) );
    record->add( DbU::getValueSlot("_axis", &_axis) );
    return record;
  }


}  // Anabatic namespace.
