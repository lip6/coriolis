// ****************************************************************************************************
// File: ./Technology.cpp
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

#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Error.h"



namespace Hurricane {



// ****************************************************************************************************
// Technology_BasicLayers declaration
// ****************************************************************************************************

class Technology_BasicLayers : public Collection<BasicLayer*> {
// **********************************************************

// Types
// *****

    public: typedef Collection<BasicLayer*> Inherit;

    public: class Locator : public Hurricane::Locator<BasicLayer*> {
    // ***********************************************************

        public: typedef Hurricane::Locator<BasicLayer*> Inherit;

        private: const Technology* _technology;
        private: Layer::Mask _mask;
        private: BasicLayerLocator _basicLayerLocator;

        public: Locator();
        public: Locator(const Technology* technology, const Layer::Mask& mask);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual BasicLayer* getElement() const;
        public: virtual Hurricane::Locator<BasicLayer*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Technology* _technology;
    private: Layer::Mask _mask;

// Constructors
// ************

    public: Technology_BasicLayers();
    public: Technology_BasicLayers(const Technology* technology, const Layer::Mask& mask);
    public: Technology_BasicLayers(const Technology_BasicLayers& basicLayers);

// Operators
// *********

    public: Technology_BasicLayers& operator=(const Technology_BasicLayers& basicLayers);

// Accessors
// *********

    public: virtual Collection<BasicLayer*>* getClone() const;
    public: virtual Hurricane::Locator<BasicLayer*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};

// ****************************************************************************************************
// Technology implementation
// ****************************************************************************************************

Technology::Technology(DataBase* dataBase, const Name& name)
// *********************************************************
:    Inherit(),
    _dataBase(dataBase),
    _name(name),
    _layerMap(),
    _layerMaskMap()
{
    if (!_dataBase)
        throw Error("Can't create " + _TName("Technology") + " : null data base");

    if (_dataBase->getTechnology())
        throw Error("Can't create " + _TName("Technology") + " : already exists");

    if (_name.isEmpty())
        throw Error("Can't create " + _TName("Technology") + " : empty name");
}

Technology* Technology::create(DataBase* dataBase, const Name& name)
// *****************************************************************
{
    Technology* technology = new Technology(dataBase, name);

    technology->_postCreate();

    return technology;
}

// Layer* Technology::getLayer ( Layer::Mask mask ) const
// // ***************************************************
// {
//   LayerMaskMap::const_iterator ilayer = _layerMaskMap.find(mask);
//   if ( ilayer == _layerMaskMap.end() )
//     return NULL;

//   return ilayer->second;
// }

BasicLayer* Technology::getBasicLayer(const Name& name) const
// **********************************************************
{
    Layer* layer = getLayer(name);
    return (layer && dynamic_cast<BasicLayer*>(layer)) ? (BasicLayer*)layer : NULL;
}

RegularLayer* Technology::getRegularLayer(const Name& name) const
// **********************************************************
{
    Layer* layer = getLayer(name);
    return (layer && dynamic_cast<RegularLayer*>(layer)) ? (RegularLayer*)layer : NULL;
}

ViaLayer* Technology::getViaLayer(const Name& name) const
// ******************************************************************
{
    Layer* layer = getLayer(name);
    return (layer && dynamic_cast<ViaLayer*>(layer)) ? (ViaLayer*)layer : NULL;
}

BasicLayers Technology::getBasicLayers() const
// *******************************************
{
    // return getLayers().getSubSet<BasicLayer*>();
    return SubTypeCollection<Layer*, BasicLayer*>(getLayers());
}

BasicLayers Technology::getBasicLayers(const Layer::Mask& mask) const
// ******************************************************************
{
    return Technology_BasicLayers(this, mask);
}

RegularLayers Technology::getRegularLayers() const
// ***************************************************
{
    return SubTypeCollection<Layer*, RegularLayer*>(getLayers());
}

ViaLayers Technology::getViaLayers() const
// ***************************************************
{
    return SubTypeCollection<Layer*, ViaLayer*>(getLayers());
}


  Layer* Technology::getLayer ( const Layer::Mask& mask, bool useWorking ) const
  {
    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound ( mask );
    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound ( mask );
    for ( ; lb != ub ; lb++ ) {
      if ( !useWorking || lb->second->isWorking() ) return lb->second;
    }
    return NULL;
  }


  Layer* Technology::getMetalAbove ( const Layer* layer, bool useWorking ) const
  {
    if ( !layer ) return NULL;

    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound ( layer->getMask() );
    for ( ; ub != _layerMaskMap.end() ; ub++ ) {
      if (    _metalMask.contains(ub->second->getMask())
         && ( !useWorking || ub->second->isWorking() ) )
        return ub->second;
    }
    return NULL;
  }


