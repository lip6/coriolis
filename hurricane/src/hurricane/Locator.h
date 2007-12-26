// ****************************************************************************************************
// File: Locator.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#ifndef HURRICANE_LOCATOR
#define HURRICANE_LOCATOR

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Locator declaration
// ****************************************************************************************************

template<class Type> class Locator : public NestedSlotAdapter {
// **********************************************************

// Constructors
// ************

	protected: Locator()
	// *****************
	{
	};

	private: Locator(const Locator& locator);
	// *******************************************
	// not implemented to forbid copy construction
	// *******************************************

// Destructor
// **********

	public: virtual ~Locator()
	// ***********************
	{
	};

// Operators
// *********

	private: Locator& operator=(const Locator& locator);
	// *************************************************
	// not implemented to forbid assignment
	// *************************************************

// Accessors
// *********

	public: virtual Type GetElement() const = 0;
	public: virtual Locator<Type>* GetClone() const = 0;

	public: virtual Locator<Type>* GetLocator()      // 21-10-2003
	// ****************************************
	{
	  return ( this );
	}

// Predicates
// **********

	public: virtual bool IsValid() const = 0;

// Updators
// ********

	public: virtual void Progress() = 0;

// Others
// ******

    public: virtual string _GetTypeName() const
	// **************************************
    {
      return _TName("Locator");
    };

	public: virtual string _GetString() const
	// **************************************
	{
		if (!IsValid())
			return "<" + _GetTypeName() + " invalid>";
		else
			return "<" + _GetTypeName() + " " + GetString(GetElement()) + ">";
	};

};



// ****************************************************************************************************
// GenericLocator declaration
// ****************************************************************************************************

template<class Type> class GenericLocator : public Locator<Type> {
// *************************************************************

// Types
// *****

	public: typedef Locator<Type> Inherit;

// Attributes
// **********

	private: Locator<Type>* _locator;

// Constructors
// ************

	public: GenericLocator()
	// *********************
	:	Inherit(),
		_locator(NULL)
	{
	};

	public: GenericLocator(const Locator<Type>& locator)
	// *************************************************
	:	Inherit(),
		_locator(locator.GetClone())
	{
	};

	public: GenericLocator(const GenericLocator& genericLocator)
	// *********************************************************
	:	Inherit(),
		_locator(genericLocator.GetClone())
	{
	};

	public: GenericLocator(Locator<Type>* locator)
	// *******************************************************
	// CAUTION : locator will be deleted by the GenericLocator
	// *******************************************************
	:	Inherit(),
		_locator(locator)
	{
	};

// Destructor
// **********

	public: virtual ~GenericLocator()
	// ******************************
	{
		if (_locator) delete _locator;
	};

// Operators
// *********

	public: GenericLocator& operator=(const Locator<Type>& locator)
	// ************************************************************
	{
		if (_locator) delete _locator;
		_locator = locator.GetClone();
		return *this;
	};

	public: GenericLocator& operator=(const GenericLocator& genericLocator)
	// ********************************************************************
	{
		if (_locator) delete _locator;
		_locator = genericLocator.GetClone();
		return *this;
	};

	public: GenericLocator& operator=(Locator<Type>* locator)
	// *******************************************************
	// CAUTION : locator will be deleted by the GenericLocator
	// *******************************************************
	{
		if (_locator) delete _locator;
		_locator = locator;
		return *this;
	};

// Accessors
// *********

	public: virtual Type GetElement() const
	// ************************************
	{
		return ((_locator) ? _locator->GetElement() : Type());
	};

	public: virtual Locator<Type>* GetClone() const
	// ********************************************
	{
		return ((_locator) ? _locator->GetClone() : NULL);
	};

	public: virtual Locator<Type>* GetLocator()   // 21-10-2003
	// ****************************************
	{
	  return ( _locator->GetLocator () );
	}


// Predicates
// **********

	public: virtual bool IsValid() const
	// *********************************
	{
		return (_locator && _locator->IsValid());
	};

// Updators
// ********

	public: virtual void Progress()
	// ****************************
	{
		if (_locator) _locator->Progress();
	};

// Others
// ******

    public: virtual string _GetTypeName() const
	// **************************************
    {
      return _TName("GenericLocator");
    };

	public: virtual string _GetString() const
	// **************************************
	{
		if (!_locator)
			return "<" + _GetTypeName() + " unbound>";
		else
			return "<" + _GetTypeName() + " " + GetString(_locator) + ">";
	};

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************


template<typename T>
  class IsNestedSlotAdapter<const Hurricane::GenericLocator<T> > {
      public:
        enum { True=1, False=0 };
    };



} // End of Hurricane namespace.


#endif // HURRICANE_LOCATOR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
