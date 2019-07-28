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
// |  C++ Module  :  "./Technology.cpp"                              |
// +-----------------------------------------------------------------+

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
#include "hurricane/UnitRule.h"
#include "hurricane/PhysicalRule.h"
#include "hurricane/TwoLayersPhysicalRule.h"


namespace {

  using namespace std;
  using namespace Hurricane;


  class CompareByMask {
    public:
      bool operator() ( const Hurricane::Layer* lhs, const Hurricane::Layer* rhs )
      {
        if (not lhs) return rhs;
        if (not rhs) return false;
        return lhs->getMask() < rhs->getMask();
      }
  };


  inline string  quote ( string field ) { return "'"+field+"'"; }
  inline string  quote ( Name   field ) { return "'"+getString(field)+"'"; }


  void printRule ( const PhysicalRule* rule )
  {
    cout << "  - name  = " << rule->getName() 
         << "    value = " << rule->getValue() 
         << "    ref   = " << rule->getReference() << endl;
  }


  void printRules ( const Technology::PhysicalRules& rules )
  { for ( PhysicalRule* rule : rules ) printRule( rule ); }


  void printRules ( const Technology::TwoLayersRulesSet& rules )
  { for ( TwoLayersPhysicalRule* rule : rules ) printRule( rule ); }


}  // Anonymous namespace.


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology_BasicLayers" Collection.


  class Technology_BasicLayers : public Collection<BasicLayer*> {
    public:
      typedef Collection<BasicLayer*> Super;

    public:
      class Locator : public Hurricane::Locator<BasicLayer*> {
        public:
          typedef Hurricane::Locator<BasicLayer*> Super;
        public:
                                                   Locator    ();
                                                   Locator    ( const Technology* , const Layer::Mask& );
                                                   Locator    ( const Locator& );
                  Locator&                         operator=  ( const Locator& );
          virtual BasicLayer*                      getElement () const;
          virtual Hurricane::Locator<BasicLayer*>* getClone   () const;
          virtual bool                             isValid    () const;
          virtual void                             progress   ();
          virtual string                           _getString () const;
        private:
          const Technology*        _technology;
                Layer::Mask        _mask;
                BasicLayerLocator  _basicLayerLocator;
      };

    public:
                                               Technology_BasicLayers ();
                                               Technology_BasicLayers ( const Technology* , const Layer::Mask& );
                                               Technology_BasicLayers ( const Technology_BasicLayers& );
              Technology_BasicLayers&          operator=              ( const Technology_BasicLayers& );
      virtual Collection<BasicLayer*>*         getClone               () const;
      virtual Hurricane::Locator<BasicLayer*>* getLocator             () const;
      virtual string                           _getString             () const;
    private:
      const Technology*  _technology;
            Layer::Mask  _mask;
      
  };


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology".


  Technology::Technology ( DataBase* dataBase, const Name& name )
    : Super()
    , _dataBase      (dataBase)
    , _name          (name)
    , _layerMap      ()
    , _layerMaskMap  ()
    , _unitRules     ()
    , _noLayerRules  ()
    , _oneLayerRules ()
    , _twoLayersRules()
  {
    if (not _dataBase)
      throw Error( "Technology::Technology(): Can't create Technology, NULL data base." );

    if (_dataBase->getTechnology())
      throw Error( "Technology::Technology(): Can't create Technology, already exists.");

    if (_name.isEmpty())
      throw Error( "Technology::Technology(): Can't create Technology, empty name.");
  }


  Technology* Technology::create ( DataBase* dataBase, const Name& name )
  {
    Technology* technology = new Technology( dataBase, name );
    technology->_postCreate();
    
    return technology;
  }


  BasicLayer* Technology::getBasicLayer ( const Name& name ) const
  {
    Layer* layer = getLayer(name);
    return (layer and dynamic_cast<BasicLayer*>(layer)) ? (BasicLayer*)layer : NULL;
  }


  RegularLayer* Technology::getRegularLayer ( const Name& name ) const
  {
    Layer* layer = getLayer(name);
    return (layer and dynamic_cast<RegularLayer*>(layer)) ? (RegularLayer*)layer : NULL;
  }


  ViaLayer* Technology::getViaLayer ( const Name& name ) const
  {
    Layer* layer = getLayer(name);
    return (layer and dynamic_cast<ViaLayer*>(layer)) ? (ViaLayer*)layer : NULL;
  }


  BasicLayers Technology::getBasicLayers () const
  { return SubTypeCollection<Layer*, BasicLayer*>(getLayers()); }


  BasicLayers Technology::getBasicLayers ( const Layer::Mask& mask ) const
  { return Technology_BasicLayers(this, mask); }


  RegularLayers Technology::getRegularLayers () const
  { return SubTypeCollection<Layer*, RegularLayer*>(getLayers()); }


  ViaLayers Technology::getViaLayers () const
  { return SubTypeCollection<Layer*, ViaLayer*>(getLayers()); }


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


  void Technology::setName ( const Name& name )
  {
    if (name != _name) {
      if (name.isEmpty())
        throw Error( "Technology::setName(): Empty name argument." );
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
  {
    Super::_postCreate();
    _dataBase->_setTechnology( this );
  }


  void Technology::_preDestroy ()
  {
    while (not _layerMap.isEmpty()) {
      _layerMap.getElements().getFirst()->destroy();
    }

    _dataBase->_setTechnology( NULL );
    Super::_preDestroy();
  }


  void Technology::_insertInLayerMaskMap ( Layer* layer )
  { _layerMaskMap.insert ( make_pair(layer->getMask(),layer) ); }


  void Technology::_removeFromLayerMaskMap ( Layer* layer )
  {
    LayerMaskMap::iterator lb = _layerMaskMap.lower_bound( layer->getMask() );
    LayerMaskMap::iterator ub = _layerMaskMap.upper_bound( layer->getMask() );

    for ( ; lb != ub ; lb++ ) {
      if ( lb->second == layer ) {
        _layerMaskMap.erase ( lb );
        break;
      }
    }
  }


  DeviceDescriptor* Technology::addDeviceDescriptor ( const Name& name )
  {
    DeviceDescriptor fakedd ( name );
    DeviceDescriptors::iterator ddit = _deviceDescriptors.find( &fakedd );
    if (ddit != _deviceDescriptors.end())
      throw Error( "Technology::addDeviceDescriptor(): Redifinition of device \"%s\"."
                 , getString(name).c_str() );

    DeviceDescriptor* dd = new DeviceDescriptor ( name );
    _deviceDescriptors.insert( dd );
    return dd;
  }


  DeviceDescriptor* Technology::getDeviceDescriptor ( const Name& name )
  {
    DeviceDescriptor fakedd( name );
    DeviceDescriptors::iterator ddit = _deviceDescriptors.find( &fakedd );
    if (ddit != _deviceDescriptors.end()) return *ddit;
    return NULL;
  }


  ModelDescriptor* Technology::addModelDescriptor( const Name&   name
                                                 , const Name&   simul
                                                 , const Name&   model
                                                 ,       string  netlist
                                                 , const Name&   name_n
                                                 , const Name&   name_p
                                                 , bool          precise )
  {
    ModelDescriptor fakemd ( name, simul, model, netlist, name_n, name_p, precise );
    ModelDescriptors::iterator mdit = _modelDescriptors.find(&fakemd);
    if (mdit != _modelDescriptors.end())
      throw Error( "Technology::addModelDescriptor(): Redefinition of model \"%s\"."
                 , getString(name).c_str() );

    ModelDescriptor* md = new ModelDescriptor( name, simul, model, netlist, name_n, name_p, precise );
    _modelDescriptors.insert(md);
    return md;
  }

  
  ModelDescriptor* Technology::getModelDescriptor ( const Name& name )
  {
    for ( ModelDescriptor* model : _modelDescriptors ) {
      if (model->getName() == name) return model;
    }
    return NULL;
  }
  
  
  void  Technology::toDtr ( ostream& o )
  {
    o << "\n";
    o << "from Hurricane            import DbU\n";
    o << "from helpers.AnalogTechno import Length\n";
    o << "from helpers.AnalogTechno import Area\n";
    o << "from helpers.AnalogTechno import Asymmetric\n";
    o << "\n";
    o << "\n";
    o << "analogTechnologyTable = \\\n";
    o << "    ( ('Header', '" << getName() << "', DbU.UnitPowerMicro, 'rev.LIP6-Unknown')\n";
    o << "    # ------------------------------------------------------------------------------------\n";
    o << "    # ( Rule name          , [Layer1]  , [Layer2]  , Value, Rule flags       , Reference )\n";
  
    for ( auto rule : _noLayerRules ) {
      o << "    ("
        << setw(46) << left << quote(rule->getName()) << ", "
        << setw( 5) << left << DbU::toPhysical(rule->getValue(),DbU::UnitPower::Micro) << ", "
        << setw(17) << left << "Length" << ", "
        << setw(10) << left << quote(rule->getReference()) << ")\n";
    }
  
    for ( auto layerRuleSet : _oneLayerRules ) {
      for ( auto rule : layerRuleSet.second ) {
        string flags    = "Length";
        string ruleName = getString(rule->getName());
        double value    = DbU::toPhysical(rule->getValue(),DbU::UnitPower::Micro);
        if (ruleName.substr(ruleName.size()-4,4) == "Area") {
          flags += "|Area";
          value  = DbU::toPhysical(value,DbU::UnitPower::Micro);
        }
  
        o << "    ("
          << setw(22) << left << quote(rule->getName()) << ", "
          << setw(22) << left << quote(layerRuleSet.first->getName()) << ", "
          << setw( 5) << left << value << ", "
          << setw(17) << left << flags << ", "
          << setw(10) << left << quote(rule->getReference()) << ")\n";
      }
    }
  
    for ( auto layerRuleSet : _twoLayersRules ) {
      for ( auto rule : layerRuleSet.second ) {
        string                 flags   = "Length";
        TwoLayersPhysicalRule* twoRule = dynamic_cast<TwoLayersPhysicalRule*>(rule);
        if (twoRule and not twoRule->isSymetric())
          flags += "|Asymmetric";
  
        o << "    ("
          << setw(22) << left << quote(rule->getName()) << ", "
          << setw(10) << left << quote(layerRuleSet.first.first->getName())  << ", "
          << setw(10) << left << quote(layerRuleSet.first.second->getName()) << ", "
          << setw( 5) << left << DbU::toPhysical(rule->getValue(),DbU::UnitPower::Micro) << ", "
          << setw(17) << left << flags << ", "
          << setw(10) << left << quote(rule->getReference())  << ")\n";
      }
    }
    o << "    )\n";
  }
  
  
  UnitRule  Technology::getUnitRule ( const string& ruleNameStr ) const
  {
    Name     ruleName ( ruleNameStr );
    UnitRule search   ( ruleName, 0, "" );
  
    UnitRules::iterator irule = _unitRules.find( &search );
    if (irule == _unitRules.end()) {
      throw Error( "Technology::getUnitRule(): No rule named \"%s\"." , ruleNameStr.c_str() );
    }
    return **irule;
  }
  
  
  PhysicalRule  Technology::getPhysicalRule ( const string& ruleNameStr ) const
  {
    Name         ruleName ( ruleNameStr );
    PhysicalRule search   ( ruleName, 0, "" );
  
    PhysicalRules::iterator irule = _noLayerRules.find( &search );
    if (irule == _noLayerRules.end()) {
      throw Error( "Technology::getPhysicalRule(): No rule named \"%s\"." , ruleNameStr.c_str() );
    }
    return **irule;
  }
  
  
  PhysicalRule  Technology::getPhysicalRule ( const string& ruleNameStr, const string& layerStr ) const
  {
    Name         layerName ( layerStr );
    const Layer* layer     = getLayer( layerName );
  
    OneLayerRules::const_iterator ilayer = _oneLayerRules.find( layer );
    if (ilayer == _oneLayerRules.end())
      throw Error( "Technology::getPhysicalRule(): No rules for %s."
                 , getString(layer->getName()).c_str() );
  
    const PhysicalRules& rules    = ilayer->second;
    Name                 ruleName ( ruleNameStr );
    PhysicalRule         search   ( ruleName, 0, "" );
  
    PhysicalRules::iterator irule = rules.find( &search );
    if (irule == rules.end())
      throw Error( "Technology::getPhysicalRule(): No rule named \"%s\" for %s."
                 , ruleNameStr.c_str()
                 , getString(layer->getName()).c_str() );
  
    return **irule;
  }
  
  
  PhysicalRule  Technology::getPhysicalRule ( const string& ruleNameStr
                                            , const string& layer1Str
                                            , const string& layer2Str ) const
  {
    Name         layer1Name ( layer1Str );
    Name         layer2Name ( layer2Str );
    const Layer* layer1     = getLayer( layer1Name );
    const Layer* layer2     = getLayer( layer2Name );
  
    LayerPair searchLayerPair(layer1, layer2);
    TwoLayersRules::const_iterator ilp = _twoLayersRules.find( searchLayerPair );
    if (ilp != _twoLayersRules.end()) {
      const TwoLayersRulesSet& rules    = ilp->second;
      Name                     ruleName ( ruleNameStr );
      TwoLayersPhysicalRule    search   ( ruleName, 0, "", true );
  
      TwoLayersRulesSet::const_iterator irule = rules.find( &search );
      if (irule != rules.end()) return **irule;
    }
  
    LayerPair reverseSearchLayerPair ( layer2, layer1 );
    ilp = _twoLayersRules.find( reverseSearchLayerPair );
    if (ilp != _twoLayersRules.end()) {
      const TwoLayersRulesSet& rules    = ilp->second;
      Name                     ruleName ( ruleNameStr );
      TwoLayersPhysicalRule    search   ( ruleName, 0, "", true );
      
      TwoLayersRulesSet::const_iterator irule = rules.find( &search );
      if (irule != rules.end() and (*irule)->isSymetric() ) return **irule;
    }
  
    throw Error( "Technology::getPhysicalRule(): No rule named \"%s\" for pair (%s,%s)."
               , ruleNameStr.c_str(), layer1Str.c_str(), layer2Str.c_str() ); 
  }
  
  
  void Technology::addUnitRule ( const string&   name
                               ,       double    value
                               , const string&   reference )
  {
    Name     ruleName ( name );
    UnitRule search   ( ruleName, 0, "" );
  
    UnitRules::iterator irule = _unitRules.find( &search );
    if (irule != _unitRules.end())
      throw Error( "Technology::addUnitRule(): Attempt to redefine rule \"%s\"." , name.c_str() );
  
    _unitRules.insert( new UnitRule ( ruleName, value, reference ) ); 
  }
  
  
  void Technology::addPhysicalRule ( const string&   name
                                   ,       DbU::Unit value
                                   , const string&   reference )
  {
    Name         ruleName ( name );
    PhysicalRule search   ( ruleName, 0, "" );
  
    PhysicalRules::iterator irule = _noLayerRules.find( &search );
    if (irule != _noLayerRules.end())
      throw Error( "Technology::addPhysicalRule(): Attempt to redefine rule \"%s\"." , name.c_str() );
  
    _noLayerRules.insert( new PhysicalRule ( ruleName, value, reference ) ); 
  }
  
  
  void Technology::addPhysicalRule ( const string&   ruleNameStr
                                   , const string&   layerStr
                                   ,       DbU::Unit value
                                   , const string&   reference )
  {
    Name ruleName  ( ruleNameStr );
    Name layerName ( layerStr );
  
    const Layer* layer = getLayer( layerName );
    OneLayerRules::iterator ilayer = _oneLayerRules.find(layer);
    if (ilayer == _oneLayerRules.end()) {
      pair<OneLayerRules::iterator, bool> result =
        _oneLayerRules.insert( OneLayerRules::value_type( layer, PhysicalRules()) );
      result.first->second.insert( new PhysicalRule(ruleName,value,reference) );
    } else {
      PhysicalRules& rules  = ilayer->second;
      PhysicalRule   search ( ruleName, 0, "" );
      if (rules.find(&search) != rules.end()) {
        throw Error( "Technology::addPhysicalRule(): Attempt to redefine rule \"%s\"." , ruleNameStr.c_str() );
      }
  
      rules.insert( new PhysicalRule(ruleName,value,reference) );
    }
  }
  
  
  void Technology::addPhysicalRule ( const string&   ruleNameStr
                                   , const string&   layer1Str
                                   , const string&   layer2Str
                                   ,       bool      symetric
                                   ,       DbU::Unit value
                                   , const string&   reference )
  {
    Name ruleName   ( ruleNameStr );
    Name layer1Name ( layer1Str );
    Name layer2Name ( layer2Str );
  
    const Layer* layer1    = getLayer(layer1Name);
    const Layer* layer2    = getLayer(layer2Name);
    LayerPair    layerPair ( layer1, layer2 );
  
    TwoLayersRules::iterator ilp = _twoLayersRules.find( layerPair );
    if (ilp == _twoLayersRules.end()) {
      pair<TwoLayersRules::iterator, bool> result =
        _twoLayersRules.insert( TwoLayersRules::value_type(layerPair,TwoLayersRulesSet()) );
  
      result.first->second.insert( new TwoLayersPhysicalRule(ruleName,value,reference,symetric) );
    } else {
      TwoLayersRulesSet&    rules  = ilp->second;
      TwoLayersPhysicalRule search ( ruleName, 0, "", true );
      if (rules.find(&search) != rules.end()) {
        throw Error( "Technology::addPhysicalRule(): Attempt to redefine rule \"%s\"." , ruleNameStr.c_str() );
      }
      rules.insert( new TwoLayersPhysicalRule(ruleName,value,reference,symetric) );
    }
  }


  string  Technology::_getTypeName () const
  { return _TName("Technology"); }


  string Technology::_getString () const
  {
    string s = Super::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    return s;
  }


  Record* Technology::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_dataBase"      , _dataBase       ) );
      record->add( getSlot("_name"          , &_name          ) );
      record->add( getSlot("_layerMaskMap"  , &_layerMaskMap  ) );
      record->add( getSlot("_cutMask"       , &_cutMask       ) );
      record->add( getSlot("_metalMask"     , &_metalMask     ) );
      record->add( getSlot("_unitRules"     , &_unitRules     ) );
      record->add( getSlot("_noLayerRules"  , &_noLayerRules  ) );
      record->add( getSlot("_oneLayerRules" , &_oneLayerRules ) );
      record->add( getSlot("_twoLayersRules", &_twoLayersRules) );
    }
    return record;
  }


  void Technology::_toJson ( JsonWriter* writer ) const
  {
    Super::_toJson( writer );

    jsonWrite( writer, "_name", getName() );
  }


  void  Technology::_toJsonCollections ( JsonWriter* writer ) const
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


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology::LayerMap"


  Technology::LayerMap::LayerMap ()
    : Super()
  { }


  Name Technology::LayerMap::_getKey ( Layer* layer ) const
  { return layer->getName(); }


  unsigned Technology::LayerMap::_getHashValue ( Name name ) const
  { return name._getSharedName()->getHash() / 8; }


  Layer* Technology::LayerMap::_getNextElement ( Layer* layer ) const
  { return layer->_getNextOfTechnologyLayerMap(); }


  void Technology::LayerMap::_setNextElement ( Layer* layer, Layer* nextLayer ) const
  { layer->_setNextOfTechnologyLayerMap(nextLayer); };


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology_BasicLayers" Collection.


  Technology_BasicLayers::Technology_BasicLayers ()
    : Super()
    , _technology(NULL)
    , _mask      (0)
  { }


  Technology_BasicLayers::Technology_BasicLayers ( const Technology* technology, const Layer::Mask& mask )
    : Super()
    , _technology(technology)
    , _mask      (mask)
  { }


  Technology_BasicLayers::Technology_BasicLayers ( const Technology_BasicLayers& basicLayers )
    : Super()
    , _technology(basicLayers._technology)
    , _mask      (basicLayers._mask)
  { }


  Technology_BasicLayers& Technology_BasicLayers::operator= ( const Technology_BasicLayers& basicLayers )
  {
    _technology = basicLayers._technology;
    _mask       = basicLayers._mask;
    return *this;
  }


  Collection<BasicLayer*>* Technology_BasicLayers::getClone () const
  { return new Technology_BasicLayers( *this ); }


  Locator<BasicLayer*>* Technology_BasicLayers::getLocator () const
  { return new Locator( _technology, _mask ); }


  string Technology_BasicLayers::_getString () const
  {
    string s = "<" + _TName("Technology::BasicLayers");
    if (_technology) {
      s += " " + getString(_technology);
      s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


  Technology_BasicLayers::Locator::Locator ()
    : Super()
    , _technology       (NULL)
    , _mask             (0)
    , _basicLayerLocator()
  { }


  Technology_BasicLayers::Locator::Locator ( const Technology* technology, const Layer::Mask& mask )
    : Super()
    , _technology       (technology)
    , _mask             (mask)
    , _basicLayerLocator()
  {
    if (_technology) {
      _basicLayerLocator = _technology->getBasicLayers().getLocator();
      while (_basicLayerLocator.isValid() and not (_basicLayerLocator.getElement()->getMask() & _mask))
        _basicLayerLocator.progress();
    }
  }


  Technology_BasicLayers::Locator::Locator ( const Locator& locator )
    : Super()
    , _technology       (locator._technology)
    , _mask             (locator._mask)
    , _basicLayerLocator(locator._basicLayerLocator)
  { }


  Technology_BasicLayers::Locator& Technology_BasicLayers::Locator::operator = ( const Locator& locator )
  {
    _technology        = locator._technology;
    _mask              = locator._mask;
    _basicLayerLocator = locator._basicLayerLocator;
    return *this;
  }


  BasicLayer* Technology_BasicLayers::Locator::getElement () const
  { return _basicLayerLocator.getElement(); }


  Locator<BasicLayer*>* Technology_BasicLayers::Locator::getClone () const
  { return new Locator( *this ); }


  bool Technology_BasicLayers::Locator::isValid () const
  { return _basicLayerLocator.isValid(); }


  void Technology_BasicLayers::Locator::progress ()
  {
    do {
      _basicLayerLocator.progress();
    }
    while (_basicLayerLocator.isValid() and not (_basicLayerLocator.getElement()->getMask() & _mask));
  }


  string Technology_BasicLayers::Locator::_getString () const
  {
    string s = "<" + _TName("Technology::BasicLayers::Locator");
    if (_technology) {
      s += " " + getString(_technology);
      s += " " + getString(_mask);
    }
    s += ">";
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonTechnology".


  Initializer<JsonTechnology>  jsonTechnologyInit ( 0 );


  void  JsonTechnology::initialize ()
  { JsonTypes::registerType( new JsonTechnology (JsonWriter::RegisterMode) ); }


  JsonTechnology::JsonTechnology ( unsigned long flags )
    : JsonDBo      (flags)
    , _blockagesMap()
  {
    if (flags & JsonWriter::RegisterMode) return;

    cdebug_log(19,0) << "JsonTechnology::JsonTechnology()" << endl;

    add( "_name"  , typeid(string)    );
    add( "+layers", typeid(JsonArray) );
  }


  JsonTechnology::~JsonTechnology ()
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


  string  JsonTechnology::getTypeName () const
  { return "Technology"; }


  JsonTechnology* JsonTechnology::clone ( unsigned long flags ) const
  { return new JsonTechnology ( flags ); }


  void JsonTechnology::addBlockageRef ( const string& blockageLayer, BasicLayer* layer )
  {
    map< string, vector<BasicLayer*> >::iterator im = _blockagesMap.find( blockageLayer );
    if (im != _blockagesMap.end()) {
      (*im).second.push_back( layer );
    } else {
      _blockagesMap.insert( make_pair( blockageLayer, vector<BasicLayer*>(1,layer) ) );
    }
  }


  void JsonTechnology::toData ( JsonStack& stack )
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


} // Hurricane namespace.
