// -*- mode: C++; explicit-buffer-name: "Constants.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Constants.cpp"                               |
// +-----------------------------------------------------------------+


#include <string>
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;

  const uint64_t  Flags::NoFlags             =  0;
// Flags used for both objects states & functions arguments.
  const uint64_t  Flags::Horizontal          = (1L <<  0);
  const uint64_t  Flags::Vertical            = (1L <<  1);
  const uint64_t  Flags::Source              = (1L <<  2);
  const uint64_t  Flags::Target              = (1L <<  3);
  const uint64_t  Flags::Invalidated         = (1L <<  4);
// Flags for GCell objects states only.                        
  const uint64_t  Flags::DeviceGCell         = (1L <<  5);
  const uint64_t  Flags::HChannelGCell       = (1L <<  6);
  const uint64_t  Flags::VChannelGCell       = (1L <<  7);
  const uint64_t  Flags::StrutGCell          = (1L <<  8);
  const uint64_t  Flags::MatrixGCell         = (1L <<  9);
  const uint64_t  Flags::IoPadGCell          = (1L << 10);
  const uint64_t  Flags::Saturated           = (1L << 11);
// Flags for Anabatic objects states only.                      
  const uint64_t  Flags::DemoMode            = (1L <<  5);
  const uint64_t  Flags::WarnOnGCellOverload = (1L <<  6);
  const uint64_t  Flags::DestroyGCell        = (1L <<  7);
  const uint64_t  Flags::DestroyBaseContact  = (1L <<  8);
  const uint64_t  Flags::DestroyBaseSegment  = (1L <<  9);
// Flags for NetDatas objects states only.                      
  const uint64_t  Flags::GlobalRouted        = (1L <<  5);
// Masks.                                      
  const uint64_t  Flags::WestSide            = Horizontal|Target;
  const uint64_t  Flags::EastSide            = Horizontal|Source;
  const uint64_t  Flags::SouthSide           = Vertical  |Target;
  const uint64_t  Flags::NorthSide           = Vertical  |Source;
  const uint64_t  Flags::AllSides            = WestSide|EastSide|SouthSide|NorthSide ;
  const uint64_t  Flags::EndsMask            = Source|Target;
  const uint64_t  Flags::DirectionMask       = Horizontal|Vertical;
  const uint64_t  Flags::DestroyMask         = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
  const uint64_t  Flags::GCellTypeMask       = DeviceGCell|HChannelGCell|VChannelGCell|StrutGCell|MatrixGCell|IoPadGCell;
// Flags for functions arguments only.           
  const uint64_t  Flags::Create              = (1L <<  5);
  const uint64_t  Flags::WithPerpands        = (1L <<  6);
  const uint64_t  Flags::WithDoglegs         = (1L <<  7);
  const uint64_t  Flags::WithSelf            = (1L <<  8);
  const uint64_t  Flags::AboveLayer          = (1L <<  9);
  const uint64_t  Flags::BelowLayer          = (1L << 10);
  const uint64_t  Flags::OpenSession         = (1L << 11);
  const uint64_t  Flags::Realignate          = (1L << 12);
  const uint64_t  Flags::NativeConstraints   = (1L << 13);
  const uint64_t  Flags::ForceMove           = (1L << 14);
  const uint64_t  Flags::WarnOnError         = (1L << 15);
  const uint64_t  Flags::Topology            = (1L << 16);
  const uint64_t  Flags::GlobalSegment       = (1L << 17);
  const uint64_t  Flags::AllowTerminal       = (1L << 18);
  const uint64_t  Flags::AllowLocal          = (1L << 19);
  const uint64_t  Flags::IgnoreContacts      = (1L << 20);
  const uint64_t  Flags::Propagate           = (1L << 21);
  const uint64_t  Flags::Superior            = (1L << 22);
  const uint64_t  Flags::DoglegOnLeft        = (1L << 23);
  const uint64_t  Flags::DoglegOnRight       = (1L << 24);
  const uint64_t  Flags::WithNeighbors       = (1L << 25);
  const uint64_t  Flags::NoCheckLayer        = (1L << 26);
  const uint64_t  Flags::HalfSlacken         = (1L << 27);
  const uint64_t  Flags::NoGCellShrink       = (1L << 28);
  const uint64_t  Flags::CParanoid           = (1L << 29);
  const uint64_t  Flags::CheckLowDensity     = (1L << 30);
  const uint64_t  Flags::CheckLowUpDensity   = (1L << 31);
  const uint64_t  Flags::NoUpdate            = (1L << 32);


  Flags::~Flags ()
  { }


  string  Flags::_getTypeName () const
  { return "Anabatic::Flags"; }


  string Flags::_getString () const
  {
    string s = "";
    s += (_flags & Horizontal   ) ? 'h'  : '-';
    s += (_flags & Vertical     ) ? 'v'  : '-';
    s += (_flags & Source       ) ? 'S'  : '-';
    s += (_flags & Target       ) ? 'T'  : '-';
    s += (_flags & DeviceGCell  ) ? 'd'  : '-';
    s += (_flags & HChannelGCell) ? 'c' : '-';
    s += (_flags & VChannelGCell) ? 'c' : '-';
    s += (_flags & StrutGCell   ) ? 's' : '-';
    s += (_flags & MatrixGCell  ) ? 'm'  : '-';
    s += ",";
    s += (_flags & Invalidated  ) ? 'i'  : '-';
    s += (_flags & DestroyGCell ) ? 'D'  : '-';
    s += (_flags & AboveLayer   ) ? 'A'  : '-';
    s += (_flags & BelowLayer   ) ? 'B'  : '-';

    return s;
  }


}  // Anabatic namespace.
