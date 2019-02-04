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
// |  C++ Header  :  "./bora/Constants.h"                            |
// +-----------------------------------------------------------------+


#ifndef BORA_CONSTANTS_H
#define BORA_CONSTANTS_H

#include <utility>
#include <vector>
#include <list>


namespace Bora {


  const unsigned int presetMask           =     0x1;
  const unsigned int setMask              =     0x2;
  const unsigned int placedMask           =     0x4;
  const unsigned int alignmentMask        =   0x1F8;
  const unsigned int slicingTypeMask      =  0x3E00;
  const unsigned int overCellMask         =  0x8000;
  const unsigned int routingEstimatedMask = 0x10000;
  const unsigned int routingCreatedMask   = 0x20000;
  const unsigned int symmetricMask        = 0xC0000;

  enum Flags { Preset            = 1 << 0
             , Set               = 1 << 1
             , Placed            = 1 << 2
             , UnknownAlignment  = 1 << 3
             , AlignLeft         = 1 << 4
             , AlignRight        = 1 << 5
             , AlignCenter       = 1 << 6
             , AlignTop          = 1 << 7
             , AlignBottom       = 1 << 8
             , UnknownType       = 1 << 9
             , HorizontalSNode   = 1 << 10
             , VerticalSNode     = 1 << 11
             , DeviceSNode       = 1 << 12
             , RoutingSNode      = 1 << 13
             , None              = 1 << 14
             , MX                = 1 << 15
             , MY                = 1 << 16
             , FixedRoutingSNode = 1 << 17
             , OverCell          = 1 << 19
             , RoutingEstimated  = 1 << 20
             , RoutingCreated    = 1 << 21
             , HSymmetry         = 1 << 23
             , VSymmetry         = 1 << 24
             };

  const unsigned int sideMask         =  0x1F;
  const unsigned int targetMask       =  0x20;
  const unsigned int notPassableMask  = 0x3C0; // North - South - East - West
  const unsigned int notPassableNMask =  0x40; // North 
  const unsigned int notPassableSMask =  0x80; // South 
  const unsigned int notPassableEMask = 0x100; // East
  const unsigned int notPassableWMask = 0x200; // West

  enum FunctionFlags { NoFlags = 0
                     , ShowDiff = (1<<0)
                     };

// Make sure these restrictions are the same than in the class Device
  enum Restriction { NorthBlocked = (1<<0)
                   , SouthBlocked = (1<<1)
                   , EastBlocked  = (1<<2)
                   , WestBlocked  = (1<<3)
                   , AllBlocked   = NorthBlocked|SouthBlocked|EastBlocked|WestBlocked
                   };


  class BoxSet;
  class NodeSets;
  class ChannelRouting;
  class SlicingNode;
  class HVSlicingNode;
  class HSlicingNode;
  class VSlicingNode;
  class RHVSlicingNode;
  class RHSlicingNode;
  class RVSlicingNode;

  typedef  std::pair<size_t,size_t>         Symmetry;
  typedef  std::list<Symmetry>              LSymmetries;
  typedef  std::list<SlicingNode*>          LSlicingNodes;
  typedef  std::vector<SlicingNode*>        VSlicingNodes;


}  // Bora namespace.

#endif  // BORA_CONSTANTS_H
