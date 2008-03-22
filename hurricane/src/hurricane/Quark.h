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

    public: const Occurrence& getOccurrence() const {return _occurrence;};

// Others
// ******

    public: static Quark* _create(const Occurrence& occurrence);
    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Quark");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Quark* _getNextOfSharedPathQuarkMap() const {return _nextOfSharedPathQuarkMap;};

    public: void _setNextOfSharedPathQuarkMap(Quark* quark) {_nextOfSharedPathQuarkMap = quark;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Quark)

#endif // HURRICANE_QUARK

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
