// ****************************************************************************************************
// File: Unit.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_UNIT
#define HURRICANE_UNIT

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Unit declaration
// ****************************************************************************************************

typedef long Unit;

# define  UNIT_MAX  LONG_MAX
# define  UNIT_MIN  LONG_MIN



// ****************************************************************************************************
// Precision managers (precision defines the maximal number of decimals allowed)
// ****************************************************************************************************

unsigned getPrecision();

unsigned getMaximalPrecisionAllowed();

void SetPrecision(unsigned precision);
// Should be called only once at the beginning of the program



// ****************************************************************************************************
// Resolution managers (the value associate to the unit 1)
// ****************************************************************************************************

double getResolution();



// ****************************************************************************************************
// Grid managers
// ****************************************************************************************************

const Unit& getGridStep();

void SetGridStep(const Unit& gridStep);

bool IsOnGrid(const Unit& unit, int n = 1);

Unit getOnGridUnit(const Unit& unit, int s = 0);



// ****************************************************************************************************
// Others
// ****************************************************************************************************

Unit getUnit(double value);

double getValue(const Unit& unit);

string getValueString(const Unit& unit);




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Unit*>".

/* template<> */
/*   inline string  ProxyTypeName<Unit> ( const Unit* object ) */
/*                                      { return "<PointerSlotAdapter<Unit>>"; } */

/* template<> */
/*   inline string  ProxyString<Unit>   ( const Unit* object ) */
/*                                      { return getValueString(); } */

/* template<> */
/*   inline Record* ProxyRecord<Unit>   ( const Unit* object ) */
/*                                      { */
/*                                        Record* record = new Record(getString(object)); */
/*                                        record->Add(getSlot("Unit", (unsigned int*)_object)); */
/*                                        return record; */
/*                                      } */



} // End of Hurricane namespace.



bool Scan(const string& s, Hurricane::Unit& unit);

#endif // HURRICANE_UNIT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
