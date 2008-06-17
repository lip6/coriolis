// ****************************************************************************************************
// File: Locator.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#ifndef HURRICANE_LOCATOR
#define HURRICANE_LOCATOR

#include "hurricane/Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Locator declaration
// ****************************************************************************************************

template<class Type> class Locator {
// *********************************

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

    public: virtual Type getElement() const = 0;
    public: virtual Locator<Type>* getClone() const = 0;

    public: virtual Locator<Type>* getLocator()      // 21-10-2003
    // ****************************************
    {
      return ( this );
    }

// Predicates
// **********

    public: virtual bool isValid() const = 0;

// Updators
// ********

    public: virtual void progress() = 0;

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("Locator");
    };

    public: virtual string _getString() const
    // **************************************
    {
        if (!isValid())
            return "<" + _getTypeName() + " invalid>";
        else
            return "<" + _getTypeName() + " " + getString(getElement()) + ">";
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
    :    Inherit(),
        _locator(NULL)
    {
    };

    public: GenericLocator(const Locator<Type>& locator)
    // *************************************************
    :    Inherit(),
        _locator(locator.getClone())
    {
    };

    public: GenericLocator(const GenericLocator& genericLocator)
    // *********************************************************
    :    Inherit(),
        _locator(genericLocator.getClone())
    {
    };

    public: GenericLocator(Locator<Type>* locator)
    // *******************************************************
    // CAUTION : locator will be deleted by the GenericLocator
    // *******************************************************
    :    Inherit(),
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
        _locator = locator.getClone();
        return *this;
    };

    public: GenericLocator& operator=(const GenericLocator& genericLocator)
    // ********************************************************************
    {
        if (_locator) delete _locator;
        _locator = genericLocator.getClone();
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

    public: virtual Type getElement() const
    // ************************************
    {
        return ((_locator) ? _locator->getElement() : Type());
    };

    public: virtual Locator<Type>* getClone() const
    // ********************************************
    {
        return ((_locator) ? _locator->getClone() : NULL);
    };

    public: virtual Locator<Type>* getLocator()   // 21-10-2003
    // ****************************************
    {
      return ( _locator->getLocator () );
    }


// Predicates
// **********

    public: virtual bool isValid() const
    // *********************************
    {
        return (_locator && _locator->isValid());
    };

// Updators
// ********

    public: virtual void progress()
    // ****************************
    {
        if (_locator) _locator->progress();
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("GenericLocator");
    };

    public: virtual string _getString() const
    // **************************************
    {
        if (!_locator)
            return "<" + _getTypeName() + " unbound>";
        else
            return "<" + _getTypeName() + " " + getString(_locator) + ">";
    };

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************


} // End of Hurricane namespace.


#endif // HURRICANE_LOCATOR

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
