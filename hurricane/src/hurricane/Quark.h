// ****************************************************************************************************
// File: Quark.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_QUARK
#define HURRICANE_QUARK

#include "DBo.h"
#include "Quarks.h"
#include "Occurrence.h"

namespace Hurricane {



// ****************************************************************************************************
// Quark declaration
// ****************************************************************************************************

class Quark : public DBo {
// *********************

// Types
// *****

	public: typedef DBo Inherit;

// Attributes
// **********

	private: Occurrence _occurrence;
	private: Quark* _nextOfSharedPathQuarkMap;

// Constructors
// ************

	protected: Quark(const Occurrence& occurrence);

// Accessors
// *********

	public: const Occurrence& GetOccurrence() const {return _occurrence;};

// Others
// ******

	public: static Quark* _Create(const Occurrence& occurrence);
	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Quark");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: Quark* _GetNextOfSharedPathQuarkMap() const {return _nextOfSharedPathQuarkMap;};

	public: void _SetNextOfSharedPathQuarkMap(Quark* quark) {_nextOfSharedPathQuarkMap = quark;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Quark)

#endif // HURRICANE_QUARK

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
