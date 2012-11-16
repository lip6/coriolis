
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BasicLayer.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "hurricane/BasicLayer.h"
# include  "hurricane/Technology.h"
# include  "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::BasicLayer_BasicLayers".


  class BasicLayer_BasicLayers : public Collection<BasicLayer*> {

    public:
    // Subclass: Locator.
      class Locator : public Hurricane::Locator<BasicLayer*> {
        public:
        // Constructors.
                                                   Locator    ( const BasicLayer* basicLayer = NULL );
                                                   Locator    ( const Locator& locator );
        // Methods.
                  Locator&                         operator=  ( const Locator& locator );
          virtual BasicLayer*                      getElement () const;
          virtual Hurricane::Locator<BasicLayer*>* getClone   () const;
          virtual bool                             isValid    () const;
          virtual void                             progress   ();
          virtual string                           _getString () const;

        private:
        // Internal: Attributes.
                  const BasicLayer*                _basicLayer;
    };

    public:
    // Constructors.
                                               BasicLayer_BasicLayers ( const BasicLayer* basicLayer = NULL );
                                               BasicLayer_BasicLayers ( const BasicLayer_BasicLayers& );
    // Methods.
              BasicLayer_BasicLayers&          operator=              ( const BasicLayer_BasicLayers& );
      virtual Collection<BasicLayer*>*         getClone               () const;
      virtual Hurricane::Locator<BasicLayer*>* getLocator             () const;
      virtual string                           _getString             () const;

    private:
    // Internal: Attributes.
              const BasicLayer*                _basicLayer;
  };


  BasicLayer_BasicLayers::BasicLayer_BasicLayers ( const BasicLayer* basicLayer )
    : Collection<BasicLayer*>()
    , _basicLayer(basicLayer)
  { }


  BasicLayer_BasicLayers::BasicLayer_BasicLayers ( const BasicLayer_BasicLayers& basicLayers )
    : Collection<BasicLayer*>()
    , _basicLayer(basicLayers._basicLayer)
  { }


  BasicLayer_BasicLayers& BasicLayer_BasicLayers::operator= ( const BasicLayer_BasicLayers& basicLayers )
  {
    _basicLayer = basicLayers._basicLayer;
    return *this;
  }


  Collection<BasicLayer*>* BasicLayer_BasicLayers::getClone () const
  { return new BasicLayer_BasicLayers(*this); }


  Locator<BasicLayer*>* BasicLayer_BasicLayers::getLocator () const
  { return new Locator(_basicLayer); }


  string BasicLayer_BasicLayers::_getString () const
  {
    string s = "<" + _TName("BasicLayer::BasicLayers");
    if (_basicLayer) s += " " + getString(_basicLayer);
    s += ">";
    return s;
  }


  BasicLayer_BasicLayers::Locator::Locator ( const BasicLayer* basicLayer )
    : Hurricane::Locator<BasicLayer*>()
    , _basicLayer(basicLayer)
  { }


  BasicLayer_BasicLayers::Locator::Locator ( const Locator& locator )
    : Hurricane::Locator<BasicLayer*>()
    , _basicLayer(locator._basicLayer)
  { }


  BasicLayer_BasicLayers::Locator& BasicLayer_BasicLayers::Locator::operator= ( const Locator& locator )
  {
    _basicLayer = locator._basicLayer;
    return *this;
  }


  BasicLayer* BasicLayer_BasicLayers::Locator::getElement () const
  { return (BasicLayer*)_basicLayer; }


  Locator<BasicLayer*>* BasicLayer_BasicLayers::Locator::getClone () const
  { return new Locator(*this); }


  bool BasicLayer_BasicLayers::Locator::isValid () const
  { return (_basicLayer != NULL); }


  void BasicLayer_BasicLayers::Locator::progress ()
  { _basicLayer = NULL; }


  string BasicLayer_BasicLayers::Locator::_getString () const
  {
    string s = "<" + _TName("BasicLayer::BasicLayers::Locator");
    if (_basicLayer) s += " " + getString(_basicLayer);
    s += ">";
    return s;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::BasicLayer".


  BasicLayer::BasicLayer ( Technology*      technology
                         , const Name&      name
                         , const Material&  material
                         , unsigned         extractNumber
                         , const DbU::Unit& minimalSize
                         , const DbU::Unit& minimalSpacing
                         ) : Layer(technology
                                  ,name
                                  ,minimalSize
                                  ,minimalSpacing)
                           ,_material      (material)
                           ,_extractNumber (extractNumber)
                           ,_blockageLayer (NULL)
                           ,_realName      ("<not associated>")
  { }


  BasicLayer* BasicLayer::create ( Technology*      technology
                                 , const Name&      name
                                 , const Material&  material
                                 , unsigned         extractNumber
                                 , const DbU::Unit& minimalSize
                                 , const DbU::Unit& minimalSpacing
                                 )
  {
    BasicLayer* basicLayer =
      new BasicLayer(technology, name, material, extractNumber, minimalSize, minimalSpacing);

    basicLayer->_postCreate();

    return basicLayer;
  }


  BasicLayer* BasicLayer::getBlockageLayer () const
  { return _blockageLayer; }
 

  BasicLayers BasicLayer::getBasicLayers () const
  { return BasicLayer_BasicLayers(this); }


  void BasicLayer::_postCreate ()
  {
    Mask basicLayersMask = 0;
    forEach ( BasicLayer*, basicLayer, getTechnology()->getBasicLayers() ) {
      basicLayersMask |= (*basicLayer)->getMask();
    }

    Mask mask = 1;
    while (mask && (mask & basicLayersMask)) mask = mask.lshift(1);

    if (!mask)
      throw Error("Can't create " + _TName("BasicLayer") + " : mask capacity overflow");

    _setMask(mask);
    if ( _material == Material::metal ) getTechnology()->_getMetalMask().set(getMask());
    if ( _material == Material::cut   ) getTechnology()->_getCutMask  ().set(getMask());

    if (_extractNumber) {
      Mask extractMask = (1 << _extractNumber);

      if (!extractMask)
        throw Error("Can't create " + _TName("BasicLayer") + " : extract mask capacity overflow");

      _setExtractMask(extractMask);
    }

    Layer::_postCreate();
  }


  void BasicLayer::_preDestroy ()
  {
    if ( _material == Material::metal ) getTechnology()->_getMetalMask().unset(getMask());
    if ( _material == Material::cut   ) getTechnology()->_getCutMask  ().unset(getMask());
    Layer::_preDestroy();
  }


  BasicLayer* BasicLayer::_getSymbolicBasicLayer ()
  { return this; }


  string  BasicLayer::_getTypeName () const
  { return _TName("BasicLayer"); }


  string BasicLayer::_getString () const
  {
    string s = Layer::_getString();
  // s.insert(s.length() - 1, " " + getString(_type));
    return s;
  }


  Record* BasicLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("Material", &_material));
      record->add(getSlot("RealName", &_realName));
    }
    return record;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::BasicLayer::Material".


  BasicLayer::Material::Material (const Code& code )
     : _code(code)
  { }


  BasicLayer::Material::Material ( const Material& material )
     : _code(material._code)
  { }


  BasicLayer::Material& BasicLayer::Material::operator= ( const Material& material )
  {
    _code = material._code;
    return *this;
  }


  string BasicLayer::Material::_getString () const
  { return getString(&_code); }


  Record* BasicLayer::Material::_getRecord () const
  {
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", &_code));
    return record;
  }


} // End of Hurricane namespace.
