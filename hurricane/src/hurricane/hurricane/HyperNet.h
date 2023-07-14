// ****************************************************************************************************
// File: ./hurricane/HyperNet.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_HYPER_NET
#define HURRICANE_HYPER_NET

#include "hurricane/Occurrences.h"

namespace Hurricane {



// ****************************************************************************************************
// HyperNet declaration
// ****************************************************************************************************

class HyperNet {
// ***********

// Attributes
// **********

    private: Occurrence _netOccurrence;

// Constructors
// ************

    public: HyperNet(const Occurrence& occurrence);
    public: HyperNet(const HyperNet& hyperNet);

// Operators
// *********

    private: HyperNet& operator=(const HyperNet& hyperNet); // not implemented to forbid assignment

// Accessors
// *********

    public: const Occurrence& getNetOccurrence() const {return _netOccurrence;};
    public: Cell* getCell() const {return _netOccurrence.getOwnerCell();};
    public: Occurrences getNetOccurrences(bool doExtraction = false, bool allowInterruption = false) const;
    public: Occurrences getNetOccurrencesUnder(Box area, bool doExtraction = false,
                                                            bool allowInterruption = false) const;
    public: Occurrences getTerminalNetlistPlugOccurrences(bool doExtraction = false , bool allowInterruption = false) const;
    public: Occurrences getComponentOccurrences(bool doExtraction = false , bool allowInterruption = false) const;

// Predicates
// **********

    public: bool isValid() const {return _netOccurrence.isValid();};

// Others
// ******

    public: string _getTypeName() const { return _TName("HyperNet"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};

Occurrence getHyperNetRootNetOccurrence(const Occurrence& netoccurrence);

bool isHyperNetRootNetOccurrence(Occurrence netoccurrence);



} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::HyperNet);
IOSTREAM_VALUE_SUPPORT(Hurricane::HyperNet);




// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

#endif // HURRICANE_HYPER_NET


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
