// ****************************************************************************************************
// File: HyperNet.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#ifndef HURRICANE_HYPER_NET
#define HURRICANE_HYPER_NET

#include "Occurrences.h"

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

	public: const Occurrence& GetNetOccurrence() const {return _netOccurrence;};
	public: Cell* GetCell() const {return _netOccurrence.GetOwnerCell();};
	public: Occurrences GetNetOccurrences(bool doExtraction = false, bool allowInterruption = false) const;
	public: Occurrences GetNetOccurrencesUnder(Box area, bool doExtraction = false,
															bool allowInterruption = false) const;
    public: Occurrences GetLeafPlugOccurrences(bool doExtraction = false , bool allowInterruption = false) const;

// Predicates
// **********

	public: bool IsValid() const {return _netOccurrence.IsValid();};

// Others
// ******

    public: string _GetTypeName() const { return _TName("HyperNet"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

};

Occurrence GetHyperNetRootNetOccurrence(const Occurrence& netoccurrence);

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