  Layer* Technology::getMetalBelow ( const Layer* layer, bool useWorking ) const
  {
    if ( !layer ) return NULL;

    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound ( layer->getMask() );
    if ( lb->second == layer ) lb--;
    for ( ; lb != _layerMaskMap.begin() ; lb-- ) {
      if (    _metalMask.contains(lb->second->getMask())
         && ( !useWorking || lb->second->isWorking() ) )
        return lb->second;
    }
    return NULL;
  }


  Layer* Technology::getCutAbove ( const Layer* layer, bool useWorking ) const
  {
    if ( !layer ) return NULL;

    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound ( layer->getMask() );
    for ( ; ub != _layerMaskMap.end() ; ub++ ) {
      if (    _cutMask.contains(ub->second->getMask())
         && ( !useWorking || ub->second->isWorking() ) )
        return ub->second;
    }
    return NULL;
  }


  Layer* Technology::getCutBelow ( const Layer* layer, bool useWorking ) const
  {
    if ( !layer ) return NULL;

    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound ( layer->getMask() );
    if ( lb->second == layer ) lb--;
    for ( ; lb != _layerMaskMap.begin() ; lb-- ) {
      if (    _cutMask.contains(lb->second->getMask())
         && ( !useWorking || lb->second->isWorking() ) )
        return lb->second;
    }
    return NULL;
  }


  Layer* Technology::getViaBetween ( const Layer* metal1, const Layer* metal2 ) const
  {
    if ( !metal1 || !metal2 ) return NULL;
    if (  metal1 ==  metal2 ) return const_cast<Layer*>(metal1);
    if ( metal1->above(metal2) ) swap ( metal1, metal2 );

    Layer* cutLayer = getCutBelow ( metal2 );
    if ( !cutLayer ) return NULL;

    return getLayer ( metal1->getMask() | metal2->getMask() | cutLayer->getMask() ); 
  }


  Layer* Technology::getNthMetal ( int nth ) const
  {
    return getLayer ( _metalMask.nthbit(nth) );
  }


  void  Technology::_onDbuChange ( float scale )
  {
    forEach ( Layer*, layer, getLayers() )
      layer->_onDbuChange ( scale );
  }


void Technology::setName(const Name& name)
// ***************************************
{
    if (name != _name) {
        if (name.isEmpty())
            throw Error("Can't change technology name : empty name");

        _name = name;
    }
}


  bool  Technology::setWorkingLayer ( const Name& name )
  {
    Layer* layer = getLayer ( name );
    if ( !layer ) return false;

    return setWorkingLayer ( layer );
  }


  bool  Technology::setWorkingLayer ( const Layer* layer )
  {
    bool  found = false;
    LayerMaskMap::iterator lb = _layerMaskMap.lower_bound ( layer->getMask() );
    LayerMaskMap::iterator ub = _layerMaskMap.upper_bound ( layer->getMask() );
    for ( ; lb != ub ; lb++ ) {
      if ( lb->second == layer ) {
        lb->second->setWorking ( true );
        found = true;
      } else
        lb->second->setWorking ( false );
    }
    return found;
  }


void Technology::_postCreate()
// ***************************
{
    Inherit::_postCreate();

    _dataBase->_setTechnology(this);
}

void Technology::_preDestroy()
// **************************
{

    while ( !_layerMap.isEmpty() ) {
      _layerMap.getElements().getFirst()->destroy();
    }
  //for_each_layer(layer, getLayers()) layer->destroy(); end_for;

    _dataBase->_setTechnology(NULL);
    DBo::_preDestroy();
}

void Technology::_insertInLayerMaskMap ( Layer* layer )
// ****************************************************
{
  _layerMaskMap.insert ( make_pair(layer->getMask(),layer) );
}

void Technology::_removeFromLayerMaskMap ( Layer* layer )
// ******************************************************
{
  LayerMaskMap::iterator lb = _layerMaskMap.lower_bound ( layer->getMask() );
  LayerMaskMap::iterator ub = _layerMaskMap.upper_bound ( layer->getMask() );

  for ( ; lb != ub ; lb++ ) {
    if ( lb->second == layer ) {
      _layerMaskMap.erase ( lb );
      break;
    }
  }
}


string  Technology::_getTypeName () const
{
  return _TName("Technology");
}



string Technology::_getString() const
// **********************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    return s;
}

Record* Technology::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
      record->add(getSlot("DataBase", _dataBase));
      record->add(getSlot("Name", &_name));
      record->add(getSlot("Layers", &_layerMaskMap));
      record->add(getSlot("cutMask", &_cutMask));
      record->add(getSlot("metalMask", &_metalMask));
    }
    return record;
}

