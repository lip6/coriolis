// ****************************************************************************************************
// File: ./hurricane/SetCollection.h
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

#ifndef HURRICANE_SET_COLLECTION
#define HURRICANE_SET_COLLECTION

#include "hurricane/Commons.h"

#include "hurricane/Tabulation.h"


namespace Hurricane {



// ****************************************************************************************************
// SetCollection declaration
// ****************************************************************************************************

template<class Element, class Compare = less<Element> >
    class SetCollection : public Collection<Element> {
// ************************************************

// Types
// *****

    public: typedef Collection<Element> Inherit;

    public: typedef set<Element, Compare> ElementSet;

    public: class Locator : public Hurricane::Locator<Element> {
    // *******************************************************

        public: typedef Hurricane::Locator<Element> Inherit;

        private: const ElementSet* _elementSet;
        private: typename ElementSet::const_iterator _iterator; // AD

        public: Locator(const ElementSet* elementSet)
        // ******************************************
        :    Inherit(),
            _elementSet(elementSet),
            _iterator()
        {
            if (_elementSet) _iterator = _elementSet->begin();
        };

        public: virtual Element getElement() const
        // ***************************************
        {
            return (isValid()) ? *_iterator : Element();
        };

        public: virtual Hurricane::Locator<Element>* getClone() const
        // **********************************************************
        {
            return new Locator(_elementSet);
        };

        public: virtual bool isValid() const
        // *********************************
        {
            return (_elementSet && (_iterator != _elementSet->end()));
        };

        public: virtual void progress()
        // ****************************
        {
            ++_iterator;
        };

    };

// Attributes
// **********

    private: const ElementSet* _elementSet;

// Constructors
// ************

    public: SetCollection(const ElementSet* elementSet = NULL)
    // *******************************************************
    :    Inherit(),
        _elementSet(elementSet)
    {
    };

    public: SetCollection(const ElementSet& elementSet)
    // ************************************************
    :    Inherit(),
        _elementSet(&elementSet)
    {
    };

    public: SetCollection(const SetCollection& setCollection)
    // ******************************************************
    :    Inherit(),
        _elementSet(setCollection._elementSet)
    {
    };

// Operators
// *********

    public: SetCollection& operator=(const SetCollection& setCollection)
    // *****************************************************************
    {
        _elementSet = setCollection._elementSet;
        return *this;
    };

// Accessors
// *********

    public: virtual Collection<Element>* getClone() const
    // **************************************************
    {
        return new SetCollection(*this);
    }

    public: virtual Hurricane::Locator<Element>* getLocator() const
    // ************************************************************
    {
        // return (_elementSet) ? new Locator<Element, Compare>(_elementSet) : NULL;
        // V3
        return (_elementSet) ? new Locator(_elementSet) : NULL;
    }

    public: virtual unsigned getSize() const
    // *************************************
    {
        return (_elementSet) ? _elementSet->size() : 0;
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("SetCollection");
    };

    public: virtual string _getString() const
    // **************************************
    {
        if (!_elementSet)
            return "<" + _getTypeName() + " unbound>";
        else {
            if (_elementSet->empty())
                return "<" + _getTypeName() + " empty>";
            else
                return "<" + _getTypeName() + " " + getString(_elementSet->size()) + ">";
        }
    };

    Record* _getRecord() const
    // *****************
    {
      Record* record = NULL;
      if (!_elementSet->empty()) {
        record = new Record(_getString());
        unsigned n = 1;
        typename set<Element, Compare>::const_iterator iterator = _elementSet->begin(); // AD
        while (iterator != _elementSet->end()) {
          record->add(getSlot(getString(n++), *iterator));
          ++iterator;
        }
      }
      return record;
    }

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

template<class Element, class Compare>
    inline GenericCollection<Element> getCollection(const set<Element, Compare>& elementSet)
// ****************************************************************************************
{
    return SetCollection<Element, Compare>(elementSet);
}

template<class Element, class Compare>
    inline GenericCollection<Element> getCollection(const set<Element, Compare>* elementSet)
// ****************************************************************************************
{
    return SetCollection<Element, Compare>(elementSet);
}



} // End of Hurricane namespace.



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

#endif // HURRICANE_SET_COLLECTION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
