// ****************************************************************************************************
// File: Property.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PROPERTY
#define HURRICANE_PROPERTY

#include "Name.h"
#include "Properties.h"
#include "DBos.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Property declaration
// ****************************************************************************************************

class Property : public NestedSlotAdapter {
// **************************************

// Constructors
// ************

	protected: Property();

	private: Property(const Property& property); // not implemented to forbid copy construction

// Destructors
// ***********

	protected: virtual ~Property();

	public: virtual void Delete();

// Operators
// *********

	private: Property& operator=(const Property& property); // not implemented to forbid assignment

// Accessors
// *********

	public: virtual Name GetName() const = 0;

// Managers
// ********

	public: virtual void OnCapturedBy(DBo* owner) = 0;
	public: virtual void OnReleasedBy(DBo* owner) = 0;

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const = 0;
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};



// ****************************************************************************************************
// PrivateProperty declaration
// ****************************************************************************************************

class PrivateProperty : public Property {
// ************************************

// Types
// *****

	public: typedef Property Inherit;

// Attributes
// **********

	private: DBo* _owner;

// Constructors
// ************

	protected: PrivateProperty();

// Accessors
// *********

	public: DBo* GetOwner() const {return _owner;};

// Managers
// ********

	public: virtual void OnCapturedBy(DBo* owner);
	public: virtual void OnReleasedBy(DBo* owner);
	public: virtual void OnNotOwned();

// Others
// ******

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};



// ****************************************************************************************************
// StandardPrivateProperty declaration
// ****************************************************************************************************

template<class Value> class StandardPrivateProperty : public PrivateProperty {
// *************************************************************************

// Types
// *****

	public: typedef PrivateProperty Inherit;

// Attributes
// **********

	private: Name _name;
	private: Value _value;

// Constructors
// ************

	protected: StandardPrivateProperty(const Name& name, const Value& value)
	// *********************************************************************
	:	Inherit(),
		_name(name),
		_value(value)
	{
	};

	public: static StandardPrivateProperty* Create(const Name& name, const Value& value )
	// **********************************************************************************
	{
		StandardPrivateProperty* property = new StandardPrivateProperty(name, value);

		property->_PostCreate();

		return property;
	};

// Accessors
// *********

	public: virtual Name GetName() const
	// *********************************
	{
		return _name;
	};

	public: const Value& GetValue() const
	// **********************************
	{
		return _value;
	};

// Modifieurs
// **********

	public: void SetValue(const Value& value)
	// **************************************
	{
		_value = value;
	};

// Others
// ******

	public: virtual string _GetTypeName() const
	// ****************************************
	{
		return _TName("StandardPrivateProperty");
	};

	public: virtual string _GetString() const
	// **************************************
	{
		string s = Inherit::_GetString();
		s.insert(s.length() - 1, " " + GetString(_value));
		return s;
	};

	public: virtual Record* _GetRecord() const
	// *********************************
	{
		Record* record = Inherit::_GetRecord();
		if (record) {
			record->Add(GetSlot("Name", &_name));
			record->Add(GetSlot("Value", _value));
		}
		return record;
	};

};



// ****************************************************************************************************
// SharedProperty declaration
// ****************************************************************************************************

class SharedProperty : public Property {
// ***********************************

// Types
// *****

	public: typedef Property Inherit;
	public: typedef set<DBo*> DBoSet;

// Attributes
// **********

	private: DBoSet _ownerSet;

// Constructors
// ************

	protected: SharedProperty();

// Accessors
// *********

	public: DBos GetOwners() const {return GetCollection(_ownerSet);};

// Managers
// ********

	public: virtual void OnCapturedBy(DBo* owner);
	public: virtual void OnReleasedBy(DBo* owner);
	public: virtual void OnNotOwned();

// Accessors
// *********

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: DBoSet& _GetOwnerSet() {return _ownerSet;};

};



// ****************************************************************************************************
// StandardSharedProperty declaration
// ****************************************************************************************************

template<class Value> class StandardSharedProperty : public SharedProperty {
// ***********************************************************************

// Types
// *****

	public: typedef SharedProperty Inherit;

// Attributes
// **********

	private: Name _name;
	private: Value _value;

// Constructors
// ************

	protected: StandardSharedProperty(const Name& name, const Value& value)
	// ********************************************************************
	:	Inherit(),
		_name(name),
		_value(value)
	{
	};

	public: static StandardSharedProperty* Create(const Name& name, const Value& value )
	// *********************************************************************************
	{
		StandardSharedProperty* property = new StandardSharedProperty(name, value);

		property->_PostCreate();

		return property;
	};

// Accessors
// *********

	public: virtual Name GetName() const
	// *********************************
	{
		return _name;
	};

	public: const Value& GetValue() const
	// **********************************
	{
		return _value;
	};

// Modifieurs
// **********

	public: void SetValue(const Value& value)
	// **************************************
	{
		_value = value;
	};

// Others
// ******

	public: virtual string _GetTypeName() const
	// ****************************************
	{
		return _TName("StandardSharedProperty");
	};

	public: virtual string _GetString() const
	// **************************************
	{
		string s = Inherit::_GetString();
		s.insert(s.length() - 1, " " + GetString(_value));
		return s;
	};

	public: virtual Record* _GetRecord() const
	// *********************************
	{
		Record* record = Inherit::_GetRecord();
		if (record) {
			record->Add(GetSlot("Name", &_name));
			record->Add(GetSlot("Value", &_value));
		}
		return record;
	};

};


template<typename T>
  class IsNestedSlotAdapter<StandardPrivateProperty<T> > {
    public:
      enum { True=1, False=0 };
  };


template<typename T>
  class IsNestedSlotAdapter<StandardSharedProperty<T> > {
    public:
      enum { True=1, False=0 };
  };


} // End of Hurricane namespace.

#endif // HURRICANE_PROPERTY

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
