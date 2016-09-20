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

  const unsigned int  Flags::NoFlags             =  0;
// Flags used for both objects states & functions arguments.
  const unsigned int  Flags::Horizontal          = (1 <<  0);
  const unsigned int  Flags::Vertical            = (1 <<  1);
  const unsigned int  Flags::Source              = (1 <<  2);
  const unsigned int  Flags::Target              = (1 <<  3);
  const unsigned int  Flags::Invalidated         = (1 <<  4);
// Flags for GCell objects states only.                        
  const unsigned int  Flags::DeviceGCell         = (1 <<  5);
  const unsigned int  Flags::HChannelGCell       = (1 <<  6);
  const unsigned int  Flags::VChannelGCell       = (1 <<  7);
  const unsigned int  Flags::HStrutGCell         = (1 <<  8);
  const unsigned int  Flags::VStrutGCell         = (1 <<  9);
  const unsigned int  Flags::MatrixGCell         = (1 << 10);
  const unsigned int  Flags::IoPadGCell          = (1 << 11);
  const unsigned int  Flags::Saturated           = (1 << 12);
// Flags for Anabatic objects states only.                      
  const unsigned int  Flags::DemoMode            = (1 <<  5);
  const unsigned int  Flags::WarnOnGCellOverload = (1 <<  6);
  const unsigned int  Flags::DestroyGCell        = (1 <<  7);
  const unsigned int  Flags::DestroyBaseContact  = (1 <<  8);
  const unsigned int  Flags::DestroyBaseSegment  = (1 <<  9);
// Flags for NetDatas objects states only.                      
  const unsigned int  Flags::GlobalRouted        = (1 <<  5);
// Masks.                                      
  const unsigned int  Flags::WestSide            = Horizontal|Target;
  const unsigned int  Flags::EastSide            = Horizontal|Source;
  const unsigned int  Flags::SouthSide           = Vertical  |Target;
  const unsigned int  Flags::NorthSide           = Vertical  |Source;
  const unsigned int  Flags::AllSides            = WestSide|EastSide|SouthSide|NorthSide ;
  const unsigned int  Flags::EndsMask            = Source|Target;
  const unsigned int  Flags::DirectionMask       = Horizontal|Vertical;
  const unsigned int  Flags::DestroyMask         = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
  const unsigned int  Flags::GCellTypeMask       = DeviceGCell|HChannelGCell|VChannelGCell|HStrutGCell|VStrutGCell|MatrixGCell|IoPadGCell;
// Flags for functions arguments only.           
  const unsigned int  Flags::Create              = (1 <<  5);
  const unsigned int  Flags::WithPerpands        = (1 <<  6);
  const unsigned int  Flags::WithSelf            = (1 <<  7);
  const unsigned int  Flags::AboveLayer          = (1 <<  8);
  const unsigned int  Flags::BelowLayer          = (1 <<  9);
  const unsigned int  Flags::OpenSession         = (1 << 10);
  const unsigned int  Flags::Realignate          = (1 << 11);
  const unsigned int  Flags::NativeConstraints   = (1 << 12);
  const unsigned int  Flags::ForceMove           = (1 << 13);
  const unsigned int  Flags::WarnOnError         = (1 << 14);
  const unsigned int  Flags::Topology            = (1 << 15);
  const unsigned int  Flags::GlobalSegment       = (1 << 16);
  const unsigned int  Flags::AllowTerminal       = (1 << 17);
  const unsigned int  Flags::AllowLocal          = (1 << 18);
  const unsigned int  Flags::IgnoreContacts      = (1 << 19);
  const unsigned int  Flags::Propagate           = (1 << 20);
  const unsigned int  Flags::Superior            = (1 << 21);
  const unsigned int  Flags::DoglegOnLeft        = (1 << 22);
  const unsigned int  Flags::DoglegOnRight       = (1 << 23);
  const unsigned int  Flags::WithNeighbors       = (1 << 24);
  const unsigned int  Flags::NoCheckLayer        = (1 << 25);
  const unsigned int  Flags::HalfSlacken         = (1 << 26);
  const unsigned int  Flags::NoGCellShrink       = (1 << 27);
  const unsigned int  Flags::CParanoid           = (1 << 28);
  const unsigned int  Flags::CheckLowDensity     = (1 << 29);
  const unsigned int  Flags::CheckLowUpDensity   = (1 << 30);
  const unsigned int  Flags::NoUpdate            = (1 << 31);


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
    s += (_flags & HStrutGCell  ) ? 's' : '-';
    s += (_flags & VStrutGCell  ) ? 's' : '-';
    s += (_flags & MatrixGCell  ) ? 'm'  : '-';
    s += ",";
    s += (_flags & Invalidated  ) ? 'i'  : '-';
    s += (_flags & DestroyGCell ) ? 'D'  : '-';
    s += (_flags & AboveLayer   ) ? 'A'  : '-';
    s += (_flags & BelowLayer   ) ? 'B'  : '-';

    return s;
  }


}  // Anabatic namespace.
