// ****************************************************************************************************
// File: ./hurricane/Filter.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_FILTER
#define HURRICANE_FILTER

#include "hurricane/Commons.h"

namespace Hurricane {

template<class Type> class GenericFilter;
template<class Type> class NotFilter;



// ****************************************************************************************************
// Filter declaration
// ****************************************************************************************************

template<class Type> class Filter {
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

    public: virtual Filter<Type>* getClone() const = 0;

// Predicates
// **********

    public: virtual bool accept(Type type) const = 0;

// Others
// ******

    public: virtual string _getTypeName() const { return _TName("Filter"); };
    public: virtual string _getString() const = 0;

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
    :    Inherit(),
        _filter(NULL)
    {
    };

    public: GenericFilter(const Filter<Type>& filter)
    // **********************************************
    :    Inherit(),
        _filter(filter.getClone())
    {
    };

    public: GenericFilter(const GenericFilter<Type>& genericFilter)
    // ************************************************************
    :    Inherit(),
        _filter(genericFilter.getClone())
    {
    };

    public: GenericFilter(Filter<Type>* filter)
    // *****************************************************
    // CAUTION : filter will be deleted by the GenericFilter
    // *****************************************************
    :    Inherit(),
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
        _filter = filter.getClone();
        return *this;
    };

    public: GenericFilter& operator=(const GenericFilter& genericFilter)
    // *****************************************************************
    {
        if (_filter) delete _filter;
        _filter = genericFilter.getClone();
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

    public: virtual Filter<Type>* getClone() const
    // *******************************************
    {
        return (_filter) ? _filter->getClone() : NULL;
    };

// Predicates
// **********

    public: virtual bool accept(Type type) const
    // *****************************************
    {
        return (_filter) ? _filter->accept(type) : false;
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("GenericFilter");
    }

    public: virtual string _getString() const
    // **************************************
    {
        if (!_filter)
          return "<" + _getTypeName() + " unbound>";
        else
            return "<" + _getTypeName() + " " + getString(_filter) + ">";
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
    :    Inherit(),
        _genericFilter()
    {
    };

    public: NotFilter(const Filter<Type>& filter)
    // ******************************************
    :    Inherit(),
        _genericFilter(filter)
    {
    };

    public: NotFilter(const NotFilter<Type>& notFilter)
    // ************************************************
    :    Inherit(),
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

    public: virtual Filter<Type>* getClone() const
    // *******************************************
    {
        return new NotFilter(*this);
    };

// Predicates
// **********

    public: virtual bool accept(Type type) const
    // *****************************************
    {
        return !_genericFilter.accept(type);
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("GenericNotFilter");
    }

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _getTypeName() + " " + getString(_genericFilter) + ">";
    };

};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************



} // End of Hurricane namespace.


#endif // HURRICANE_FILTER


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
