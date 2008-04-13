// ****************************************************************************************************
// File: IntrusiveSet.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INTRUSIVE_SET
#define HURRICANE_INTRUSIVE_SET

#include <algorithm>

#include "Collection.h"
#include "Error.h" // AD

namespace Hurricane {



// ****************************************************************************************************
// IntrusiveSet declaration
// ****************************************************************************************************

template<class Element> class IntrusiveSet : public NestedSlotAdapter {
// ******************************************************************

// Types
// *****

    class Elements : public Collection<Element*> {
    // *****************************************
        
    // Types
    // *****
    
        public: typedef Collection<Element*> Inherit;
    
        public: class Locator : public Hurricane::Locator<Element*> {
        // ********************************************************
    
        // Types
        // *****
    
            public: typedef Hurricane::Locator<Element*> Inherit;
    
        // Attributes
        // **********
    
            private: const IntrusiveSet* _set;
            private: unsigned _index;
            private: Element* _element;
    
        // Constructors
        // ************
    
            public: Locator(const IntrusiveSet* set = NULL)
            // ********************************************
            :    Inherit(),
                _set(set),
                _index(0),
                _element(NULL)
            {
                if (_set) {
                    unsigned length = _set->_getLength();
                    do {
                        _element = _set->_getArray()[_index++];
                    } while (!_element && (_index < length));
                }
            };

            public: Locator(const Locator& locator)
            // ************************************
            :    Inherit(),
                _set(locator._set),
                _index(locator._index),
                _element(locator._element)
            {
            };
    
        // Operators
        // *********
    
            public: Locator& operator=(const Locator& locator)
            // ***********************************************
            {
                _set = locator._set;
                _index = locator._index;
                _element = locator._element;
                return *this;
            };
    
        // Accessors
        // *********
    
            public: virtual Element* getElement() const
            // ****************************************
            {
                return _element;
            };

            public: virtual Hurricane::Locator<Element*>* getClone() const
            // ***********************************************************
            {
                return new Locator(_set);
            };
    
        // Predicates
        // **********
    
            public: virtual bool isValid() const
            // *********************************
            {
                return (_element != NULL);
            };
    
        // Updators
        // ********
    
            public: virtual void progress()
            // ****************************
            {
                if (_element) {
                    _element = _set->_getNextElement(_element);
                    if (!_element) {
                        unsigned length = _set->_getLength();
                        if (_index < length) {
                            do {
                                _element = _set->_getArray()[_index++];
                            } while (!_element && (_index < length));
                        }
                    }
                }
            };
    
        // Others
        // ******
    
            public: virtual string _getTypeName() const
            // ****************************************
            {
              return _TName("IntrusiveSet::Elements::Locator");
            };

            public: virtual string _getString() const
            // **************************************
            {
                string s = "<" + _getTypeName();
                if (_set) s += " " + getString(_set);
                s += ">";
                return s;
            };
    
        };
    
    // Attributes
    // **********
    
        private: const IntrusiveSet* _set;
    
    // Constructors
    // ************
    
        public: Elements(const IntrusiveSet* set = NULL)
        // *********************************************
        :     Inherit(),
            _set(set)
        {
        };

        public: Elements(const Elements& elements)
        // ***************************************
        :     Inherit(),
            _set(elements._set)
        {
        };
    
    // Operators
    // *********
    
        public: Elements& operator=(const Elements& elements)
        // **************************************************
        {
            _set = elements._set;
            return *this;
        };
    
    // Accessors
    // *********
    
        public: virtual Collection<Element*>* getClone() const
        // ***************************************************
        {
            return new Elements(*this);
        };

        public: virtual Hurricane::Locator<Element*>* getLocator() const
        // *************************************************************
        {
            return new Locator(_set);
        };
    
    // Others
    // ******

        public: virtual string _getTypeName() const
        // **************************************
        {
          return _TName("IntrusiveSet::Elements");
        };
    
        public: virtual string _getString() const
        // **************************************
        {
            string s = "<" + _getTypeName();
            if (_set) s += " " + getString(_set);
            s += ">";
            return s;
        };
    
    };

// Attributes
// **********

    private: unsigned _size;
    private: unsigned _length;
    private: Element** _array;

// Constructors
// ************

    public: IntrusiveSet()
    // *******************
    :    _size(0),
        _length(1),
        _array(new Element*[1])
    {
        _array[0] = NULL;
    };

    private: IntrusiveSet(const IntrusiveSet& set); // not implemented to forbid copy

// Destructor
// **********

    public: virtual ~IntrusiveSet()
    // ****************************
    {
        for (unsigned index = 0; index < _length; index++) {
            Element* element = _array[index];
            while (element) {
                _array[index] = _getNextElement(element);
                _setNextElement(element, NULL);
                element = _array[index];
            }
            _array[index] = NULL;
        }
        delete[] _array;
    };

// Operators
// *********

    private: IntrusiveSet& operator=(const IntrusiveSet& set); // not implemented to forbid assignment

// Accessors
// *********

    public: Elements getElements() const
    // *********************************
    {
        return Elements(this);
    };

// Predicates
// **********

    public: bool isEmpty() const
    // *************************
    {
        return (_size == 0);
    };

// Overridables
// ************

    public: virtual unsigned _getHashValue(Element* element) const = 0;

    // public: virtual Element* _getNextElement(Element* element) const = 0; // AD
    public: virtual Element* _getNextElement(Element* element) const
    // *************************************************************
    {
        throw Error(_TName("IntrusiveSet") + "::_getNextElement(...) : should be overrided");
        return NULL;
    };

    // public: virtual void _setNextElement(Element* element, Element* nextElement) const = 0; // AD
    public: virtual void _setNextElement(Element* element, Element* nextElement) const
    // *******************************************************************************
    {
        throw Error(_TName("IntrusiveSet") + "::_setNextElement(...) : should be overrided");
    };

// Others
// ******

    public: virtual string _getTypeName() const
    // **************************************
    {
      return _TName("InstrusiveSet");
    };

    public: string _getString() const
    // ******************************
    {
        if (isEmpty())
            return "<" + _getTypeName() + " empty>";
        else
            return "<" + _getTypeName() + " " + getString(_size) + ">";
    };

    public: Record* _getRecord() const
    // *************************
    {
        Record* record = NULL;
        if (!isEmpty()) {
            record = new Record(getString(this));
            unsigned n = 1;
            for (unsigned index = 0; index < _length; index++) {
                /**/
                n = 1;
                /**/
                Element* element = _array[index];
                while (element) {
                    // record->add(getSlot(getString(n++), element));
                    record->add(getSlot(getString(index) + ":" + getString(n++), element));
                    /**/
                    element = _getNextElement(element);
                }
            }
        }
        return record;
    };

    public: unsigned _getSize() const
    // ******************************
    {
        return _size;
    };

    public: unsigned _getLength() const
    // ********************************
    {
        return _length;
    };

    public: Element** _getArray() const
    // ********************************
    {
        return _array;
    };

    public: bool _contains(Element* element) const
    // *******************************************
    {
        unsigned index = (_getHashValue(element) / 8) % _length;
        Element* currentElement = _array[index];
        while (currentElement && (currentElement != element))
            currentElement = _getNextElement(currentElement);
        return (currentElement != NULL);
    };

    public: void _insert(Element* element)
    // ***********************************
    {
        if (!_contains(element)) {
            unsigned index = (_getHashValue(element) / 8) % _length;
            _setNextElement(element, _array[index]);
            _array[index] = element;
              _size++;
            _resize();
        }
    };

    public: void _remove(Element* element)
    // ***********************************
    {
        if (_contains(element)) {
            unsigned index = (_getHashValue(element) / 8) % _length;
            Element* currentElement = _array[index];
            if (currentElement) {
                if (currentElement == element) {
                    _array[index] = _getNextElement(element);
                    _setNextElement(element, NULL);
                    _size--;
                }
                else {
                    while (_getNextElement(currentElement) && (_getNextElement(currentElement) != element))
                        currentElement = _getNextElement(currentElement);
                    if (currentElement && (_getNextElement(currentElement) == element)) {
                        _setNextElement(currentElement, _getNextElement(element));
                        _setNextElement(element, NULL);
                        _size--;
                    }
                }
            }
        }
    };

    public: void _resize()
    // *******************
    {
        unsigned newLength = _length;
        double ratio = (double)_size / (double)_length;
        if (ratio < 3)
            newLength = max(_size / 8, (unsigned)1);
        else if (10 < ratio)
            newLength = min(_size / 5, (unsigned)512);

        if (newLength != _length) {
            // cerr << "Resizing: " << this << " " << _length << " " << newLength << endl;
            unsigned oldLength = _length;
            Element** oldArray = _array;
            _length = newLength;
            _array = new Element*[_length];
            memset(_array, 0, _length * sizeof(Element*));
            for (unsigned index = 0; index < oldLength; index++) {
                Element* element = oldArray[index];
                while (element) {
                    Element* nextElement = _getNextElement(element);
                    unsigned newIndex = (_getHashValue(element) / 8) % _length;
                    _setNextElement(element, _array[newIndex]);
                    _array[newIndex] = element;
                    element = nextElement;
                }
            }
            delete[] oldArray;
        }
    };

    public: void _Clear()
    // ******************
    {
        for (unsigned index = 0; index < _length; index++) {
            Element* element = _array[index];
            while (element) {
                _array[index] = _getNextElement(element);
                _setNextElement(element, NULL);
                element = _array[index];
            }
            _array[index] = NULL;
        }
        delete[] _array;

        _size = 0;
        _length = 1;
        _array = new Element*[1];
        _array[0] = NULL;
    };

};



} // End of Hurricane namespace.

#endif // HURRICANE_INTRUSIVE_SET

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
