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
// |  C++ Header  :  "./bora/VSlicingNode.h"                         |
// +-----------------------------------------------------------------+


#ifndef BORA_V_SLICING_NODE_H
#define BORA_V_SLICING_NODE_H

#include "bora/HVSlicingNode.h"


namespace Bora {


// -------------------------------------------------------------------
// Class  :  "Bora::VSlicingNode".


  class VSlicingNode: public HVSlicingNode
  {
    public:
      typedef  HVSlicingNode  Super;
    private:
                                  VSlicingNode         ( unsigned int type, unsigned int alignment=UnknownAlignment);
                                 ~VSlicingNode         (); 
    public:
      static        VSlicingNode* create               ( unsigned int alignment=AlignBottom );
                    void          createRouting        ( DbU::Unit space );
                                                       
                    void          print                () const;
                    VSlicingNode* clone                ( unsigned int tr=None );
                    void          place                ( DbU::Unit x=0, DbU::Unit y=0 );
                    void          replace              ( DbU::Unit x=0, DbU::Unit y=0 );
                    void          _place               ( DbU::Unit x=0, DbU::Unit y=0, bool replace=false );
                    void          updateGlobalSize     ();
      static inline int           getCount             ();
      static inline void          printCount           ();
      static inline void          printCountAll        ();
                    void          preDestroy           ();
                    void          destroy              ();
                    void          preRecursiveDestroy  ();
                    void          recursiveDestroy     ();
                    DbU::Unit     getHeight            () const;
                    DbU::Unit     getWidth             () const;
                    void          createSlicingRouting ();
                    void          setGCell             ( Anabatic::GCell* );
                    void          adjustBorderChannels (); 
      virtual       std::string   _getString           () const;
      virtual       std::string   _getTypeName         () const;
    private:
      static int  _count;
      static int  _countAll;
};


  inline int  VSlicingNode::getCount      () { return _count; }
  inline void VSlicingNode::printCount    () { std::cerr << "VSlicingNode::Count      = " << _count    << std::endl; }
  inline void VSlicingNode::printCountAll () { std::cerr << "VSlicingNode::CountAll   = " << _countAll << std::endl; }


}  // Bora namespace.


GETSTRING_POINTER_SUPPORT(Bora::VSlicingNode)
IOSTREAM_POINTER_SUPPORT(Bora::VSlicingNode)

#endif  // BORA_V_SLICING_NODE_H
