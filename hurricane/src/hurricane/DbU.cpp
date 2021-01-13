// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DbU.cpp"                                     |
// +-----------------------------------------------------------------+


#include <cstring>
#include <cstdlib>
#include <limits>
#include "hurricane/DbU.h"
#include "hurricane/Error.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"


namespace Hurricane {


  const unsigned int  DbU::_maximalPrecision     = 3;
  unsigned int        DbU::_precision            = 1;
  double              DbU::_resolution           = 0.1;
  double              DbU::_gridsPerLambda       = 10.0;
  double              DbU::_physicalsPerGrid     = 1.0;
  double              DbU::_gridMax              = DbU::toGrid    ( DbU::Max );
  double              DbU::_lambdaMax            = DbU::toLambda  ( DbU::Max );
  double              DbU::_physicalMax          = DbU::toPhysical( DbU::Max, DbU::Unity );
  unsigned int        DbU::_stringMode           = DbU::Symbolic;
  DbU::UnitPower      DbU::_stringModeUnitPower  = DbU::Nano;
  DbU::Unit           DbU::_symbolicSnapGridStep = DbU::fromLambda( 1.0);
  DbU::Unit           DbU::_realSnapGridStep     = DbU::fromGrid  (10.0);
  DbU::Unit           DbU::_polygonStep          = DbU::fromGrid  ( 1.0);
  const DbU::Unit     DbU::Min                   = std::numeric_limits<DbU::Unit>::min();
  const DbU::Unit     DbU::Max                   = std::numeric_limits<DbU::Unit>::max();


// -------------------------------------------------------------------
// Class :  "Hurricane::DbUSlot".


  class DbUSlot : public Slot {

    public:
    // Constructor.
                       DbUSlot       ( const string& name, const DbU::Unit* data );
                       DbUSlot       (       string& name, const DbU::Unit* data );
    // Accessors.
      virtual string   getDataString () const;
      virtual Record*  getDataRecord () const;
      virtual DbUSlot* getClone      () const;

    protected:
    // Internal: Attributes.
      const DbU::Unit* _unit;

    private:
    // Internal: Constructors.
                       DbUSlot      ( const DbUSlot& );
              DbUSlot& operator=    ( const DbUSlot& );
  };


// Inline Member Functions.
           DbUSlot::DbUSlot       ( const string& name, const DbU::Unit* unit ) : Slot(name), _unit(unit) {}
           DbUSlot::DbUSlot       (       string& name, const DbU::Unit* unit ) : Slot(name), _unit(unit) {}
  string   DbUSlot::getDataString () const { return DbU::getValueString(*_unit); }
  Record*  DbUSlot::getDataRecord () const { return DbU::getValueRecord( _unit); }
  DbUSlot* DbUSlot::getClone      () const { return new DbUSlot(_name,_unit); }


// -------------------------------------------------------------------
// Class :  "Hurricane::DbU".


  void  DbU::_updateBounds ()
  {
    _gridMax     = toGrid    ( Max );
    _lambdaMax   = toLambda  ( Max );
    _physicalMax = toPhysical( Max, Unity );
  }


  void  DbU::checkGridBound ( double value )
  {
    if (value < 0) value = -value;
    if (value >= _gridMax)
      throw Error( "Grid value %.1f converts to out of range DbU value (maximum is: %.1f)."
                 , value, _gridMax );
  }


  void  DbU::checkLambdaBound ( double value )
  {
    if (value < 0) value = -value;
    if (value >= _lambdaMax)
      throw Error( "Lambda value %.1f converts to out of range DbU (maximum is: %.1f)."
                 , value, _lambdaMax );
  }


  void  DbU::checkPhysicalBound ( double value, UnitPower p )
  {
    if (value < 0) value = -value;
    value *= getUnitPower(p);
    if (value >= _physicalMax)
      throw Error( "Physical value %.1fnm converts to out of range DbU (maximum is: %.1f)."
                 , (value/1.0e-9), _physicalMax );
  }


  unsigned int DbU::getPrecision ()
  { return _precision; }


  unsigned int DbU::getMaximalPrecision ()
  { return _maximalPrecision; }


  double  DbU::getResolution ()
  { return _resolution; }


  void DbU::setPrecision ( unsigned int precision, unsigned int flags )
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

    if (not (flags & NoTechnoUpdate) and DataBase::getDB()->getTechnology())
      DataBase::getDB()->getTechnology()->_onDbuChange ( scale );

    setSymbolicSnapGridStep ( DbU::lambda( 1.0) );
    setRealSnapGridStep     ( DbU::grid  (10.0) );

    _updateBounds();
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
  {
    _physicalsPerGrid = physicalsPerGrid * getUnitPower(p);
    _updateBounds();
  }


