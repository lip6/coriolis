
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/MultimapCollection.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_MULTIMAP_COLLECTION__
#define  __HURRICANE_MULTIMAP_COLLECTION__

#include "hurricane/Commons.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class Template  :  "Hurricane::MultimapCollection".

  template<class Key, class Element, class Compare=less<Key> >
  class MultimapCollection : public Collection<Element> {
    public:
    // Types.
      typedef std::multimap<Key,Element,Compare> Multimap;
	public:
    // Locator sub-class.
      class Locator : public Hurricane::Locator<Element> {
		public:
                                                      Locator    ( const Multimap* );
          virtual Element                             getElement () const;
          virtual Hurricane::Locator<Element>*        getClone   () const;
          virtual bool                                isValid    () const;
          virtual void                                progress   ();
		private:
                   const Multimap*                    _elements;
                   typename Multimap::const_iterator  _iterator;
      };

	public:
    // Methods.
                                           MultimapCollection ( const Multimap* elements=NULL );
                                           MultimapCollection ( const Multimap& );
                                           MultimapCollection ( const MultimapCollection& );
              MultimapCollection&          operator=          ( const MultimapCollection& );
      virtual Collection<Element>*         getClone           () const;
      virtual Hurricane::Locator<Element>* getLocator         () const;
      virtual unsigned                     getSize            () const;
      virtual string                       _getTypeName       () const;
      virtual string                       _getString         () const;
              Record*                      _getRecord         () const;

	private:
    // Internal: Attributes.
              const Multimap*              _elements;
  };


// -------------------------------------------------------------------
// Class Definition  :  "Hurricane::MultimapCollection::Locator".


  template<class Key, class Element, class Compare>
  MultimapCollection<Key,Element,Compare>::Locator::Locator ( const Multimap* elements )
    : Hurricane::Locator<Element>()
    , _elements(elements)
    , _iterator()
  { if (_elements) _iterator = _elements->begin(); };


  template<class Key, class Element, class Compare>
  Element MultimapCollection<Key,Element,Compare>::Locator::getElement () const
  {
    return (isValid()) ? (*_iterator).second : Element();
  }


  template<class Key, class Element, class Compare>
  Hurricane::Locator<Element>* MultimapCollection<Key,Element,Compare>::Locator::getClone () const
  {
    return new Locator(_elements);
  }


  template<class Key, class Element, class Compare>
  bool MultimapCollection<Key,Element,Compare>::Locator::isValid () const
  {
    return (_elements && (_iterator != _elements->end()));
  }
  

  template<class Key, class Element, class Compare>
  void MultimapCollection<Key,Element,Compare>::Locator::progress ()
  {
    ++_iterator;
  }


// -------------------------------------------------------------------
// Class Definition  :  "Hurricane::MultimapCollection::Locator".


  template<class Key, class Element, class Compare>
  MultimapCollection<Key,Element,Compare>::MultimapCollection ( const Multimap* elements )
    : Collection<Element>()
    , _elements(elements)
  { }


  template<class Key, class Element, class Compare>
  MultimapCollection<Key,Element,Compare>::MultimapCollection ( const Multimap& elements )
	: Collection<Element>()
    , _elements(&elements)
  { }


  template<class Key, class Element, class Compare>
  MultimapCollection<Key,Element,Compare>::MultimapCollection ( const MultimapCollection& collection )
    : Collection<Element>()
    , _elements(collection._elements)
  { }


  template<class Key, class Element, class Compare>
  MultimapCollection<Key,Element,Compare>& MultimapCollection<Key,Element,Compare>::operator= ( const MultimapCollection& collection )
  {
    _elements = collection._elements;
    return *this;
  }


  template<class Key, class Element, class Compare>
  Collection<Element>* MultimapCollection<Key,Element,Compare>::getClone () const
  {
    return new MultimapCollection(*this);
  }


  template<class Key, class Element, class Compare>
  Hurricane::Locator<Element>* MultimapCollection<Key,Element,Compare>::getLocator () const
  {
    return (_elements) ? new Locator(_elements) : NULL;
  }


  template<class Key, class Element, class Compare>
  unsigned  MultimapCollection<Key,Element,Compare>::getSize () const
  {
    return (_elements) ? _elements->size() : 0;
  }
  

  template<class Key, class Element, class Compare>
  string  MultimapCollection<Key,Element,Compare>::_getTypeName () const
  {
    return _TName("MultimapCollection");
  }


  template<class Key, class Element, class Compare>
  string  MultimapCollection<Key,Element,Compare>::_getString () const
  {
    if (!_elements)
      return "<" + _getTypeName() + " unbound>";
    else {
      if (_elements->empty())
        return "<" + _getTypeName() + " empty>";
      else
        return "<" + _getTypeName() + " " + getString(_elements->size()) + ">";
    }
  }


  template<class Key, class Element, class Compare>
  Record* MultimapCollection<Key,Element,Compare>::_getRecord () const
  {
    Record* record = NULL;
    if (!_elements->empty()) {
      record = new Record(_getString());
      typename map<Key,Element,Compare>::const_iterator iterator = _elements->begin();
      while (iterator != _elements->end()) {
        record->add(getSlot<Element>(getString((*iterator).first), (*iterator).second));
        ++iterator;
      }
    }
    return record;
  }


