// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                         Eric LAO                 |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/SlicingNode.h"                          |
// +-----------------------------------------------------------------+


#ifndef BORA_SLICING_NODE_H
#define BORA_SLICING_NODE_H

#include "hurricane/DbU.h"
#include "hurricane/analog/AnalogCellExtension.h"
namespace Hurricane {
  class Technology;
  class Net;
  class Segment;
  class Plug;
  class Instance;
}
namespace CRL {
  class RoutingGauge;
}
#include "anabatic/Dijkstra.h"
namespace Katana {
  class KatanaEngine;
}
#include "bora/Constants.h"
#include "bora/NodeSets.h"


namespace Bora {

  using Hurricane::DbU;
  using Hurricane::Technology;
  using Hurricane::Net;
  using Hurricane::Plug;
  using Hurricane::Segment;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Analog::AnalogData;
  using CRL::RoutingGauge;
  using Anabatic::GCell;
  using Katana::KatanaEngine;


// -------------------------------------------------------------------
// Class  :  "Bora::SlicingNode".
//
// Description:
//   Node used for building a slicing tree used for device's placement.
//   Step 1: Build  - build the slicing tree
//   Step 2: Update - possible dimensions (updateGlobalSize)
//   Step 3: Select - select the desired dimension (setGlobalSize)


