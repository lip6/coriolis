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
// |  C++ Header  :  "./bora/HVSetState.h"                           |
// +-----------------------------------------------------------------+


#ifndef BORA_HV_SETSTATE_H
#define BORA_HV_SETSTATE_H


#include "hurricane/DbU.h"
#include "bora/Constants.h"
#include "bora/HVSlicingNode.h"


namespace Bora {

  using Hurricane::DbU;
  class NodeSets;
  class HSlicingNode;
  class VSlicingNode;


// -------------------------------------------------------------------
// Class  :  "Bora::HVSetState".
//
// Description:
//
// HVSetState, HSetState  and VSetState  are classes used  in the  algorithm to
// determine the possible  dimensions for a slicing tree. They  are not part of
// the slicing tree, they only help during the use of updateGlobalSize.  At the
// end of the algorithm, it should provide the resulting NodeSets.
//
// In a  horizontal/vertical slicing  node, the  number of  possible dimensions
// (EndCounter) to study is:
//
//   EndCounter = N(child0) * N(child1) * ... * N(childn)
//
// We  call  a   "state"  a  dimension  for   the  horizontal/vertical  slicing
// node. "EndCounter" states needs to be studied.
//
// For each states, we check if these conditions are filled:
//
// - Horizontal case: toleranceBandW <= max(children's width )-min(children's width )
// - Vertical   case: toleranceBandH <= max(children's height)-min(children's height)
//
// When the condition is  filled, we add the dimensions to  the NodeSets and we
// proceed to the next combinations.


  class HVSetState
  {
    protected:
                        HVSetState    ( HVSlicingNode* );
      virtual          ~HVSetState    ();
  
    public:
      virtual DbU::Unit getCurrentH   () = 0;
      virtual DbU::Unit getCurrentW   () = 0;
      inline  bool      end           ();
      inline  int       getEndCounter ();
              NodeSets* getNodeSets   (); 
              bool      isSymmetry    ( size_t index, Symmetry& symmetry );
              bool      isSymmetry    ( size_t index );
  
      virtual void      print         ();
              void      initSet       ();
              void      initModulos   (); // see notes in .cpp
              void      next          (); // see notes in .cpp
      virtual void      push_back     () = 0;
  
    protected: 
      HVSlicingNode*       _HVSnode; 
      size_t               _counter;
      std::vector<size_t>  _modulos;
      std::vector<size_t>  _currentSet;
      std::vector<size_t>  _nextSet;
      NodeSets*            _nodeSets;
  };
  

  inline bool HVSetState::end           ()                                   { return (_counter == _modulos.back()+1); }
  inline int  HVSetState::getEndCounter ()                                   { return _modulos.back()+1; }
  inline bool HVSetState::isSymmetry    ( size_t index, Symmetry& symmetry ) { return _HVSnode->isSymmetry(index,symmetry); }
  inline bool HVSetState::isSymmetry    ( size_t index )                     { return _HVSnode->isSymmetry(index); }


// -------------------------------------------------------------------
// Class  :  "Bora::HSetState".


  class HSetState: public HVSetState
  {
    public:
                                               HSetState    ( HSlicingNode* );
                                              ~HSetState    ();
               std::pair<DbU::Unit,DbU::Unit>  getCurrentWs (); // See notes in .cpp
      virtual  DbU::Unit                       getCurrentH  (); // See notes in .cpp
      virtual  DbU::Unit                       getCurrentW  (); // See notes in .cpp
               void                            print        ();
               void                            next         ();
               void                            push_back    (); // See notes in .cpp
  };


// -------------------------------------------------------------------
// Class  :  "Bora::HSetState".


  class VSetState: public HVSetState
  {
    public:
                                               VSetState    ( VSlicingNode* );
                                              ~VSetState    ();
               std::pair<DbU::Unit,DbU::Unit>  getCurrentHs (); // See notes in .cpp
      virtual  DbU::Unit                       getCurrentH  (); // See notes in .cpp
      virtual  DbU::Unit                       getCurrentW  (); // See notes in .cpp
               void                            print        ();
               void                            next         ();
               void                            push_back    (); // See notes in .cpp
  };


}  // Bora namespace.

#endif  // BORA_HV_SETSTATE_H
