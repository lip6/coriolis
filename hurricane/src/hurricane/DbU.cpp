

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
// |  C++ Module  :       "./DbU.cpp"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <climits>

# include  "hurricane/DbU.h"
# include  "hurricane/Error.h"


namespace Hurricane {


  const unsigned int  DbU::_maximalPrecision = 3;
  unsigned int        DbU::_precision        = 1;
  double              DbU::_resolution       = 0.1;
  double              DbU::_realsPerLambda   = 10.0;
  unsigned int        DbU::_stringMode       = DbU::Symbolic;
  const DbU::Unit     DbU::Min               = LONG_MIN;
  const DbU::Unit     DbU::Max               = LONG_MAX;


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

    _precision  = precision;

    _resolution = 1;
    while ( precision-- ) _resolution /= 10;

  //setGridStep(getUnit(1));
  }


  void  DbU::setRealsPerLambda ( double realsPerLambda )
  {
    if (   ( rint(realsPerLambda) != realsPerLambda ) 
        || ( remainder(realsPerLambda,2.0) != 0.0   ) )
      throw Error ( "DbU::Unit::setRealPerLambdas(): \"realsPerLambda\" (%f) must be an even integer."
                  , realsPerLambda
                  );

    _realsPerLambda = realsPerLambda;
  }


  double  DbU::getRealsPerLambda ()
  { return _realsPerLambda; }




 


// ****************************************************************************************************
// Grid managers
// ****************************************************************************************************

// const DbU::Unit& getGridStep()
// // **********************
// {
//     return GRID_STEP;
// }

// void setGridStep(const DbU::Unit& gridStep)
// // ***********************************
// {
//     if (!gridStep) throw Error("Can't set grid step : invalid value");

//     GRID_STEP = gridStep;
// }

// bool isOnGrid(const DbU::Unit& unit, int n)
// // ***********************************
// {
//     if (n <= 0) throw Error("Can't compute : invalid value");

//     n *= GRID_STEP;

//     return (((abs(unit) / n) * n) == abs(unit));
// }

// DbU::Unit getOnGridUnit(const DbU::Unit& unit, int s)
// // ****************************************
// {
//     switch (s) {
//         case -1 : {
//             if (0 < unit) return (unit / GRID_STEP) * GRID_STEP;
//             else if (unit < 0) return ((unit / GRID_STEP) - 1) * GRID_STEP;
//             return unit;
//         }
//         case 0 : {
//             int g1 = (unit / GRID_STEP) * GRID_STEP;
//            int g2 = ((g1 < unit) ? (g1 + GRID_STEP) : (g1 - GRID_STEP));
//             return (abs(g1 - unit) <= abs(g2 - unit)) ? g1 : g2;
//         }
//         case +1 : {
//             if (0 < unit) return ((unit / GRID_STEP) + 1) * GRID_STEP;
//             else if (unit < 0) return (unit / GRID_STEP) * GRID_STEP;
//             return unit;
//         }
//     }
//     throw Error("Can't get on grid unit : invalid parameter s (should be -1, 0 or +1)");
//     return 0;
// }


  string  DbU::getValueString ( DbU::Unit u )
  {
    char buffer[1024];
    char unitSymbol = 'u';

    if ( _stringMode == Real ) {
      if ( u == 0 ) return "0g";

      unitSymbol = 'g';
      snprintf ( buffer, 1024, "%.1f", getReal(u) );
    } else if ( _stringMode == Symbolic ) {
      if ( u == 0 ) return "0l";

      unitSymbol = 'l';
      snprintf ( buffer, 1024, "%.1f", getLambda(u) );
    } else {
      if ( _stringMode != Db )
        cerr << "[ERROR] Unknown Unit representation mode: " << _stringMode << endl;

      if ( u == 0 ) return "0u";

      snprintf ( buffer, 1024, "%ld", u );
    }

    size_t length = strlen(buffer) - 1;
    for ( ; length > 0 ; length-- ) {
      if ( (buffer[length] != '0') && (buffer[length] != '.') )
        break;
    }
    buffer[++length] = unitSymbol;
    buffer[++length] = '\0';

    return buffer;
  }


} // End of Hurricane namespace.
