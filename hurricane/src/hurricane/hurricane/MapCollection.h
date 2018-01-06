// ****************************************************************************************************
// File: ./hurricane/MapCollection.h
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

#ifndef HURRICANE_MAP_COLLECTION
#define HURRICANE_MAP_COLLECTION

#include "hurricane/Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// MapCollection declaration
// ****************************************************************************************************

template<class Key, class Element, class Compare = less<Key> >
	class MapCollection : public Collection<Element> {
// ************************************************

// Types
// *****

	public: typedef Collection<Element> Inherit;

	public: typedef map<Key, Element, Compare> ElementMap;

	public: class Locator : public Hurricane::Locator<Element> {
	// *******************************************************

		public: typedef Hurricane::Locator<Element> Inherit;

		private: const ElementMap* _elementMap;
		private: typename ElementMap::const_iterator _iterator; // AD

		public: Locator(const ElementMap* elementMap)
		// ******************************************
		:	Inherit(),
			_elementMap(elementMap),
			_iterator()
		{
			if (_elementMap) _iterator = _elementMap->begin();
		};

		public: virtual Element getElement() const
		// ***************************************
		{
			return (isValid()) ? (*_iterator).second : Element();
		};

		public: virtual Hurricane::Locator<Element>* getClone() const
		// **********************************************************
		{
			return new Locator(_elementMap);
		};

		public: virtual bool isValid() const
		// *********************************
		{
			return (_elementMap && (_iterator != _elementMap->end()));
		};

		public: virtual void progress()
		// ****************************
		{
			++_iterator;
		};

	};

// Attributes
// **********

	private: const ElementMap* _elementMap;

// Constructors
// ************

	public: MapCollection(const ElementMap* elementMap = NULL)
	// *******************************************************
	:	Inherit(),
		_elementMap(elementMap)
	{
	};

	public: MapCollection(const ElementMap& elementMap)
	// ************************************************
	:	Inherit(),
		_elementMap(&elementMap)
	{
	};

	public: MapCollection(const MapCollection& mapCollection)
	// ******************************************************
	:	Inherit(),
		_elementMap(mapCollection._elementMap)
	{
	};

// Operators
// *********

	public: MapCollection& operator=(const MapCollection& mapCollection)
	// *****************************************************************
	{
		_elementMap = mapCollection._elementMap;
		return *this;
	};

// Accessors
// *********

	public: virtual Collection<Element>* getClone() const
	// **************************************************
	{
		return new MapCollection(*this);
	}

	public: virtual Hurricane::Locator<Element>* getLocator() const
	// ************************************************************
	{
		// return (_elementMap) ? new Locator<Key, Element, Compare>(_elementMap) : NULL;
		// V3
		return (_elementMap) ? new Locator(_elementMap) : NULL;
	}

	public: virtual unsigned getSize() const
	// *************************************
	{
		return (_elementMap) ? _elementMap->size() : 0;
	};

// Others
// ******

    public: virtual string _getTypeName() const
	// **************************************
    {
      return _TName("MapCollection");
    };

	public: virtual string _getString() const
	// **************************************
	{
		if (!_elementMap)
			return "<" + _getTypeName() + " unbound>";
		else {
			if (_elementMap->empty())
				return "<" + _getTypeName() + " empty>";
			else
				return "<" + _getTypeName() + " " + getString(_elementMap->size()) + ">";
		}
	};

	Record* _getRecord() const
    // ********************
    {
      Record* record = NULL;
      if (!_elementMap->empty()) {
		record = new Record(_getString());
		typename map<Key, Element, Compare>::const_iterator iterator = _elementMap->begin(); // AD
		while (iterator != _elementMap->end()) {
          record->add(getSlot<Element>(getString((*iterator).first), (*iterator).second));
          ++iterator;
		}
      }
      return record;
    }

};



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

template<class Key, class Element, class Compare>
	inline GenericCollection<Element> getCollection(const map<Key, Element, Compare>& elementMap)
// *********************************************************************************************
{
	return MapCollection<Key, Element, Compare>(elementMap);
}

template<class Key, class Element, class Compare>
	inline GenericCollection<Element> getCollection(const map<Key, Element, Compare>* elementMap)
// *********************************************************************************************
{
	return MapCollection<Key, Element, Compare>(elementMap);
}



} // End of Hurricane namespace.

#endif // HURRICANE_MAP_COLLECTION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
