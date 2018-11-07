// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/HVSlicingNode.h"                        |
// +-----------------------------------------------------------------+


#ifndef BORA_HV_SLICING_NODE_H
#define BORA_HV_SLICING_NODE_H

#include "bora/SlicingNode.h"


namespace Bora {
  
  class RHVSlicingNode;


// -------------------------------------------------------------------
// Class  :  "Bora::HVSlicingNode".


  class HVSlicingNode: public SlicingNode
  {
    public:
      typedef  SlicingNode                                                 Super;
      typedef  std::tuple<unsigned int, Hurricane::Net*, Hurricane::Net*>  NetSymmetry;
      typedef  std::vector<NetSymmetry>                                    VNetSymmetries;
    protected:
                                  HVSlicingNode                ( unsigned int type, unsigned int alignment = AlignLeft );
      virtual                    ~HVSlicingNode                ();
    public:                                                    
             DbU::Unit            getToleranceRatioH           () const;
             DbU::Unit            getToleranceRatioW           () const;
             void                 setToleranceRatioH           ( DbU::Unit );
             void                 setToleranceRatioW           ( DbU::Unit );
             DbU::Unit            getToleranceBandH            () const;
             DbU::Unit            getToleranceBandW            () const;
             void                 setToleranceBandH            ( DbU::Unit );
             void                 setToleranceBandW            ( DbU::Unit );
             void                 recursiveSetToleranceRatioH  ( DbU::Unit );
             void                 recursiveSetToleranceRatioW  ( DbU::Unit );
             void                 recursiveSetToleranceBandH   ( DbU::Unit );
             void                 recursiveSetToleranceBandW   ( DbU::Unit );
                                
