// ****************************************************************************************************
// File: DBo.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_DBO
#define HURRICANE_DBO

#include "DBos.h"
#include "Name.h"
#include "Property.h"

namespace Hurricane {



// ****************************************************************************************************
// DBo declaration
// ****************************************************************************************************

class DBo : public NestedSlotAdapter {
// *********************************

#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

	public: typedef set<Property*> PropertySet;

// Attributs
// *********

	private: PropertySet _propertySet;

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

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const = 0;
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: PropertySet& _GetPropertySet() {return _propertySet;};

	public: void _OnDeleted(Property* property);

# endif

// Destructors
// ***********

	public: virtual void Delete();

// Accessors
// *********

	public: Property* GetProperty(const Name& name) const;
	public: Properties GetProperties() const {return GetCollection(_propertySet);};

// Predicates
// **********

	public: bool HasProperty() const {return !_propertySet.empty();};

// Updators
// ********

	public: void Put(Property* property);
	public: void Remove(Property* property);
	public: void RemoveProperty(const Name& name);
	public: void ClearProperties();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::DBo)
PointerIOStreamSupport(Hurricane::DBo)

#endif // HURRICANE_DBO

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
