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
// |  C++ Header  :  "./hurricane/Technology.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_TECHNOLOGY_H
#define  HURRICANE_TECHNOLOGY_H

#include <set>
#include <map>
#include "hurricane/Mask.h"
#include "hurricane/DBo.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayers.h"
#include "hurricane/RegularLayers.h"
#include "hurricane/ViaLayers.h"
#include "hurricane/IntrusiveMap.h"
#include "hurricane/DeviceDescriptor.h"
#include "hurricane/ModelDescriptor.h"
#include "hurricane/Rule.h"


namespace Hurricane {

  using std::set;
  using std::multimap;

  class DataBase;
  class BasicLayer;
  class RegularLayer;
  class ViaLayer;
  class UnitRule;
  class PhysicalRule;
  class TwoLayersPhysicalRule;


// -------------------------------------------------------------------
// Class  :  "Hurricane::Technology".

  class Technology : public DBo {

    public:
      typedef DBo Super;
      typedef multimap<Layer::Mask,Layer*> LayerMaskMap;
      typedef set<DeviceDescriptor*, DeviceDescriptor::DeviceDescriptorComp> DeviceDescriptors;
      typedef set<ModelDescriptor* , ModelDescriptor::ModelDescriptorComp>   ModelDescriptors;
    public:
      struct RuleNameCompare:
        public std::binary_function<const Rule*, const Rule*, bool> {
          bool operator() ( const Rule* rule1, const Rule* rule2 ) const
          { return rule1->getName() < rule2->getName(); }
      };
    public:
      typedef std::pair<const Hurricane::Layer*, const Hurricane::Layer*> LayerPair;
      typedef std::set<UnitRule*               , RuleNameCompare>         UnitRules;
      typedef std::set<PhysicalRule*           , RuleNameCompare>         PhysicalRules;
      typedef std::set<TwoLayersPhysicalRule*  , RuleNameCompare>         TwoLayersRulesSet;
      typedef std::map<const Hurricane::Layer* , PhysicalRules>           OneLayerRules;
      typedef std::map<LayerPair               , TwoLayersRulesSet>       TwoLayersRules;

    public:
    // Sub-class : LayerMap.
      class LayerMap : public IntrusiveMap<Name,Layer> {
        public:
          typedef IntrusiveMap<Name,Layer> Super;
        public:
                            LayerMap        ();
          virtual Name      _getKey         ( Layer* ) const;
          virtual unsigned  _getHashValue   ( Name ) const;
          virtual Layer*    _getNextElement ( Layer* ) const;
          virtual void      _setNextElement ( Layer* , Layer* nextLayer) const;
      };

    public:
    // Constructor.
      static  Technology*          create                  ( DataBase* , const Name& );
    // Accessors.                  
      inline  bool                 isMetal                 ( const Layer* ) const;
      inline  DataBase*            getDataBase             () const;
      inline  const Name&          getName                 () const;
      inline  Layer*               getLayer                ( const Name& ) const;
              BasicLayer*          getBasicLayer           ( const Name& ) const;
              RegularLayer*        getRegularLayer         ( const Name& ) const;
              ViaLayer*            getViaLayer             ( const Name& ) const;
      inline  Layers               getLayers               () const;
              BasicLayers          getBasicLayers          () const;
              BasicLayers          getBasicLayers          ( const Layer::Mask& ) const;
              RegularLayers        getRegularLayers        () const;
              ViaLayers            getViaLayers            () const;
              Layer*               getLayer                ( const Layer::Mask&, bool useSymbolic=true ) const;
              Layer*               getMetalAbove           ( const Layer*, bool useSymbolic=true ) const;
              Layer*               getMetalBelow           ( const Layer*, bool useSymbolic=true ) const;
              Layer*               getCutAbove             ( const Layer*, bool useSymbolic=true ) const;
              Layer*               getCutBelow             ( const Layer*, bool useSymbolic=true ) const;
              Layer*               getViaBetween           ( const Layer*, const Layer*, bool useSymbolic=true ) const;
              Layer*               getNthMetal             ( int ) const;
              Layer*               getNthCut               ( int ) const;
              DeviceDescriptor*    getDeviceDescriptor     ( const Name& );
              ModelDescriptor*     getModelDescriptor      (const Name& );
      inline  ModelDescriptors&    getModelDescriptors     ();
              UnitRule             getUnitRule             ( const std::string& ruleName ) const;
              PhysicalRule         getPhysicalRule         ( const std::string& ruleName ) const;
              PhysicalRule         getPhysicalRule         ( const std::string& ruleName
                                                           , const std::string& layerName ) const;
              PhysicalRule         getPhysicalRule         ( const std::string& ruleName
                                                           , const std::string& layer1Name
                                                           , const std::string& layer2Name ) const;
      inline const UnitRules&      getUnitRules            () const;
      inline const PhysicalRules&  getNoLayerRules         () const;
      inline const OneLayerRules&  getOneLayerRules        () const;
      inline const TwoLayersRules& getTwoLayersRules       () const;
                   void            toDtr                   ( std::ostream& );
      inline       void            setName                 ( const std::string& name );
    // Updators.
              void                 setName                 ( const Name& );
              bool                 setSymbolicLayer        ( const Name& );
              bool                 setSymbolicLayer        ( const Layer* );
              DeviceDescriptor*    addDeviceDescriptor     ( const Name& );
              ModelDescriptor*     addModelDescriptor      ( const Name& name
                                                           , const Name& simul
                                                           , const Name& model
                                                           , std::string netlist
                                                           , const Name& name_n
                                                           , const Name& name_p
                                                           , bool        precise );
              void                 addUnitRule             ( const std::string& ruleName
                                                           ,       double       value
                                                           , const std::string& reference );
              void                 addPhysicalRule         ( const std::string& ruleName
                                                           ,       DbU::Unit    value
                                                           , const std::string& reference );
              void                 addPhysicalRule         ( const std::string& ruleName
                                                           , const std::string& layerName
                                                           ,       DbU::Unit    value
                                                           , const std::string& reference);
              void                 addPhysicalRule         ( const std::string& ruleName
                                                           , const std::string& layer1Name
                                                           , const std::string& layer2Name
                                                           ,       bool         symetric
                                                           ,       DbU::Unit    value
                                                           , const std::string& reference );
    // Others.                     
      inline  LayerMap&            _getLayerMap            ();
      inline  LayerMaskMap&        _getLayerMaskMap        ();
              void                 _insertInLayerMaskMap   ( Layer* );
              void                 _removeFromLayerMaskMap ( Layer* );
      inline  Layer::Mask&         _getCutMask             ();
      inline  Layer::Mask&         _getMetalMask           ();
              void                 _onDbuChange            ( float scale );
    // Hurricane Managment.        
      virtual void                 _toJson                 ( JsonWriter* ) const;
      virtual void                 _toJsonCollections      ( JsonWriter* ) const;
      virtual string               _getTypeName            () const;
      virtual string               _getString              () const;
      virtual Record*              _getRecord              () const;
      