// -------------------------------------------------------------------
// Functions Templates.


//   template<class Key, class Element, class Compare>
//   inline GenericCollection<Element> getCollection ( const std::multimap<Key,Element,Compare>& elements )
//   {
// 	return MultimapCollection<Key,Element,Compare>(elements);
//   }


  template<class Key, class Element, class Compare>
  inline GenericCollection<Element> getCollection ( const std::multimap<Key,Element,Compare>* elements )
  {
	return MultimapCollection<Key,Element,Compare>(elements);
  }


// -------------------------------------------------------------------
// Class Template  :  "Hurricane::MultimapRangeCollection".

  template<class Key, class Element, class Compare=less<Key> >
  class MultimapRangeCollection : public Collection<Element> {
    public:
    // Types.
      typedef std::multimap<Key,Element,Compare> Multimap;
	public:
    // Locator sub-class.
      class Locator : public Hurricane::Locator<Element> {
		public:
                                                      Locator    ( const Multimap*, const Key& );
          virtual Element                             getElement () const;
          virtual Hurricane::Locator<Element>*        getClone   () const;
          virtual bool                                isValid    () const;
          virtual void                                progress   ();
		private:
                   const Multimap*                    _elements;
                   const Key                          _key;
                   typename Multimap::const_iterator  _iterator;
                   typename Multimap::const_iterator  _upperbound;
      };

	public:
    // Methods.
                                           MultimapRangeCollection ( const Multimap*, const Key& );
                                           MultimapRangeCollection ( const Multimap&, const Key& );
                                           MultimapRangeCollection ( const MultimapRangeCollection& );
              MultimapRangeCollection&     operator=               ( const MultimapRangeCollection& );
      virtual Collection<Element>*         getClone                () const;
      virtual Hurricane::Locator<Element>* getLocator              () const;
      virtual unsigned                     getSize                 () const;
      virtual string                       _getTypeName            () const;
      virtual string                       _getString              () const;
              Record*                      _getRecord              () const;

	private:
    // Internal: Attributes.
              const Multimap*              _elements;
              const Key                    _key;
  };


// -------------------------------------------------------------------
// Class Definition  :  "Hurricane::MultimapRangeCollection::Locator".


  template<class Key, class Element, class Compare>
  MultimapRangeCollection<Key,Element,Compare>::Locator::Locator ( const Multimap* elements, const Key& key )
    : Hurricane::Locator<Element>()
    , _elements(elements)
    , _key(key)
    , _iterator()
    , _upperbound()
  {
    if (_elements) {
      _iterator   = _elements->lower_bound(key);
      _upperbound = _elements->upper_bound(key);
    }
 };


  template<class Key, class Element, class Compare>
  Element MultimapRangeCollection<Key,Element,Compare>::Locator::getElement () const
  {
    return (isValid()) ? (*_iterator).second : Element();
  }


  template<class Key, class Element, class Compare>
  Hurricane::Locator<Element>* MultimapRangeCollection<Key,Element,Compare>::Locator::getClone () const
  {
    return new Locator(_elements,_key);
  }


  template<class Key, class Element, class Compare>
  bool MultimapRangeCollection<Key,Element,Compare>::Locator::isValid () const
  {
    return (_elements && (_iterator != _upperbound));
  }
  

  template<class Key, class Element, class Compare>
  void MultimapRangeCollection<Key,Element,Compare>::Locator::progress ()
  {
    ++_iterator;
  }


