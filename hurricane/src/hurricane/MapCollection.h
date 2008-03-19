// ****************************************************************************************************
// File: MapCollection.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MAP_COLLECTION
#define HURRICANE_MAP_COLLECTION

#include "Commons.h"

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

		public: virtual Element GetElement() const
		// ***************************************
		{
			return (isValid()) ? (*_iterator).second : Element();
		};

		public: virtual Hurricane::Locator<Element>* GetClone() const
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

	public: virtual Collection<Element>* GetClone() const
	// **************************************************
	{
		return new MapCollection(*this);
	}

	public: virtual Hurricane::Locator<Element>* GetLocator() const
	// ************************************************************
	{
		// return (_elementMap) ? new Locator<Key, Element, Compare>(_elementMap) : NULL;
		// V3
		return (_elementMap) ? new Locator(_elementMap) : NULL;
	}

	public: virtual unsigned GetSize() const
	// *************************************
	{
		return (_elementMap) ? _elementMap->size() : 0;
	};

// Others
// ******

    public: virtual string _GetTypeName() const
	// **************************************
    {
      return _TName("MapCollection");
    };

	public: virtual string _GetString() const
	// **************************************
	{
		if (!_elementMap)
			return "<" + _GetTypeName() + " unbound>";
		else {
			if (_elementMap->empty())
				return "<" + _GetTypeName() + " empty>";
			else
				return "<" + _GetTypeName() + " " + GetString(_elementMap->size()) + ">";
		}
	};

	Record* _GetRecord() const
    // ********************
    {
      Record* record = NULL;
      if (!_elementMap->empty()) {
		record = new Record(_GetString());
		typename map<Key, Element, Compare>::const_iterator iterator = _elementMap->begin(); // AD
		while (iterator != _elementMap->end()) {
          record->Add(GetSlot(GetString((*iterator).first), (*iterator).second));
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
	inline GenericCollection<Element> GetCollection(const map<Key, Element, Compare>& elementMap)
// *********************************************************************************************
{
	return MapCollection<Key, Element, Compare>(elementMap);
}

template<class Key, class Element, class Compare>
	inline GenericCollection<Element> GetCollection(const map<Key, Element, Compare>* elementMap)
// *********************************************************************************************
{
	return MapCollection<Key, Element, Compare>(elementMap);
}



} // End of Hurricane namespace.

#endif // HURRICANE_MAP_COLLECTION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
