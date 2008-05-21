// ****************************************************************************************************
// File: HyperNet.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

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

    private: HyperNet(const HyperNet& hyperNet); // not implemented to forbid copy construction

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
    public: Occurrences getLeafPlugOccurrences(bool doExtraction = false , bool allowInterruption = false) const;

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

bool IsHyperNetRootNetOccurrence(Occurrence netoccurrence);



} // End of Hurricane namespace.


PointerIOStreamSupport(Hurricane::HyperNet)




// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

#endif // HURRICANE_HYPER_NET

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
