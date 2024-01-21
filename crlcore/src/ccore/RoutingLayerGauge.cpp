// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |               C o r e   L i b r a r y                           |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingLayerGauge.cpp"                  |
// +-----------------------------------------------------------------+




#include  <sstream>
#include "hurricane/BasicLayer.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "crlcore/RoutingLayerGauge.h"
#include "crlcore/RoutingGauge.h"


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
  using Hurricane::JsonTypes;
  using Hurricane::Tabulation;
  using Hurricane::BasicLayer;
  using Hurricane::tab;
  using Hurricane::DataBase;
  using Hurricane::Technology;


// -------------------------------------------------------------------
// Class  :  "RoutingLayerGauge"

  RoutingLayerGauge::RoutingLayerGauge ( const Layer*              layer
                                       , Constant::Direction       direction
                                       , Constant::LayerGaugeType  type
                                       , unsigned int              depth
                                       , double                    density
                                       , DbU::Unit                 offset
                                       , DbU::Unit                 pitch
                                       , DbU::Unit                 wireWidth
                                       , DbU::Unit                 pwireWidth
                                       , DbU::Unit                 viaWidth
                                       , DbU::Unit                 obsDw )
    : _routingGauge (nullptr)
    , _layer        (layer)
    , _blockageLayer(layer->getBlockageLayer())
    , _direction    (direction)
    , _type         (type)
    , _depth        (depth)
    , _density      (density)
    , _offset       (offset)
    , _pitch        (pitch)
    , _wireWidth    (wireWidth)
    , _pwireWidth   (pwireWidth)
    , _viaWidth     (viaWidth)
    , _obstacleDw   (obsDw)
  { }


  RoutingLayerGauge* RoutingLayerGauge::create ( const Layer*              layer
                                               , Constant::Direction       direction
                                               , Constant::LayerGaugeType  type
                                               , unsigned int              depth
                                               , double                    density
                                               , DbU::Unit                 offset
                                               , DbU::Unit                 pitch
                                               , DbU::Unit                 wireWidth
                                               , DbU::Unit                 pwireWidth
                                               , DbU::Unit                 viaWidth
                                               , DbU::Unit                 obsDw )
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
                                                     , pwireWidth
                                                     , viaWidth
                                                     , obsDw );

    if (offset % DbU::oneGrid)
      cerr << Error( "RoutingLayerGauge::create(): For %s,\n"
                     "        offset %s is not on foundry grid %s."
                   , getString( gauge ).c_str()
                   , DbU::getValueString( offset ).c_str()
                   , DbU::getValueString( DbU::oneGrid ).c_str()
                   ) << endl;
    if (pitch % DbU::oneGrid)
      cerr << Error( "RoutingLayerGauge::create(): For %s,\n"
                     "        pitch %s is not on foundry grid %s."
                   , getString( gauge ).c_str()
                   , DbU::getValueString( pitch ).c_str()
                   , DbU::getValueString( DbU::oneGrid ).c_str()
                   ) << endl;
    if (wireWidth % DbU::twoGrid)
      cerr << Error( "RoutingLayerGauge::create(): For %s,\n"
                     "        wireWidth %s is not an even multiple of foundry grid %s."
                   , getString( gauge ).c_str()
                   , DbU::getValueString( wireWidth ).c_str()
                   , DbU::getValueString( DbU::oneGrid ).c_str()
                   ) << endl;
    if (pwireWidth % DbU::twoGrid)
      cerr << Error( "RoutingLayerGauge::create(): For %s,\n"
                     "        pwireWidth %s is not an even multiple of foundry grid %s."
                   , getString( gauge ).c_str()
                   , DbU::getValueString( pwireWidth ).c_str()
                   , DbU::getValueString( DbU::oneGrid ).c_str()
                   ) << endl;
    if (viaWidth % DbU::twoGrid)
      cerr << Error( "RoutingLayerGauge::create(): For %s,\n"
                     "        viaWidth %s is not an even multiple of foundry grid %s."
                   , getString( gauge ).c_str()
                   , DbU::getValueString( viaWidth ).c_str()
                   , DbU::getValueString( DbU::oneGrid ).c_str()
                   ) << endl;

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
    quotient = ( dividend - _offset ) / _pitch - ((dividend < _offset) ? 1 : 0);
    modulo   = ( dividend - _offset ) % _pitch;
  }


  unsigned  RoutingLayerGauge::getTrackNumber ( DbU::Unit start, DbU::Unit stop ) const
  {
    if (start > stop)
      throw Error( badInterval
                 , getString(this).c_str()
                 , DbU::getValueString(start).c_str()
                 , DbU::getValueString(stop).c_str()
                 );

    long startModulo;
    long startQuotient;
    long stopModulo;
    long stopQuotient;
    long trackNumber;

    divide( start, startQuotient, startModulo );
    divide( stop , stopQuotient , stopModulo  );

    // cerr << "getTrackNumber() " << getLayer()->getName()
    //      << " start:" << startQuotient << " modulo:" << startModulo
    //      << " stop:"  << stopQuotient  << " modulo:" << stopModulo
    //      << endl;

    if (startModulo != 0) startQuotient += 1;
    if (stopModulo  == 0) stopQuotient  -= 1;

    trackNumber = stopQuotient - startQuotient + 1;

    // cerr << "getTrackNumber() " << getLayer()->getName()
    //      << " [" << DbU::getValueString(start) << " " << DbU::getValueString(stop)
    //      << "] tracks:" << trackNumber
    //      << " start:" << startQuotient << " stop:" << stopQuotient
    //      << endl;

    return (trackNumber>0) ? trackNumber : 0;
  }


  long  RoutingLayerGauge::getTrackIndex ( DbU::Unit start, DbU::Unit stop, DbU::Unit position, unsigned mode ) const
  {
    cdebug_log(100,1) << "RoutingLayerGauge::getTrackIndex ( " << DbU::getValueString(position)
                      << " DbU=" << position << " )" << endl;

    long  modulo;
    long  index;

    divide ( position-start, index, modulo );
    cdebug_log(100,0) << "index := " << index << endl;

    if ( index < 0 ) {
    //cdebug_tabw(100,-1);
    //return 0;

    //throw Error ( negativeIndex
    //            , getString(this).c_str()
    //            , DbU::getValueString(position).c_str()
    //            , DbU::getValueString(start).c_str()
    //            , DbU::getValueString(stop).c_str()
    //            );
    }

    if ( ( mode & Constant::Exact ) and ( modulo != 0 ) )
      throw Error ( badExactPosition, getString(this).c_str(), DbU::getValueString(position).c_str() );

    if ( mode & Constant::Superior ) {
      if ( modulo != 0 ) index++;
    } else if ( mode & Constant::Nearest ) {
      if ( modulo > _pitch / 2 ) index++;
    }

    // unsigned int tracksNumber = getTrackNumber(start,stop);
    // if ( (unsigned)index > tracksNumber ) {
    //   cdebug_tabw(100,-1);
    //   return (tracksNumber > 0) ? tracksNumber-1 : 0;
    //   // throw Error ( overflowIndex
    //   //             , getString(this).c_str()
    //   //             , DbU::getValueString(position).c_str()
    //   //             , DbU::getValueString(start).c_str()
    //   //             , DbU::getValueString(stop).c_str()
    //   //             );
    // }

    cdebug_tabw(100,-1);

    return index;
  }


  DbU::Unit  RoutingLayerGauge::getTrackPosition ( DbU::Unit start, long index ) const
  { return index * _pitch + _offset + start; }


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

    os << _depth << " "
       << DbU::getValueString(_offset)    << ",+"
       << DbU::getValueString(_pitch)     << " "
       << _density                        << "% ("
       << DbU::getValueString(_wireWidth) << ","
       << DbU::getValueString(_viaWidth)  << ")>";

    return os.str();
  }


  Record* RoutingLayerGauge::_getRecord () const
  {
    Record* record = new Record ( getString(this) );

    record->add ( getSlot ( "_routingGauge",  _routingGauge ) );
    record->add ( getSlot ( "_layer"       ,  _layer        ) );
    record->add ( getSlot ( "_direction"   ,  _direction    ) );
    record->add ( getSlot ( "_type"        ,  _type         ) );
    record->add ( getSlot ( "_depth"       ,  _depth        ) );
    record->add ( getSlot ( "_density"     ,  _density      ) );

    record->add ( DbU::getValueSlot ( "_offset"   , &_offset    ) );
    record->add ( DbU::getValueSlot ( "_pitch"    , &_pitch     ) );
    record->add ( DbU::getValueSlot ( "_wireWidth", &_wireWidth ) );
    record->add ( DbU::getValueSlot ( "_viaWidth" , &_viaWidth  ) );

    return ( record );
  }


  void  RoutingLayerGauge::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", "RoutingLayerGauge" );
    jsonWrite( w, "_layer"        , _layer->getName() );
    jsonWrite( w, "_direction"    , getString(&_direction) );
    jsonWrite( w, "_type"         , getString(&_type     ) );
    jsonWrite( w, "_depth"        , _depth      );
    jsonWrite( w, "_density"      , _density    );
    jsonWrite( w, "_offset"       , _offset     );
    jsonWrite( w, "_pitch"        , _pitch      );
    jsonWrite( w, "_wireWidth"    , _wireWidth  );
    jsonWrite( w, "_pwireWidth"   , _pwireWidth );
    jsonWrite( w, "_viaWidth"     , _viaWidth   );
    jsonWrite( w, "_obstacleDw"   , _obstacleDw );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonRoutingLayerGauge"

  Initializer<JsonRoutingLayerGauge>  jsonRoutingLayerGaugeInit ( 0 );


  void  JsonRoutingLayerGauge::initialize ()
  { JsonTypes::registerType( new JsonRoutingLayerGauge (JsonWriter::RegisterMode) ); }


  JsonRoutingLayerGauge::JsonRoutingLayerGauge( unsigned long flags )
    : JsonObject(flags)
  {
    add( ".RoutingGauge" , typeid(RoutingGauge*) );
    add( "_layer"        , typeid(string) );
    add( "_direction"    , typeid(string) );
    add( "_type"         , typeid(string) );
    add( "_depth"        , typeid(unsigned int) );
    add( "_density"      , typeid(double)  );
    add( "_offset"       , typeid(int64_t) );
    add( "_pitch"        , typeid(int64_t) );
    add( "_wireWidth"    , typeid(int64_t) );
    add( "_viaWidth"     , typeid(int64_t) );
    add( "_obstacleDw"   , typeid(int64_t) );
  }


  string JsonRoutingLayerGauge::getTypeName () const
  { return "RoutingLayerGauge"; }


  JsonRoutingLayerGauge* JsonRoutingLayerGauge::clone ( unsigned long flags ) const
  { return new JsonRoutingLayerGauge ( flags ); }


  void JsonRoutingLayerGauge::toData ( JsonStack& stack )
  {
    check( stack, "JsonRoutingLayerGauge::toData" );

    Technology*        techno      = DataBase::getDB()->getTechnology();
    RoutingGauge*      rg          = get<RoutingGauge*>( stack, ".RoutingGauge"  );
    RoutingLayerGauge* rlg         = NULL;
    string             layer       = get<string>       ( stack, "_layer"         );
    unsigned int       depth       = get<int64_t>      ( stack, "_depth"         );
    double             density     = get<double>       ( stack, "_density"       );
    DbU::Unit          offset      = get<int64_t>      ( stack, "_offset"        );
    DbU::Unit          pitch       = get<int64_t>      ( stack, "_pitch"         );
    DbU::Unit          wireWidth   = get<int64_t>      ( stack, "_wireWidth"     );
    DbU::Unit          pwireWidth  = get<int64_t>      ( stack, "_pwireWidth"    );
    DbU::Unit          viaWidth    = get<int64_t>      ( stack, "_viaWidth"      );
    DbU::Unit          obstacleDw  = get<int64_t>      ( stack, "_obstacleDw"    );

    Constant::Direction       direction;
    Constant::LayerGaugeType  type;
    from( direction, get<string>(stack,"_direction") );
    from( type     , get<string>(stack,"_type"     ) );

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
      if (rg) {
        if (not layer.empty()) {
          rlg = RoutingLayerGauge::create( techno->getLayer(layer)
                                         , direction
                                         , type
                                         , depth
                                         , density
                                         , offset
                                         , pitch
                                         , wireWidth
                                         , pwireWidth
                                         , viaWidth
                                         , obstacleDw
                                         );
          rg->addLayerGauge( rlg );
        }
      } else {
        cerr << Error( "JsonRoutingLayerGauge::toData(): Missing \".RoutingGauge\" in stack context." ) << endl;
      }
    } else {
      if (rg) {
        rlg = rg->getLayerGauge( techno->getLayer(layer) );
      }
    }
  
    update( stack, rlg );
}


} // Coriolis namespace.
