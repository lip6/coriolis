// ****************************************************************************************************
// File: Collection.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment LIP6 & BULL versions

#ifndef HURRICANE_COLLECTION
#define HURRICANE_COLLECTION

#include "Locator.h"
#include "Filter.h"

namespace Hurricane {

template<class Type> class GenericCollection;
template<class Type, class SubType> class SubTypeCollection;
template<class Type> class SubSetCollection;



// ****************************************************************************************************
// Collection declaration
// ****************************************************************************************************

template<class Type> class Collection : public NestedSlotAdapter {
// *************************************************************

// Constructors
// ************

	protected: Collection()
	// ********************
	{
	}

	private: Collection(const Collection& collection);
	// ***********************************************
	// not implemented to forbid copy construction
	// ***********************************************

// Destructor
// **********

	public: virtual ~Collection()
	// **************************
	{
	}

// Operators
// *********

	private: Collection& operator=(const Collection& collection);
	// **********************************************************
	// not implemented to forbid assignment
	// **********************************************************

// Accessors
// *********

	public: virtual Collection<Type>* GetClone() const = 0;

	public: virtual Locator<Type>* GetLocator() const = 0;

	public: virtual unsigned GetSize() const
	// *************************************
	{
		unsigned size = 0;
		// we use a GenericLocator to delete the locator allocated by GetLocator()
		GenericLocator<Type> locator = GetLocator();
		while (locator.IsValid()) {
			size++;
			locator.Progress();
		}
		return size;
	}

	public: Type GetFirst() const
	// **************************
	{
		// we use a GenericLocator to delete the locator allocated by GetLocator()
		return GenericLocator<Type>(GetLocator()).GetElement();
	}

	public: GenericCollection<Type> GetSubSet(const Filter<Type>& filter) const
	// ************************************************************************
	{
		return SubSetCollection<Type>(*this, filter);
	}

	public: template<class SubType> GenericCollection<SubType> GetSubSet() const
	// *************************************************************************
	{
		return SubTypeCollection<Type, SubType>(this);
	}

	public: template<class SubType>
			  GenericCollection<SubType> GetSubSet(const Filter<SubType>& filter) const
	// ******************************************************************************
	{
		return GetSubSet<SubType>().GetSubSet(filter);
	}

// Predicates
// **********

	public: bool IsEmpty() const
	// *************************
	{
		// we use a GenericLocator to delete the locator allocated by GetLocator()
		return !GenericLocator<Type>(GetLocator()).IsValid();
	}

// Utilitarians
// ************

	public: void Fill(list<Type>& list) const
	// **************************************
	{
		GenericLocator<Type> locator = GetLocator();
		while (locator.IsValid()) {
			list.push_back(locator.GetElement());
			locator.Progress();
		}
	}

	public: void Fill(set<Type>& set) const
	// ************************************
	{
		GenericLocator<Type> locator = GetLocator();
		while (locator.IsValid()) {
			set.insert(locator.GetElement());
			locator.Progress();
		}
	}

	public: template<class Compare> void Fill(set<Type, Compare>& set) const
	// *********************************************************************
	{
		GenericLocator<Type> locator = GetLocator();
		while (locator.IsValid()) {
			set.insert(locator.GetElement());
			locator.Progress();
		}
	}

	public: void Fill(vector<Type>& vector) const
	// ******************************************
	{
		GenericLocator<Type> locator = GetLocator();
		while (locator.IsValid()) {
			vector.push_back(locator.GetElement());
			locator.Progress();
		}
	}

// Others
// ******

    public: virtual string _GetTypeName() const
	// **************************************
    {
      return _TName("Collection<Type>");
    };

	public: virtual string _GetString() const = 0;

	public: Record* _GetRecord() const
	// *************************
	{
		Record* record = NULL;
		if (!IsEmpty()) {
			record = new Record(GetString(this));
			unsigned n = 1;
			GenericLocator<Type> locator = GetLocator();
			while (locator.IsValid()) {
              string  slotName   = GetString(n++);
              Type    slotRecord = locator.GetElement();
              record->Add(GetSlot(slotName, slotRecord));
				locator.Progress();
			}
		}
		return record;
	}

};



// ****************************************************************************************************
// GenericCollection declaration
// ****************************************************************************************************

template<class Type> class GenericCollection : public Collection<Type> {
// *******************************************************************

// Types
// *****

	public: typedef Collection<Type> Inherit;

// Attributes
// **********

	private: Collection<Type>* _collection;

// Constructors
// ************

	public: GenericCollection()
	// ***********************
	:	Inherit(),
		_collection(NULL)
	{
	}

	public: GenericCollection(const Collection<Type>& collection)
	// **********************************************************
	:	Inherit(),
		_collection(collection.GetClone())
	{
	}

	public: GenericCollection(const GenericCollection<Type>& genericCollection)
	// ************************************************************************
	:	Inherit(),
		_collection(genericCollection.GetClone())
	{
	}

	public: GenericCollection(Collection<Type>* collection)
	// *************************************************************
	// CAUTION : collection will be deleted by the GenericCollection
	// *************************************************************
	:	Inherit(),
		_collection(collection)
	{
	}

// Destructor
// **********

	public: virtual ~GenericCollection()
	// *********************************
	{
		if (_collection) delete _collection;
	}

// Operators
// *********

	public: GenericCollection& operator=(const Collection<Type>& collection)
	// *********************************************************************
	{
		if (_collection) delete _collection;
		_collection = collection.GetClone();
		return *this;
	}

	public: GenericCollection& operator=(const GenericCollection& genericCollection)
	// *****************************************************************************
	{
		if (_collection) delete _collection;
		_collection = genericCollection.GetClone();
		return *this;
	}

	public: GenericCollection& operator=(Collection<Type>* collection)
	// ***************************************************************
	// CAUTION : collection will be deleted by the GenericCollection
	// ***************************************************************
	{
		if (_collection) delete _collection;
		_collection = collection;
		return *this;
	}

// Accessors
// *********

	public: virtual Collection<Type>* GetClone() const
	// ***********************************************
	{
		return (_collection) ? _collection->GetClone() : NULL;
	}

	public: virtual Locator<Type>* GetLocator() const
	// **********************************************
	{
		return (_collection) ? _collection->GetLocator() : NULL;
	}

	public: virtual unsigned GetSize() const
	// *************************************
	{
		return (_collection) ? _collection->GetSize() : 0;
	}

// Others
// ******

    public: virtual string _GetTypeName() const
	// **************************************
    {
      return _TName("GenericCollection");
    };

	public: virtual string _GetString() const
	// **************************************
	{
		if (!_collection)
			return "<" + _GetTypeName() + " unbound>";
		else
            return "<" + _GetTypeName()+ " " + GetString(_collection) + ">";
	}

};



// ****************************************************************************************************
// ElementCollection declaration
// ****************************************************************************************************

template<class Type> class ElementCollection : public Collection<Type> {
// *********************************************************************

    // -----------------------------------------------------------------
    // Sub-Class : "::ElementCollection::Locator".
    public: template<class ElType> class Locator : public Hurricane::Locator<ElType> {

    // Attributes
    // **********
        protected: const ElType _element;
        protected: bool _done;

    // Constructors
    // ************
        public: Locator ( const ElType  _element ) : _element(_element), _done(false) {};
        public: Locator ( const Locator &locator ) : _element(locator._element), _done(locator._done) {};

    // Accessors
    // *********
        public: virtual ElType           GetElement () const { return const_cast<ElType>(_element); };
        public: virtual Locator<ElType>* GetClone   () const { return new Locator(*this); };
        public: virtual bool             IsValid    () const { return !_done; };
        public: virtual void             Progress   ()       { _done = true; };

    // Hurricane Management
    // ********************
        public: virtual string _GetString () const {
			if (!_element)
				return "<" + _TName("ElementCollection::Locator") + " unbound>";
			else
				return "<" + _TName("ElementCollection::Locator") + " " + GetString(_element) + ">";
		}

    };

// Types
// *****

	public: typedef Collection<Type> Inherit;

// Attributes
// **********

	private: Type _element;

// Constructors
// ************

	public: ElementCollection()
	// ***********************
	:	Inherit(),
		_element(NULL)
	{
	}

	public: ElementCollection(const Type element)
	// **********************************************************
	:	Inherit(),
		_element(element)
	{
	}

	public: ElementCollection(const ElementCollection<Type>& elementCollection)
	// ************************************************************************
	:	Inherit(),
		_element(elementCollection._element)
	{
	}

// Accessors
// *********

	public: virtual Collection<Type>* GetClone() const
	// ***********************************************
	{
		return ( new ElementCollection (*this) );
	}

	public: virtual Locator<Type>* GetLocator() const
	// **********************************************
	{
		return ( new Locator<Type> (_element) );
	}

	public: virtual unsigned GetSize() const
	// *************************************
	{
		return (_element) ? 1 : 0;
	}

// Others
// ******

	public: virtual string _GetString() const
	// **************************************
	{
		if (!_element)
			return "<" + _TName("ElementCollection") + " unbound>";
		else
			return "<" + _TName("ElementCollection") + " " + GetString(_element) + ">";
	}

};



// ****************************************************************************************************
// SubTypeCollection declaration
// ****************************************************************************************************

template<class Type, class SubType> class SubTypeCollection : public Collection<SubType> {
// *************************************************************************************

// Types
// *****

	public: typedef Collection<SubType> Inherit;

	public: class Locator : public Hurricane::Locator<SubType> {
	// *******************************************************

	// Types
	// *****

		public: typedef Hurricane::Locator<SubType> Inherit;

	// Attributes
	// **********

		private: GenericLocator<Type> _locator;

	// Constructors
	// ************

		public: Locator(const GenericCollection<Type>& collection)
		// ********************************************************
		:	Inherit(),
			_locator(collection.GetLocator())
		{
			while (_locator.IsValid() && !is_a<SubType>(_locator.GetElement()))
				_locator.Progress();
		}

		public: Locator(const GenericLocator<Type>& genericLocator)
		// ********************************************************
		:	Inherit(),
			_locator(genericLocator.GetClone())
		{
			while (_locator.IsValid() && !is_a<SubType>(_locator.GetElement()))
				_locator.Progress();
		}

	// Accessors
	// *********

		public: virtual SubType GetElement() const
		// ***************************************
		{
			return (_locator.IsValid()) ? (SubType)_locator.GetElement() : SubType();
		}

		public: virtual Hurricane::Locator<SubType>* GetClone() const
		// **********************************************************
		{
			return new Locator(_locator);
		}

		public: virtual Hurricane::Locator<SubType>* GetLocator() // 21-10-03
	        // *************************************************
		{
		  return dynamic_cast<Hurricane::Locator<SubType>*> (
				 _locator.GetLocator()->GetLocator() );
		}


	// Predicates
	// **********

		public: virtual bool IsValid() const
		// *********************************
		{
			return _locator.IsValid();
		}

	// Updators
	// ********

		public: virtual void Progress()
		// ****************************
		{
			if (_locator.IsValid()) {
				do {
					_locator.Progress();
				} while (_locator.IsValid() && !is_a<SubType>(_locator.GetElement()));
			}
		}

	};

// Attributes
// **********

	private: GenericCollection<Type> _collection;

// Constructors
// ************

	public: SubTypeCollection()
	// ********************
	:	Inherit(),
		_collection()
	{
	}

	public: SubTypeCollection(const Collection<Type>* collection)
	// **********************************************************
	:	Inherit(),
		_collection(collection->GetClone())
	{
	}

	public: SubTypeCollection(const GenericCollection<Type>& collection)
	// *****************************************************************
	:	Inherit(),
		_collection(collection)
	{
	}

	public: SubTypeCollection(const SubTypeCollection& subTypeCollection)
	// ******************************************************************
	:	Inherit(),
		_collection(subTypeCollection._collection)
	{
	}

// Operators
// *********

	public: SubTypeCollection& operator=(const SubTypeCollection& subTypeCollection)
	// *****************************************************************************
	{
		_collection = subTypeCollection._collection;
		return *this;
	}

// Accessors
// *********

	public: virtual Collection<SubType>* GetClone() const
	// **************************************************
	{
		return new SubTypeCollection(_collection);
	}

	public: virtual Hurricane::Locator<SubType>* GetLocator() const
	// ************************************************************
	{
		return new Locator(_collection);
	}

// Accessors
// *********

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _TName("SubTypeCollection") + " " + GetString(_collection) + ">";
	}

};



