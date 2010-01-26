
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DbU.cpp"                                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstring>
#include  <cstdlib>
#include  <limits>

#include  "hurricane/DbU.h"
#include  "hurricane/Error.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"


namespace Hurricane {


  const unsigned int  DbU::_maximalPrecision     = 3;
  unsigned int        DbU::_precision            = 1;
  double              DbU::_resolution           = 0.1;
  double              DbU::_gridsPerLambda       = 10.0;
  double              DbU::_physicalsPerGrid     = 1.0;
  unsigned int        DbU::_stringMode           = DbU::Symbolic;
  DbU::UnitPower      DbU::_stringModeUnitPower  = DbU::Nano;
  DbU::Unit           DbU::_symbolicSnapGridStep = DbU::lambda(1.0);
  DbU::Unit           DbU::_realSnapGridStep     = DbU::grid  (10.0);
  const DbU::Unit     DbU::Min                   = std::numeric_limits<long>::min();
  const DbU::Unit     DbU::Max                   = std::numeric_limits<long>::max();


// -------------------------------------------------------------------
// Class :  "Hurricane::DbUSlot".


  class DbUSlot : public Slot {

    public:
    // Constructor.
                               DbUSlot       ( const string& name, const DbU::Unit* data );
                               DbUSlot       (       string& name, const DbU::Unit* data );
    // Accessors.
      virtual string           getDataString () const;
      virtual Record*          getDataRecord () const;
      virtual DbUSlot*         getClone      () const;

    protected:
    // Internal: Attributes.
              const DbU::Unit* _unit;

    private:
    // Internal: Constructors.
                               DbUSlot      ( const DbUSlot& );
              DbUSlot&         operator=    ( const DbUSlot& );
  };


// Inline Member Functions.
           DbUSlot::DbUSlot       ( const string& name, const DbU::Unit* unit ) : Slot(name), _unit(unit) {}
           DbUSlot::DbUSlot       (       string& name, const DbU::Unit* unit ) : Slot(name), _unit(unit) {}
  string   DbUSlot::getDataString () const { return DbU::getValueString(*_unit); }
  Record*  DbUSlot::getDataRecord () const { return DbU::getValueRecord( _unit); }
  DbUSlot* DbUSlot::getClone      () const { return new DbUSlot(_name,_unit); }


// -------------------------------------------------------------------
// Class :  "Hurricane::DbU".


  unsigned int DbU::getPrecision ()
  { return _precision; }


  unsigned int DbU::getMaximalPrecision ()
  { return _maximalPrecision; }


  double  DbU::getResolution ()
  { return _resolution; }


  void DbU::setPrecision ( unsigned int precision )
  {
    if ( _maximalPrecision < precision)
      throw Error ( "DbU::Unit::setPrecision(): Precision %ud exceed maximal precision %ud."
                  , precision
                  , _maximalPrecision
                  );

    float scale = (float)precision / (float)_precision;

    _precision  = precision;

    _resolution = 1;
    while ( precision-- ) _resolution /= 10;

    DataBase::getDB()->getTechnology()->_onDbuChange ( scale );

    setSymbolicSnapGridStep ( DbU::lambda( 1.0) );
    setRealSnapGridStep     ( DbU::grid  (10.0) );
  }


  double DbU::getUnitPower ( UnitPower p )
  {
    switch ( p ) {
      case Pico:  return 1.0e-12;
      case Nano:  return 1.0e-9;
      case Micro: return 1.0e-6;
      case Milli: return 1.0e-3;
      case Unity: return 1.0;
      case Kilo:  return 1.0e+3;
    }
    return 1.0;
  }


  void  DbU::setPhysicalsPerGrid ( double physicalsPerGrid, UnitPower p )
  { _physicalsPerGrid = physicalsPerGrid * getUnitPower(p); }


  double  DbU::getPhysicalsPerGrid ()
  { return _physicalsPerGrid; }


  double  DbU::physicalToGrid ( double physical, UnitPower p )
  { return ( physical * getUnitPower(p) ) / _physicalsPerGrid; }


  void  DbU::setGridsPerLambda ( double gridsPerLambda )
  {
    if (   ( rint(gridsPerLambda) != gridsPerLambda ) 
        || ( remainder(gridsPerLambda,2.0) != 0.0   ) )
      throw Error ( "DbU::Unit::setGridPerLambdas(): \"gridsPerLambda\" (%f) must be an even integer."
                  , gridsPerLambda
                  );

    float scale = gridsPerLambda / (float)_gridsPerLambda;

    _gridsPerLambda = gridsPerLambda;

    DataBase::getDB()->getTechnology()->_onDbuChange ( scale );

    setSymbolicSnapGridStep ( DbU::lambda(1) );
  }


