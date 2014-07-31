// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |               C o r e   L i b r a r y                           |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingLayerGauge.cpp"                  |
// +-----------------------------------------------------------------+




#include  <sstream>

#include  "hurricane/BasicLayer.h"

#include  "crlcore/RoutingLayerGauge.h"


namespace {


  // ---------------------------------------------------------------
  // Local Variables.

  // const char* notBasicLayer =
  //   "%s :\n\n"
  //   "    %s is not a <BasicLayer>.\n";

  // const char* notConductingLayer =
  //   "%s :\n\n"
  //   "    %s is not a CONDUCTING <BasicLayer>.\n";

  // const char* negativeIndex =
  //   "RoutingLayerGauge::getTrackIndex() :\n\n"
  //   "    In gauge %s :\n"
  //   "      Track position %s leads to a negative index [%s,%s].\n";
  // 
  // const char* overflowIndex =
  //   "RoutingLayerGauge::getTrackIndex() :\n\n"
  //   "    In gauge %s :\n"
  //   "      Track position %s leads to an index out of bounds [%s,%s].\n";

  const char* badExactPosition =
    "RoutingLayerGauge::getTrackIndex() :\n\n"
    "    In gauge %s :\n"
    "      Track position %s do not exactly match a track.\n";

  const char* badInterval =
    "RoutingLayerGauge::getTrackNumber() :\n\n"
    "    In gauge %s :\n"
    "      Invalid interval [%s,%s] given for track number calculation.\n";


} // End of anonymous namespace.



namespace Constant {


  Direction  perpandicular ( unsigned int direction )
  {
    switch ( direction ) {
      case Horizontal: return Vertical;
      case Vertical:
      default:
        break;
    }
    return Horizontal;
  }


} // End of Constants namespace.




namespace CRL {


  using namespace std;

  using Hurricane::Tabulation;
  using Hurricane::BasicLayer;
  using Hurricane::trace_in;
  using Hurricane::trace_out;
  using Hurricane::in_trace;
  using Hurricane::tab;


// -------------------------------------------------------------------
// Class  :  "RoutingLayerGauge"


  RoutingLayerGauge::RoutingLayerGauge ( const Layer*  layer
                                       , unsigned int  direction
                                       , unsigned int  type
                                       , unsigned int  depth
                                       , double        density
                                       , DbU::Unit     offset
                                       , DbU::Unit     pitch
                                       , DbU::Unit     wireWidth
                                       , DbU::Unit     viaWidth )
      : _layer        (layer)
      , _blockageLayer(layer->getBlockageLayer())
      , _direction    (direction)
      , _type         (type)
      , _depth        (depth)
      , _density      (density)
      , _offset       (offset)
      , _pitch        (pitch)
      , _wireWidth    (wireWidth)
      , _viaWidth     (viaWidth)
  { }


  RoutingLayerGauge* RoutingLayerGauge::create ( const Layer*  layer
                                               , unsigned int  direction
                                               , unsigned int  type
                                               , unsigned int  depth
                                               , double        density
                                               , DbU::Unit     offset
                                               , DbU::Unit     pitch
                                               , DbU::Unit     wireWidth
                                               , DbU::Unit     viaWidth )
  {
//  Temporary: must write a more smart check.
//  BasicLayer* basicLayer = dynamic_cast<BasicLayer*>(layer);
//  if ( basicLayer == NULL )
//    throw Error ( notBasicLayer, "CRL::RoutingLayerGauge::create()", getString(layer).c_str() );

//  if ( basicLayer->getMaterial() != BasicLayer::Material::metal )
//    throw Error ( notConductingLayer, "CRL::RoutingLayerGauge::create()", getString(layer).c_str() );

    RoutingLayerGauge* gauge = new RoutingLayerGauge ( layer
                                                     , direction
                                                     , type
                                                     , depth
                                                     , density
                                                     , offset
                                                     , pitch
                                                     , wireWidth
                                                     , viaWidth );

    return gauge;
  }


  void  RoutingLayerGauge::_preDestroy ()
  { }


  void  RoutingLayerGauge::destroy ()
  {
    _preDestroy ();

    delete this;
  }


  RoutingLayerGauge::~RoutingLayerGauge ()
  { }