  double  DbU::getPhysicalsPerGrid ()
  { return _physicalsPerGrid; }


  double  DbU::physicalToGrid ( double physical, UnitPower p )
  { return ( physical * getUnitPower(p) ) / _physicalsPerGrid; }


  void  DbU::setGridsPerLambda ( double gridsPerLambda, unsigned int flags )
  {
    if ((rint(gridsPerLambda) != gridsPerLambda) /*or (remainder(gridsPerLambda,2.0) != 0.0)*/)
      throw Error ( "DbU::Unit::setGridPerLambdas(): \"gridsPerLambda\" (%f) must be an even integer."
                  , gridsPerLambda
                  );

    float scale = gridsPerLambda / (float)_gridsPerLambda;

    _gridsPerLambda = gridsPerLambda;

    if (not (flags & NoTechnoUpdate) and DataBase::getDB()->getTechnology())
      DataBase::getDB()->getTechnology()->_onDbuChange ( scale );

    setSymbolicSnapGridStep ( DbU::lambda(1) );

    _updateBounds();
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


  void  DbU::getStringMode ( unsigned int& mode, UnitPower& p )
  {
    mode = _stringMode;
    p    = _stringModeUnitPower;
  }


  string  DbU::getValueString ( DbU::Unit u, int mode )
  {
    ostringstream os;
    char unitPower  = ' ';
    char unitSymbol = 'u';

    os << fixed;  
    if (_stringMode == Grid) {
      unitSymbol = 'g';
      os << setprecision(1) << toGrid(u);
    } else if (_stringMode == Symbolic) {
      unitSymbol = 'L';
      os << setprecision(2) << toLambda(u);
    } else if (_stringMode == Physical) {
      unitSymbol = 'm';
      switch ( _stringModeUnitPower ) {
        case Pico:  unitPower = 'p'; break;
        case Nano:  unitPower = 'n'; break;
        case Micro: unitPower = 'u'; break;
        case Milli: unitPower = 'm'; break;
        case Unity: unitPower = 'U'; break;
        case Kilo:  unitPower = 'k'; break;
        default:    unitPower = '?'; break;
      }
      switch ( u ) {
        case Min: os << "MIN:"; break;
        case Max: os << "MAX:"; break;
        default:
          os << setprecision(4) << toPhysical(u,_stringModeUnitPower);
      }
    } else {
      if (_stringMode != Db)
        cerr << "[ERROR] Unknown Unit representation mode: " << _stringMode << endl;
      os << u;
    }

    string s = os.str();
    if (_stringMode == Symbolic) {
      size_t dot = s.rfind( '.' );
      if (dot != string::npos) s.erase( dot + 2 );
    } else if (mode & SmartTruncate) {
      size_t dot = s.rfind( '.' );
      if (dot != string::npos) {
        size_t end     = dot;
        size_t nonzero = end;
        for ( ; end < s.size() ; ++end ) {
          if (s[end] != '0') nonzero = end;
        }
        if (nonzero == dot) s.erase( dot );
        else {
          if (nonzero < s.size()) s.erase( nonzero+1 );
        }
      }
    }
    if (unitPower != ' ') s += unitPower;
    s += unitSymbol;

    return s;
  }


  string  DbU::getValueString ( double u, int mode )
  {
    ostringstream os;
    char unitPower  = ' ';
    char unitSymbol = 'u';

    os << fixed;  
    if (_stringMode == Grid) {
      unitSymbol = 'g';
      os << setprecision(1) << toGrid(u);
    } else if (_stringMode == Symbolic) {
      unitSymbol = 'l';
      os << setprecision(1) << toLambda(u);
    } else if (_stringMode == Physical) {
      unitSymbol = 'm';
      switch ( _stringModeUnitPower ) {
        case Pico:  unitPower = 'p'; break;
        case Nano:  unitPower = 'n'; break;
        case Micro: unitPower = 'u'; break;
        case Milli: unitPower = 'm'; break;
        case Unity: unitPower = 'U'; break;
        case Kilo:  unitPower = 'k'; break;
        default:    unitPower = '?'; break;
      }
      os << setprecision(3) << toPhysical(u,_stringModeUnitPower);
    } else {
      if (_stringMode != Db)
        cerr << "[ERROR] Unknown Unit representation mode: " << _stringMode << endl;

      os << u;
    }

    string s = os.str();
    if (mode & SmartTruncate) {
      size_t dot = s.rfind( '.' );
      if (dot != string::npos) {
        size_t end = dot+1;
        for ( ; end < s.size() ; ++end ) if (s[end] != '0') break;
        if (end == s.size()) s.erase( dot );
      }
    }

    if (unitPower != ' ') s += unitPower;
    s += unitSymbol;

    return s;
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