  double  DbU::getGridsPerLambda ()
  { return _gridsPerLambda; }


  DbU::Unit  DbU::getSymbolicSnapGridStep ()
  { return _symbolicSnapGridStep; }


  DbU::Unit  DbU::getOnSymbolicSnapGrid ( DbU::Unit u, SnapMode mode )
  {
    DbU::Unit  inferior = ( u / _symbolicSnapGridStep ) * _symbolicSnapGridStep;
    DbU::Unit  modulo   =   u % _symbolicSnapGridStep;

    if ( !modulo ) return u;
    if (  modulo < 0  ) inferior -= _symbolicSnapGridStep;

    if      ( mode == Inferior ) { return inferior; }
    else if ( mode == Superior ) { return inferior + _symbolicSnapGridStep; }
     
    if ( modulo < 0 )
      return inferior + ( (modulo > - (_symbolicSnapGridStep/2)) ? _symbolicSnapGridStep : 0 );

    return inferior + ( (modulo > (_symbolicSnapGridStep/2)) ? _symbolicSnapGridStep : 0 );
  }


  DbU::Unit  DbU::getRealSnapGridStep ()
  { return _realSnapGridStep; }


  DbU::Unit  DbU::getOnRealSnapGrid ( DbU::Unit u, SnapMode mode )
  {
    DbU::Unit  inferior = ( u / _realSnapGridStep ) * _realSnapGridStep;
    DbU::Unit  modulo   =   u % _realSnapGridStep;

    if ( !modulo ) return u;
    if (  modulo < 0  ) inferior -= _realSnapGridStep;

    if      ( mode == Inferior ) { return inferior; }
    else if ( mode == Superior ) { return inferior + _realSnapGridStep; }
     
    if ( modulo < 0 )
      return inferior + ( (modulo > - (_realSnapGridStep/2)) ? _realSnapGridStep : 0 );
     
    return inferior + ( (modulo > (_realSnapGridStep/2)) ? _realSnapGridStep : 0 );
  }


  DbU::Unit  DbU::getOnCustomGrid ( DbU::Unit u, DbU::Unit step, SnapMode mode )
  {
    DbU::Unit  inferior = ( u / step ) * step;
    DbU::Unit  modulo   = ( u % step );

    if ( !modulo ) return u;
    if (  modulo < 0  ) inferior -= step;

    if      ( mode == Inferior ) { return inferior; }
    else if ( mode == Superior ) { return inferior + step; }
     
    if ( modulo < 0 )
      return inferior + ( (modulo > - (step/2)) ? step : 0 );
     
    return inferior + ( (modulo > (step/2)) ? step : 0 );
  }


  void  DbU::setStringMode ( unsigned int mode, UnitPower p )
  {
    _stringMode = mode;
    if ( _stringMode == Physical ) _stringModeUnitPower = p;
  }


  string  DbU::getValueString ( DbU::Unit u, int mode )
  {
    char buffer[1024];
    char unitSymbol = 'u';

    if ( _stringMode == Grid ) {
      unitSymbol = 'g';
      snprintf ( buffer, 1024, "%.1f", getGrid(u) );
    } else if ( _stringMode == Symbolic ) {
      unitSymbol = 'l';
      snprintf ( buffer, 1024, "%.1f", getLambda(u) );
    } else if ( _stringMode == Physical ) {
      switch ( _stringModeUnitPower ) {
        case Pico:  unitSymbol = 'p'; break;
        case Nano:  unitSymbol = 'n'; break;
        case Micro: unitSymbol = 'm'; break;
        case Milli: unitSymbol = 'M'; break;
        case Unity: unitSymbol = 'U'; break;
        case Kilo:  unitSymbol = 'k'; break;
        default:    unitSymbol = '?'; break;
      }
      snprintf ( buffer, 1024, "%.1f", getPhysical(u,_stringModeUnitPower) );
    } else {
      if ( _stringMode != Db )
        cerr << "[ERROR] Unknown Unit representation mode: " << _stringMode << endl;

      snprintf ( buffer, 1024, "%ld", u );
    }

    size_t length = strlen(buffer) - 1;
    if ( mode & SmartTruncate ) {
      for ( ; length > 0 ; length-- ) {
        if ( buffer[length] == '.' ) { length--; break; }
        if ( buffer[length] != '0' ) break;
      }
    }
    buffer[++length] = unitSymbol;
    buffer[++length] = '\0';

    return buffer;
  }


  Record* DbU::getValueRecord ( const DbU::Unit* u )
  {
    Record* record = new Record(getValueString(*u));
    record->add(getSlot("DbU::Unit", u));
    return record;
  }


  Slot* DbU::getValueSlot ( const string& name, const DbU::Unit* u )
  {
    return new DbUSlot ( name, u );
  }


} // End of Hurricane namespace.