// ****************************************************************************************************
// Technology::LayerMap implementation
// ****************************************************************************************************

Technology::LayerMap::LayerMap()
// *****************************
:    Inherit()
{
}

Name Technology::LayerMap::_getKey(Layer* layer) const
// ***************************************************
{
    return layer->getName();
}

unsigned Technology::LayerMap::_getHashValue(Name name) const
// **********************************************************
{
    return ( (unsigned int)( (unsigned long)name._getSharedName() ) ) / 8;
}

Layer* Technology::LayerMap::_getNextElement(Layer* layer) const
// *************************************************************
{
    return layer->_getNextOfTechnologyLayerMap();
}

void Technology::LayerMap::_setNextElement(Layer* layer, Layer* nextLayer) const
// *****************************************************************************
{
    layer->_setNextOfTechnologyLayerMap(nextLayer);
};



// ****************************************************************************************************
// Technology_BasicLayers implementation
// ****************************************************************************************************

Technology_BasicLayers::Technology_BasicLayers()
// *********************************************
:     Inherit(),
    _technology(NULL),
    _mask(0)
{
}

Technology_BasicLayers::Technology_BasicLayers(const Technology* technology, const Layer::Mask& mask)
// **************************************************************************************************
:     Inherit(),
    _technology(technology),
    _mask(mask)
{
}

Technology_BasicLayers::Technology_BasicLayers(const Technology_BasicLayers& basicLayers)
// **************************************************************************************
:     Inherit(),
    _technology(basicLayers._technology),
    _mask(basicLayers._mask)
{
}

Technology_BasicLayers& Technology_BasicLayers::operator=(const Technology_BasicLayers& basicLayers)
// *************************************************************************************************
{
    _technology = basicLayers._technology;
    _mask = basicLayers._mask;
    return *this;
}

Collection<BasicLayer*>* Technology_BasicLayers::getClone() const
// **************************************************************
{
    return new Technology_BasicLayers(*this);
}

Locator<BasicLayer*>* Technology_BasicLayers::getLocator() const
// *************************************************************
{
    return new Locator(_technology, _mask);
}

string Technology_BasicLayers::_getString() const
// **********************************************
{
    string s = "<" + _TName("Technology::BasicLayers");
    if (_technology) {
        s += " " + getString(_technology);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Technology_BasicLayers::Locator implementation
// ****************************************************************************************************

Technology_BasicLayers::Locator::Locator()
// ***************************************
:    Inherit(),
    _technology(NULL),
    _mask(0),
    _basicLayerLocator()
{
}

Technology_BasicLayers::Locator::Locator(const Technology* technology, const Layer::Mask& mask)
// ********************************************************************************************
:    Inherit(),
    _technology(technology),
    _mask(mask),
    _basicLayerLocator()
{
    if (_technology) {
        _basicLayerLocator = _technology->getBasicLayers().getLocator();
        while (_basicLayerLocator.isValid() && !(_basicLayerLocator.getElement()->getMask() & _mask))
            _basicLayerLocator.progress();
    }
}

Technology_BasicLayers::Locator::Locator(const Locator& locator)
// *************************************************************
:    Inherit(),
    _technology(locator._technology),
    _mask(locator._mask),
    _basicLayerLocator(locator._basicLayerLocator)
{
}

Technology_BasicLayers::Locator& Technology_BasicLayers::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
    _technology = locator._technology;
    _mask = locator._mask;
    _basicLayerLocator = locator._basicLayerLocator;
    return *this;
}

BasicLayer* Technology_BasicLayers::Locator::getElement() const
// ************************************************************
{
    return _basicLayerLocator.getElement();
}

Locator<BasicLayer*>* Technology_BasicLayers::Locator::getClone() const
// ********************************************************************
{
    return new Locator(*this);
}

bool Technology_BasicLayers::Locator::isValid() const
// **************************************************
{
    return _basicLayerLocator.isValid();
}

void Technology_BasicLayers::Locator::progress()
// *********************************************
{
    do {
        _basicLayerLocator.progress();
    }
    while (_basicLayerLocator.isValid() && !(_basicLayerLocator.getElement()->getMask() & _mask));
}

string Technology_BasicLayers::Locator::_getString() const
// *******************************************************
{
    string s = "<" + _TName("Technology::BasicLayers::Locator");
    if (_technology) {
        s += " " + getString(_technology);
        s += " " + getString(_mask);
    }
    s += ">";
    return s;
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