             void                 _recursiveSetToleranceRatioH ( DbU::Unit );
             void                 _recursiveSetToleranceRatioW ( DbU::Unit );
             void                 _recursiveSetToleranceBandH  ( DbU::Unit );
             void                 _recursiveSetToleranceBandW  ( DbU::Unit );
             void                 setTolerances                ( DbU::Unit trh
                                                               , DbU::Unit trw
                                                               , DbU::Unit tbh
                                                               , DbU::Unit tbw 
                                                               );
             void                 recursiveSetTolerances       ( DbU::Unit trh
                                                               , DbU::Unit trw
                                                               , DbU::Unit tbh
                                                               , DbU::Unit tbw 
                                                               );
             void                 _recursiveSetTolerances      ( DbU::Unit trh
                                                               , DbU::Unit trw
                                                               , DbU::Unit tbh
                                                               , DbU::Unit tbw 
                                                               );
             bool                 hasEmptyChildrenNodeSets     () const;
      inline const VSlicingNodes& getChildren                  () const;
             SlicingNode*         getChild                     ( size_t       index ) const;
             size_t               getChildIndex                ( SlicingNode* node  ) const;
      inline size_t               getNbChild                   () const;
             void                 createChild                  ( unsigned int type, unsigned int alignment );
             void                 createChild                  ( NodeSets*    nodeSets  
                                                               , unsigned int alignment = UnknownAlignment
                                                               , Instance*    instance  = NULL
                                                               , BoxSet*      boxSet    = NULL
                                                               );
             void                 createChild                  ( size_t childIndex, size_t copyIndex, unsigned int tr );
             void                 insertNode                   ( SlicingNode* , size_t index );
             void                 push_back                    ( SlicingNode* , bool reset = true );  
             void                 push_front                   ( SlicingNode* ); 
             void                 removeNode                   ( SlicingNode* ); 
      inline void                 removeAllNodes               ();
      inline const LSymmetries&   getSymmetries                () const;
      inline void                 setSymmetries                ( const LSymmetries& sym );
             bool                 isSymmetry                   ( size_t index, Symmetry& );
             bool                 isSymmetry                   ( size_t index );
             void                 addSymmetry                  ( size_t childIndex, size_t copyIndex, bool reset = true );
             void                 removeSymmetry               ( size_t index );
             void                 normalizeSymmetries          (); // see notes in .cpp
             void                 preDestroy                   ();
             void                 destroy                      ();
             void                 preRecursiveDestroy          ();
             void                 recursiveDestroy             ();
             void                 _resetSlicingTree            ();
             void                 print                        () const;
             void                 printChildren                () const;
             void                 printLine                    () const;
             void                 printChildrenLine            () const;
             bool                 recursiveCheckPreset         () const;
             bool                 recursiveCheckSet            () const;
             bool                 recursiveCheckPlaced         () const;
             int                  getLeafNumber                () const;
             double               getDevicesArea               () const;
             double               getOccupationArea            () const;
             void                 setGlobalSize                ( DbU::Unit height, DbU::Unit width );
             void                 setGlobalSize                ( size_t index );
             void                 _setGlobalSize               ( BoxSet* boxSet );
             LSlicingNodes        getLeaves                    ();
             bool                 checkInitialPlacement        ( int& cpt ) const; // see notes in .cpp
             bool                 isSymmetric                  ( SlicingNode* , unsigned int symmetryType, unsigned int flags=NoFlags  ) const;
             bool                 isSame                       ( SlicingNode* , unsigned int flags=NoFlags ) const;
             void                 restrictDevices              ();
             void                 setVertexRestriction         ( Net* , Katana::KatanaEngine* );
             void                 estimateChannelsSize         ();
             void                 expandRoutingChannel         ();
             void                 _expandRoutingChannel        ();
             void                 expandRoutingChannel         ( DbU::Unit height, DbU::Unit width );
             bool                 isRoutingEstimated           () const;
             void                 updateGCellPosition          ();
             void                 updateGContacts              ();
             int                  getNbDevices                 ();
             virtual std::string  _getString                   () const;
             virtual std::string  _getTypeName                 () const;
             void                 setSymmetryFlag              ( unsigned int flag   );
             void                 setMaster                    ( SlicingNode* );
             bool                 checkCellInstances           ( Cell* );
             SlicingNode*         findInstance                 ( Instance* );
             SlicingNode*         findSlicingNode              ( Anabatic::GCell* );
             void                 resetSlicingRouting          ();
             void                 destroySlicingRouting        ();
             size_t               getRoutingIndex              ( SlicingNode* ) const;
             SlicingNode*         getSlicingRouting            ( size_t index ) const;
             void                 clearGCells                  (); 
             bool                 checkSymmetryNet             ( unsigned int type, Net* net1, Net* net2 = NULL ) const;
             void                 addSymmetryNet               ( unsigned int type, Net* net1, Net* net2 = NULL );
             void                 updateNetConstraints         ();
             void                 updateSymNetAxis             ();
             void                 flattenDigitalNets           ();
                                                               
             void                 updateWireOccupation         ( Anabatic::Dijkstra* );
             void                 resetWireOccupation          ();
    protected:
      VSlicingNodes                    _children;
      DbU::Unit                        _toleranceRatioH;
      DbU::Unit                        _toleranceRatioW;
      DbU::Unit                        _toleranceBandH;
      DbU::Unit                        _toleranceBandW;
      LSymmetries                      _symmetries;
      std::vector<RHVSlicingNode*>     _slicingRouting;
      VNetSymmetries                   _netSymmetries;
  };


  inline const VSlicingNodes& HVSlicingNode::getChildren        () const            { return _children; }
  inline size_t               HVSlicingNode::getNbChild         () const            { return _children.size(); }
  inline void                 HVSlicingNode::removeAllNodes     ()                  { _children.clear(); }
  inline const LSymmetries&   HVSlicingNode::getSymmetries      () const            { return _symmetries; }
  inline void                 HVSlicingNode::setSymmetries      ( const LSymmetries& syms ) { _symmetries = syms;  }


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::HVSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::HVSlicingNode)

#endif  // BORA_HV_SLICING_NODE_H
