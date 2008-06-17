// ****************************************************************************************************
// File: MultisetCollection.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MULTISET_COLLECTION
#define HURRICANE_MULTISET_COLLECTION

#include "hurricane/Commons.h"


namespace Hurricane {



// ****************************************************************************************************
// MultisetCollection declaration
// ****************************************************************************************************

template<class Element, class Compare = less<Element> >
    class MultisetCollection : public Collection<Element> {
// ************************************************

// Types
// *****

    public: typedef Collection<Element> Inherit;

    public: typedef multiset<Element, Compare> ElementMultiset;

    public: class Locator : public Hurricane::Locator<Element> {
    // *******************************************************

        public: typedef Hurricane::Locator<Element> Inherit;

        private: const ElementMultiset* _elementMultiset;
        private: typename ElementMultiset::const_iterator _iterator; // AD

        public: Locator(const ElementMultiset* elementMultiset)
        // ******************************************
        :    Inherit(),
            _elementMultiset(elementMultiset),
            _iterator()
        {
            if (_elementMultiset) _iterator = _elementMultiset->begin();
        };

        public: virtual Element getElement() const
        // ***************************************
        {
            return (isValid()) ? *_iterator : Element();
        };

        public: virtual Hurricane::Locator<Element>* getClone() const
        // **********************************************************
        {
            return new Locator(_elementMultiset);
        };

        public: virtual bool isValid() const
        // *********************************
        {
            return (_elementMultiset && (_iterator != _elementMultiset->end()));
        };

        public: virtual void progress()
        // ****************************
        {
            ++_iterator;
        };

    };

// Attributes
// **********

    private: const ElementMultiset* _elementMultiset;

// Constructors
// ************

    public: MultisetCollection(const ElementMultiset* elementMultiset = NULL)
    // *******************************************************
    :    Inherit(),
        _elementMultiset(elementMultiset)
    {
    };

    public: MultisetCollection(const ElementMultiset& elementMultiset)
    // ************************************************
    :    Inherit(),
        _elementMultiset(&elementMultiset)
    {
    };

    public: MultisetCollection(const MultisetCollection& multisetCollection)
    // ******************************************************
    :    Inherit(),
        _elementMultiset(multisetCollection._elementMultiset)
    {
    };

// Operators
// *********

    public: MultisetCollection& operator=(const MultisetCollection& multisetCollection)
    // *****************************************************************
    {
        _elementMultiset = multisetCollection._elementMultiset;
        return *this;
    };

// Accessors
// *********

    public: virtual Collection<Element>* getClone() const
    // **************************************************
    {
        return new MultisetCollection(*this);
    }

    public: virtual Hurricane::Locator<Element>* getLocator() const
    // ************************************************************
    {
        // return (_elementMultiset) ? new Locator<Element, Compare>(_elementMultiset) : NULL;
        // V3
        return (_elementMultiset) ? new Locator(_elementMultiset) : NULL;
    }

    public: virtual unsigned getSize() const
    // *************************************
    {
        return (_elementMultiset) ? _elementMultiset->size() : 0;
    };

// Others
// ******

    public: virtual string _getString() const
    // **************************************
    {
        if (!_elementMultiset)
            return "<" + _TName("MultisetCollection") + " unbounnd>";
        else {
            if (_elementMultiset->empty())
                return "<" + _TName("MultisetCollection") + " empty>";
            else
                return "<" + _TName("MultisetCollection") + " " + getString(_elementMultiset->size()) + ">";
        }
    };

    Record* _getRecord() const
    // *****************
    {
      Record* record = NULL;
      if (!_elementMultiset->empty()) {
        record = new Record(_getString());
        unsigned n = 1;
        typename multiset<Element, Compare>::const_iterator iterator = _elementMultiset->begin(); // AD
        while (iterator != _elementMultiset->end()) {
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
    inline GenericCollection<Element> getCollection(const multiset<Element, Compare>& elementMultiset)
// ****************************************************************************************
{
    return MultisetCollection<Element, Compare>(elementMultiset);
}

template<class Element, class Compare>
    inline GenericCollection<Element> getCollection(const multiset<Element, Compare>* elementMultiset)
// ****************************************************************************************
{
    return MultisetCollection<Element, Compare>(elementMultiset);
}



} // End of Hurricane namespace.

#endif // HURRICANE_SET_COLLECTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
