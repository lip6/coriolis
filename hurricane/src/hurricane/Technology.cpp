// ****************************************************************************************************
// File: ./Technology.cpp
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

#include "hurricane/Warning.h"
#include "hurricane/Error.h"
#include "hurricane/SharedName.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Error.h"


namespace {

  class CompareByMask {
    public:
      bool operator() ( const Hurricane::Layer* lhs, const Hurricane::Layer* rhs )
      {
        if (not lhs) return rhs;
        if (not rhs) return false;

        return lhs->getMask() < rhs->getMask();
      }
  };

}  // Anonymous namespace.


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
// ***********************************************
{
    return SubTypeCollection<Layer*, RegularLayer*>(getLayers());
}

ViaLayers Technology::getViaLayers() const
// ***************************************
{
    return SubTypeCollection<Layer*, ViaLayer*>(getLayers());
}


  Layer* Technology::getLayer ( const Layer::Mask& mask, bool useSymbolic ) const
  {
    Layer* layer = NULL;
    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound( mask );
    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound( mask );
    for ( ; lb != ub ; lb++ ) {
      layer = lb->second;
      if (not useSymbolic or layer->isSymbolic()) return layer;
    }
    return layer;
  }


  Layer* Technology::getMetalAbove ( const Layer* layer, bool useSymbolic ) const
  {
    if (not layer) return NULL;

    Layer* above = NULL;
    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound( layer->getMask() );
    for ( ; ub != _layerMaskMap.end() ; ub++ ) {
      if (_metalMask.contains(ub->second->getMask())) {
        above = ub->second;
        if (not useSymbolic or above->isSymbolic()) return above;
      }
    }
    return above;
  }


  Layer* Technology::getMetalBelow ( const Layer* layer, bool useSymbolic ) const
  {
    if (not layer) return NULL;

    Layer* below = NULL;
    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound( layer->getMask() );
    if (lb->second->getMask() == layer->getMask()) lb--;
    for ( ; lb != _layerMaskMap.begin() ; lb-- ) {
      if (_metalMask.contains(lb->second->getMask())) {
        below = lb->second;
        if (not useSymbolic or below->isSymbolic()) return below;
      }
    }
    return below;
  }


  Layer* Technology::getCutAbove ( const Layer* layer, bool useSymbolic ) const
  {
    if (not layer) return NULL;

    Layer* cut = NULL;
    LayerMaskMap::const_iterator ub = _layerMaskMap.upper_bound( layer->getMask() );
    for ( ; ub != _layerMaskMap.end() ; ub++ ) {
      if (_cutMask.contains(ub->second->getMask())) {
        cut = ub->second;
        if (not useSymbolic or cut->isSymbolic()) return cut;
      }
    }
    return cut;
  }


  Layer* Technology::getCutBelow ( const Layer* layer, bool useSymbolic ) const
  {
    if (not layer) return NULL;

    Layer* cut = NULL;
    LayerMaskMap::const_iterator lb = _layerMaskMap.lower_bound( layer->getMask() );
    if (lb->second->getMask() == layer->getMask()) lb--;
    for ( ; lb != _layerMaskMap.begin() ; lb-- ) {
      if (_cutMask.contains(lb->second->getMask())) {
        cut = lb->second;
        if (not useSymbolic or cut->isSymbolic()) return cut;
      }
    }
    return cut;
  }


  Layer* Technology::getViaBetween ( const Layer* metal1, const Layer* metal2, bool useSymbolic ) const
  {
    if (not metal1 or not metal2) return NULL;
    if (metal1 ==  metal2) return const_cast<Layer*>(metal1);
    if (metal1->above(metal2)) swap( metal1, metal2 );

    Layer* cutLayer = getCutBelow( metal2, false );
    if (not cutLayer) return NULL;

    return getLayer( metal1->getMask() | metal2->getMask() | cutLayer->getMask(), useSymbolic ); 
  }


  Layer* Technology::getNthMetal ( int nth ) const
  { return getLayer ( _metalMask.nthbit(nth) ); }


  Layer* Technology::getNthCut ( int nth ) const
  { return getLayer ( _cutMask.nthbit(nth) ); }


  void  Technology::_onDbuChange ( float scale )
  { for ( Layer* layer : getLayers() ) layer->_onDbuChange( scale ); }


