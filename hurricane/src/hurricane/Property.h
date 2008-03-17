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

    public: virtual void destroy();

// Operators
// *********

    private: Property& operator=(const Property& property); // not implemented to forbid assignment

// Accessors
// *********

    public: virtual Name getName() const = 0;

// Managers
// ********

    public: virtual void onCapturedBy(DBo* owner) = 0;
    public: virtual void onReleasedBy(DBo* owner) = 0;

// Others
// ******

    protected: virtual void _postCreate() {};

    protected: virtual void _preDestroy() {};

    public: virtual string _getTypeName() const = 0;
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

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

    public: DBo* getOwner() const {return _owner;};

// Managers
// ********

    public: virtual void onCapturedBy(DBo* owner);
    public: virtual void onReleasedBy(DBo* owner);
    public: virtual void onNotOwned();

// Others
// ******

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

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
    :    Inherit(),
        _name(name),
        _value(value)
    {
    };

    public: static StandardPrivateProperty* create(const Name& name, const Value& value )
    // **********************************************************************************
    {
        StandardPrivateProperty* property = new StandardPrivateProperty(name, value);

        property->_postCreate();

        return property;
    };

// Accessors
// *********

    public: virtual Name getName() const
    // *********************************
    {
        return _name;
    };

    public: const Value& getValue() const
    // **********************************
    {
        return _value;
    };

// Modifieurs
// **********

    public: void setValue(const Value& value)
    // **************************************
    {
        _value = value;
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // ****************************************
    {
        return _TName("StandardPrivateProperty");
    };

    public: virtual string _getString() const
    // **************************************
    {
        string s = Inherit::_getString();
        s.insert(s.length() - 1, " " + getString(_value));
        return s;
    };

    public: virtual Record* _getRecord() const
    // *********************************
    {
        Record* record = Inherit::_getRecord();
        if (record) {
            record->Add(getSlot("Name", &_name));
            record->Add(getSlot("Value", _value));
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

    public: DBos getOwners() const {return getCollection(_ownerSet);};

// Managers
// ********

    public: virtual void onCapturedBy(DBo* owner);
    public: virtual void onReleasedBy(DBo* owner);
    public: virtual void onNotOwned();

// Accessors
// *********

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: DBoSet& _getOwnerSet() {return _ownerSet;};

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
    :    Inherit(),
        _name(name),
        _value(value)
    {
    };

    public: static StandardSharedProperty* Create(const Name& name, const Value& value )
    // *********************************************************************************
    {
        StandardSharedProperty* property = new StandardSharedProperty(name, value);

        property->_postCreate();

        return property;
    };

// Accessors
// *********

    public: virtual Name getName() const
    // *********************************
    {
        return _name;
    };

    public: const Value& getValue() const
    // **********************************
    {
        return _value;
    };

// Modifieurs
// **********

    public: void setValue(const Value& value)
    // **************************************
    {
        _value = value;
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // ****************************************
    {
        return _TName("StandardSharedProperty");
    };

    public: virtual string _getString() const
    // **************************************
    {
        string s = Inherit::_getString();
        s.insert(s.length() - 1, " " + getString(_value));
        return s;
    };

    public: virtual Record* _getRecord() const
    // *********************************
    {
        Record* record = Inherit::_getRecord();
        if (record) {
            record->Add(getSlot("Name", &_name));
            record->Add(getSlot("Value", &_value));
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
