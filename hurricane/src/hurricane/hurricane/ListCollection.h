// ****************************************************************************************************
// File: ./hurricane/ListCollection.h
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

#ifndef HURRICANE_LIST_COLLECTION
#define HURRICANE_LIST_COLLECTION

#include "hurricane/Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// ListCollection declaration
// ****************************************************************************************************

template<class Element> class ListCollection : public Collection<Element> {
// **********************************************************************

// Types
// *****

    public: typedef Collection<Element> Inherit;

    public: typedef list<Element> ElementList;

    public: class Locator : public Hurricane::Locator<Element> {
    // *******************************************************

        public: typedef Hurricane::Locator<Element> Inherit;

        private: const ElementList* _elementList;
        private: typename ElementList::const_iterator _iterator; // AD

        public: Locator(const ElementList* elementList)
        // ********************************************
        :    Inherit(),
            _elementList(elementList),
            _iterator()
        {
            if (_elementList) _iterator = _elementList->begin();
        };

        public: virtual Element getElement() const
        // ***************************************
        {
            return (isValid()) ? *_iterator : Element();
        };

        public: virtual Hurricane::Locator<Element>* getClone() const
        // **********************************************************
        {
            return new Locator(_elementList);
        };

        public: virtual bool isValid() const
        // *********************************
        {
            return (_elementList && (_iterator != _elementList->end()));
        };

        public: virtual void progress()
        // ****************************
        {
            ++_iterator;
        };

    };

// Attributes
// **********

    private: const ElementList* _elementList;

// Constructors
// ************

    public: ListCollection(const ElementList* elementList = NULL)
    // **********************************************************
    :    Inherit(),
        _elementList(elementList)
    {
    };

    public: ListCollection(const ElementList& elementList)
    // ***************************************************
    :    Inherit(),
        _elementList(&elementList)
    {
    };

    public: ListCollection(const ListCollection& listCollection)
    // *********************************************************
    :    Inherit(),
        _elementList(listCollection._elementList)
    {
    };

// Operators
// *********

    public: ListCollection& operator=(const ListCollection& listCollection)
    // ********************************************************************
    {
        _elementList = listCollection._elementList;
        return *this;
    };

// Accessors
// *********

    public: virtual Collection<Element>* getClone() const
    // **************************************************
    {
        return new ListCollection(*this);
    }

    public: virtual Hurricane::Locator<Element>* getLocator() const
    // ************************************************************
    {
        // return (_elementList) ? new Locator<Element>(_elementList) : NULL;
        // V3
        return (_elementList) ? new Locator(_elementList) : NULL;
    }

    public: virtual unsigned getSize() const
    // *************************************
    {
        return (_elementList) ? _elementList->size() : 0;
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("ListCollection");
    };

    public: virtual string _getString() const
    // **************************************
    {
        if (!_elementList)
            return "<" + _getTypeName() + " unbound>";
        else {
            if (_elementList->empty())
                return "<" + _getTypeName() + " empty>";
            else
                return "<" + _getTypeName() + " " + getString(_elementList->size()) + ">";
        }
    };

    public: Record* _getRecord() const
    // *************************
    {
      Record* record = NULL;
      if (!_elementList->empty()) {
        record = new Record(_getString());
        unsigned n = 1;
        typename list<Element>::const_iterator iterator = _elementList->begin(); // AD
        while (iterator != _elementList->end()) {
          string   slotName   = getString(n++);
          Element  slotObject = *iterator;
          record->add(getSlot(slotName, slotObject));
          ++iterator;
        }
      }
      return record;
    }

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

template<class Element>
    inline GenericCollection<Element> getCollection(const list<Element>& elementList)
// *********************************************************************************
{
    return ListCollection<Element>(elementList);
}

template<class Element>
    inline GenericCollection<Element> getCollection(const list<Element>* elementList)
// *********************************************************************************
{
    return ListCollection<Element>(elementList);
}



} // End of Hurricane namespace.

#endif // HURRICANE_LIST_COLLECTION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