void Technology::setName(const Name& name)
// ***************************************
{
    if (name != _name) {
        if (name.isEmpty())
            throw Error("Can't change technology name : empty name");

        _name = name;
    }
}


  bool  Technology::setSymbolicLayer ( const Name& name )
  {
    Layer* layer = getLayer( name );
    if (not layer) return false;

    return setSymbolicLayer( layer );
  }


  bool  Technology::setSymbolicLayer ( const Layer* layer )
  {
    bool  found = false;
    LayerMaskMap::iterator lb = _layerMaskMap.lower_bound( layer->getMask() );
    LayerMaskMap::iterator ub = _layerMaskMap.upper_bound( layer->getMask() );
    for ( ; lb != ub ; lb++ ) {
      if (lb->second == layer) {
        lb->second->setSymbolic( true );
        found = true;
      } else
        lb->second->setSymbolic( false );
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

    while ( not _layerMap.isEmpty() ) {
      _layerMap.getElements().getFirst()->destroy();
    }

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
// **************************************
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

void Technology::_toJson(JsonWriter* writer) const
// ***********************************************
{
  Inherit::_toJson( writer );

  jsonWrite( writer, "_name", getName() );
}

void  Technology::_toJsonCollections(JsonWriter* writer) const
// ***********************************************************
{
  writer->key( "+layers" );
  writer->startArray();

  vector<BasicLayer*> basicLayers;
  vector<Layer*>      layers;
  for ( Layer* layer : getLayers() ) {
    BasicLayer* basicLayer = dynamic_cast<BasicLayer*>( layer );
    if (basicLayer) basicLayers.push_back( basicLayer );
    else            layers     .push_back( layer );
  }

  sort( basicLayers.begin(), basicLayers.end(), CompareByMask() );
  sort( layers     .begin(), layers     .end(), CompareByMask() );

  for ( BasicLayer* basicLayer : basicLayers ) jsonWrite( writer, basicLayer );
  for ( Layer*      layer      : layers      ) jsonWrite( writer, layer );

  writer->endArray();
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
  return name._getSharedName()->getId() / 8;
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



// ****************************************************************************************************
// JsonTechnology implementation
// ****************************************************************************************************

Initializer<JsonTechnology>  jsonTechnologyInit ( 0 );

void  JsonTechnology::initialize()
// *******************************
{ JsonTypes::registerType( new JsonTechnology (JsonWriter::RegisterMode) ); }

JsonTechnology::JsonTechnology(unsigned long flags)
// ************************************************
  : JsonDBo      (flags)
  , _blockagesMap()
{
  if (flags & JsonWriter::RegisterMode) return;

  cdebug_log(19,0) << "JsonTechnology::JsonTechnology()" << endl;

  add( "_name"  , typeid(string)    );
  add( "+layers", typeid(JsonArray) );
}

JsonTechnology::~JsonTechnology()
// ******************************
{
  const Technology* techno = getObject<Technology*>();

  for ( auto element : _blockagesMap ) {
    BasicLayer* blockage = techno->getBasicLayer( element.first );
    if (blockage) {
      for ( BasicLayer* layer : element.second ) {
        layer->setBlockageLayer( blockage );
      }
    }
  }
}

string  JsonTechnology::getTypeName() const
// ****************************************
{ return "Technology"; }

JsonTechnology* JsonTechnology::clone(unsigned long flags) const
// *************************************************************
{ return new JsonTechnology ( flags ); }

void JsonTechnology::addBlockageRef(const string& blockageLayer, BasicLayer* layer )
// *********************************************************************************
{
  map< string, vector<BasicLayer*> >::iterator im = _blockagesMap.find( blockageLayer );
  if (im != _blockagesMap.end()) {
    (*im).second.push_back( layer );
  } else {
    _blockagesMap.insert( make_pair( blockageLayer, vector<BasicLayer*>(1,layer) ) );
  }
}

void JsonTechnology::toData(JsonStack& stack)
// ******************************************
{
  cdebug_tabw(19,1);

  check( stack, "JsonTechnology::toData" );

  string      technoName = get<string>( stack, "_name" );
  Technology* techno     = DataBase::getDB()->getTechnology();

  if (techno) {
    if (techno->getName() == technoName) {
      cerr << Warning( "JsonTechnology::toData(): A technology with the same name (%s) is already loaded."
                     , technoName.c_str()
                     ) << endl;
    } else {
      cerr << Error( "JsonTechnology::toData(): Try to load \"%s\", but \"%s\" is already loaded."
                   , technoName.c_str()
                   , getString(techno->getName()).c_str()
                   ) << endl;
    }
  } else {
    techno = Technology::create( DataBase::getDB(), technoName );
    stack.setFlags( JsonWriter::TechnoMode );
  }

  update( stack, techno );

  cdebug_tabw(19,-1);
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