    private:
    // Internal: Attributes.
      DataBase*          _dataBase;
      Name               _name;
      LayerMap           _layerMap;
      LayerMaskMap       _layerMaskMap;
      Layer::Mask        _cutMask;
      Layer::Mask        _metalMask;
      DeviceDescriptors  _deviceDescriptors;
      ModelDescriptors   _modelDescriptors;
      UnitRules          _unitRules;
      PhysicalRules      _noLayerRules;
      OneLayerRules      _oneLayerRules;
      TwoLayersRules     _twoLayersRules;

    protected:
    // Constructors & Destructors.
                             Technology              ( DataBase* , const Name& );
      virtual void           _postCreate             ();
      virtual void           _preDestroy             ();
  };


// Inline Functions.
  inline  bool                          Technology::isMetal             ( const Layer* layer ) const { return _metalMask.contains(layer->getMask()); }
  inline  DataBase*                     Technology::getDataBase         () const { return _dataBase; }
  inline  const Name&                   Technology::getName             () const { return _name; }
  inline  Layer*                        Technology::getLayer            ( const Name& name ) const { return _layerMap.getElement(name); }
  inline  Layers                        Technology::getLayers           () const { return getCollection(&_layerMaskMap); }
  inline  Technology::ModelDescriptors& Technology::getModelDescriptors () { return _modelDescriptors; }
  inline  Technology::LayerMap&         Technology::_getLayerMap        () { return _layerMap; }
  inline  Technology::LayerMaskMap&     Technology::_getLayerMaskMap    () { return _layerMaskMap; }
  inline  Layer::Mask&                  Technology::_getCutMask         () { return _cutMask; }
  inline  Layer::Mask&                  Technology::_getMetalMask       () { return _metalMask; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonTechnology".

  class JsonTechnology : public JsonDBo {
    public:
      static  void            initialize     ();
                              JsonTechnology ( unsigned long flags );
      virtual                ~JsonTechnology ();
      virtual string          getTypeName    () const;
      virtual JsonTechnology* clone          ( unsigned long ) const;
      virtual void            toData         ( JsonStack& ); 
              void            addBlockageRef ( const std::string&, BasicLayer* );
    private:
      std::map< string, vector<BasicLayer*> >  _blockagesMap;
  };


}  // Hurricane namespace.


// -------------------------------------------------------------------
// Inspector Support for  :  Hurricane::Technology::LayerPair".

template<>
inline std::string getString<Hurricane::Technology::LayerPair>( Hurricane::Technology::LayerPair lp )
{ return "<LayerPair layer1=" + getString(lp.first) + ", layer2=" + getString(lp.second) + ">"; }


INSPECTOR_P_SUPPORT(Hurricane::Technology);


#endif  // HURRICANE_TECHNOLOGY_H