  class SlicingNode : public AnalogData
  {
    public:
      typedef  AnalogData  Super;
    protected:
                                        SlicingNode                  ( unsigned int type );
                                        SlicingNode                  ( unsigned int type       
                                                                     , NodeSets*    nodeSets
                                                                     , unsigned int alignment = UnknownAlignment
                                                                     , BoxSet*      boxSet    = NULL
                                                                     );
      virtual                          ~SlicingNode                  ();
    public:
    // Predicates.
              inline bool               isPreset                     () const;
              inline bool               isSet                        () const;
              inline bool               isPlaced                     () const;
              inline bool               isAlignLeft                  () const;
              inline bool               isAlignRight                 () const;
              inline bool               isAlignCenter                () const;
              inline bool               isAlignTop                   () const;
              inline bool               isAlignBottom                () const;
              inline bool               isHorizontal                 () const;
              inline bool               isVertical                   () const;
              inline bool               isDevice                     () const;
              inline bool               isRouting                    () const;
                     bool               isSameSize                   ( BoxSet* ) const;
                     bool               isSameSize                   ( DbU::Unit height, DbU::Unit width ) const;
                     bool               isEmpty                      () const;
              inline bool               isOverCell                   () const;
              inline bool               isRoutingCreated             () const;
      virtual inline bool               isRoutingEstimated           () const;
                     bool               isHSymmetry                  () const;
                     bool               isVSymmetry                  () const;
      virtual inline bool               isAnalog                     () const; 
      virtual inline bool               isDigital                    () const; 
    // Accessors.                                                          
              inline size_t             getId                        () const;
              inline unsigned int       getPreset                    () const;
              inline unsigned int       getSet                       () const;
              inline unsigned int       getPlaced                    () const;
              inline unsigned int       getAlignment                 () const;
              inline unsigned int       getType                      () const;
      virtual        DbU::Unit          getHeight                    () const;
      virtual        DbU::Unit          getWidth                     () const;
                     DbU::Unit          getRootHeight                () const;
                     DbU::Unit          getRootWidth                 () const;
              inline BoxSet*            getBoxSet                    () const;
              inline DbU::Unit          getX                         () const;
              inline DbU::Unit          getY                         () const;
                     DbU::Unit          getRootX                     () const;
                     DbU::Unit          getRootY                     () const;
              inline DbU::Unit          getXCenter                   () const;
              inline DbU::Unit          getYCenter                   () const;
              inline NodeSets*          getNodeSets                  () const;
              inline SlicingNode*       getParent                    () const;
              inline BoxSet*            getPairH                     ( DbU::Unit height ) const; 
              inline BoxSet*            getPairHW                    ( DbU::Unit height, DbU::Unit width ) const;
              inline BoxSet*            getPairHW                    ( int index ) const;
              inline SlicingNode*       getRoot                      () const;
                     int                getCpt                       ();
                     double             getRatio                     ();
                     double             getArea                      ();
      static  inline Technology*        getTechnology                (); 
      static  inline Cell*              getCell                      (); 
              inline Anabatic::GCell*   getGCell                     () const; 
              inline CRL::RoutingGauge* getRoutingGauge              () const;
              inline SlicingNode*       getMaster                    () const;
                     DbU::Unit          getVPitch                    ();     
                     DbU::Unit          getHPitch                    ();       
    // Mutators.                                                     
              inline void               setPreset                    ( unsigned int );
              inline void               setSet                       ( unsigned int );
      virtual inline void               setPlaced                    ( unsigned int );
              inline void               setAlignment                 ( unsigned int );
              inline void               setType                      ( unsigned int );
                     void               setBoxSet                    ( BoxSet* );
                     void               setBoxSet                    ( DbU::Unit height, DbU::Unit width );
                     void               setBoxSet                    ( size_t       index );
                     void               _setBoxSet                   ( BoxSet*      boxSet );
                     void               setX                         ( DbU::Unit    x );
                     void               setY                         ( DbU::Unit    y );
              inline void               setNodeSets                  ( NodeSets* );
              inline void               setParent                    ( SlicingNode* );
              inline void               setPairH                     ( DbU::Unit height );
              inline void               setPairHW                    ( DbU::Unit height, DbU::Unit width );
              inline void               setOverCell                  ( unsigned int overCell );
              inline void               setRoutingCreated            ( unsigned int );
              inline void               setRoutingEstimated          ( unsigned int );
      static         void               setRoutingGauge              ( CRL::RoutingGauge* );
                     void               setCell                      ( Cell* ); 
                     void               setKatana                    ( Katana::KatanaEngine* ); 
      virtual inline void               setSymmetryFlag              ( unsigned int );
      virtual inline void               setMaster                    ( SlicingNode* );
      virtual        void               setHeight                    ( DbU::Unit );
      virtual        void               setWidth                     ( DbU::Unit );
              inline void               clearNodeSets                ();
                     void               clearParentsNodeSets         ();
              inline void               removeParent                 ();
      virtual        void               resetSlicingTree             (); // see notes in .cpp
      virtual        void               _resetSlicingTree            ();
      virtual        void               print                        () const;
      virtual        void               printLine                    () const;
      virtual        void               printParent                  () const;
                     void               printBoxSet                  () const;
      virtual        void               place                        ( DbU::Unit x=0 , DbU::Unit y=0 ); 
      virtual        void               replace                      ( DbU::Unit x=0 , DbU::Unit y=0 ); 
      virtual        void               _place                       ( DbU::Unit x=0 , DbU::Unit y=0, bool replace=false ); 
      virtual        void               updateGlobalSize             () { }; 
      virtual        void               setGlobalSize                ( DbU::Unit height, DbU::Unit width );
      virtual        void               setGlobalSize                ( size_t index );
      virtual        void               _setGlobalSize               ( BoxSet* );
      virtual        void               _setGlobalSize               ( DbU::Unit height, DbU::Unit width );
      virtual inline bool               recursiveCheckPreset         () const;
      virtual inline bool               recursiveCheckSet            () const;
      virtual inline bool               recursiveCheckPlaced         () const;
      virtual inline int                getLeafNumber                () const;
      virtual        void               preDestroy                   ();
      virtual        void               destroy                      ();
      virtual        void               preRecursiveDestroy          ();
      virtual        void               recursiveDestroy             ();
      virtual        LSlicingNodes      getLeaves                    ();
      virtual        bool               checkInitialPlacement        ( int& cpt ) const; // see notes in .cpp
                     void               updateCellAbutmentBox        ();
      virtual        void               setGCell                     ( Anabatic::GCell* );
      virtual        bool               isSymmetric                  ( SlicingNode* , unsigned int symmetryType, unsigned int flags=NoFlags  ) const;
      virtual        bool               isSame                       ( SlicingNode* , unsigned int flags=NoFlags ) const;
      static         void               createRoutingPads            (); 
      static         void               clearRoutingPads             (); 
      virtual        void               restrictDevices              ();
      virtual        void               setVertexRestriction         ( Net* , Katana::KatanaEngine* );
      virtual        void               estimateChannelsSize         ();
      virtual        void               expandRoutingChannel         ();
      virtual        void               _expandRoutingChannel        ();
      virtual        void               expandRoutingChannel         ( DbU::Unit height, DbU::Unit width );
      virtual        void               clearGlobalRouting           (); 
      virtual        void               clearGCells                  (); 
      virtual        void               adjustBorderChannels         () {}; 
                     bool               containAxis                  ( DbU::Unit axis, unsigned int symmetryType ); 
      virtual        void               updateMasterSize             ( DbU::Unit size ) { }
    // Error Methods
      virtual        DbU::Unit          getToleranceRatioH           () const;
      virtual        DbU::Unit          getToleranceRatioW           () const;
      virtual        void               setToleranceRatioH           ( DbU::Unit );
      virtual        void               setToleranceRatioW           ( DbU::Unit );
      virtual        DbU::Unit          getToleranceBandH            () const;
      virtual        DbU::Unit          getToleranceBandW            () const;
      virtual        void               setToleranceBandH            ( DbU::Unit );
      virtual        void               setToleranceBandW            ( DbU::Unit );
      virtual        void               recursiveSetToleranceRatioH  ( DbU::Unit );
      virtual        void               recursiveSetToleranceRatioW  ( DbU::Unit );
      virtual        void               recursiveSetToleranceBandH   ( DbU::Unit );
      virtual        void               recursiveSetToleranceBandW   ( DbU::Unit );
      virtual        void               _recursiveSetToleranceRatioH ( DbU::Unit );
      virtual        void               _recursiveSetToleranceRatioW ( DbU::Unit );
      virtual        void               _recursiveSetToleranceBandH  ( DbU::Unit );
      virtual        void               _recursiveSetToleranceBandW  ( DbU::Unit );
      virtual        void               setTolerances                ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );
      virtual        void               recursiveSetTolerances       ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );
      virtual        void               _recursiveSetTolerances      ( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw );
      virtual        bool               hasEmptyChildrenNodeSets     () const              ;
      virtual const  VSlicingNodes&     getChildren                  () const              ;
      virtual        SlicingNode*       getChild                     ( size_t ) const      ;
      virtual        size_t             getChildIndex                ( SlicingNode* ) const;
      virtual        size_t             getNbChild                   () const              ;
      virtual        void               createChild                  ( unsigned int type, unsigned int alignment );
      virtual        void               createChild                  ( NodeSets*    nodeSets  
                                                                     , unsigned int alignment = UnknownAlignment
                                                                     , Instance*    instance  = NULL
                                                                     , BoxSet*      boxSet    = NULL
                                                                     );
      virtual        void               createChild                  ( size_t childIndex, size_t copyIndex, unsigned int tr ); 
      virtual        void               createRouting                ( DbU::Unit );
      virtual        void               insertNode                   ( SlicingNode* , size_t index ); 
      virtual        void               push_back                    ( SlicingNode* , bool reset=false ); 
      virtual        void               push_front                   ( SlicingNode* ); 
      virtual        void               removeNode                   ( SlicingNode* );
      virtual        void               removeAllNodes               ();     
      virtual const  LSymmetries&       getSymmetries                () const;
      virtual        void               setSymmetries                ( const LSymmetries& );
      virtual        bool               isSymmetry                   ( size_t index, Symmetry& );
      virtual        bool               isSymmetry                   ( size_t index );
      virtual        void               addSymmetry                  ( size_t childIndex, size_t copyIndex, bool reset=true );
      virtual        void               removeSymmetry               ( size_t index );
      virtual        void               normalizeSymmetries          ();
      virtual        void               printChildren                () const;
      virtual        void               printChildrenLine            () const;
      virtual        SlicingNode*       clone                        ( unsigned int tr=None );
      virtual        double             getDevicesArea               () const;
      virtual        double             getOccupationArea            () const;
      virtual        void               setBoxSetIndex               ( size_t );
      virtual        size_t             getBoxSetIndex               () const;
      virtual        Instance*          getInstance                  () const;
      virtual        void               setInstance                  ( Instance* );
      virtual        bool               checkCellInstances           ( Cell* );
      virtual        SlicingNode*       findInstance                 ( Instance* );
      virtual        SlicingNode*       findSlicingNode              ( Anabatic::GCell* );
      virtual        void               updateGCellPosition          ();
      virtual        void               updateGContacts              ();
      virtual        void               updateMatrixGCellPosition    ();
      virtual        void               updateMatrixGContacts        ();
      virtual        int                getNbDevices                 ();
    // Methods to be tested.
      virtual        void               createSlicingRouting         ();
      virtual        void               resetSlicingRouting          ();
      virtual        void               destroySlicingRouting        ();
      virtual        size_t             getRoutingIndex              ( SlicingNode* ) const;
      virtual        SlicingNode*       getSlicingRouting            ( size_t index ) const;
                     SlicingNode*       findHorizontalParent         () const;
                     SlicingNode*       findVerticalParent           () const;
                     size_t             getIndexInHorizontalParent   () const;
                     size_t             getIndexInVerticalParent     () const;
      virtual        bool               checkSymmetryNet             ( unsigned int type, Net* net1, Net* net2=NULL ) const;
      virtual        void               addSymmetryNet               ( unsigned int type, Net* net1, Net* net2=NULL );
      virtual        void               updateNetConstraints         ();
      virtual        void               updateSymNetAxis             ();
      virtual        std::string        _getString                   () const;
      virtual        std::string        _getTypeName                 () const;
      static         void               addRailSegments              ( Hurricane::Segment* );
      static         bool               isRailSegments               ( Hurricane::Segment* );
      static         bool               isRailSegments               ( Hurricane::Plug* );
                     void               createRailCell               ();
      virtual        void               flattenDigitalNets           ();
      virtual        void               updateWireOccupation         ( Anabatic::Dijkstra* );
      virtual        void               resetWireOccupation          ();
      virtual        void               addWireOccupation            ( DbU::Unit min, DbU::Unit max, Net* );
      virtual        int                getMaxWireOccupation         ();
    protected:
      static CRL::RoutingGauge*    _rg;
      static Cell*                 _cell;
      static Katana::KatanaEngine* _katana;
      static std::vector<Segment*> _railSegments;
      static size_t                _idCounter;
             size_t                _id;
             SlicingNode*          _parent;
             unsigned int          _flags;
             NodeSets*             _nodeSets;    
             DbU::Unit             _x;         
             DbU::Unit             _y;       
             BoxSet*               _boxSet;
             Anabatic::GCell*      _gcell;
             SlicingNode*          _master;
  };


  inline bool               SlicingNode::isPreset             () const { return (_flags & Preset); }
  inline bool               SlicingNode::isSet                () const { return (_flags & Set); }
  inline bool               SlicingNode::isPlaced             () const { return (_flags & Placed); }
  inline bool               SlicingNode::isAlignLeft          () const { return (_flags & AlignLeft); }
  inline bool               SlicingNode::isAlignRight         () const { return (_flags & AlignRight); }
  inline bool               SlicingNode::isAlignCenter        () const { return (_flags & AlignCenter); }
  inline bool               SlicingNode::isAlignTop           () const { return (_flags & AlignTop); }
  inline bool               SlicingNode::isAlignBottom        () const { return (_flags & AlignBottom); }
  inline bool               SlicingNode::isHorizontal         () const { return (_flags & HorizontalSNode); }
  inline bool               SlicingNode::isVertical           () const { return (_flags & VerticalSNode); }
  inline bool               SlicingNode::isDevice             () const { return (_flags & DeviceSNode); }
  inline bool               SlicingNode::isRouting            () const { return (_flags & RoutingSNode); }
  inline bool               SlicingNode::isHSymmetry          () const { return (_flags & HSymmetry); }
  inline bool               SlicingNode::isVSymmetry          () const { return (_flags & VSymmetry); }
  inline bool               SlicingNode::isOverCell           () const { return (_flags & OverCell); }
  inline bool               SlicingNode::isRoutingCreated     () const { return (_flags & RoutingCreated); }
  inline bool               SlicingNode::isRoutingEstimated   () const { return true; }
  inline bool               SlicingNode::isAnalog             () const { return false; }
  inline bool               SlicingNode::isDigital            () const { return false; }

  inline size_t             SlicingNode::getId                () const { return _id; }
  inline unsigned int       SlicingNode::getPreset            () const { return (_flags & presetMask); }
  inline unsigned int       SlicingNode::getSet               () const { return (_flags & setMask); }
  inline unsigned int       SlicingNode::getPlaced            () const { return (_flags & placedMask); }
  inline unsigned int       SlicingNode::getAlignment         () const { return (_flags & alignmentMask); }
  inline unsigned int       SlicingNode::getType              () const { return (_flags & slicingTypeMask); }
  inline BoxSet*            SlicingNode::getBoxSet            () const { return _boxSet; }
  inline DbU::Unit          SlicingNode::getX                 () const { return _x; }
  inline DbU::Unit          SlicingNode::getY                 () const { return _y; }
  inline DbU::Unit          SlicingNode::getXCenter           () const { return getX() + getWidth()/2; }
  inline DbU::Unit          SlicingNode::getYCenter           () const { return getY() + getHeight()/2; }
  inline NodeSets*          SlicingNode::getNodeSets          () const { return _nodeSets; }
  inline SlicingNode*       SlicingNode::getParent            () const { return _parent; }
  inline Cell*              SlicingNode::getCell              ()       { return _cell; } 
  inline Anabatic::GCell*   SlicingNode::getGCell             () const { return _gcell; } 
  inline CRL::RoutingGauge* SlicingNode::getRoutingGauge      () const { return _rg; } 
  inline SlicingNode*       SlicingNode::getMaster            () const { return _master; }
                                                              
  inline BoxSet*            SlicingNode::getPairH             ( DbU::Unit height )                  const { return _nodeSets->getPairH(height);         }  
  inline BoxSet*            SlicingNode::getPairHW            ( DbU::Unit height, DbU::Unit width ) const { return _nodeSets->getPairHW(height, width); }  
  inline BoxSet*            SlicingNode::getPairHW            ( int index )                         const { return _nodeSets->getPairHW(index); }  
  inline void               SlicingNode::setPreset            ( unsigned int preset )               { _flags    = ((_flags & ~presetMask      ) | preset   ); } 
  inline void               SlicingNode::setSet               ( unsigned int set )                  { _flags    = ((_flags & ~setMask         ) | set      ); } 
  inline void               SlicingNode::setPlaced            ( unsigned int placed )               { _flags    = ((_flags & ~placedMask      ) | placed   ); } 
  inline void               SlicingNode::setAlignment         ( unsigned int alignment )            { _flags    = ((_flags & ~alignmentMask   ) | alignment); }
  inline void               SlicingNode::setType              ( unsigned int type )                 { _flags    = ((_flags & ~slicingTypeMask ) | type     ); }
  inline void               SlicingNode::setNodeSets          ( NodeSets*    nodeSets )             { _nodeSets = nodeSets; }  
  inline void               SlicingNode::setParent            ( SlicingNode* parent )               { _parent   = parent;   }
  inline void               SlicingNode::setPairH             ( DbU::Unit    height )               { _boxSet   = getPairH(height); }
  inline void               SlicingNode::setPairHW            ( DbU::Unit height, DbU::Unit width ) { _boxSet   = getPairHW(height, width); }
  inline void               SlicingNode::setSymmetryFlag      ( unsigned int flag )                 { _flags  = ((_flags & ~(symmetricMask) ) | flag ); }
  inline void               SlicingNode::setMaster            ( SlicingNode* master )               { _master = master; }
                                                              
  inline void               SlicingNode::setOverCell          ( unsigned int overCell )         { _flags = ((_flags & ~(overCellMask        )) | overCell)        ; }
  inline void               SlicingNode::setRoutingCreated    ( unsigned int routingCreated   ) { _flags = ((_flags & ~(routingCreatedMask  )) | routingCreated  ); }
  inline void               SlicingNode::setRoutingEstimated  ( unsigned int routingEstimated ) { _flags = ((_flags & ~(routingEstimatedMask)) | routingEstimated); }
                                                              
  inline void               SlicingNode::clearNodeSets        () { _nodeSets->clear(); }
  inline void               SlicingNode::removeParent         () { _parent   = NULL;  }
      
  inline bool               SlicingNode::recursiveCheckPreset () const { return isPreset(); }
  inline bool               SlicingNode::recursiveCheckSet    () const { return isSet   (); }
  inline bool               SlicingNode::recursiveCheckPlaced () const { return isPlaced(); }
  inline int                SlicingNode::getLeafNumber        () const { return 1; }

  inline SlicingNode* SlicingNode::getRoot() const
  {
    SlicingNode* parent = const_cast<SlicingNode*>( this );
    while ( parent->getParent() ) parent = parent->getParent();
    return parent; 
  }


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::SlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::SlicingNode)

#endif  // BORA_SLICING_NODE_H
