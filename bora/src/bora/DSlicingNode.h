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
// |  C++ Header  :  "./bora/DSlicingNode.h"                         |
// +-----------------------------------------------------------------+


#ifndef BORA_D_SLICING_NODE_H
#define BORA_D_SLICING_NODE_H

#include "bora/SlicingNode.h"


namespace Bora {


// -------------------------------------------------------------------
// Class  :  "Bora::DSlicingNode".

  
  class DSlicingNode: public SlicingNode
  {
    public:
      typedef  SlicingNode  Super;
    private:
                                  DSlicingNode              ( unsigned int type
                                                            , NodeSets*    nodeSets  
                                                            , unsigned int alignment
                                                            , Instance*    instance
                                                            , BoxSet*      boxSet
                                                            );
                                 ~DSlicingNode              ();

    public:
      static        DSlicingNode* create                    ( NodeSets*    nodeSets  
                                                            , unsigned int alignment = UnknownAlignment
                                                            , Instance*    instance  = NULL
                                                            , BoxSet*      boxSet    = NULL
                                                            );
                    void          print                     () const;
                    DSlicingNode* clone                     ( unsigned int tr=None );
      inline        double        getDevicesArea            () const;
                    void          setBoxSetIndex            ( size_t );
                    size_t        getBoxSetIndex            () const;
      inline        Instance*     getInstance               () const;
      inline        void          setInstance               ( Instance* );
                    void          place                     ( DbU::Unit x=0, DbU::Unit y=0 );
                    void          _place                    ( DbU::Unit x=0, DbU::Unit y=0, bool replace=false );

      static inline int           getCount                  ();
      static inline void          printCount                ();
      static inline void          printCountAll             ();
                    void          preDestroy                ();
                    void          destroy                   ();
                    void          preRecursiveDestroy       ();
                    void          recursiveDestroy          ();
                    bool          checkInitialPlacement     ( int& cpt ) const;// see notes in .cpp
                    void          setGCell                  ( Anabatic::GCell* );
                    bool          isSame                    ( SlicingNode* ) const;    
                    bool          isSymmetric               ( SlicingNode* , unsigned int symmetryType, unsigned int flags=NoFlags ) const;
                    bool          isSame                    ( SlicingNode* , unsigned int glags=NoFlags ) const;
                    void          updateGCellPosition       ();
                    void          updateGContacts           ();
                    void          updateMatrixGCellPosition ();
                    void          updateMatrixGContacts     ();
                    void          restrictDevices           ();
                    void          printLine                 () const;
                    bool          isAnalog                  () const;
                    bool          isDigital                 () const;
                    bool          checkCellInstances        ( Cell* cell );
      virtual       void          resetWireOccupation       ();
      virtual       std::string   _getString                () const;
      virtual       std::string   _getTypeName              () const;
    private:
      static int        _count;
      static int        _countAll;
             Instance*  _instance;
  };


  inline double    DSlicingNode::getDevicesArea () const               { return getHeight()*getWidth(); }
  inline Instance* DSlicingNode::getInstance    () const               { return _instance; }
  inline void      DSlicingNode::setInstance    ( Instance* instance ) { _instance = instance; }
  inline int       DSlicingNode::getCount       ()                     { return _count  ; }
  inline void      DSlicingNode::printCount     ()                     { std::cerr << "DSlicingNode::Count      = " << _count    << std::endl; }
  inline void      DSlicingNode::printCountAll  ()                     { std::cerr << "DSlicingNode::CountAll   = " << _countAll << std::endl; }




}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::DSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::DSlicingNode)

#endif  // BORA_D_SLICING_NODE_H