// -------------------------------------------------------------------
// Class Definition  :  "Hurricane::MultimapRangeCollection::Locator".


  template<class Key, class Element, class Compare>
  MultimapRangeCollection<Key,Element,Compare>::MultimapRangeCollection ( const Multimap* elements, const Key& key )
    : Collection<Element>()
    , _elements(elements)
    , _key(key)
  { }


  template<class Key, class Element, class Compare>
  MultimapRangeCollection<Key,Element,Compare>::MultimapRangeCollection ( const Multimap& elements, const Key& key )
	: Collection<Element>()
    , _elements(&elements)
    , _key(key)
  { }


  template<class Key, class Element, class Compare>
  MultimapRangeCollection<Key,Element,Compare>::MultimapRangeCollection ( const MultimapRangeCollection& collection )
    : Collection<Element>()
    , _elements(collection._elements)
    , _key(collection._key)
  { }


  template<class Key, class Element, class Compare>
  MultimapRangeCollection<Key,Element,Compare>& MultimapRangeCollection<Key,Element,Compare>::operator= ( const MultimapRangeCollection& collection )
  {
    _elements = collection._elements;
    _key      = collection._key;
    return *this;
  }


  template<class Key, class Element, class Compare>
  Collection<Element>* MultimapRangeCollection<Key,Element,Compare>::getClone () const
  {
    return new MultimapRangeCollection(*this);
  }


  template<class Key, class Element, class Compare>
  Hurricane::Locator<Element>* MultimapRangeCollection<Key,Element,Compare>::getLocator () const
  {
    return (_elements) ? new Locator(_elements,_key) : NULL;
  }


  template<class Key, class Element, class Compare>
  unsigned  MultimapRangeCollection<Key,Element,Compare>::getSize () const
  {
    if ( !_elements ) return 0;

    size_t size = 0;
    typename Multimap::const_iterator lb = _elements->upper_bound(_key);
    typename Multimap::const_iterator ub = _elements->upper_bound(_key);
    for ( ; lb != ub ; lb++, size++ );

    return size;
  }
  

  template<class Key, class Element, class Compare>
  string  MultimapRangeCollection<Key,Element,Compare>::_getTypeName () const
  {
    return _TName("MultimapRangeCollection");
  }


  template<class Key, class Element, class Compare>
  string  MultimapRangeCollection<Key,Element,Compare>::_getString () const
  {
    if (!_elements)
      return "<" + _getTypeName() + " unbound>";
    else {
      if (_elements->empty())
        return "<" + _getTypeName() + " empty>";
      else
        return "<" + _getTypeName() + " " + getString(_elements->size()) + ">";
    }
  }


  template<class Key, class Element, class Compare>
  Record* MultimapRangeCollection<Key,Element,Compare>::_getRecord () const
  {
    Record* record = NULL;
    if (!_elements->empty()) {
      typename Multimap::const_iterator upperbound = _elements->upper_bound(_key);
      record = new Record(_getString());
      typename map<Key,Element,Compare>::const_iterator iterator = _elements->lower_bound(_key);
      while (iterator != upperbound) {
        record->add(getSlot<Element>(getString((*iterator).first), (*iterator).second));
        ++iterator;
      }
    }
    return record;
  }


// -------------------------------------------------------------------
// Functions Templates.


//   template<class Key, class Element, class Compare>
//   inline GenericCollection<Element> getRangeCollection ( const std::multimap<Key,Element,Compare>& elements, Key& key )
//   {
//     return MultimapRangeCollection<Key,Element,Compare>(elements,key);
//   }


  template<class Key, class Element, class Compare>
  inline GenericCollection<Element> getRangeCollection ( const std::multimap<Key,Element,Compare>* elements, const Key& key )
  {
	return MultimapRangeCollection<Key,Element,Compare>(elements,key);
  }


} // End of Hurricane namespace.


#endif  // __HURRICANE_MULTIMAP_COLLECTION__