// ****************************************************************************************************
// SubSetCollection implementation
// ****************************************************************************************************

template<class Type> class SubSetCollection : public Collection<Type> {
// ******************************************************************

// Types
// *****

	public: typedef Collection<Type> Inherit;

	public: class Locator : public Hurricane::Locator<Type> {
	// ****************************************************

	// Types
	// *****

		public: typedef Hurricane::Locator<Type> Inherit;

	// Attributes
	// **********

		private: GenericLocator<Type> _locator;
		private: GenericFilter<Type> _filter;

	// Constructors
	// ************

		public: Locator(const SubSetCollection<Type>& collection, const Filter<Type>& filter)
		// **********************************************************************************
		:	Inherit(),
			_locator(collection.GetLocator()),
			_filter(filter)
		{
			while (_locator.IsValid() && !_filter.Accept(_locator.GetElement()))
				_locator.Progress();
		}

		public: Locator(const Collection<Type>& collection, const Filter<Type>& filter)
		// ****************************************************************************
		:	Inherit(),
			_locator(collection.GetLocator()),
			_filter(filter)
		{
			while (_locator.IsValid() && !_filter.Accept(_locator.GetElement()))
				_locator.Progress();
		}

		public: Locator(const GenericCollection<Type>& genericCollection, const Filter<Type>& filter)
		// ******************************************************************************************
		:	Inherit(),
			_locator(genericCollection.GetLocator()),
			_filter(filter)
		{
			while (_locator.IsValid() && !_filter.Accept(_locator.GetElement()))
				_locator.Progress();
		}

		public: Locator(const GenericLocator<Type>& genericLocator, const Filter<Type>& filter)
		// ************************************************************************************
		:	Inherit(),
			_locator(genericLocator),
			_filter(filter)
		{
			while (_locator.IsValid() && !_filter.Accept(_locator.GetElement()))
				_locator.Progress();
		}

	// Accessors
	// *********

		public: virtual Type GetElement() const
		// ************************************
		{
			return (_locator.IsValid()) ? _locator.GetElement() : Type();
		}

		public: virtual Hurricane::Locator<Type>* GetClone() const
		// *******************************************************
		{
			return new Locator(_locator, _filter);
		}

		public: virtual Hurricane::Locator<Type>* GetLocator() // 21-10-03
	   // ***************************************************
		{
		  return ( _locator.GetLocator()->GetLocator() );
		}

	// Predicates
	// **********

		public: virtual bool IsValid() const
		// *********************************
		{
			return _locator.IsValid();
		}

	// Updators
	// ********

		public: virtual void Progress()
		// ****************************
		{
			if (_locator.IsValid()) {
				do {
					_locator.Progress();
				} while (_locator.IsValid() && !_filter.Accept(_locator.GetElement()));
			}
		}

	};

// Attributes
// **********

	private: GenericCollection<Type> _collection;
	private: GenericFilter<Type> _filter;

// Constructors
// ************

	public: SubSetCollection()
	// ***********************
	:	Inherit(),
		_collection(),
		_filter()
	{
	}

	public: SubSetCollection(const Collection<Type>& collection, const Filter<Type>& filter)
	// *************************************************************************************
	:	Inherit(),
		_collection(collection),
		_filter(filter)
	{
	}

	public: SubSetCollection(const SubSetCollection& subSetCollection)
	// ***************************************************************
	:	Inherit(),
		_collection(subSetCollection._collection),
		_filter(subSetCollection._filter)
	{
	}

// Operators
// *********

	public: SubSetCollection& operator=(const SubSetCollection& subSetCollection)
	// **************************************************************************
	{
		_collection = subSetCollection._collection;
		_filter = subSetCollection._filter;
		return *this;
	}

// Accessors
// *********

	public: virtual Collection<Type>* GetClone() const
	// ***********************************************
	{
		return new SubSetCollection(_collection, _filter);
	}

	public: virtual Hurricane::Locator<Type>* GetLocator() const
	// *********************************************************
	{
		return new Locator(_collection, _filter);
	}

// Accessors
// *********

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _TName("SubSetCollection") + " " + GetString(_collection) + ">";
	}

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************



// ****************************************************************************************************
// Macros declaration
// ****************************************************************************************************

#define end_for\
/**************/\
		}\
	}

#define for_each_object(Type, element, collection)\
/*************************************************/\
{\
	GenericLocator<Type> _locator = collection.GetLocator();\
	while (_locator.IsValid()) {\
		Type element = _locator.GetElement();\
		_locator.Progress();

#define for_each_element(Type, element, collection)\
/*************************************************/\
{\
    ElementCollection<Type>::Locator<Type>* _locator = collection.GetLocator();\
	while (_locator->IsValid()) {\
		Type element = _locator->GetElement();\
		_locator->Progress();


template<typename T>
    class IsNestedSlotAdapter<const Hurricane::GenericCollection<T> > {
      public:
        enum { True=1, False=0 };
    };



} // End of Hurricane namespace.


#include "MultisetCollection.h"
#include "SetCollection.h"
#include "MapCollection.h"
#include "ListCollection.h"
#include "VectorCollection.h"

#endif // HURRICANE_COLLECTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
