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
  const uint64_t  Flags::Horizontal          = (1 <<  0);
  const uint64_t  Flags::Vertical            = (1 <<  1);
  const uint64_t  Flags::Source              = (1 <<  2);
  const uint64_t  Flags::Target              = (1 <<  3);
  const uint64_t  Flags::Invalidated         = (1 <<  4);
// Flags for GCell objects states only.                        
  const uint64_t  Flags::DeviceGCell         = (1 <<  5);
  const uint64_t  Flags::HChannelGCell       = (1 <<  6);
  const uint64_t  Flags::VChannelGCell       = (1 <<  7);
  const uint64_t  Flags::StrutGCell          = (1 <<  8);
  const uint64_t  Flags::MatrixGCell         = (1 <<  9);
  const uint64_t  Flags::IoPadGCell          = (1 << 10);
  const uint64_t  Flags::Saturated           = (1 << 11);
// Flags for Anabatic objects states only.                      
  const uint64_t  Flags::DemoMode            = (1 <<  5);
  const uint64_t  Flags::WarnOnGCellOverload = (1 <<  6);
  const uint64_t  Flags::DestroyGCell        = (1 <<  7);
  const uint64_t  Flags::DestroyBaseContact  = (1 <<  8);
  const uint64_t  Flags::DestroyBaseSegment  = (1 <<  9);
// Flags for NetDatas objects states only.                      
  const uint64_t  Flags::GlobalRouted        = (1 <<  5);
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
  const uint64_t  Flags::Create              = (1 <<  5);
  const uint64_t  Flags::WithPerpands        = (1 <<  6);
  const uint64_t  Flags::WithSelf            = (1 <<  7);
  const uint64_t  Flags::AboveLayer          = (1 <<  8);
  const uint64_t  Flags::BelowLayer          = (1 <<  9);
  const uint64_t  Flags::OpenSession         = (1 << 10);
  const uint64_t  Flags::Realignate          = (1 << 11);
  const uint64_t  Flags::NativeConstraints   = (1 << 12);
  const uint64_t  Flags::ForceMove           = (1 << 13);
  const uint64_t  Flags::WarnOnError         = (1 << 14);
  const uint64_t  Flags::Topology            = (1 << 15);
  const uint64_t  Flags::GlobalSegment       = (1 << 16);
  const uint64_t  Flags::AllowTerminal       = (1 << 17);
  const uint64_t  Flags::AllowLocal          = (1 << 18);
  const uint64_t  Flags::IgnoreContacts      = (1 << 19);
  const uint64_t  Flags::Propagate           = (1 << 20);
  const uint64_t  Flags::Superior            = (1 << 21);
  const uint64_t  Flags::DoglegOnLeft        = (1 << 22);
  const uint64_t  Flags::DoglegOnRight       = (1 << 23);
  const uint64_t  Flags::WithNeighbors       = (1 << 24);
  const uint64_t  Flags::NoCheckLayer        = (1 << 25);
  const uint64_t  Flags::HalfSlacken         = (1 << 26);
  const uint64_t  Flags::NoGCellShrink       = (1 << 27);
  const uint64_t  Flags::CParanoid           = (1 << 28);
  const uint64_t  Flags::CheckLowDensity     = (1 << 29);
  const uint64_t  Flags::CheckLowUpDensity   = (1 << 30);
  const uint64_t  Flags::NoUpdate            = (1 << 31);


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
