// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./CellGauge.cpp"                          |
// +-----------------------------------------------------------------+


#include <sstream>
#include "crlcore/CellGauge.h"
#include "crlcore/AllianceFramework.h"


namespace CRL {

  using namespace std;
  using Hurricane::JsonTypes;


// -------------------------------------------------------------------
// Class  :  "CRL::CellGauge".

  CellGauge::CellGauge ( const char*      name
                       , const char*      pinLayerName
                       , const DbU::Unit  pitch
                       , const DbU::Unit  sliceHeight
                       , const DbU::Unit  sliceStep
                       ) : _name        (name)
                         , _pinLayerName(pinLayerName)
                         , _pitch       (pitch)
                         , _sliceHeight (sliceHeight)
                         , _sliceStep   (sliceStep)
                         , _flags       (Flags::NoFlags)
  { }


  CellGauge::CellGauge ( const CellGauge& other )
    : _name        (other._name)
    , _pinLayerName(other._pinLayerName)
    , _pitch       (other._pitch)
    , _sliceHeight (other._sliceHeight)
    , _sliceStep   (other._sliceStep)
    , _flags       (other._flags)
  { }


  CellGauge* CellGauge::create ( const char*      name
                               , const char*      pinLayerName
                               , const DbU::Unit  pitch
                               , const DbU::Unit  sliceHeight
                               , const DbU::Unit  sliceStep
                               )
  {
    if ( !name || (name[0] == '\0') )
      throw Error ( "CellGauge::create() : Empty name." );
    
    CellGauge* gauge = new CellGauge ( name, pinLayerName, pitch, sliceHeight, sliceStep );

    if ( gauge == NULL )
      throw Error ( BadCreate, "CRL::CellGauge::create()" );

    return gauge;
  }


  CellGauge::~CellGauge()
  { }


  void  CellGauge::destroy ()
  { delete this; }


  CellGauge* CellGauge::getClone () const
  { return new CellGauge(*this); }


  string  CellGauge::_getTypeName () const
  { return _TName("CellGauge"); }


  string  CellGauge::_getString () const
  {
    return ( "<" + _TName("CellGauge ")
           + getString(_name)
         //+ " pinLayerName=" + getString(_pinLayerName)
         //+ " pitch="  + getValueString(_pitch)
         //+ " height=" + getValueString(_sliceHeight)
         //+ " step="   + getValueString(_sliceStep)
           + ">" );
  }


  Record* CellGauge::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot          ( "_name"            , &_name         ) );
    record->add( getSlot          ( "_pinLayerName"    , &_pinLayerName ) );
    record->add( DbU::getValueSlot( "_pitch"           , &_pitch        ) );
    record->add( DbU::getValueSlot( "_sliceHeight"     , &_sliceHeight  ) );
    record->add( DbU::getValueSlot( "_sliceStep"       , &_sliceStep    ) );
    record->add( getSlot          ( "_flags"           , &_flags        ) );
    return ( record );
  }


  void  CellGauge::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename"    , "CellGauge"   );
    jsonWrite( w, "_name"        , _name         );
    jsonWrite( w, "_pinLayerName", _pinLayerName );
    jsonWrite( w, "_pitch"       , _pitch        );
    jsonWrite( w, "_sliceHeight" , _sliceHeight  );
    jsonWrite( w, "_sliceStep"   , _sliceStep    );
    jsonWrite( w, "_flags"       , _flags        );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonCellGauge"

  Initializer<JsonCellGauge>  jsonCellGaugeInit ( 0 );


  void  JsonCellGauge::initialize ()
  { JsonTypes::registerType( new JsonCellGauge (JsonWriter::RegisterMode) ); }


  JsonCellGauge::JsonCellGauge( unsigned long flags )
    : JsonObject(flags)
  {
    add( "_name"        , typeid(string)  );
    add( "_pinLayerName", typeid(string)  );
    add( "_pitch"       , typeid(int64_t) );
    add( "_sliceHeight" , typeid(int64_t) );
    add( "_sliceStep"   , typeid(int64_t) );
  }


  string JsonCellGauge::getTypeName () const
  { return "CellGauge"; }


  JsonCellGauge* JsonCellGauge::clone ( unsigned long flags ) const
  { return new JsonCellGauge ( flags ); }


  void JsonCellGauge::toData ( JsonStack& stack )
  {
    check( stack, "JsonCellGauge::toData" );

    AllianceFramework* af           = get<AllianceFramework*>( stack, "_framework" );
    CellGauge*         cg           = NULL;
    string             name         = get<string> ( stack, "_name"         );
    string             pinLayerName = get<string> ( stack, "_pinLayerName" );
    DbU::Unit          pitch        = get<int64_t>( stack, "_pitch"        );
    DbU::Unit          sliceHeight  = get<int64_t>( stack, "_sliceHeight"  );
    DbU::Unit          sliceStep    = get<int64_t>( stack, "_sliceStep"    );
    unsigned long      flags        = get<int64_t>( stack, "_flags"        );

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
      if (af) {
        if (not name.empty()) {
          cg = CellGauge::create( name.c_str()
                                , pinLayerName.c_str()
                                , pitch
                                , sliceHeight
                                , sliceStep
                                );
          cg->setFlags( flags );
          af->addCellGauge( cg );
        }
      } else {
        cerr << Error( "JsonCellGauge::toData(): Missing \"_framework\" in stack context." ) << endl;
      }
    } else {
      if (af) {
        cg = af->getCellGauge( name );
      }
    }
  
    update( stack, cg );
}




} // End of CRL namespace.
