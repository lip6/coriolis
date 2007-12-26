// ****************************************************************************************************
// File: Filter.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_FILTER
#define HURRICANE_FILTER

#include "Commons.h"

namespace Hurricane {

template<class Type> class GenericFilter;
template<class Type> class NotFilter;



// ****************************************************************************************************
// Filter declaration
// ****************************************************************************************************

template<class Type> class Filter : public NestedSlotAdapter {
// *********************************************************

// Constructors
// ************

	public: Filter() {};

	private: Filter(const Filter& filter); // not implemented to forbid copie

// Destructor
// **********

	public: virtual ~Filter() {};

// Operators
// *********

	private: Filter& operator=(const Filter& filter); // not implemented to forbid assigment

	public: GenericFilter<Type> operator!() const
	// ******************************************
	{
		return NotFilter<Type>(*this);
	};

// Accessors
// *********

	public: virtual Filter<Type>* GetClone() const = 0;

// Predicates
// **********

	public: virtual bool Accept(Type type) const = 0;

// Others
// ******

    public: virtual string _GetTypeName() const { return _TName("Filter"); };
	public: virtual string _GetString() const = 0;

};



// ****************************************************************************************************
// GenericFilter declaration
// ****************************************************************************************************

template<class Type> class GenericFilter : public Filter<Type> {
// ***********************************************************

// Types
// *****

	public: typedef Filter<Type> Inherit;

// Attributes
// **********

	private: Filter<Type>* _filter;

// Constructors
// ************

	public: GenericFilter()
	// ********************
	:	Inherit(),
		_filter(NULL)
	{
	};

	public: GenericFilter(const Filter<Type>& filter)
	// **********************************************
	:	Inherit(),
		_filter(filter.GetClone())
	{
	};

	public: GenericFilter(const GenericFilter<Type>& genericFilter)
	// ************************************************************
	:	Inherit(),
		_filter(genericFilter.GetClone())
	{
	};

	public: GenericFilter(Filter<Type>* filter)
	// *****************************************************
	// CAUTION : filter will be deleted by the GenericFilter
	// *****************************************************
	:	Inherit(),
		_filter(filter)
	{
	};

// Destructor
// **********

	public: virtual ~GenericFilter()
	// *****************************
	{
		if (_filter) delete _filter;
	};

// Operators
// *********

	public: GenericFilter& operator=(const Filter<Type>& filter)
	// *********************************************************
	{
		if (_filter) delete _filter;
		_filter = filter.GetClone();
		return *this;
	};

	public: GenericFilter& operator=(const GenericFilter& genericFilter)
	// *****************************************************************
	{
		if (_filter) delete _filter;
		_filter = genericFilter.GetClone();
		return *this;
	};

	public: GenericFilter& operator=(Filter<Type>* filter)
	// *****************************************************
	// CAUTION : filter will be deleted by the GenericFilter
	// *****************************************************
	{
		if (_filter) delete _filter;
		_filter = filter;
		return *this;
	};

// Accessors
// *********

	public: virtual Filter<Type>* GetClone() const
	// *******************************************
	{
		return (_filter) ? _filter->GetClone() : NULL;
	};

// Predicates
// **********

	public: virtual bool Accept(Type type) const
	// *****************************************
	{
		return (_filter) ? _filter->Accept(type) : false;
	};

// Others
// ******

	public: virtual string _GetTypeName() const
	// **************************************
	{
      return _TName("GenericFilter");
    }

	public: virtual string _GetString() const
	// **************************************
	{
		if (!_filter)
          return "<" + _GetTypeName() + " unbound>";
		else
			return "<" + _GetTypeName() + " " + GetString(_filter) + ">";
	};

};



// ****************************************************************************************************
// NotFilter declaration
// ****************************************************************************************************

template<class Type> class NotFilter : public Filter<Type> {
// *******************************************************

// Types
// *****

	public: typedef Filter<Type> Inherit;

// Attributes
// **********

	private: GenericFilter<Type> _genericFilter;

// Constructors
// ************

	public: NotFilter()
	// ****************
	:	Inherit(),
		_genericFilter()
	{
	};

	public: NotFilter(const Filter<Type>& filter)
	// ******************************************
	:	Inherit(),
		_genericFilter(filter)
	{
	};

	public: NotFilter(const NotFilter<Type>& notFilter)
	// ************************************************
	:	Inherit(),
		_genericFilter(notFilter._genericFilter)
	{
	};

// Operators
// *********

	public: NotFilter& operator=(const NotFilter<Type>& notFilter)
	// ***********************************************************
	{
		_genericFilter = notFilter._genericFilter;
		return *this;
	};

// Accessors
// *********

	public: virtual Filter<Type>* GetClone() const
	// *******************************************
	{
		return new NotFilter(*this);
	};

// Predicates
// **********

	public: virtual bool Accept(Type type) const
	// *****************************************
	{
		return !_genericFilter.Accept(type);
	};

// Others
// ******

	public: virtual string _GetTypeName() const
	// **************************************
	{
      return _TName("GenericNotFilter");
    }

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _GetTypeName() + " " + GetString(_genericFilter) + ">";
	};

};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************


template<typename T>
    class IsNestedSlotAdapter<const Hurricane::GenericFilter<T> > {
      public:
        enum { True=1, False=0 };
    };



} // End of Hurricane namespace.


#endif // HURRICANE_FILTER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
