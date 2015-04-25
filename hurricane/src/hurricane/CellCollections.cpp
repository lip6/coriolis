// ****************************************************************************************************
// File: ./CellCollections.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#include "hurricane/Slice.h"
#include "hurricane/Cell.h"
//#include "hurricane/MainView.h"
#include "hurricane/HyperNet.h"

namespace Hurricane {

// ****************************************************************************************************
// Cell_Slices implementation
// ****************************************************************************************************

class Cell_Slices : public Collection<Slice*> {
// ******************************************

// Types
// *****

    public: typedef Collection<Slice*> Inherit;

    public: class Locator : public Hurricane::Locator<Slice*> {
    // ******************************************************

        public: typedef Hurricane::Locator<Slice*> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Slice* getElement() const;
        public: virtual Hurricane::Locator<Slice*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_Slices();
    public: Cell_Slices(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_Slices(const Cell_Slices& slices);

// Operators
// *********

    public: Cell_Slices& operator=(const Cell_Slices& slices);

// Accessors
// *********

    public: virtual Collection<Slice*>* getClone() const;
    public: virtual Hurricane::Locator<Slice*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_Components implementation
// ****************************************************************************************************

class Cell_Components : public Collection<Component*> {
// **************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;
        private: ComponentLocator _componentLocator;
        private: Component* _component;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* getElement() const;
        public: virtual Hurricane::Locator<Component*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_Components();
    public: Cell_Components(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_Components(const Cell_Components& components);

// Operators
// *********

    public: Cell_Components& operator=(const Cell_Components& components);

// Accessors
// *********

    public: virtual Collection<Component*>* getClone() const;
    public: virtual Hurricane::Locator<Component*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_ComponentsUnder implementation
// ****************************************************************************************************

class Cell_ComponentsUnder : public Collection<Component*> {
// *******************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: Layer::Mask _mask;
        private: SliceLocator _sliceLocator;
        private: ComponentLocator _componentLocator;
        private: Component* _component;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* getElement() const;
        public: virtual Hurricane::Locator<Component*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentsUnder();
    public: Cell_ComponentsUnder(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
    public: Cell_ComponentsUnder(const Cell_ComponentsUnder& components);

// Operators
// *********

    public: Cell_ComponentsUnder& operator=(const Cell_ComponentsUnder& components);

// Accessors
// *********

    public: virtual Collection<Component*>* getClone() const;
    public: virtual Hurricane::Locator<Component*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};


// -------------------------------------------------------------------
// Class  :  "Hurricane::Cell::Cell_ExtensionSlices".


  class Cell_ExtensionSlices : public Collection<ExtensionSlice*> {

    public:
      class Locator : public Hurricane::Locator<ExtensionSlice*> {
        public:
                                         Locator    ();
                                         Locator    ( const Cell* , ExtensionSlice::Mask mask=~0 );
                                         Locator    ( const Locator& );
                  Locator&               operator=  ( const Locator& );
          virtual ExtensionSlice*        getElement () const;
          virtual Hurricane::Locator<ExtensionSlice*>*
                                         getClone   () const;
          virtual bool                   isValid    () const;
          virtual void                   progress   ();
          virtual string                 _getString () const;
        private:
                  const Cell*            _cell;
                  ExtensionSlice::Mask   _mask;
                  ExtensionSliceLocator  _sliceLocator;

    };

    private:
              const Cell*           _cell;
              ExtensionSlice::Mask  _mask;

    public:
                                    Cell_ExtensionSlices     ();
                                    Cell_ExtensionSlices     ( const Cell* , ExtensionSlice::Mask mask=~0 );
                                    Cell_ExtensionSlices     ( const Cell_ExtensionSlices& );
              Cell_ExtensionSlices& operator=                ( const Cell_ExtensionSlices& );
      virtual Collection<ExtensionSlice*>*         getClone  () const;
      virtual Hurricane::Locator<ExtensionSlice*>* getLocator() const;
      virtual string                _getString               () const;

  };


  Cell_ExtensionSlices::Cell_ExtensionSlices ()
    : Collection<ExtensionSlice*>()
    , _cell(NULL)
    , _mask(0)
  { }


  Cell_ExtensionSlices::Cell_ExtensionSlices ( const Cell* cell, ExtensionSlice::Mask mask )
    : Collection<ExtensionSlice*>()
    , _cell(cell)
    , _mask(mask)
  { }


  Cell_ExtensionSlices::Cell_ExtensionSlices ( const Cell_ExtensionSlices& slices )
    : Collection<ExtensionSlice*>()
    , _cell(slices._cell)
    , _mask(slices._mask)
  { }


  Cell_ExtensionSlices& Cell_ExtensionSlices::operator= ( const Cell_ExtensionSlices& slices )
  {
    _cell = slices._cell;
    _mask = slices._mask;
    return *this;
  }


  Collection<ExtensionSlice*>* Cell_ExtensionSlices::getClone () const
  {
    return new Cell_ExtensionSlices(*this);
  }


  Locator<ExtensionSlice*>* Cell_ExtensionSlices::getLocator () const
  {
    return new Locator(_cell,_mask);
  }


  string Cell_ExtensionSlices::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionSlices");
    if (_cell) {
      s += " " + getString(_cell);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


  Cell_ExtensionSlices::Locator::Locator()
    : Hurricane::Locator<ExtensionSlice*>()
    , _cell(NULL)
    , _mask(0)
    , _sliceLocator()
  { }


  Cell_ExtensionSlices::Locator::Locator ( const Cell* cell, ExtensionSlice::Mask mask )
    : Hurricane::Locator<ExtensionSlice*>()
    , _cell(cell)
    , _mask(mask)
    , _sliceLocator()
  {
    if (_cell && !_mask.zero()) {
      _sliceLocator = getCollection(_cell->getExtensionSliceMap()).getLocator();
      while (_sliceLocator.isValid() && !(_sliceLocator.getElement()->getMask().intersect(_mask)))
        _sliceLocator.progress();
    }
  }


  Cell_ExtensionSlices::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<ExtensionSlice*>()
    , _cell(locator._cell)
    , _mask(locator._mask)
    , _sliceLocator(locator._sliceLocator)
  { }


  Cell_ExtensionSlices::Locator& Cell_ExtensionSlices::Locator::operator= ( const Locator& locator )
  {
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    return *this;
  }


  ExtensionSlice* Cell_ExtensionSlices::Locator::getElement () const
  {
    return _sliceLocator.getElement();
  }


  Locator<ExtensionSlice*>* Cell_ExtensionSlices::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool Cell_ExtensionSlices::Locator::isValid () const
  {
    return _sliceLocator.isValid();
  }


  void Cell_ExtensionSlices::Locator::progress ()
  {
    if (_sliceLocator.isValid()) {
      do {
        _sliceLocator.progress();
      }
      while (_sliceLocator.isValid() && !(_sliceLocator.getElement()->getMask().intersect(_mask)));
    }
  }


  string Cell_ExtensionSlices::Locator::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionSlices::Locator");
    if (_cell) {
      s += " " + getString(_cell);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::Cell::Cell_ExtensionGos".


  class Cell_ExtensionGos : public Collection<Go*> {

    public:
      class Locator : public Hurricane::Locator<Go*> {

        public:
                                           Locator    ();
                                           Locator    ( const Cell* , ExtensionSlice::Mask mask=~0 );
                                           Locator    ( const Locator& );
                  Locator&                 operator=  ( const Locator& );
          virtual Go*                      getElement () const;
          virtual Hurricane::Locator<Go*>* getClone   () const;
          virtual bool                     isValid    () const;
          virtual void                     progress   ();
          virtual string                   _getString () const;
        private:
                  const Cell*              _cell;
                  ExtensionSlice::Mask     _mask;
                  ExtensionSliceLocator    _sliceLocator;
                  GoLocator                _goLocator;
                  Go*                      _go;
      };

    public:
                                       Cell_ExtensionGos ();
                                       Cell_ExtensionGos ( const Cell* , ExtensionSlice::Mask mask=~0 );
                                       Cell_ExtensionGos ( const Cell_ExtensionGos& );
              Cell_ExtensionGos&       operator=         ( const Cell_ExtensionGos& );
      virtual Collection<Go*>*         getClone          () const;
      virtual Hurricane::Locator<Go*>* getLocator        () const;
      virtual string                   _getString        () const;
    private:
              const Cell*              _cell;
              ExtensionSlice::Mask     _mask;
  };


  Cell_ExtensionGos::Cell_ExtensionGos ()
    : Collection<Go*>()
    , _cell(NULL)
    , _mask(0)
  { }


  Cell_ExtensionGos::Cell_ExtensionGos ( const Cell* cell
                                       , ExtensionSlice::Mask mask
                                       )
    : Collection<Go*>()
    , _cell(cell)
    , _mask(mask)
  { }


  Cell_ExtensionGos::Cell_ExtensionGos ( const Cell_ExtensionGos& gos )
    : Collection<Go*>()
    , _cell(gos._cell)
    , _mask(gos._mask)
  { }


  Cell_ExtensionGos& Cell_ExtensionGos::operator= ( const Cell_ExtensionGos& gos )
  { 
    _cell = gos._cell;
    _mask = gos._mask;
    return *this;
  }


  Collection<Go*>* Cell_ExtensionGos::getClone () const
  {
    return new Cell_ExtensionGos(*this);
  }


  Locator<Go*>* Cell_ExtensionGos::getLocator () const
  {
    return new Locator(_cell,_mask);
  }


  string  Cell_ExtensionGos::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionGos");
    if (_cell) {
      s += " " + getString(_cell);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


  Cell_ExtensionGos::Locator::Locator()
    : Hurricane::Locator<Go*>()
    , _cell(NULL)
    , _mask(0)
    , _sliceLocator()
    , _goLocator()
    , _go(NULL)
  { }


  Cell_ExtensionGos::Locator::Locator ( const Cell* cell
                                      , ExtensionSlice::Mask mask
                                      )
    : Hurricane::Locator<Go*>()
    , _cell(cell)
    , _mask(mask)
    , _sliceLocator()
    , _goLocator()
    , _go(NULL)
  {
    if (_cell) {
      _sliceLocator = _cell->getExtensionSlices(_mask).getLocator();
      while (!_go && _sliceLocator.isValid()) {
        ExtensionSlice* slice = _sliceLocator.getElement();
        if (slice) {
          _goLocator = slice->getGos().getLocator();
          if (_goLocator.isValid())
            _go = _goLocator.getElement();
        }
        if (!_go) _sliceLocator.progress();
      }
    }
  }


  Cell_ExtensionGos::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<Go*>()
    , _cell(locator._cell)
    , _mask(locator._mask)
    , _sliceLocator(locator._sliceLocator)
    , _goLocator(locator._goLocator)
    , _go(locator._go)
  { }


  Cell_ExtensionGos::Locator& Cell_ExtensionGos::Locator::operator= ( const Locator& locator )
  {
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _goLocator = locator._goLocator;
    _go = locator._go;
    return *this;
  }


  Go* Cell_ExtensionGos::Locator::getElement () const
  {
    return _go;
  }


  Locator<Go*>* Cell_ExtensionGos::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  Cell_ExtensionGos::Locator::isValid () const
  {
    return (_go != NULL);
  }



  void Cell_ExtensionGos::Locator::progress()
  {
    if (_go) {
      _go = NULL;
      _goLocator.progress();
      if (_goLocator.isValid())
        _go = _goLocator.getElement();
      else {
        do {
          _sliceLocator.progress();
          ExtensionSlice* slice = _sliceLocator.getElement();
          if (slice) {
            _goLocator = slice->getGos().getLocator();
            if (_goLocator.isValid())
              _go = _goLocator.getElement();
          }
        } while (!_go && _sliceLocator.isValid());
      }
    }
  }


  string  Cell_ExtensionGos::Locator::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionGos::Locator");
    if (_cell) {
      s += " " + getString(_cell);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::Cell::Cell_ExtensionGosUnder".


  class Cell_ExtensionGosUnder : public Collection<Go*> {

    public:
      class Locator : public Hurricane::Locator<Go*> {

        public:
                                           Locator    ();
                                           Locator    ( const Cell* , const Box& , ExtensionSlice::Mask mask=~0 );
                                           Locator    ( const Locator& );
                  Locator&                 operator=  ( const Locator& );
          virtual Go*                      getElement () const;
          virtual Hurricane::Locator<Go*>* getClone   () const;
          virtual bool                     isValid    () const;
          virtual void                     progress   ();
          virtual string                   _getString () const;
        private:
                  const Cell*              _cell;
                  Box                      _area;
                  ExtensionSlice::Mask     _mask;
                  ExtensionSliceLocator    _sliceLocator;
                  GoLocator                _goLocator;
                  Go*                      _go;
      };

    public:
                                       Cell_ExtensionGosUnder ();
                                       Cell_ExtensionGosUnder ( const Cell* , const Box& , ExtensionSlice::Mask mask=~0 );
                                       Cell_ExtensionGosUnder ( const Cell_ExtensionGosUnder& );
              Cell_ExtensionGosUnder&  operator=              ( const Cell_ExtensionGosUnder& );
      virtual Collection<Go*>*         getClone               () const;
      virtual Hurricane::Locator<Go*>* getLocator             () const;
      virtual string                   _getString             () const;
    private:
              const Cell*              _cell;
              Box                      _area;
              ExtensionSlice::Mask     _mask;
  };


  Cell_ExtensionGosUnder::Cell_ExtensionGosUnder ()
    : Collection<Go*>()
    , _cell(NULL)
    , _area()
    , _mask(0)
  { }


  Cell_ExtensionGosUnder::Cell_ExtensionGosUnder ( const Cell* cell
                                                 , const Box& area
                                                 , ExtensionSlice::Mask mask
                                                 )
    : Collection<Go*>()
    , _cell(cell)
    , _area(area)
    , _mask(mask)
  { }


  Cell_ExtensionGosUnder::Cell_ExtensionGosUnder ( const Cell_ExtensionGosUnder& gos )
    : Collection<Go*>()
    , _cell(gos._cell)
    , _area(gos._area)
    , _mask(gos._mask)
  { }


  Cell_ExtensionGosUnder& Cell_ExtensionGosUnder::operator= ( const Cell_ExtensionGosUnder& gos )
  { 
    _cell = gos._cell;
    _area = gos._area;
    _mask = gos._mask;
    return *this;
  }


  Collection<Go*>* Cell_ExtensionGosUnder::getClone () const
  {
    return new Cell_ExtensionGosUnder(*this);
  }


  Locator<Go*>* Cell_ExtensionGosUnder::getLocator () const
  {
    return new Locator(_cell,_area,_mask);
  }


  string  Cell_ExtensionGosUnder::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionGosUnder");
    if (_cell) {
      s += " " + getString(_cell);
      s += " " + getString(_area);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


  Cell_ExtensionGosUnder::Locator::Locator()
    : Hurricane::Locator<Go*>()
    , _cell(NULL)
    , _area()
    , _mask(0)
    , _sliceLocator()
    , _goLocator()
    , _go(NULL)
  { }


  Cell_ExtensionGosUnder::Locator::Locator ( const Cell* cell
                                           , const Box& area
                                           , ExtensionSlice::Mask mask
                                           )
    : Hurricane::Locator<Go*>()
    , _cell(cell)
    , _area(area)
    , _mask(mask)
    , _sliceLocator()
    , _goLocator()
    , _go(NULL)
  {
    if (_cell && !_area.isEmpty()) {
      _sliceLocator = _cell->getExtensionSlices(_mask).getLocator();
      while (!_go && _sliceLocator.isValid()) {
        ExtensionSlice* slice = _sliceLocator.getElement();
        if (slice) {
          _goLocator = slice->getGosUnder(_area).getLocator();
          if (_goLocator.isValid())
            _go = _goLocator.getElement();
        }
        if (!_go) _sliceLocator.progress();
      }
    }
  }


  Cell_ExtensionGosUnder::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<Go*>()
    , _cell(locator._cell)
    , _area(locator._area)
    , _mask(locator._mask)
    , _sliceLocator(locator._sliceLocator)
    , _goLocator(locator._goLocator)
    , _go(locator._go)
  { }


  Cell_ExtensionGosUnder::Locator& Cell_ExtensionGosUnder::Locator::operator= ( const Locator& locator )
  {
    _cell = locator._cell;
    _area = locator._area;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _goLocator = locator._goLocator;
    _go = locator._go;
    return *this;
  }


  Go* Cell_ExtensionGosUnder::Locator::getElement () const
  {
    return _go;
  }


  Locator<Go*>* Cell_ExtensionGosUnder::Locator::getClone () const
  {
    return new Locator(*this);
  }


  bool  Cell_ExtensionGosUnder::Locator::isValid () const
  {
    return (_go != NULL);
  }



  void Cell_ExtensionGosUnder::Locator::progress()
  {
    if (_go) {
      _go = NULL;
      _goLocator.progress();
      if (_goLocator.isValid())
        _go = _goLocator.getElement();
      else {
        do {
          _sliceLocator.progress();
          ExtensionSlice* slice = _sliceLocator.getElement();
          if (slice) {
            _goLocator = slice->getGosUnder(_area).getLocator();
            if (_goLocator.isValid())
              _go = _goLocator.getElement();
          }
        } while (!_go && _sliceLocator.isValid());
      }
    }
  }


  string  Cell_ExtensionGosUnder::Locator::_getString () const
  {
    string s = "<" + _TName("Cell::ExtensionGosUnder::Locator");
    if (_cell) {
      s += " " + getString(_cell);
      s += " " + getString(_area);
    //s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


// ****************************************************************************************************
// Cell_Occurrences implementation
// ****************************************************************************************************

class Cell_Occurrences : public Collection<Occurrence> {
// *************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: unsigned _searchDepth;
        private: unsigned _state;
        private: ComponentLocator _componentLocator;
        private: RubberLocator _rubberLocator;
        private: MarkerLocator _markerLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, unsigned searchDepth);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: unsigned _searchDepth;

// Constructors
// ************

    public: Cell_Occurrences();
    public: Cell_Occurrences(const Cell* cell, unsigned searchDepth);
    public: Cell_Occurrences(const Cell_Occurrences& occurrences);

// Operators
// *********

    public: Cell_Occurrences& operator=(const Cell_Occurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_OccurrencesUnder implementation
// ****************************************************************************************************

class Cell_OccurrencesUnder : public Collection<Occurrence> {
// ******************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: unsigned _searchDepth;
        private: unsigned _state;
        private: ComponentLocator _componentLocator;
        private: RubberLocator _rubberLocator;
        private: MarkerLocator _markerLocator;
        private: GoLocator _extensionGoLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, unsigned searchDepth = (unsigned)-1);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: unsigned _searchDepth;

// Constructors
// ************

    public: Cell_OccurrencesUnder();
    public: Cell_OccurrencesUnder(const Cell* cell, const Box& area, unsigned searchDepth = (unsigned)-1);
    public: Cell_OccurrencesUnder(const Cell_OccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_OccurrencesUnder& operator=(const Cell_OccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};

// ****************************************************************************************************
// Cell_LeafInstanceOccurrences declaration
// ****************************************************************************************************

class Cell_LeafInstanceOccurrences : public Collection<Occurrence> {
// *****************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: int _state;
        private: InstanceLocator _leafInstanceLocator;
        private: InstanceLocator _nonLeafInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator(const Cell* cell = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_LeafInstanceOccurrences(const Cell* cell = NULL);
    public: Cell_LeafInstanceOccurrences(const Cell_LeafInstanceOccurrences& occurrences);

// Operators
// *********

    public: Cell_LeafInstanceOccurrences& operator=(const Cell_LeafInstanceOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder declaration
// ****************************************************************************************************

class Cell_LeafInstanceOccurrencesUnder : public Collection<Occurrence> {
// **********************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: int _state;
        private: InstanceLocator _leafInstanceLocator;
        private: InstanceLocator _nonLeafInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;

// Constructors
// ************

    public: Cell_LeafInstanceOccurrencesUnder();
    public: Cell_LeafInstanceOccurrencesUnder(const Cell* cell, const Box& area);
    public: Cell_LeafInstanceOccurrencesUnder(const Cell_LeafInstanceOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_LeafInstanceOccurrencesUnder& operator=(const Cell_LeafInstanceOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};

// ****************************************************************************************************
// Cell_NonLeafInstanceOccurrences declaration
// ****************************************************************************************************

class Cell_NonLeafInstanceOccurrences : public Collection<Occurrence> {
// ********************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // ************************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: int _state;
        private: InstanceLocator _nonLeafInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator(const Cell* cell = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_NonLeafInstanceOccurrences(const Cell* cell = NULL);
    public: Cell_NonLeafInstanceOccurrences(const Cell_NonLeafInstanceOccurrences& occurrences);

// Operators
// *********

    public: Cell_NonLeafInstanceOccurrences& operator=(const Cell_NonLeafInstanceOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences declaration
// ****************************************************************************************************

class Cell_TerminalInstanceOccurrences : public Collection<Occurrence> {
// *****************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: int _state;
        private: InstanceLocator _terminalInstanceLocator;
        private: InstanceLocator _nonTerminalInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator(const Cell* cell = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_TerminalInstanceOccurrences(const Cell* cell = NULL);
    public: Cell_TerminalInstanceOccurrences(const Cell_TerminalInstanceOccurrences& occurrences);

// Operators
// *********

    public: Cell_TerminalInstanceOccurrences& operator=(const Cell_TerminalInstanceOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder declaration
// ****************************************************************************************************

class Cell_TerminalInstanceOccurrencesUnder : public Collection<Occurrence> {
// **********************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: int _state;
        private: InstanceLocator _terminalInstanceLocator;
        private: InstanceLocator _nonTerminalInstanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;

// Constructors
// ************

    public: Cell_TerminalInstanceOccurrencesUnder();
    public: Cell_TerminalInstanceOccurrencesUnder(const Cell* cell, const Box& area);
    public: Cell_TerminalInstanceOccurrencesUnder(const Cell_TerminalInstanceOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_TerminalInstanceOccurrencesUnder& operator=(const Cell_TerminalInstanceOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_ComponentOccurrences implementation
// ****************************************************************************************************

class Cell_ComponentOccurrences : public Collection<Occurrence> {
// **********************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Layer::Mask _mask;
        private: int _state;
        private: ComponentLocator _componentLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentOccurrences();
    public: Cell_ComponentOccurrences(const Cell* cell, const Layer::Mask& mask = ~0);
    public: Cell_ComponentOccurrences(const Cell_ComponentOccurrences& occurrences);

// Operators
// *********

    public: Cell_ComponentOccurrences& operator=(const Cell_ComponentOccurrences& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder implementation
// ****************************************************************************************************

class Cell_ComponentOccurrencesUnder : public Collection<Occurrence> {
// ***************************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell* _cell;
        private: Box _area;
        private: Layer::Mask _mask;
        private: int _state;
        private: ComponentLocator _componentLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _occurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Box _area;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Cell_ComponentOccurrencesUnder();
    public: Cell_ComponentOccurrencesUnder(const Cell* cell, const Box& area, const Layer::Mask& mask = ~0);
    public: Cell_ComponentOccurrencesUnder(const Cell_ComponentOccurrencesUnder& occurrences);

// Operators
// *********

    public: Cell_ComponentOccurrencesUnder& operator=(const Cell_ComponentOccurrencesUnder& occurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};

// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences implementation
// ****************************************************************************************************

class Cell_HyperNetRootNetOccurrences : public Collection<Occurrence> {
// ****************************************************

// Types
// *****

    public: typedef Collection<Occurrence> Inherit;

    public: class Locator : public Hurricane::Locator<Occurrence> {
    // *********************************************************

        public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: Path _path;
        private: NetLocator _netLocator;
        private: InstanceLocator _instanceLocator;
        private: OccurrenceLocator _hyperNetRootNetOccurrenceLocator;

        public: Locator();
        public: Locator(const Cell* cell, const Path path);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Occurrence getElement() const;
        public: virtual Hurricane::Locator<Occurrence>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;
    private: Path _path;

// Constructors
// ************

    public: Cell_HyperNetRootNetOccurrences();
    public: Cell_HyperNetRootNetOccurrences(const Cell* cell, const Path path);
    public: Cell_HyperNetRootNetOccurrences(const Cell_HyperNetRootNetOccurrences& netOccurrences);

// Operators
// *********

    public: Cell_HyperNetRootNetOccurrences& operator=(const Cell_HyperNetRootNetOccurrences& HyperNetRootNetOccurrences);

// Accessors
// *********

    public: virtual Collection<Occurrence>* getClone() const;
    public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};


//// ****************************************************************************************************
//// Cell_ImpactedMainViews implementation
//// ****************************************************************************************************
//
//class Cell_ImpactedMainViews : public Collection<MainView*> {
//// ********************************************************
//
//// Types
//// *****
//
//    public: typedef Collection<MainView*> Inherit;
//
//    public: class Locator : public Hurricane::Locator<MainView*> {
//    // *********************************************************
//
//        public: typedef Hurricane::Locator<MainView*> Inherit;
//
//        private: const Cell* _cell;
//        private: set<Cell*> _cellSet;
//        private: stack<Cell*> _cellStack;
//        private: MainViewLocator _mainViewLocator;
//        private: MainView* _mainView;
//
//        public: Locator(const Cell* cell = NULL);
//        public: Locator(const Locator& locator);
//
//        public: Locator& operator=(const Locator& locator);
//
//        public: virtual MainView* getElement() const;
//        public: virtual Hurricane::Locator<MainView*>* getClone() const;
//
//        public: virtual bool isValid() const;
//
//        public: virtual void progress();
//
//        public: virtual string _getString() const;
//
//    };
//
//// Attributes
//// **********
//
//    private: const Cell* _cell;
//
//// Constructors
//// ************
//
//    public: Cell_ImpactedMainViews(const Cell* cell = NULL);
//    public: Cell_ImpactedMainViews(const Cell_ImpactedMainViews& impactedMainViews);
//
//// Operators
//// *********
//
//    public: Cell_ImpactedMainViews& operator=(const Cell_ImpactedMainViews& impactedMainViews);
//
//// Accessors
//// *********
//
//    public: virtual Collection<MainView*>* getClone() const;
//    public: virtual Hurricane::Locator<MainView*>* getLocator() const;
//
//// Others
//// ******
//
//    public: virtual string _getString() const;
//
//};
//
// ****************************************************************************************************
// Cell_SubCells declaration
// ****************************************************************************************************

class Cell_SubCells : public Collection<Cell*> {
// *********************************************

// Types
// *****

    public: typedef Collection<Cell*> Inherit;

    public: class Locator : public Hurricane::Locator<Cell*> {
    // *******************************************************

        public: typedef Hurricane::Locator<Cell*> Inherit;

        private: set <Cell*> _cellSet;
        private: InstanceLocator _instanceLocator;

        public: Locator();
        public: Locator(InstanceLocator instanceLocator);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Cell* getElement() const;
        public: virtual Hurricane::Locator<Cell*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Cell* _cell;

// Constructors
// ************

    public: Cell_SubCells();
    public: Cell_SubCells(const Cell* cell);
    public: Cell_SubCells(const Cell_SubCells& subCells);

// Operators
// *********

    public: Cell_SubCells& operator=(const Cell_SubCells& subCells);

// Accessors
// *********

    public: virtual Collection<Cell*>* getClone() const;
    public: virtual Hurricane::Locator<Cell*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};




// -------------------------------------------------------------------
// Class  :  "Cell_RecursiveSlavePathes".

  class Cell_RecursiveSlavePathes : public Collection<Path> {

    // Locator Sub-Class.
    public:
      class Locator : public Hurricane::Locator<Path> {
        // Attributes.
        protected:
          vector<InstanceLocator>  _instancesStack;
          Path                     _instancePath;
          bool                     _isInPop;
        // Constructors.
        public:
          Locator ( const Cell*    cell     );
          Locator ( const Locator  &locator ) : Hurricane::Locator<Path>()
                                              , _instancesStack(locator._instancesStack)
                                              , _instancePath(locator._instancePath)
                                              , _isInPop(locator._isInPop)
                                              { };
        // Accessors.
        public:
          virtual Path                      getElement () const { return _instancePath; };
          virtual Hurricane::Locator<Path>* getClone   () const { return new Locator(*this); };
          virtual bool                      isValid    () const { return _instancesStack.size(); };
          virtual void                      progress   ();
        // Hurricane Managment.
        public:
          virtual string _getString() const;
      };

    // Attributes.
    public:
      const Cell* _cell;

    // Constructors.
    public:
      Cell_RecursiveSlavePathes ( const Cell* cell )
                                : Collection<Path>()
                                , _cell(cell)
                                { };
      Cell_RecursiveSlavePathes ( const Cell_RecursiveSlavePathes& pathes )
                                : Collection<Path>()
                                , _cell(pathes._cell)
                                { };
      
    // Accessors.
    public:
      virtual Collection<Path>*         getClone   () const { return new Cell_RecursiveSlavePathes(*this); };
      virtual Hurricane::Locator<Path>* getLocator () const { return new Locator(_cell); };

    // Hurricane Managment.
    public:
      virtual string _getString() const;

  };




// ****************************************************************************************************
// Cell further definition
// ****************************************************************************************************

Instances Cell::getInstancesUnder(const Box& area) const
// *****************************************************
{
    // return _quadTree.getGosUnder(area).getSubSet<Instance*>();
    return SubTypeCollection<Go*, Instance*>(_quadTree.getGosUnder(area));
}

Instances Cell::getSlaveInstances() const
// **************************************
{
    return _slaveInstanceSet.getElements();
}

Instances Cell::getTerminalInstances() const
// *****************************************
{
    return getInstances().getSubSet(Instance::getIsTerminalFilter());
}

Instances Cell::getLeafInstances() const
// *************************************
{
    return getInstances().getSubSet(Instance::getIsLeafFilter());
}

Instances Cell::getPlacedInstances() const
// ***************************************
{
    return getInstances().getSubSet(Instance::getIsPlacedFilter());
}

Instances Cell::getFixedInstances() const
// **************************************
{
    return getInstances().getSubSet(Instance::getIsFixedFilter());
}

Instances Cell::getUnplacedInstances() const
// **************************************
{
    return getInstances().getSubSet(Instance::getIsUnplacedFilter());
}

Instances Cell::getNotUnplacedInstances() const
// **************************************
{
    return getInstances().getSubSet(Instance::getIsNotUnplacedFilter());
}

Instances Cell::getPlacedInstancesUnder(const Box& area) const
// ***********************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsPlacedFilter());
}

Instances Cell::getFixedInstancesUnder(const Box& area) const
// **********************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsFixedFilter());
}

Instances Cell::getUnplacedInstancesUnder(const Box& area) const
// *************************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsUnplacedFilter());
}

Instances Cell::getNotUnplacedInstancesUnder(const Box& area) const
// ****************************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsNotUnplacedFilter());
}

Instances Cell::getTerminalInstancesUnder(const Box& area) const
// *************************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsTerminalFilter());
}

Instances Cell::getNonTerminalInstances() const
// ********************************************
{
    return getInstances().getSubSet(!Instance::getIsTerminalFilter());
}

Instances Cell::getNonTerminalInstancesUnder(const Box& area) const
// ****************************************************************
{
    return getInstancesUnder(area).getSubSet(!Instance::getIsTerminalFilter());
}

Instances Cell::getLeafInstancesUnder(const Box& area) const
// *********************************************************
{
    return getInstancesUnder(area).getSubSet(Instance::getIsLeafFilter());
}

Instances Cell::getNonLeafInstances() const
// ****************************************
{
    return getInstances().getSubSet(!Instance::getIsLeafFilter());
}

Instances Cell::getNonLeafInstancesUnder(const Box& area) const
// ************************************************************
{
    return getInstancesUnder(area).getSubSet(!Instance::getIsLeafFilter());
}

Nets Cell::getGlobalNets() const
// *****************************
{
    return getNets().getSubSet(Net::getIsGlobalFilter());
}

Nets Cell::getExternalNets() const
// *******************************
{
    return getNets().getSubSet(Net::getIsExternalFilter());
}

Nets Cell::getInternalNets() const
// *******************************
{
    return getNets().getSubSet(Net::getIsInternalFilter());
}

Nets Cell::getClockNets() const
// ****************************
{
    return getNets().getSubSet(Net::getIsClockFilter());
}

Nets Cell::getSupplyNets() const
// *****************************
{
    return getNets().getSubSet(Net::getIsSupplyFilter());
}

Nets Cell::getPowerNets() const
// *****************************
{
    return getNets().getSubSet(Net::getIsPowerFilter());
}

Nets Cell::getGroundNets() const
// *****************************
{
    return getNets().getSubSet(Net::getIsGroundFilter());
}

Slices Cell::getSlices(const Layer::Mask& mask) const
// **************************************************
{
    return Cell_Slices(this, mask);
}

ExtensionSlices Cell::getExtensionSlices(ExtensionSlice::Mask mask) const
// *************************************************************
{
    return Cell_ExtensionSlices(this,mask);
}

//MainViews Cell::getMainViews() const
//// *********************************
//{
//    // return getViews().getSubSet<MainView*>();
//    return SubTypeCollection<View*, MainView*>(getViews());
//}
//
//MainViews Cell::getImpactedMainViews() const
//// *****************************************
//{
//    return Cell_ImpactedMainViews(this);
//}
//
Rubbers Cell::getRubbers() const
// *****************************
{
    // return _quadTree.getGos().getSubSet<Rubber*>();
    return SubTypeCollection<Go*, Rubber*>(_quadTree.getGos());
}

Rubbers Cell::getRubbersUnder(const Box& area) const
// *************************************************
{
    // return (area.isEmpty()) ? Rubbers() : _quadTree.getGosUnder(area).getSubSet<Rubber*>();
    return SubTypeCollection<Go*, Rubber*>(_quadTree.getGosUnder(area));
}

Markers Cell::getMarkersUnder(const Box& area) const
// *************************************************
{
    // return (area.isEmpty()) ? Markers() : _quadTree.getGosUnder(area).getSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.getGosUnder(area));
}

References Cell::getReferences() const
// **********************************
{
    return SubTypeCollection<Marker*, Reference*>(getMarkers());
}

Cells Cell::getSubCells() const
// ****************************
{
    return Cell_SubCells(this);
}

Cells Cell::getClonedCells() const
// *******************************
{
    return ClonedSet(this);
}

Components Cell::getComponents(const Layer::Mask& mask) const
// **********************************************************
{
    return Cell_Components(this, mask);
}

Components Cell::getComponentsUnder(const Box& area, const Layer::Mask& mask) const
// ********************************************************************************
{
    return Cell_ComponentsUnder(this, area, mask);
}

Occurrences Cell::getOccurrences(unsigned searchDepth) const
// *******************************************************
{
    return Cell_Occurrences(this, searchDepth);
}

Occurrences Cell::getOccurrencesUnder(const Box& area, unsigned searchDepth) const
// *****************************************************************************
{
    return Cell_OccurrencesUnder(this, area, searchDepth);
}

Occurrences Cell::getTerminalInstanceOccurrences() const
// ***************************************************
{
    return Cell_TerminalInstanceOccurrences(this);
}

Occurrences Cell::getTerminalInstanceOccurrencesUnder(const Box& area) const
// ***********************************************************************
{
    return Cell_TerminalInstanceOccurrencesUnder(this, area);
}

Occurrences Cell::getLeafInstanceOccurrences() const
// ***********************************************
{
    return Cell_LeafInstanceOccurrences(this);
}

Occurrences Cell::getLeafInstanceOccurrencesUnder(const Box& area) const
// *******************************************************************
{
    return Cell_LeafInstanceOccurrencesUnder(this, area);
}

Occurrences Cell::getNonLeafInstanceOccurrences() const
// ***********************************************
{
    return Cell_NonLeafInstanceOccurrences(this);
}

Occurrences Cell::getComponentOccurrences(const Layer::Mask& mask) const
// *******************************************************************
{
    return Cell_ComponentOccurrences(this, mask);
}

Occurrences Cell::getComponentOccurrencesUnder(const Box& area, const Layer::Mask& mask) const
// *****************************************************************************************
{
    return Cell_ComponentOccurrencesUnder(this, area, mask);
}

Pathes Cell::getRecursiveSlavePathes() const
// *****************************************
{
  return Cell_RecursiveSlavePathes(this);
}

Occurrences Cell::getHyperNetRootNetOccurrences() const
// ****************************************************
{
    return Cell_HyperNetRootNetOccurrences(this,Path());
}

Gos Cell::getExtensionGos ( const Name& name ) const
// **********************************************************
{
  return Cell_ExtensionGos(this,getExtensionSliceMask(name));
}

Gos Cell::getExtensionGos ( ExtensionSlice::Mask mask ) const
// ****************************************************************
{
  return Cell_ExtensionGos(this,mask);
}

Gos Cell::getExtensionGosUnder ( const Box& area, const Name& name ) const
// ********************************************************************************
{
  return Cell_ExtensionGosUnder(this,area,getExtensionSliceMask(name));
}

Gos Cell::getExtensionGosUnder ( const Box& area, ExtensionSlice::Mask mask ) const
// **************************************************************************************
{
  return Cell_ExtensionGosUnder(this,area,mask);
}

// ****************************************************************************************************
// Cell_Slices implementation
// ****************************************************************************************************

Cell_Slices::Cell_Slices()
// ***********************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_Slices::Cell_Slices(const Cell* cell, const Layer::Mask& mask)
// ****************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_Slices::Cell_Slices(const Cell_Slices& slices)
// ************************************************
:     Inherit(),
    _cell(slices._cell),
    _mask(slices._mask)
{
}

Cell_Slices& Cell_Slices::operator=(const Cell_Slices& slices)
// ***********************************************************
{
    _cell = slices._cell;
    _mask = slices._mask;
    return *this;
}

Collection<Slice*>* Cell_Slices::getClone() const
// **********************************************
{
    return new Cell_Slices(*this);
}

Locator<Slice*>* Cell_Slices::getLocator() const
// *********************************************
{
    return new Locator(_cell, _mask);
}

string Cell_Slices::_getString() const
// ***********************************
{
    string s = "<" + _TName("Cell::Slices");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Slices::Locator implementation
// ****************************************************************************************************

Cell_Slices::Locator::Locator()
// ****************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _sliceLocator()
{
}

Cell_Slices::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// *********************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _sliceLocator()
{
  if (_cell && !_mask.zero()) {
        _sliceLocator = ((Cell*)_cell)->_getSliceMap().getElements().getLocator();
        while (_sliceLocator.isValid() && !(_sliceLocator.getElement()->getLayer()->getMask() & _mask))
            _sliceLocator.progress();
    }
}

Cell_Slices::Locator::Locator(const Locator& locator)
// **************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator)
{
}

Cell_Slices::Locator& Cell_Slices::Locator::operator=(const Locator& locator)
// **************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    return *this;
}

Slice* Cell_Slices::Locator::getElement() const
// ********************************************
{
    return _sliceLocator.getElement();
}

Locator<Slice*>* Cell_Slices::Locator::getClone() const
// ****************************************************
{
    return new Locator(*this);
}

bool Cell_Slices::Locator::isValid() const
// ***************************************
{
    return _sliceLocator.isValid();
}

void Cell_Slices::Locator::progress()
// **********************************
{
    if (_sliceLocator.isValid()) {
        do {
            _sliceLocator.progress();
        }
        while (_sliceLocator.isValid() && !(_sliceLocator.getElement()->getLayer()->getMask() & _mask));
    }
}

string Cell_Slices::Locator::_getString() const
// ********************************************
{
    string s = "<" + _TName("Cell::Slices::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Components implementation
// ****************************************************************************************************

Cell_Components::Cell_Components()
// *******************************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_Components::Cell_Components(const Cell* cell, const Layer::Mask& mask)
// ************************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_Components::Cell_Components(const Cell_Components& components)
// ****************************************************************
:     Inherit(),
    _cell(components._cell),
    _mask(components._mask)
{
}

Cell_Components& Cell_Components::operator=(const Cell_Components& components)
// ***************************************************************************
{
    _cell = components._cell;
    _mask = components._mask;
    return *this;
}

Collection<Component*>* Cell_Components::getClone() const
// ******************************************************
{
    return new Cell_Components(*this);
}

Locator<Component*>* Cell_Components::getLocator() const
// *****************************************************
{
    return new Locator(_cell, _mask);
}

string Cell_Components::_getString() const
// ***************************************
{
    string s = "<" + _TName("Cell::Components");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Components::Locator implementation
// ****************************************************************************************************

Cell_Components::Locator::Locator()
// ********************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
}

Cell_Components::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// *************************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
   if (_cell && !_mask.zero() ) {
        _sliceLocator = _cell->getSlices(_mask).getLocator();
        while (!_component && _sliceLocator.isValid()) {
            Slice* slice = _sliceLocator.getElement();
            if (slice) {
                _componentLocator = slice->getComponents().getLocator();
                while (!_component && _componentLocator.isValid()) {
                    _component = _componentLocator.getElement();
                    if (!_component) _componentLocator.progress();
                }
            }
            if (!_component) _sliceLocator.progress();
        }
    }
}

Cell_Components::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator),
    _componentLocator(locator._componentLocator),
    _component(locator._component)
{
}

Cell_Components::Locator& Cell_Components::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _componentLocator = locator._componentLocator;
    _component = locator._component;
    return *this;
}

Component* Cell_Components::Locator::getElement() const
// ****************************************************
{
    return _component;
}

Locator<Component*>* Cell_Components::Locator::getClone() const
// ************************************************************
{
    return new Locator(*this);
}

bool Cell_Components::Locator::isValid() const
// *******************************************
{
    return (_component != NULL);
}

void Cell_Components::Locator::progress()
// **************************************
{
    if (_component) {
        _component = NULL;
        do {
            _componentLocator.progress();
            _component = _componentLocator.getElement();
        }
        while (!_component && _componentLocator.isValid());
        if (!_component) {
            do {
                _sliceLocator.progress();
                Slice* slice = _sliceLocator.getElement();
                if (slice) {
                    _componentLocator = slice->getComponents().getLocator();
                    while (!_component && _componentLocator.isValid()) {
                        _component = _componentLocator.getElement();
                        if (!_component) _componentLocator.progress();
                    }
                }
            }
            while (!_component && _sliceLocator.isValid());
        }
    }
}

string Cell_Components::Locator::_getString() const
// ************************************************
{
    string s = "<" + _TName("Cell::Components::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentsUnder implementation
// ****************************************************************************************************

Cell_ComponentsUnder::Cell_ComponentsUnder()
// *****************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _mask(0)
{
}

Cell_ComponentsUnder::Cell_ComponentsUnder(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ***************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask)
{
}

Cell_ComponentsUnder::Cell_ComponentsUnder(const Cell_ComponentsUnder& components)
// *******************************************************************************
:     Inherit(),
    _cell(components._cell),
    _area(components._area),
    _mask(components._mask)
{
}

Cell_ComponentsUnder& Cell_ComponentsUnder::operator=(const Cell_ComponentsUnder& components)
// ******************************************************************************************
{
    _cell = components._cell;
    _area = components._area;
    _mask = components._mask;
    return *this;
}

Collection<Component*>* Cell_ComponentsUnder::getClone() const
// ***********************************************************
{
    return new Cell_ComponentsUnder(*this);
}

Locator<Component*>* Cell_ComponentsUnder::getLocator() const
// **********************************************************
{
    return new Locator(_cell, _area, _mask);
}

string Cell_ComponentsUnder::_getString() const
// ********************************************
{
    string s = "<" + _TName("Cell::ComponentsUnder");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentsUnder::Locator implementation
// ****************************************************************************************************

Cell_ComponentsUnder::Locator::Locator()
// *************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _mask(0),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
}

Cell_ComponentsUnder::Locator::Locator(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ***********************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask),
    _sliceLocator(),
    _componentLocator(),
    _component(NULL)
{
    if (_cell && !_area.isEmpty()) {
        _sliceLocator = _cell->getSlices(_mask).getLocator();
        while (!_component && _sliceLocator.isValid()) {
            Slice* slice = _sliceLocator.getElement();
            if (slice) {
                _componentLocator = slice->getComponentsUnder(_area).getLocator();
                if (_componentLocator.isValid())
                    _component = _componentLocator.getElement();
            }
            if (!_component) _sliceLocator.progress();
        }
    }
}

Cell_ComponentsUnder::Locator::Locator(const Locator& locator)
// ***********************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _mask(locator._mask),
    _sliceLocator(locator._sliceLocator),
    _componentLocator(locator._componentLocator),
    _component(locator._component)
{
}

Cell_ComponentsUnder::Locator& Cell_ComponentsUnder::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _mask = locator._mask;
    _sliceLocator = locator._sliceLocator;
    _componentLocator = locator._componentLocator;
    _component = locator._component;
    return *this;
}

Component* Cell_ComponentsUnder::Locator::getElement() const
// *********************************************************
{
    return _component;
}

Locator<Component*>* Cell_ComponentsUnder::Locator::getClone() const
// *****************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentsUnder::Locator::isValid() const
// ************************************************
{
    return (_component != NULL);
}

void Cell_ComponentsUnder::Locator::progress()
// *******************************************
{
    if (_component) {
        _component = NULL;
        _componentLocator.progress();
        if (_componentLocator.isValid())
            _component = _componentLocator.getElement();
        else {
            do {
                _sliceLocator.progress();
                Slice* slice = _sliceLocator.getElement();
                if (slice) {
                    _componentLocator = slice->getComponentsUnder(_area).getLocator();
                    if (_componentLocator.isValid())
                        _component = _componentLocator.getElement();
                }
            } while (!_component && _sliceLocator.isValid());
        }
    }
}

string Cell_ComponentsUnder::Locator::_getString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::ComponentsUnder::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Occurrences implementation
// ****************************************************************************************************

Cell_Occurrences::Cell_Occurrences()
// *******************************
:     Inherit(),
    _cell(NULL),
    _searchDepth(0)
{
}

Cell_Occurrences::Cell_Occurrences(const Cell* cell, unsigned searchDepth)
// *********************************************************************
:     Inherit(),
    _cell(cell),
    _searchDepth(searchDepth)
{
}

Cell_Occurrences::Cell_Occurrences(const Cell_Occurrences& occurrences)
// ****************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _searchDepth(occurrences._searchDepth)
{
}

Cell_Occurrences& Cell_Occurrences::operator=(const Cell_Occurrences& occurrences)
// ***************************************************************************
{
    _cell = occurrences._cell;
    _searchDepth = occurrences._searchDepth;
    return *this;
}

Collection<Occurrence>* Cell_Occurrences::getClone() const
// *****************************************************
{
    return new Cell_Occurrences(*this);
}

Locator<Occurrence>* Cell_Occurrences::getLocator() const
// ****************************************************
{
    return new Locator(_cell, _searchDepth);
}

string Cell_Occurrences::_getString() const
// ***************************************
{
    string s = "<" + _TName("Cell::Occurrences");
    if (_cell) {
        s += " " + getString(_cell);
        if (_searchDepth != ((unsigned)-1)) s += " " + getString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_Occurrences::Locator implementation
// ****************************************************************************************************

Cell_Occurrences::Locator::Locator()
// ********************************
:    Inherit(),
    _cell(NULL),
    _searchDepth(0),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_Occurrences::Locator::Locator(const Cell* cell, unsigned searchDepth)
// **********************************************************************
:    Inherit(),
    _cell(cell),
    _searchDepth(searchDepth),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _componentLocator = _cell->getComponents().getLocator();
        if (_componentLocator.isValid())
            _state = 1;
        else {
            _rubberLocator = _cell->getRubbers().getLocator();
            if (_rubberLocator.isValid())
                _state = 2;
            else {
                _markerLocator = _cell->getMarkers().getLocator();
                if (_markerLocator.isValid())
                    _state = 3;
                else {
                    _instanceLocator = _cell->getInstances().getLocator();
                    if (_instanceLocator.isValid())
                        _state = 4;
                }
            }
        }
    }
}

Cell_Occurrences::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _cell(locator._cell),
    _searchDepth(locator._searchDepth),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _rubberLocator(locator._rubberLocator),
    _markerLocator(locator._markerLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_Occurrences::Locator& Cell_Occurrences::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _cell = locator._cell;
    _searchDepth = locator._searchDepth;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _rubberLocator = locator._rubberLocator;
    _markerLocator = locator._markerLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_Occurrences::Locator::getElement() const
// ***************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.getElement());
            case 2 : return Occurrence(_rubberLocator.getElement());
            case 3 : return Occurrence(_markerLocator.getElement());
            case 4 : return Occurrence(_instanceLocator.getElement());
            case 5 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_instanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_Occurrences::Locator::getClone() const
// ***********************************************************
{
    return new Locator(*this);
}

bool Cell_Occurrences::Locator::isValid() const
// *******************************************
{
    return (_state != 0);
}

void Cell_Occurrences::Locator::progress()
// **************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.progress();
                if (!_componentLocator.isValid()) {
                    _rubberLocator = _cell->getRubbers().getLocator();
                    if (_rubberLocator.isValid())
                        _state = 2;
                    else {
                        _markerLocator = _cell->getMarkers().getLocator();
                        if (_markerLocator.isValid())
                            _state = 3;
                        else {
                            _instanceLocator = _cell->getInstances().getLocator();
                            if (_instanceLocator.isValid())
                                _state = 4;
                            else
                                _state = 0;
                        }
                    }
                }
                break;
            case 2 :
                _rubberLocator.progress();
                if (!_rubberLocator.isValid()) {
                    _markerLocator = _cell->getMarkers().getLocator();
                    if (_markerLocator.isValid())
                        _state = 3;
                    else {
                        _instanceLocator = _cell->getInstances().getLocator();
                        if (_instanceLocator.isValid())
                            _state = 4;
                        else
                            _state = 0;
                    }
                }
                break;
            case 3 :
                _markerLocator.progress();
                if (!_markerLocator.isValid()) {
                    _instanceLocator = _cell->getInstances().getLocator();
                    if (_instanceLocator.isValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
            case 4 :
                if (!_searchDepth) {
                    _instanceLocator.progress();
                    if (!_instanceLocator.isValid()) _state = 0;
                }
                else {
                    Instance* instance = _instanceLocator.getElement();
                    if (instance->isTerminal()) {
                        _instanceLocator.progress();
                        if (!_instanceLocator.isValid()) _state = 0;
                    }
                    else {
                        _occurrenceLocator =
                            instance->getMasterCell()->getOccurrences(_searchDepth - 1).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 5;
                        else {
                            _instanceLocator.progress();
                            if (!_instanceLocator.isValid()) _state = 0;
                        }
                    }
                }
                break;
            case 5 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _instanceLocator.progress();
                    if (_instanceLocator.isValid())
                        _state = 4;
                    else
                        _state = 0;
                }
                break;
        }
    }
}

string Cell_Occurrences::Locator::_getString() const
// ************************************************
{
    string s = "<" + _TName("Cell::Occurrences::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        if (_searchDepth != ((unsigned)-1)) s += " " + getString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_OccurrencesUnder implementation
// ****************************************************************************************************

Cell_OccurrencesUnder::Cell_OccurrencesUnder()
// *****************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _searchDepth(0)
{
}

Cell_OccurrencesUnder::Cell_OccurrencesUnder(const Cell* cell, const Box& area, unsigned searchDepth)
// ************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _searchDepth(searchDepth)
{
}

Cell_OccurrencesUnder::Cell_OccurrencesUnder(const Cell_OccurrencesUnder& occurrences)
// *******************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area),
    _searchDepth(occurrences._searchDepth)
{
}

Cell_OccurrencesUnder& Cell_OccurrencesUnder::operator=(const Cell_OccurrencesUnder& occurrences)
// ******************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    _searchDepth = occurrences._searchDepth;
    return *this;
}

Collection<Occurrence>* Cell_OccurrencesUnder::getClone() const
// **********************************************************
{
    return new Cell_OccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_OccurrencesUnder::getLocator() const
// *********************************************************
{
    return new Locator(_cell, _area, _searchDepth);
}

string Cell_OccurrencesUnder::_getString() const
// ********************************************
{
    string s = "<" + _TName("Cell::OccurrencesUnder");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        if (_searchDepth != ((unsigned)-1)) s += " " + getString(_searchDepth);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_OccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_OccurrencesUnder::Locator::Locator()
// *************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _searchDepth(0),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _extensionGoLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_OccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area, unsigned searchDepth)
// ********************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _searchDepth(searchDepth),
    _state(0),
    _componentLocator(),
    _rubberLocator(),
    _markerLocator(),
    _extensionGoLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _componentLocator = _cell->getComponentsUnder(_area).getLocator();
        if (_componentLocator.isValid())
            _state = 1;
        else {
            _rubberLocator = _cell->getRubbersUnder(_area).getLocator();
            if (_rubberLocator.isValid())
                _state = 2;
            else {
                _markerLocator = _cell->getMarkersUnder(_area).getLocator();
                if (_markerLocator.isValid())
                    _state = 3;
                else {
                    _extensionGoLocator = _cell->getExtensionGosUnder(_area).getLocator();
                    if (_extensionGoLocator.isValid())
                        _state = 4;
                    else {
                      _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                      if (_instanceLocator.isValid())
                        _state = 5;
                    }
                }
            }
        }
    }
}

Cell_OccurrencesUnder::Locator::Locator(const Locator& locator)
// ***********************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _searchDepth(locator._searchDepth),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _rubberLocator(locator._rubberLocator),
    _markerLocator(locator._markerLocator),
    _extensionGoLocator(locator._extensionGoLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_OccurrencesUnder::Locator& Cell_OccurrencesUnder::Locator::operator=(const Locator& locator)
// ********************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _searchDepth = locator._searchDepth;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _rubberLocator = locator._rubberLocator;
    _markerLocator = locator._markerLocator;
    _extensionGoLocator = locator._extensionGoLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_OccurrencesUnder::Locator::getElement() const
// ********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.getElement());
            case 2 : return Occurrence(_rubberLocator.getElement());
            case 3 : return Occurrence(_markerLocator.getElement());
            case 4 : return Occurrence(_extensionGoLocator.getElement());
            case 5 : return Occurrence(_instanceLocator.getElement());
            case 6 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_instanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_OccurrencesUnder::Locator::getClone() const
// ****************************************************************
{
    return new Locator(*this);
}

bool Cell_OccurrencesUnder::Locator::isValid() const
// ************************************************
{
    return (_state != 0);
}

void Cell_OccurrencesUnder::Locator::progress()
// *******************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.progress();
                if (!_componentLocator.isValid()) {
                    _rubberLocator = _cell->getRubbersUnder(_area).getLocator();
                    if (_rubberLocator.isValid())
                        _state = 2;
                    else {
                        _markerLocator = _cell->getMarkersUnder(_area).getLocator();
                        if (_markerLocator.isValid())
                            _state = 3;
                        else {
                            _extensionGoLocator = _cell->getExtensionGosUnder(_area).getLocator();
                            if (_extensionGoLocator.isValid())
                                _state = 4;
                            else {
                              _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                              if (_instanceLocator.isValid())
                                _state = 5;
                              else
                                _state = 0;
                            }
                        }
                    }
                }
                break;
            case 2 :
                _rubberLocator.progress();
                if (!_rubberLocator.isValid()) {
                    _markerLocator = _cell->getMarkersUnder(_area).getLocator();
                    if (_markerLocator.isValid())
                        _state = 3;
                    else {
                        _extensionGoLocator = _cell->getExtensionGosUnder(_area).getLocator();
                        if (_extensionGoLocator.isValid())
                            _state = 4;
                        else {
                          _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                          if (_instanceLocator.isValid())
                            _state = 5;
                          else
                            _state = 0;
                        }
                    }
                }
                break;
            case 3 :
                _markerLocator.progress();
                if (!_markerLocator.isValid()) {
                    _extensionGoLocator = _cell->getExtensionGosUnder(_area).getLocator();
                    if (_extensionGoLocator.isValid())
                        _state = 4;
                    else {
                      _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                      if (_instanceLocator.isValid())
                        _state = 5;
                      else
                        _state = 0;
                    }
                }
                break;
            case 4 :
                _extensionGoLocator.progress();
                if (!_extensionGoLocator.isValid()) {
                  _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                  if (_instanceLocator.isValid())
                    _state = 5;
                  else
                    _state = 0;
                }
                break;
            case 5 :
                if (!_searchDepth) {
                    _instanceLocator.progress();
                    if (!_instanceLocator.isValid()) _state = 0;
                }
                else {
                    Instance* instance = _instanceLocator.getElement();
                    if (instance->isTerminal()) {
                        _instanceLocator.progress();
                        if (!_instanceLocator.isValid()) _state = 0;
                    }
                    else {
                        Box masterArea = _area;
                        instance->getTransformation().getInvert().applyOn(masterArea);
                        Cell* masterCell = instance->getMasterCell();
                        _occurrenceLocator =
                            masterCell->getOccurrencesUnder(masterArea, _searchDepth - 1).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 6;
                        else {
                            _instanceLocator.progress();
                            if (!_instanceLocator.isValid()) _state = 0;
                        }
                    }
                }
                break;
            case 6 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _instanceLocator.progress();
                    if (_instanceLocator.isValid())
                        _state = 5;
                    else
                        _state = 0;
                }
                break;
        }
    }
}

string Cell_OccurrencesUnder::Locator::_getString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::OccurrencesUnder::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        if (_searchDepth != ((unsigned)-1)) s += " " + getString(_searchDepth);
    }
    s += ">";
    return s;
}

// ****************************************************************************************************
// Cell_LeafInstanceOccurrences implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrences::Cell_LeafInstanceOccurrences(const Cell* cell)
// *******************************************************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_LeafInstanceOccurrences::Cell_LeafInstanceOccurrences(const Cell_LeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell)
{
}

Cell_LeafInstanceOccurrences& Cell_LeafInstanceOccurrences::operator=(const Cell_LeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    return *this;
}

Collection<Occurrence>* Cell_LeafInstanceOccurrences::getClone() const
// *********************************************************************
{
    return new Cell_LeafInstanceOccurrences(*this);
}

Locator<Occurrence>* Cell_LeafInstanceOccurrences::getLocator() const
// ********************************************************************
{
    return new Locator(_cell);
}

string Cell_LeafInstanceOccurrences::_getString() const
// *******************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrences");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrences::Locator implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrences::Locator::Locator(const Cell* cell)
// ****************************************************************
:    Inherit(),
    _cell(cell),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _leafInstanceLocator = _cell->getLeafInstances().getLocator();
        if (_leafInstanceLocator.isValid())
            _state = 1;
        else {
            _nonLeafInstanceLocator = _cell->getNonLeafInstances().getLocator();
            while (!_state && _nonLeafInstanceLocator.isValid()) {
                Cell* masterCell = _nonLeafInstanceLocator.getElement()->getMasterCell();
                _occurrenceLocator = masterCell->getLeafInstanceOccurrences().getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _nonLeafInstanceLocator.progress();
            }
        }
    }
}

Cell_LeafInstanceOccurrences::Locator::Locator(const Locator& locator)
// **********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _state(locator._state),
    _leafInstanceLocator(locator._leafInstanceLocator),
    _nonLeafInstanceLocator(locator._nonLeafInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_LeafInstanceOccurrences::Locator& Cell_LeafInstanceOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _state = locator._state;
    _leafInstanceLocator = locator._leafInstanceLocator;
    _nonLeafInstanceLocator = locator._nonLeafInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_LeafInstanceOccurrences::Locator::getElement() const
// *******************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_leafInstanceLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_nonLeafInstanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_LeafInstanceOccurrences::Locator::getClone() const
// ***************************************************************************
{
    return new Locator(*this);
}

bool Cell_LeafInstanceOccurrences::Locator::isValid() const
// ***********************************************************
{
    return (_state != 0);
}

void Cell_LeafInstanceOccurrences::Locator::progress()
// ******************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _leafInstanceLocator.progress();
                if (!_leafInstanceLocator.isValid()) {
                    _state = 0;
                    _nonLeafInstanceLocator = _cell->getNonLeafInstances().getLocator();
                    while (!_state && _nonLeafInstanceLocator.isValid()) {
                        Cell* masterCell = _nonLeafInstanceLocator.getElement()->getMasterCell();
                        _occurrenceLocator = masterCell->getLeafInstanceOccurrences().getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _nonLeafInstanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_nonLeafInstanceLocator.isValid()) {
                        _nonLeafInstanceLocator.progress();
                        while (!_state && _nonLeafInstanceLocator.isValid()) {
                            Cell* masterCell = _nonLeafInstanceLocator.getElement()->getMasterCell();
                            _occurrenceLocator =
                                masterCell->getLeafInstanceOccurrences().getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _nonLeafInstanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_LeafInstanceOccurrences::Locator::_getString() const
// ****************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrences::Locator");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder()
// *************************************************************************
:     Inherit(),
    _cell(NULL),
    _area()
{
}

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder(const Cell* cell, const Box& area)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area)
{
}

Cell_LeafInstanceOccurrencesUnder::Cell_LeafInstanceOccurrencesUnder(const Cell_LeafInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area)
{
}

Cell_LeafInstanceOccurrencesUnder& Cell_LeafInstanceOccurrencesUnder::operator=(const Cell_LeafInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    return *this;
}

Collection<Occurrence>* Cell_LeafInstanceOccurrencesUnder::getClone() const
// **************************************************************************
{
    return new Cell_LeafInstanceOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_LeafInstanceOccurrencesUnder::getLocator() const
// *************************************************************************
{
    return new Locator(_cell, _area);
}

string Cell_LeafInstanceOccurrencesUnder::_getString() const
// ************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrencesUnder");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_LeafInstanceOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_LeafInstanceOccurrencesUnder::Locator::Locator()
// *****************************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
}

Cell_LeafInstanceOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area)
// **************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _state(0),
    _leafInstanceLocator(),
    _nonLeafInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _leafInstanceLocator = _cell->getLeafInstancesUnder(_area).getLocator();
        if (_leafInstanceLocator.isValid())
            _state = 1;
        else {
            _nonLeafInstanceLocator = _cell->getNonLeafInstancesUnder(_area).getLocator();
            while (!_state && _nonLeafInstanceLocator.isValid()) {
                Instance* instance = _nonLeafInstanceLocator.getElement();
                Cell* masterCell = instance->getMasterCell();
                Box masterArea = _area;
                instance->getTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->getLeafInstanceOccurrencesUnder(masterArea).getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _nonLeafInstanceLocator.progress();
            }
        }
    }
}

Cell_LeafInstanceOccurrencesUnder::Locator::Locator(const Locator& locator)
// ***************************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _state(locator._state),
    _leafInstanceLocator(locator._leafInstanceLocator),
    _nonLeafInstanceLocator(locator._nonLeafInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_LeafInstanceOccurrencesUnder::Locator& Cell_LeafInstanceOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _state = locator._state;
    _leafInstanceLocator = locator._leafInstanceLocator;
    _nonLeafInstanceLocator = locator._nonLeafInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_LeafInstanceOccurrencesUnder::Locator::getElement() const
// ************************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_leafInstanceLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_nonLeafInstanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_LeafInstanceOccurrencesUnder::Locator::getClone() const
// ********************************************************************************
{
    return new Locator(*this);
}

bool Cell_LeafInstanceOccurrencesUnder::Locator::isValid() const
// ****************************************************************
{
    return (_state != 0);
}

void Cell_LeafInstanceOccurrencesUnder::Locator::progress()
// ***********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _leafInstanceLocator.progress();
                if (!_leafInstanceLocator.isValid()) {
                    _state = 0;
                    _nonLeafInstanceLocator = _cell->getNonLeafInstancesUnder(_area).getLocator();
                    while (!_state && _nonLeafInstanceLocator.isValid()) {
                        Instance* instance = _nonLeafInstanceLocator.getElement();
                        Cell* masterCell = instance->getMasterCell();
                        Box masterArea = _area;
                        instance->getTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->getLeafInstanceOccurrencesUnder(masterArea).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _nonLeafInstanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_nonLeafInstanceLocator.isValid()) {
                        _nonLeafInstanceLocator.progress();
                        while (!_state && _nonLeafInstanceLocator.isValid()) {
                            Instance* instance = _nonLeafInstanceLocator.getElement();
                            Cell* masterCell = instance->getMasterCell();
                            Box masterArea = _area;
                            instance->getTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->getLeafInstanceOccurrencesUnder(masterArea).getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _nonLeafInstanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_LeafInstanceOccurrencesUnder::Locator::_getString() const
// *********************************************************************
{
    string s = "<" + _TName("Cell::LeafInstanceOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}




// ****************************************************************************************************
// Cell_NonLeafInstanceOccurrences implementation
// ****************************************************************************************************

Cell_NonLeafInstanceOccurrences::Cell_NonLeafInstanceOccurrences(const Cell* cell)
// *******************************************************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_NonLeafInstanceOccurrences::Cell_NonLeafInstanceOccurrences(const Cell_NonLeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell)
{
}

Cell_NonLeafInstanceOccurrences& Cell_NonLeafInstanceOccurrences::operator=(const Cell_NonLeafInstanceOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    return *this;
}

Collection<Occurrence>* Cell_NonLeafInstanceOccurrences::getClone() const
// *********************************************************************
{
    return new Cell_NonLeafInstanceOccurrences(*this);
}

Locator<Occurrence>* Cell_NonLeafInstanceOccurrences::getLocator() const
// ********************************************************************
{
    return new Locator(_cell);
}

string Cell_NonLeafInstanceOccurrences::_getString() const
// *******************************************************
{
    string s = "<" + _TName("Cell::NonLeafInstanceOccurrences");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_NonLeafInstanceOccurrences::Locator implementation
// ****************************************************************************************************

Cell_NonLeafInstanceOccurrences::Locator::Locator(const Cell* cell)
// ****************************************************************
  : Inherit                ()
  , _cell                  (cell)
  , _state                 (0)
  , _nonLeafInstanceLocator()
  , _occurrenceLocator     ()
{
  if ( _cell ) {
    _nonLeafInstanceLocator = _cell->getNonLeafInstances().getLocator();
    if ( _nonLeafInstanceLocator.isValid() ) {
      _state = 1;
    }
  }
}

Cell_NonLeafInstanceOccurrences::Locator::Locator(const Locator& locator)
// **********************************************************************
  : Inherit                ()
  , _cell                  (locator._cell)
  , _state                 (locator._state)
  , _nonLeafInstanceLocator(locator._nonLeafInstanceLocator)
  , _occurrenceLocator     (locator._occurrenceLocator)
{ }

Cell_NonLeafInstanceOccurrences::Locator& Cell_NonLeafInstanceOccurrences::Locator::operator=(const Locator& locator)
// ********************************************************************************************************************
{
  _cell                   = locator._cell;
  _state                  = locator._state;
  _nonLeafInstanceLocator = locator._nonLeafInstanceLocator;
  _occurrenceLocator      = locator._occurrenceLocator;
  return *this;
}

Occurrence Cell_NonLeafInstanceOccurrences::Locator::getElement() const
// *********************************************************************
{
  if ( _state ) {
    switch ( _state ) {
      case 1 : return Occurrence(_nonLeafInstanceLocator.getElement());
      case 2 :
        {
        Occurrence occurrence = _occurrenceLocator.getElement();
        Entity*    entity     = occurrence.getEntity();
        Path       path       = Path(_nonLeafInstanceLocator.getElement(), occurrence.getPath());
        return Occurrence(entity, path);
        }
    }
  }
  return Occurrence();
}

Locator<Occurrence>* Cell_NonLeafInstanceOccurrences::Locator::getClone() const
// *****************************************************************************
{
  return new Locator(*this);
}

bool Cell_NonLeafInstanceOccurrences::Locator::isValid() const
// ************************************************************
{
  return ( _state != 0 );
}

void Cell_NonLeafInstanceOccurrences::Locator::progress()
// *******************************************************
{
  if ( _state ) {
    switch ( _state ) {
      case 1:
        {
        _nonLeafInstanceLocator.progress();
        if ( _nonLeafInstanceLocator.isValid() ) break;

        _state = 2;
        _nonLeafInstanceLocator = _cell->getNonLeafInstances().getLocator();
        if ( not _nonLeafInstanceLocator.isValid() ) {
          _state = 0;
          break;
        }

        Cell* masterCell = _nonLeafInstanceLocator.getElement()->getMasterCell();
        _occurrenceLocator = masterCell->getNonLeafInstanceOccurrences().getLocator();

        if ( _occurrenceLocator.isValid() ) break;
        }
      case 2:
        _occurrenceLocator.progress ();

        while ( (_state != 0) and not _occurrenceLocator.isValid() ) {
          _nonLeafInstanceLocator.progress();
          if ( not _nonLeafInstanceLocator.isValid() ) {
            _state = 0;
            break;
          }

          Cell* masterCell = _nonLeafInstanceLocator.getElement()->getMasterCell();
          _occurrenceLocator = masterCell->getNonLeafInstanceOccurrences().getLocator();
        }
    }
  }
}

string Cell_NonLeafInstanceOccurrences::Locator::_getString() const
// ****************************************************************
{
    string s = "<" + _TName("Cell::NonLeafInstanceOccurrences::Locator");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrences::Cell_TerminalInstanceOccurrences(const Cell* cell)
// *******************************************************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_TerminalInstanceOccurrences::Cell_TerminalInstanceOccurrences(const Cell_TerminalInstanceOccurrences& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell)
{
}

Cell_TerminalInstanceOccurrences& Cell_TerminalInstanceOccurrences::operator=(const Cell_TerminalInstanceOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    return *this;
}

Collection<Occurrence>* Cell_TerminalInstanceOccurrences::getClone() const
// *********************************************************************
{
    return new Cell_TerminalInstanceOccurrences(*this);
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrences::getLocator() const
// ********************************************************************
{
    return new Locator(_cell);
}

string Cell_TerminalInstanceOccurrences::_getString() const
// *******************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrences");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrences::Locator implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrences::Locator::Locator(const Cell* cell)
// ****************************************************************
:    Inherit(),
    _cell(cell),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell) {
        _terminalInstanceLocator = _cell->getTerminalInstances().getLocator();
        if (_terminalInstanceLocator.isValid())
            _state = 1;
        else {
            _nonTerminalInstanceLocator = _cell->getNonTerminalInstances().getLocator();
            while (!_state && _nonTerminalInstanceLocator.isValid()) {
                Cell* masterCell = _nonTerminalInstanceLocator.getElement()->getMasterCell();
                _occurrenceLocator = masterCell->getTerminalInstanceOccurrences().getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _nonTerminalInstanceLocator.progress();
            }
        }
    }
}

Cell_TerminalInstanceOccurrences::Locator::Locator(const Locator& locator)
// **********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _state(locator._state),
    _terminalInstanceLocator(locator._terminalInstanceLocator),
    _nonTerminalInstanceLocator(locator._nonTerminalInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_TerminalInstanceOccurrences::Locator& Cell_TerminalInstanceOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _state = locator._state;
    _terminalInstanceLocator = locator._terminalInstanceLocator;
    _nonTerminalInstanceLocator = locator._nonTerminalInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_TerminalInstanceOccurrences::Locator::getElement() const
// *******************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_terminalInstanceLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_nonTerminalInstanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrences::Locator::getClone() const
// ***************************************************************************
{
    return new Locator(*this);
}

bool Cell_TerminalInstanceOccurrences::Locator::isValid() const
// ***********************************************************
{
    return (_state != 0);
}

void Cell_TerminalInstanceOccurrences::Locator::progress()
// ******************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _terminalInstanceLocator.progress();
                if (!_terminalInstanceLocator.isValid()) {
                    _state = 0;
                    _nonTerminalInstanceLocator = _cell->getNonTerminalInstances().getLocator();
                    while (!_state && _nonTerminalInstanceLocator.isValid()) {
                        Cell* masterCell = _nonTerminalInstanceLocator.getElement()->getMasterCell();
                        _occurrenceLocator = masterCell->getTerminalInstanceOccurrences().getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _nonTerminalInstanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_nonTerminalInstanceLocator.isValid()) {
                        _nonTerminalInstanceLocator.progress();
                        while (!_state && _nonTerminalInstanceLocator.isValid()) {
                            Cell* masterCell = _nonTerminalInstanceLocator.getElement()->getMasterCell();
                            _occurrenceLocator =
                                masterCell->getTerminalInstanceOccurrences().getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _nonTerminalInstanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_TerminalInstanceOccurrences::Locator::_getString() const
// ****************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrences::Locator");
    if (_cell) s += " " + getString(_cell);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder()
// *************************************************************************
:     Inherit(),
    _cell(NULL),
    _area()
{
}

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder(const Cell* cell, const Box& area)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area)
{
}

Cell_TerminalInstanceOccurrencesUnder::Cell_TerminalInstanceOccurrencesUnder(const Cell_TerminalInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area)
{
}

Cell_TerminalInstanceOccurrencesUnder& Cell_TerminalInstanceOccurrencesUnder::operator=(const Cell_TerminalInstanceOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    return *this;
}

Collection<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::getClone() const
// **************************************************************************
{
    return new Cell_TerminalInstanceOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::getLocator() const
// *************************************************************************
{
    return new Locator(_cell, _area);
}

string Cell_TerminalInstanceOccurrencesUnder::_getString() const
// ************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrencesUnder");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_TerminalInstanceOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator()
// *****************************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
}

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area)
// **************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _state(0),
    _terminalInstanceLocator(),
    _nonTerminalInstanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty()) {
        _terminalInstanceLocator = _cell->getTerminalInstancesUnder(_area).getLocator();
        if (_terminalInstanceLocator.isValid())
            _state = 1;
        else {
            _nonTerminalInstanceLocator = _cell->getNonTerminalInstancesUnder(_area).getLocator();
            while (!_state && _nonTerminalInstanceLocator.isValid()) {
                Instance* instance = _nonTerminalInstanceLocator.getElement();
                Cell* masterCell = instance->getMasterCell();
                Box masterArea = _area;
                instance->getTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->getTerminalInstanceOccurrencesUnder(masterArea).getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _nonTerminalInstanceLocator.progress();
            }
        }
    }
}

Cell_TerminalInstanceOccurrencesUnder::Locator::Locator(const Locator& locator)
// ***************************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _state(locator._state),
    _terminalInstanceLocator(locator._terminalInstanceLocator),
    _nonTerminalInstanceLocator(locator._nonTerminalInstanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_TerminalInstanceOccurrencesUnder::Locator& Cell_TerminalInstanceOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _state = locator._state;
    _terminalInstanceLocator = locator._terminalInstanceLocator;
    _nonTerminalInstanceLocator = locator._nonTerminalInstanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_TerminalInstanceOccurrencesUnder::Locator::getElement() const
// ************************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_terminalInstanceLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_nonTerminalInstanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_TerminalInstanceOccurrencesUnder::Locator::getClone() const
// ********************************************************************************
{
    return new Locator(*this);
}

bool Cell_TerminalInstanceOccurrencesUnder::Locator::isValid() const
// ****************************************************************
{
    return (_state != 0);
}

void Cell_TerminalInstanceOccurrencesUnder::Locator::progress()
// ***********************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _terminalInstanceLocator.progress();
                if (!_terminalInstanceLocator.isValid()) {
                    _state = 0;
                    _nonTerminalInstanceLocator = _cell->getNonTerminalInstancesUnder(_area).getLocator();
                    while (!_state && _nonTerminalInstanceLocator.isValid()) {
                        Instance* instance = _nonTerminalInstanceLocator.getElement();
                        Cell* masterCell = instance->getMasterCell();
                        Box masterArea = _area;
                        instance->getTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->getTerminalInstanceOccurrencesUnder(masterArea).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _nonTerminalInstanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_nonTerminalInstanceLocator.isValid()) {
                        _nonTerminalInstanceLocator.progress();
                        while (!_state && _nonTerminalInstanceLocator.isValid()) {
                            Instance* instance = _nonTerminalInstanceLocator.getElement();
                            Cell* masterCell = instance->getMasterCell();
                            Box masterArea = _area;
                            instance->getTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->getTerminalInstanceOccurrencesUnder(masterArea).getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _nonTerminalInstanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_TerminalInstanceOccurrencesUnder::Locator::_getString() const
// *********************************************************************
{
    string s = "<" + _TName("Cell::TerminalInstanceOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrences implementation
// ****************************************************************************************************

Cell_ComponentOccurrences::Cell_ComponentOccurrences()
// *************************************************
:     Inherit(),
    _cell(NULL),
    _mask(0)
{
}

Cell_ComponentOccurrences::Cell_ComponentOccurrences(const Cell* cell, const Layer::Mask& mask)
// ******************************************************************************************
:     Inherit(),
    _cell(cell),
    _mask(mask)
{
}

Cell_ComponentOccurrences::Cell_ComponentOccurrences(const Cell_ComponentOccurrences& occurrences)
// *******************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _mask(occurrences._mask)
{
}

Cell_ComponentOccurrences& Cell_ComponentOccurrences::operator=(const Cell_ComponentOccurrences& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _mask = occurrences._mask;
    return *this;
}

Collection<Occurrence>* Cell_ComponentOccurrences::getClone() const
// **************************************************************
{
    return new Cell_ComponentOccurrences(*this);
}

Locator<Occurrence>* Cell_ComponentOccurrences::getLocator() const
// *************************************************************
{
    return new Locator(_cell, _mask);
}

string Cell_ComponentOccurrences::_getString() const
// ************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrences");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrences::Locator implementation
// ****************************************************************************************************

Cell_ComponentOccurrences::Locator::Locator()
// *****************************************
:    Inherit(),
    _cell(NULL),
    _mask(0),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_ComponentOccurrences::Locator::Locator(const Cell* cell, const Layer::Mask& mask)
// **********************************************************************************
:    Inherit(),
    _cell(cell),
    _mask(mask),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_mask.zero() ) {
        _componentLocator = _cell->getComponents(_mask).getLocator();
        if (_componentLocator.isValid())
            _state = 1;
        else {
            _instanceLocator = _cell->getInstances().getLocator();
            while (!_state && _instanceLocator.isValid()) {
                Cell* masterCell = _instanceLocator.getElement()->getMasterCell();
                _occurrenceLocator = masterCell->getComponentOccurrences(_mask).getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _instanceLocator.progress();
            }
        }
    }
}

Cell_ComponentOccurrences::Locator::Locator(const Locator& locator)
// ***************************************************************
:    Inherit(),
    _cell(locator._cell),
    _mask(locator._mask),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_ComponentOccurrences::Locator& Cell_ComponentOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _mask = locator._mask;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_ComponentOccurrences::Locator::getElement() const
// ************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_instanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_ComponentOccurrences::Locator::getClone() const
// ********************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentOccurrences::Locator::isValid() const
// ****************************************************
{
    return (_state != 0);
}

void Cell_ComponentOccurrences::Locator::progress()
// ***********************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.progress();
                if (!_componentLocator.isValid()) {
                    _state = 0;
                    _instanceLocator = _cell->getInstances().getLocator();
                    while (!_state && _instanceLocator.isValid()) {
                        Cell* masterCell = _instanceLocator.getElement()->getMasterCell();
                        _occurrenceLocator = masterCell->getComponentOccurrences(_mask).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _instanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_instanceLocator.isValid()) {
                        _instanceLocator.progress();
                        while (!_state && _instanceLocator.isValid()) {
                            Cell* masterCell = _instanceLocator.getElement()->getMasterCell();
                            _occurrenceLocator = masterCell->getComponentOccurrences(_mask).getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _instanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_ComponentOccurrences::Locator::_getString() const
// *********************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrences::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder implementation
// ****************************************************************************************************

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder()
// ***********************************************************
:     Inherit(),
    _cell(NULL),
    _area(),
    _mask(0)
{
}

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ****************************************************************************************************
:     Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask)
{
}

Cell_ComponentOccurrencesUnder::Cell_ComponentOccurrencesUnder(const Cell_ComponentOccurrencesUnder& occurrences)
// ****************************************************************************************************
:     Inherit(),
    _cell(occurrences._cell),
    _area(occurrences._area),
    _mask(occurrences._mask)
{
}

Cell_ComponentOccurrencesUnder& Cell_ComponentOccurrencesUnder::operator=(const Cell_ComponentOccurrencesUnder& occurrences)
// ****************************************************************************************************
{
    _cell = occurrences._cell;
    _area = occurrences._area;
    _mask = occurrences._mask;
    return *this;
}

Collection<Occurrence>* Cell_ComponentOccurrencesUnder::getClone() const
// *******************************************************************
{
    return new Cell_ComponentOccurrencesUnder(*this);
}

Locator<Occurrence>* Cell_ComponentOccurrencesUnder::getLocator() const
// ******************************************************************
{
    return new Locator(_cell, _area, _mask);
}

string Cell_ComponentOccurrencesUnder::_getString() const
// *****************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrencesUnder");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_ComponentOccurrencesUnder::Locator implementation
// ****************************************************************************************************

Cell_ComponentOccurrencesUnder::Locator::Locator()
// **********************************************
:    Inherit(),
    _cell(NULL),
    _area(),
    _mask(0),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
}

Cell_ComponentOccurrencesUnder::Locator::Locator(const Cell* cell, const Box& area, const Layer::Mask& mask)
// ****************************************************************************************************
:    Inherit(),
    _cell(cell),
    _area(area),
    _mask(mask),
    _state(0),
    _componentLocator(),
    _instanceLocator(),
    _occurrenceLocator()
{
    if (_cell && !_area.isEmpty() && !_mask.zero() ) {
        _componentLocator = _cell->getComponentsUnder(_area, _mask).getLocator();
        if (_componentLocator.isValid())
            _state = 1;
        else {
            _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
            while (!_state && _instanceLocator.isValid()) {
                Instance* instance = _instanceLocator.getElement();
                Cell* masterCell = instance->getMasterCell();
                Box masterArea = _area;
                instance->getTransformation().getInvert().applyOn(masterArea);
                _occurrenceLocator =
                    masterCell->getComponentOccurrencesUnder(masterArea, _mask).getLocator();
                if (_occurrenceLocator.isValid())
                    _state = 2;
                else
                    _instanceLocator.progress();
            }
        }
    }
}

Cell_ComponentOccurrencesUnder::Locator::Locator(const Locator& locator)
// ********************************************************************
:    Inherit(),
    _cell(locator._cell),
    _area(locator._area),
    _mask(locator._mask),
    _state(locator._state),
    _componentLocator(locator._componentLocator),
    _instanceLocator(locator._instanceLocator),
    _occurrenceLocator(locator._occurrenceLocator)
{
}

Cell_ComponentOccurrencesUnder::Locator& Cell_ComponentOccurrencesUnder::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _cell = locator._cell;
    _area = locator._area;
    _mask = locator._mask;
    _state = locator._state;
    _componentLocator = locator._componentLocator;
    _instanceLocator = locator._instanceLocator;
    _occurrenceLocator = locator._occurrenceLocator;
    return *this;
}

Occurrence Cell_ComponentOccurrencesUnder::Locator::getElement() const
// *****************************************************************
{
    if (_state) {
        switch (_state) {
            case 1 : return Occurrence(_componentLocator.getElement());
            case 2 : {
                Occurrence occurrence = _occurrenceLocator.getElement();
                Entity* entity = occurrence.getEntity();
                Path path = Path(_instanceLocator.getElement(), occurrence.getPath());
                return Occurrence(entity, path);
            }
        }
    }
    return Occurrence();
}

Locator<Occurrence>* Cell_ComponentOccurrencesUnder::Locator::getClone() const
// *************************************************************************
{
    return new Locator(*this);
}

bool Cell_ComponentOccurrencesUnder::Locator::isValid() const
// *********************************************************
{
    return (_state != 0);
}

void Cell_ComponentOccurrencesUnder::Locator::progress()
// ****************************************************
{
    if (_state) {
        switch (_state) {
            case 1 :
                _componentLocator.progress();
                if (!_componentLocator.isValid()) {
                    _state = 0;
                    _instanceLocator = _cell->getInstancesUnder(_area).getLocator();
                    while (!_state && _instanceLocator.isValid()) {
                        Instance* instance = _instanceLocator.getElement();
                        Cell* masterCell = instance->getMasterCell();
                        Box masterArea = _area;
                        instance->getTransformation().getInvert().applyOn(masterArea);
                        _occurrenceLocator =
                            masterCell->getComponentOccurrencesUnder(masterArea, _mask).getLocator();
                        if (_occurrenceLocator.isValid())
                            _state = 2;
                        else
                            _instanceLocator.progress();
                    }
                }
                break;
            case 2 :
                _occurrenceLocator.progress();
                if (!_occurrenceLocator.isValid()) {
                    _state = 0;
                    if (_instanceLocator.isValid()) {
                        _instanceLocator.progress();
                        while (!_state && _instanceLocator.isValid()) {
                            Instance* instance = _instanceLocator.getElement();
                            Cell* masterCell = instance->getMasterCell();
                            Box masterArea = _area;
                            instance->getTransformation().getInvert().applyOn(masterArea);
                            _occurrenceLocator =
                                masterCell->getComponentOccurrencesUnder(masterArea, _mask).getLocator();
                            if (_occurrenceLocator.isValid())
                                _state = 2;
                            else
                                _instanceLocator.progress();
                        }
                    }
                }
                break;
        }
    }
}

string Cell_ComponentOccurrencesUnder::Locator::_getString() const
// **************************************************************
{
    string s = "<" + _TName("Cell::ComponentOccurrencesUnder::Locator");
    if (_cell) {
        s += " " + getString(_cell);
        s += " " + getString(_area);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}

// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences implementation
// ****************************************************************************************************

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences()
// *************************************
:     Inherit(),
    _cell(NULL),
    _path()
{
}

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences(const Cell* cell, Path path)
// ****************************************************************
:     Inherit(),
    _cell(cell),
    _path(path)
{
}

Cell_HyperNetRootNetOccurrences::Cell_HyperNetRootNetOccurrences(const Cell_HyperNetRootNetOccurrences& netOccurrences)
// ****************************************************************************
:     Inherit(),
    _cell(netOccurrences._cell),
    _path(netOccurrences._path)
{
}

Cell_HyperNetRootNetOccurrences& Cell_HyperNetRootNetOccurrences::operator=(const Cell_HyperNetRootNetOccurrences& netOccurrences)
// ***************************************************************************************
{
    _cell = netOccurrences._cell;
    _path = netOccurrences._path;
    return *this;
}

Collection<Occurrence>* Cell_HyperNetRootNetOccurrences::getClone() const
// ********************************************************
{
    return new Cell_HyperNetRootNetOccurrences(*this);
}

Locator<Occurrence>* Cell_HyperNetRootNetOccurrences::getLocator() const
// *******************************************************
{
    return new Locator(_cell, _path);
}

string Cell_HyperNetRootNetOccurrences::_getString() const
// ******************************************
{
    string s = "<" + _TName("Cell::HyperNetRootNetOccurrences");
    if (_cell) {
        s += " " + getString(_cell);
        if (!_path.isEmpty()) s += " " + getString(_path);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_HyperNetRootNetOccurrences::Locator implementation
// ****************************************************************************************************

Cell_HyperNetRootNetOccurrences::Locator::Locator()
// ***********************************
:    Inherit(),
    _path(),
    _netLocator(),
    _instanceLocator(),
    _hyperNetRootNetOccurrenceLocator()
{
}

Cell_HyperNetRootNetOccurrences::Locator::Locator(const Cell* cell, Path path)
// **************************************************************
:    Inherit(),
    _path(path),
    _netLocator(),
    _instanceLocator(),
    _hyperNetRootNetOccurrenceLocator()
{
    _netLocator=cell->getNets().getLocator();

    _instanceLocator=cell->getInstances().getLocator();

    while (_netLocator.isValid() && !isHyperNetRootNetOccurrence(Occurrence(_netLocator.getElement(),_path)))
        _netLocator.progress();

    if (!_netLocator.isValid())
        while (!_hyperNetRootNetOccurrenceLocator.isValid() && _instanceLocator.isValid())
        {
            Instance* instance = _instanceLocator.getElement();
            _hyperNetRootNetOccurrenceLocator=Locator(instance->getMasterCell(),Path(_path,instance));
            _instanceLocator.progress();
        }
}

Cell_HyperNetRootNetOccurrences::Locator::Locator(const Locator& locator)
// *********************************************************
:    Inherit(),
    _path(locator._path),
    _netLocator(locator._netLocator),
    _instanceLocator(locator._instanceLocator),
    _hyperNetRootNetOccurrenceLocator(locator._hyperNetRootNetOccurrenceLocator)
{
}

Cell_HyperNetRootNetOccurrences::Locator& Cell_HyperNetRootNetOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************
{
    _path = locator._path;
    _netLocator = locator._netLocator;
    _instanceLocator = locator._instanceLocator;
    _hyperNetRootNetOccurrenceLocator = locator._hyperNetRootNetOccurrenceLocator;
    return *this;
}

Occurrence Cell_HyperNetRootNetOccurrences::Locator::getElement() const
// ******************************************************
{
    if (_netLocator.isValid())
        return Occurrence(_netLocator.getElement(),_path);
    else if (_hyperNetRootNetOccurrenceLocator.isValid())
        return _hyperNetRootNetOccurrenceLocator.getElement();
    else
        return Occurrence();

}

Locator<Occurrence>* Cell_HyperNetRootNetOccurrences::Locator::getClone() const
// **************************************************************
{
    return new Locator(*this);
}

bool Cell_HyperNetRootNetOccurrences::Locator::isValid() const
// **********************************************
{
    return (_netLocator.isValid() || (_hyperNetRootNetOccurrenceLocator.isValid()));
}

void Cell_HyperNetRootNetOccurrences::Locator::progress()
// *****************************************
{
    if (_netLocator.isValid())
    {
      _netLocator.progress();
      while ( _netLocator.isValid() ) {
        if (   not dynamic_cast<DeepNet*>(_netLocator.getElement())
           and isHyperNetRootNetOccurrence(Occurrence(_netLocator.getElement(),_path))) break;

        _netLocator.progress();
      }
    }
    else if (_hyperNetRootNetOccurrenceLocator.isValid())
        _hyperNetRootNetOccurrenceLocator.progress();

    if (!_netLocator.isValid())
        while (!_hyperNetRootNetOccurrenceLocator.isValid() && _instanceLocator.isValid())
        {
            Instance* instance = _instanceLocator.getElement();
            _hyperNetRootNetOccurrenceLocator=Locator(instance->getMasterCell(),Path(_path,instance));
            _instanceLocator.progress();
        }

}

string Cell_HyperNetRootNetOccurrences::Locator::_getString() const
// ***************************************************
{
    string s = "<" + _TName("Cell::HyperNetRootNetOccurrences::Locator");
    if (!_path.isEmpty())
        s += " " + _path.getName();
    s += ">";
    return s;
}

// ****************************************************************************************************
// Cell_SubCells implementation
// ****************************************************************************************************

Cell_SubCells::Cell_SubCells()
// ***************************
:     Inherit(),
    _cell(NULL)
{
}

Cell_SubCells::Cell_SubCells(const Cell* cell)
// *******************************************
:     Inherit(),
    _cell(cell)
{
}

Cell_SubCells::Cell_SubCells(const Cell_SubCells& subCells)
// ********************************************************
:     Inherit(),
    _cell(subCells._cell)
{
}

Cell_SubCells& Cell_SubCells::operator=(const Cell_SubCells& subCells)
// *******************************************************************
{
    _cell = subCells._cell;
    return *this;
}

Collection<Cell*>* Cell_SubCells::getClone() const
// ***********************************************
{
    return new Cell_SubCells(*this);
}

Locator<Cell*>* Cell_SubCells::getLocator() const
// **********************************************
{
    return new Locator(_cell->getInstances().getLocator());
}

string Cell_SubCells::_getString() const
// *************************************
{
    string s = "<" + _TName("Cell::SubCells");
    if (_cell) {
        s += " " + getString(_cell);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Cell_SubCells::Locator implementation
// ****************************************************************************************************

Cell_SubCells::Locator::Locator()
// **********************************
:    Inherit(),
    _cellSet(),
    _instanceLocator()
{
}

Cell_SubCells::Locator::Locator(InstanceLocator instanceLocator)
// *************************************************************
:    Inherit(),
    _cellSet(),
    _instanceLocator(instanceLocator)
{
    if ( isValid() )
        _cellSet.insert ( _instanceLocator.getElement()->getMasterCell() );
}

Cell_SubCells::Locator::Locator(const Locator& locator)
// ****************************************************
:    Inherit(),
    _cellSet(locator._cellSet),
    _instanceLocator(locator._instanceLocator)
{
}

Cell_SubCells::Locator& Cell_SubCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
    _cellSet = locator._cellSet;
    _instanceLocator = locator._instanceLocator;
    return *this;
}

Cell* Cell_SubCells::Locator::getElement() const
// *********************************************
{
    return _instanceLocator.getElement()->getMasterCell();
}

Locator<Cell*>* Cell_SubCells::Locator::getClone() const
// *****************************************************
{
    return new Locator(*this);
}

bool Cell_SubCells::Locator::isValid() const
// *****************************************
{
    return _instanceLocator.isValid();
}

void Cell_SubCells::Locator::progress()
// ************************************
{
    while ( isValid() && ( _cellSet.find ( _instanceLocator.getElement()->getMasterCell() ) != _cellSet.end() ) )
        _instanceLocator.progress();
    if ( isValid() )
        _cellSet.insert ( _instanceLocator.getElement()->getMasterCell() );
}

string Cell_SubCells::Locator::_getString() const
// **************************************************
{
    string s = "<" + _TName("Cell::SubCells::Locator");
    s += " " + _getString();
    s += " " + getString(_instanceLocator);
    s += ">";
    return s;
}




// x-----------------------------------------------------------------x
// |    "::Cell_RecursiveSlavePathes::Locator" Class Definitions     |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Constructor  :  "Cell_RecursiveSlavePathes::Locator::Locator ()".

Cell_RecursiveSlavePathes::Locator::Locator ( const Cell* cell )
  : Hurricane::Locator<Path>()
  , _instancesStack()
  , _instancePath()
  , _isInPop(false)
{
  const Cell* topCell = cell;

  do {
    Instances slaveInstances = topCell->getSlaveInstances();

    if ( !slaveInstances.isEmpty() ) {
      _instancesStack.push_back ( *slaveInstances.getLocator() );
      _instancePath = Path ( _instancesStack.back().getElement(), _instancePath );

//       cerr << "Instances of I: " << topCell->getName() << endl;
//       cerr << "Head         I: " << _instancesStack.back().getElement()->getName() << endl;
//       cerr << "TailPath     I: " << _instancePath.getName() << endl;

      topCell = _instancesStack.back().getElement()->getCell();
    } else
      topCell = NULL;

  } while ( topCell );
}




// -------------------------------------------------------------------
// Modifier  :  "Cell_RecursiveSlavePathes::Locator::progress ()".

void  Cell_RecursiveSlavePathes::Locator::progress ()
{
  if ( !isValid() ) return;

  _instancesStack.back().progress();
  if ( _instancesStack.back().isValid() ) {
//     cerr << "Head         N: " << _instancesStack.back().getElement()->getName() << endl;
//     cerr << "TailPath     N: " << _instancePath.getTailPath().getName() << endl;
    _instancePath = Path ( _instancesStack.back().getElement(), _instancePath.getTailPath() );

    Cell* topCell = _instancesStack.back().getElement()->getCell();

    do {
      Instances slaveInstances = topCell->getSlaveInstances();

      if ( !slaveInstances.isEmpty() ) {
        _instancesStack.push_back ( *slaveInstances.getLocator() );
//         cerr << "Instances of R: " << topCell->getName() << endl;
//         cerr << "Head         R: " << _instancesStack.back().getElement()->getName() << endl;
//         cerr << "TailPath     R: " << _instancePath.getName() << endl;
        _instancePath = Path ( _instancesStack.back().getElement(), _instancePath );

        topCell = _instancesStack.back().getElement()->getCell();
      } else
        topCell = NULL;
    } while ( topCell );

    return;
  } else {
    _instancesStack.pop_back();
    _instancePath = _instancePath.getTailPath();
//     cerr << "TailPath     F: " << _instancePath.getName() << endl;
    return;
  }
}




// -------------------------------------------------------------------
// Hurricane Management  :  "Cell_RecursiveSlavePathes::Locator::_getString ()".

string  Cell_RecursiveSlavePathes::Locator::_getString () const
{
  Name  masterCellName = "Finished";

  if ( isValid() )
    masterCellName = _instancesStack.front().getElement()->getMasterCell()->getName();

  string s = "<" + _TName("Cell_RecursiveSlavePathes::Locator")
                 + getString(masterCellName)
                 + ">";

  return s;
}




// x-----------------------------------------------------------------x
// |        "::Cell_RecursiveSlavePathes" Class Definitions          |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Hurricane Management  :  "Cell_RecursiveSlavePathes::_getString ()".

string  Cell_RecursiveSlavePathes::_getString () const
{
  string s = "<" + _TName("Cell_RecursiveSlavePathes") + " "
                 + getString(_cell) + " "
                 + ">";

  return s;
}




} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