  void  RoutingLayerGauge::divide ( DbU::Unit dividend, long& quotient, long& modulo ) const
  {
    quotient = ( dividend - _offset ) / _pitch;
    modulo   = ( dividend - _offset ) % _pitch;
  }


  unsigned  RoutingLayerGauge::getTrackNumber ( DbU::Unit start, DbU::Unit stop ) const
  {
    if ( start > stop )
      throw Error ( badInterval
                  , getString(this).c_str()
                  , DbU::getValueString(start).c_str()
                  , DbU::getValueString(stop).c_str()
                  );

    long startModulo;
    long startQuotient;
    long stopModulo;
    long stopQuotient;
    long trackNumber;

    divide ( start, startQuotient, startModulo );
    divide ( stop , stopQuotient , stopModulo  );

    if ( startModulo != 0 ) startQuotient += 1;

    trackNumber = stopQuotient - startQuotient + 1;

    return ( (trackNumber>0) ? trackNumber : 0 );
  }


  unsigned  RoutingLayerGauge::getTrackIndex ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const
  {
    trace << "RoutingLayerGauge::getTrackIndex ( " << position << " )" << endl;
    trace_in ();

    long  modulo;
    long  depth;

    divide ( position-start, depth, modulo );

    trace << "depth := " << depth << endl;

    if ( depth < 0 ) {
      trace_out ();
      return 0;

      // throw Error ( negativeIndex
      //             , getString(this).c_str()
      //             , DbU::getValueString(position).c_str()
      //             , DbU::getValueString(start).c_str()
      //             , DbU::getValueString(stop).c_str()
      //             );
    }

    if ( ( mode & Constant::Exact ) and ( modulo != 0 ) )
      throw Error ( badExactPosition, getString(this).c_str(), DbU::getValueString(position).c_str() );

    if ( mode & Constant::Superior ) {
      if ( modulo != 0 ) depth++;
    } else if ( mode & Constant::Nearest ) {
      if ( modulo > _pitch / 2 ) depth++;
    }

    unsigned int tracksNumber = getTrackNumber(start,stop);
    if ( (unsigned)depth >= tracksNumber ) {
      trace_out ();
      return (tracksNumber > 0) ? tracksNumber-1 : 0;
      // throw Error ( overflowIndex
      //             , getString(this).c_str()
      //             , DbU::getValueString(position).c_str()
      //             , DbU::getValueString(start).c_str()
      //             , DbU::getValueString(stop).c_str()
      //             );
    }

    trace_out ();

    return depth;
  }


  DbU::Unit  RoutingLayerGauge::getTrackPosition ( DbU::Unit start, unsigned depth ) const
  {
    return depth * _pitch + _offset + start;
  }


  string  RoutingLayerGauge::_getTypeName () const
  {
    return "CRL::RoutingLayerGauge";
  }


  string  RoutingLayerGauge::_getString () const
  {
    ostringstream  os;

    os << "<" << "RoutingLayerGauge " << _layer->getName() << " ";

    if ( _direction == Constant::Horizontal ) os << "Horizontal ";
    else                                      os << "Vertical ";

    os << DbU::getValueString(_offset)    << ",+"
       << DbU::getValueString(_pitch)     << " "
       << _density                        << "% ("
       << DbU::getValueString(_wireWidth) << ","
       << DbU::getValueString(_viaWidth)  << ")>";

    return os.str();
  }


  Record* RoutingLayerGauge::_getRecord () const
  {
    Record* record = new Record ( getString(this) );

    record->add ( getSlot ( "_layer"    ,  _layer     ) );
    record->add ( getSlot ( "_direction",  _direction ) );
    record->add ( getSlot ( "_type"     ,  _type      ) );
    record->add ( getSlot ( "_depth"    ,  _depth     ) );
    record->add ( getSlot ( "_density"  ,  _density   ) );
    record->add ( getSlot ( "_offset"   ,  _offset    ) );
    record->add ( getSlot ( "_pitch"    ,  _pitch     ) );
    record->add ( getSlot ( "_wireWidth",  _wireWidth ) );
    record->add ( getSlot ( "_viaWidth" ,  _viaWidth  ) );

    return ( record );
  }


} // End of Coriolis namespace.
