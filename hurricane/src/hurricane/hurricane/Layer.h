
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
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
// |  C++ Header  :       "./Layer.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef __HURRICANE_LAYER__
# define __HURRICANE_LAYER__

# include  "hurricane/Mask.h"
# include  "hurricane/DBo.h"
# include  "hurricane/Layers.h"
# include  "hurricane/DbU.h"
# include  "hurricane/BasicLayers.h"


namespace Hurricane {


  class Technology;


  class Layer : public DBo {

    public:
    // Types.
      typedef Hurricane::Mask<unsigned long long> Mask;
    // Accessors.
      inline  Technology*       getTechnology                () const;
      inline  const Name&       getName                      () const;
      inline  const Mask&       getMask                      () const;
      inline  const Mask&       getExtractMask               () const;
      inline  const DbU::Unit&  getMinimalSize               () const;
      inline  const DbU::Unit&  getMinimalSpacing            () const;
      inline  DbU::Unit         getPitch                     () const;
      virtual BasicLayers       getBasicLayers               () const = 0;
      virtual Layer*            getConnectorLayer            () const;
      virtual const Layer*      getTop                       () const;
      virtual const Layer*      getBottom                    () const;
      virtual const Layer*      getOpposite                  ( const Layer* ) const;
              Layer*            getMetalAbove                ( bool useWorking=true ) const;
              Layer*            getMetalBelow                ( bool useWorking=true ) const;
              Layer*            getCutAbove                  ( bool useWorking=true ) const;
              Layer*            getCutBelow                  ( bool useWorking=true ) const;
      virtual Layer*            getObstructionLayer          () const;
      virtual DbU::Unit         getEnclosure                 () const;
      virtual DbU::Unit         getExtentionCap              () const;
      virtual DbU::Unit         getExtentionWidth            () const;
      virtual DbU::Unit         getEnclosure                 ( const BasicLayer* layer ) const;
      virtual DbU::Unit         getExtentionCap              ( const BasicLayer* layer ) const;
      virtual DbU::Unit         getExtentionWidth            ( const BasicLayer* layer ) const;
    // Predicates
      inline  bool              above                        ( const Layer* layer ) const;
      inline  bool              below                        ( const Layer* layer ) const;
              bool              contains                     ( const Layer* layer ) const;
              bool              intersect                    ( const Layer* layer ) const;
      inline  bool              isWorking                    () const;
    // Updators
              void              setName                      ( const Name& name );
      inline  void              setWorking                   ( bool );
              void              setMinimalSize               ( const DbU::Unit& minimalSize );
              void              setMinimalSpacing            ( const DbU::Unit& minimalSpacing );
              void              setPitch                     ( const DbU::Unit& pitch );
      virtual void              setEnclosure                 ( const BasicLayer* layer, DbU::Unit );
      virtual void              setExtentionCap              ( const BasicLayer* layer, DbU::Unit );
      virtual void              setExtentionWidth            ( const BasicLayer* layer, DbU::Unit );
    // Hurricane Managment.
      virtual string            _getString                   () const;
      virtual Record*           _getRecord                   () const;
      inline  Layer*            _getNextOfTechnologyLayerMap () const;
      inline  void              _setMask                     ( const Mask& mask );
      inline  void              _setExtractMask              ( const Mask& extractMask );
      inline  void              _setNextOfTechnologyLayerMap ( Layer* layer );
    public:
      struct MaskCompare {
          inline bool operator () ( const Layer*, const Layer* ) const;
      };

    private:
    // Internal: Attributes
              Technology*       _technology;
              Name              _name;
              Mask              _mask;
              Mask              _extractMask;
              DbU::Unit         _minimalSize;
              DbU::Unit         _minimalSpacing;
              DbU::Unit         _pitch;
              Layer*            _nextOfTechnologyLayerMap;
              bool              _working;

    protected:
    // Internal: Constructors & Destructors.
                                Layer                        ( Technology* technology
                                                             , const Name& name
                                                             , const DbU::Unit& minimalSize    = 0
                                                             , const DbU::Unit& minimalSpacing = 0
                                                             , const DbU::Unit& pitch          = 0
                                                             );
      virtual void              _postCreate                  ();
      virtual void              _preDestroy                  ();
  };


// Inline Functions.
  inline  bool                Layer::isWorking                    () const { return _working; }
  inline  bool                Layer::above                        ( const Layer* layer ) const { return _mask > layer->getMask(); }
  inline  bool                Layer::below                        ( const Layer* layer ) const { return _mask < layer->getMask(); }
  inline  Technology*         Layer::getTechnology                () const { return _technology; }
  inline  const Name&         Layer::getName                      () const { return _name; }
  inline  const Layer::Mask&  Layer::getMask                      () const { return _mask; }
  inline  const Layer::Mask&  Layer::getExtractMask               () const { return _extractMask; }
  inline  const DbU::Unit&    Layer::getMinimalSize               () const { return _minimalSize; }
  inline  const DbU::Unit&    Layer::getMinimalSpacing            () const { return _minimalSpacing; }
  inline  DbU::Unit           Layer::getPitch                     () const { return (!_pitch?(_minimalSize + _minimalSpacing):_pitch); }
  inline  void                Layer::setWorking                   ( bool state ) { _working = state; }
  inline  Layer*              Layer::_getNextOfTechnologyLayerMap () const { return _nextOfTechnologyLayerMap; }
  inline  void                Layer::_setMask                     ( const Mask& mask ) { _mask = mask; }
  inline  void                Layer::_setExtractMask              ( const Mask& extractMask ) { _extractMask = extractMask; }
  inline  void                Layer::_setNextOfTechnologyLayerMap ( Layer* layer ) { _nextOfTechnologyLayerMap = layer; }

  inline  bool  Layer::MaskCompare::operator () ( const Layer* layer1, const Layer* layer2 ) const
  { return layer1->getMask() < layer2->getMask(); }



} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Layer);
INSPECTOR_PV_SUPPORT(Hurricane::Layer::Mask);


# endif
