// ****************************************************************************************************
// File: DBo.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_DBO
#define HURRICANE_DBO

#include "hurricane/DBos.h"
#include "hurricane/Name.h"
#include "hurricane/Property.h"

namespace Hurricane {



// ****************************************************************************************************
// DBo declaration
// ****************************************************************************************************

class DBo : public NestedSlotAdapter {
// *********************************

#if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

    public: typedef set<Property*> PropertySet;

// Attributs
// *********

    private: mutable PropertySet _propertySet;

// Constructors
// ************

    protected: DBo();

    private: DBo(const DBo& dbo); // not implemented to forbid copy construction

// Destructors
// ***********

    protected: virtual ~DBo();

// Operators
// *********

    private: DBo& operator=(const DBo& dbo); // not implemented to forbid assignment

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const;
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: PropertySet& _getPropertySet() {return _propertySet;};

    public: void _onDestroyed(Property* property);

#endif

// Destructors
// ***********

    public: virtual void destroy();

// Accessors
// *********

    public: Property* getProperty(const Name& name) const;
    public: Properties getProperties() const {return getCollection(_propertySet);};

// Predicates
// **********

    public: bool hasProperty() const {return !_propertySet.empty();};

// Updators
// ********

    public: void put(Property* property);
    public: void remove(Property* property);
    public: void removeProperty(const Name& name);
    public: void clearProperties();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::DBo)
PointerIOStreamSupport(Hurricane::DBo)

#endif // HURRICANE_DBO

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
