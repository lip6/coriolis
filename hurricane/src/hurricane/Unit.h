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

unsigned GetPrecision();

unsigned GetMaximalPrecisionAllowed();

void SetPrecision(unsigned precision);
// Should be called only once at the beginning of the program



// ****************************************************************************************************
// Resolution managers (the value associate to the unit 1)
// ****************************************************************************************************

double GetResolution();



// ****************************************************************************************************
// Grid managers
// ****************************************************************************************************

const Unit& GetGridStep();

void SetGridStep(const Unit& gridStep);

bool IsOnGrid(const Unit& unit, int n = 1);

Unit GetOnGridUnit(const Unit& unit, int s = 0);



// ****************************************************************************************************
// Others
// ****************************************************************************************************

Unit GetUnit(double value);

double GetValue(const Unit& unit);

string GetValueString(const Unit& unit);




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Unit*>".

/* template<> */
/*   inline string  ProxyTypeName<Unit> ( const Unit* object ) */
/*                                      { return "<PointerSlotAdapter<Unit>>"; } */

/* template<> */
/*   inline string  ProxyString<Unit>   ( const Unit* object ) */
/*                                      { return GetValueString(); } */

/* template<> */
/*   inline Record* ProxyRecord<Unit>   ( const Unit* object ) */
/*                                      { */
/*                                        Record* record = new Record(GetString(object)); */
/*                                        record->Add(GetSlot("Unit", (unsigned int*)_object)); */
/*                                        return record; */
/*                                      } */



} // End of Hurricane namespace.



bool Scan(const string& s, H::Unit& unit);

#endif // HURRICANE_UNIT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
