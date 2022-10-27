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
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BasicLayer.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"


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
                         , unsigned         gds2Layer
                         , unsigned         gds2Datatype
                         , const DbU::Unit& minimalSize
                         , const DbU::Unit& minimalSpacing
                         ) : Layer(technology
                                  ,name
                                  ,minimalSize
                                  ,minimalSpacing)
                           ,_material      (material)
                           ,_gds2Layer     (gds2Layer)
                           ,_gds2Datatype  (gds2Datatype)
                           ,_blockageLayer (NULL)
                           ,_realName      ("<not associated>")
                           ,_hasGds        (false)
  { }


  BasicLayer* BasicLayer::create ( Technology*      technology
                                 , const Name&      name
                                 , const Material&  material
                                 , unsigned         gds2Layer
                                 , unsigned         gds2Datatype
                                 , const DbU::Unit& minimalSize
                                 , const DbU::Unit& minimalSpacing
                                 )
  {
    BasicLayer* basicLayer =
      new BasicLayer(technology, name, material, gds2Layer, gds2Datatype, minimalSize, minimalSpacing);

    basicLayer->_postCreate();

    return basicLayer;
  }


  BasicLayer* BasicLayer::getBlockageLayer () const
  { return _blockageLayer; }
 

  BasicLayers BasicLayer::getBasicLayers () const
  { return BasicLayer_BasicLayers(this); }


  const Layer* BasicLayer::getTop () const
  { return this; }


  const Layer* BasicLayer::getBottom () const
  { return this; }


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

    if (_gds2Layer) {
      Mask extractMask = (1 << _gds2Layer);

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
      record->add(getSlot("_material"     , &_material));
      record->add(getSlot("_realName"     , &_realName));
      record->add(getSlot("_blockageLayer",  _blockageLayer));
      record->add(getSlot("_gds2Layer"    ,  _gds2Layer));
      record->add(getSlot("_gds2Datatype" ,  _gds2Datatype));
    }
    return record;
  }


  void  BasicLayer::_toJson ( JsonWriter* writer ) const
  {
    Super::_toJson( writer );

    jsonWrite( writer, "_material"     , getString(&_material) );
    jsonWrite( writer, "_gds2Layer"    , _gds2Layer            );
    jsonWrite( writer, "_gds2Datatype" , _gds2Datatype         );
    jsonWrite( writer, "_realName"     , _realName             );
    if (_blockageLayer) {
      jsonWrite( writer, "_blockageLayer", _blockageLayer->getName() );
    } else {
      jsonWrite( writer, "_blockageLayer", "no_blockage_layer" );
    }
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


  BasicLayer::Material  BasicLayer::Material::fromString ( const string& s )
  {
    Code code = other;
    if      (s == "nWell"   ) code = nWell;
    else if (s == "pWell"   ) code = pWell;
    else if (s == "nImplant") code = nImplant;
    else if (s == "pImplant") code = pImplant;
    else if (s == "active"  ) code = active;
    else if (s == "poly"    ) code = poly;
    else if (s == "cut"     ) code = cut;
    else if (s == "metal"   ) code = metal;
    else if (s == "blockage") code = blockage;

    return Material(code);
  }


  string BasicLayer::Material::_getString () const
  { return getString(&_code); }


  Record* BasicLayer::Material::_getRecord () const
  {
    Record* record = new Record(getString(this));
    record->add(getSlot("Code", &_code));
    return record;
  }


// -------------------------------------------------------------------
// Class :  "Hurricane::JsonBasicLayer".

  Initializer<JsonBasicLayer>  jsonBasicLayerInit ( 0 );


  void  JsonBasicLayer::initialize ()
  { JsonTypes::registerType( new JsonBasicLayer (JsonWriter::RegisterMode) ); }


  JsonBasicLayer::JsonBasicLayer ( unsigned long flags )
    : JsonLayer(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonBasicLayer::JsonBasicLayer()" << endl;

    add( "_material"     , typeid(string)  );
    add( "_gds2Layer"    , typeid(int64_t) );
    add( "_gds2Datatype" , typeid(int64_t) );
    add( "_blockageLayer", typeid(string)  );
    add( "_realName"     , typeid(string)  );
  }


  JsonBasicLayer::~JsonBasicLayer ()
  { }


  string  JsonBasicLayer::getTypeName () const
  { return "BasicLayer"; }


  JsonBasicLayer* JsonBasicLayer::clone ( unsigned long flags ) const
  { return new JsonBasicLayer ( flags ); }


  void JsonBasicLayer::toData(JsonStack& stack)
  {
    cdebug_tabw(19,1);

    check( stack, "JsonBasicLayer::toData" );

    Technology* techno     = lookupTechnology( stack, "JsonBasicLayer::toData" );
    BasicLayer* basicLayer = NULL;

    if (techno) {
      string       name           = get<string> ( stack, "_name"           );
      string       smask          = get<string> ( stack, "_mask"           );
      DbU::Unit    minimalSize    = get<int64_t>( stack, "_minimalSize"    );
      DbU::Unit    minimalSpacing = get<int64_t>( stack, "_minimalSpacing" );
      bool         isSymbolic     = get<bool>   ( stack, "_symbolic"       );
      string       materialName   = get<string> ( stack, "_material"       );
      unsigned int gds2Layer      = get<int64_t>( stack, "_gds2Layer"      );
      unsigned int gds2Datatype   = get<int64_t>( stack, "_gds2Datatype"   );
      string       blockageLayer  = get<string> ( stack, "_blockageLayer"  );
      string       realName       = get<string> ( stack, "_realName"       );

      BasicLayer::Material material = BasicLayer::Material::fromString(materialName);
      Layer::Mask          mask     = Layer::Mask::fromString(smask);

      if (stack.issetFlags(JsonWriter::TechnoMode)) {
      // Actual creation.
        basicLayer = BasicLayer::create( techno
                                       , name
                                       , material
                                       , gds2Layer
                                       , gds2Datatype
                                       , minimalSize
                                       , minimalSpacing
                                       );
        basicLayer->setSymbolic( isSymbolic );

        if (blockageLayer != "no_blockage_layer") {
          JsonTechnology* jtechno = jget<JsonTechnology>( stack );
          if (jtechno) {
            jtechno->addBlockageRef( blockageLayer, basicLayer );
          }
        }

        if (basicLayer->getMask() != mask) {
          cerr << Error( "JsonBasicLayer::toData(): Layer mask re-creation discrepency on \"%s\":\n"
                         "        Blob:     %s\n"
                         "        DataBase: %s"
                       , name.c_str()
                       , getString(mask).c_str()
                       , getString(basicLayer->getMask()).c_str()
                       ) << endl;
        }
      // Add here association with blockage layer...
      } else {
      // Check coherency with existing layer.
        basicLayer = dynamic_cast<BasicLayer*>( techno->getLayer(name) );
        if (basicLayer) {
          if (basicLayer->getMask() != mask) {
            cerr << Error( "JsonBasicLayer::toData(): Layer mask discrepency on \"%s\":\n"
                           "        Blob:     %s\n"
                           "        DataBase: %s"
                         , name.c_str()
                         , getString(mask).c_str()
                         , getString(basicLayer->getMask()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "JsonBasicLayer::toData(): No BasicLayer \"%s\" in the existing technology."
                       , name.c_str()
                       ) << endl;
        }
      }
    } else {
      cerr << Error( "JsonBasicLayer::toData(): Cannot find technology, aborting BasicLayer creation." ) << endl;
    }
    
    update( stack, basicLayer );

    cdebug_tabw(19,-1);
  }


} // Hurricane namespace.
