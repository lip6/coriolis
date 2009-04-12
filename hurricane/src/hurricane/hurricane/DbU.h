

// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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
// |  C++ Header  :       "./DbU.h"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_DBU__
# define  __HURRICANE_DBU__


# include  <cmath>

# include  "hurricane/Commons.h"


namespace Hurricane {


  class DbU {
    public:
      enum UnitPower { Pico = 1
                     , Nano
                     , Micro
                     , Milli
                     , Unity
                     , Kilo
                     };
    public:
      typedef  long  Unit;
    public:
      enum  StringMode { Db            = 1
                       , Grid          = 2
                       , Symbolic      = 4
                       , SmartTruncate = 8
                       };
      enum  SnapMode   { Inferior      = 1
                       , Superior      = 2
                       , Nearest       = 4
                       };

    public:
    // User to DB Converters.
      static inline Unit                db                      ( long value );
      static inline Unit                grid                    ( double value );
      static inline Unit                lambda                  ( double value );
    // Precision & Resolution Managment.                        
      static        unsigned int        getPrecision            ();
      static        unsigned int        getMaximalPrecision     ();
      static        double              getResolution           ();
      static        void                setPrecision            ( unsigned int precision );
    // Founder Grid Managment.                                  
      static        double              getUnitPower            ( UnitPower p );
      static        void                setPhysicalsPerGrid     ( double gridsPerLambda, UnitPower p );
      static        double              getPhysicalsPerGrid     ();
      static        double              physicalToGrid          ( double physical, UnitPower p );
    // Lamba Managment.                                         
      static        void                setGridsPerLambda       ( double gridsPerLambda );
      static        double              getGridsPerLambda       ();
    // Snap Grid Managment.
      static        DbU::Unit           getRealSnapGridStep     ();
      static        DbU::Unit           getOnRealSnapGrid       ( DbU::Unit u, SnapMode mode=Nearest );
      static inline void                setRealSnapGridStep     ( DbU::Unit step );
      static        DbU::Unit           getSymbolicSnapGridStep ();
      static        DbU::Unit           getOnSymbolicSnapGrid   ( DbU::Unit u, SnapMode mode=Nearest );
      static inline void                setSymbolicSnapGridStep ( DbU::Unit step );
    // Conversions.
      static inline long                getDb                   ( Unit u );
      static inline double              getGrid                 ( Unit u );
      static inline double              getLambda               ( Unit u );
      static        string              getValueString          ( Unit u, int mode=SmartTruncate );
      static        Record*             getValueRecord          ( const Unit* u );
      static        Slot*               getValueSlot            ( const string& name, const Unit* u );
      static inline void                setStringMode           ( unsigned int mode );

    public:
    // Static Attributes: constants.
      static const Unit          Min;
      static const Unit          Max;
    private:
    // Internal: Static Attributes.
      static const unsigned int  _maximalPrecision;
      static unsigned int        _precision;
      static double              _resolution;
      static double              _gridsPerLambda;
      static double              _physicalsPerGrid;
      static unsigned int        _stringMode;
      static DbU::Unit           _realSnapGridStep;
      static DbU::Unit           _symbolicSnapGridStep;
  };


// Inline Functions.
  inline DbU::Unit  DbU::db                      ( long value )        { return value; }
  inline DbU::Unit  DbU::grid                    ( double value )      { return (long)rint( value/_resolution ); }
  inline DbU::Unit  DbU::lambda                  ( double value )      { return grid(value*_gridsPerLambda); }
  inline long       DbU::getDb                   ( DbU::Unit u )       { return u; }
  inline double     DbU::getGrid                 ( DbU::Unit u )       { return _resolution*(double)u; }
  inline double     DbU::getLambda               ( DbU::Unit u )       { return getGrid(u)/_gridsPerLambda; }
  inline void       DbU::setStringMode           ( unsigned int mode ) { _stringMode = mode; }
  inline void       DbU::setRealSnapGridStep     ( DbU::Unit step )    { _realSnapGridStep = step; }
  inline void       DbU::setSymbolicSnapGridStep ( DbU::Unit step )    { _symbolicSnapGridStep = step; }


} // End of Hurricane namespace.


# endif  // __HURRICANE_DBU__
