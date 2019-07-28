// -*- mode: C++; explicit-buffer-name: "Constants.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
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


#include "anabatic/Constants.h"


namespace Anabatic {

  using std::string;
  using std::ostringstream;
  using Hurricane::BaseFlags;


  const BaseFlags  Flags::NoFlags             =  0;
// Flags used for both objects states & functions arguments.
  const BaseFlags  Flags::Horizontal          = (1L <<  0);
  const BaseFlags  Flags::Vertical            = (1L <<  1);
  const BaseFlags  Flags::Source              = (1L <<  2);
  const BaseFlags  Flags::Target              = (1L <<  3);
  const BaseFlags  Flags::Invalidated         = (1L <<  4);
// Flags for GCell objects states only.                        
  const BaseFlags  Flags::DeviceGCell         = (1L <<  5);
  const BaseFlags  Flags::HChannelGCell       = (1L <<  6);
  const BaseFlags  Flags::VChannelGCell       = (1L <<  7);
  const BaseFlags  Flags::StrutGCell          = (1L <<  8);
  const BaseFlags  Flags::MatrixGCell         = (1L <<  9);
  const BaseFlags  Flags::IoPadGCell          = (1L << 10);
  const BaseFlags  Flags::Saturated           = (1L << 11);
  const BaseFlags  Flags::StdCellRow          = (1L << 12);
  const BaseFlags  Flags::ChannelRow          = (1L << 13);
  const BaseFlags  Flags::HRailGCell          = (1L << 14);
  const BaseFlags  Flags::VRailGCell          = (1L << 15);
  const BaseFlags  Flags::GoStraight          = (1L << 16);
// Flags for Edge objects states only.                      
  const BaseFlags  Flags::NullCapacity        = (1L <<  5);
  const BaseFlags  Flags::InfiniteCapacity    = (1L <<  6);
// Flags for Anabatic objects states only.                      
  const BaseFlags  Flags::DemoMode            = (1L <<  5);
  const BaseFlags  Flags::WarnOnGCellOverload = (1L <<  6);
  const BaseFlags  Flags::DestroyGCell        = (1L <<  7);
  const BaseFlags  Flags::DestroyBaseContact  = (1L <<  8);
  const BaseFlags  Flags::DestroyBaseSegment  = (1L <<  9);
// Flags for NetDatas objects states only.                      
  const BaseFlags  Flags::GlobalRouted        = (1L <<  5);
  const BaseFlags  Flags::GlobalEstimated     = (1L <<  6);
  const BaseFlags  Flags::ExcludeRoute        = (1L <<  7);
// Masks.                                      
  const BaseFlags  Flags::WestSide            = Horizontal|Target;
  const BaseFlags  Flags::EastSide            = Horizontal|Source;
  const BaseFlags  Flags::SouthSide           = Vertical  |Target;
  const BaseFlags  Flags::NorthSide           = Vertical  |Source;
  const BaseFlags  Flags::AllSides            = WestSide|EastSide|SouthSide|NorthSide ;
  const BaseFlags  Flags::EndsMask            = Source|Target;
  const BaseFlags  Flags::DirectionMask       = Horizontal|Vertical;
  const BaseFlags  Flags::DestroyMask         = DestroyGCell|DestroyBaseContact|DestroyBaseSegment;
  const BaseFlags  Flags::GCellTypeMask       = DeviceGCell
                                              | HChannelGCell
                                              | VChannelGCell
                                              | StrutGCell
                                              | MatrixGCell
                                              | IoPadGCell
                                              | StdCellRow
                                              | ChannelRow
                                              | HRailGCell
                                              | VRailGCell;
  const BaseFlags  Flags::RowGCellMask        = StdCellRow|ChannelRow;
  const BaseFlags  Flags::AnalogGCellMask     = DeviceGCell
                                              | HChannelGCell
                                              | VChannelGCell
                                              | StrutGCell
                                              | HRailGCell
                                              | VRailGCell;
  const BaseFlags  Flags::EdgeCapacityMask    = Horizontal
                                              | Vertical
                                              | NullCapacity
                                              | InfiniteCapacity ;
// Flags for functions arguments only.           
  const BaseFlags  Flags::Create              = (1L <<  5);
  const BaseFlags  Flags::WithPerpands        = (1L <<  6);
  const BaseFlags  Flags::WithDoglegs         = (1L <<  7);
  const BaseFlags  Flags::WithSelf            = (1L <<  8);
  const BaseFlags  Flags::AboveLayer          = (1L <<  9);
  const BaseFlags  Flags::BelowLayer          = (1L << 10);
  const BaseFlags  Flags::OpenSession         = (1L << 11);
  const BaseFlags  Flags::Realignate          = (1L << 12);
  const BaseFlags  Flags::NativeConstraints   = (1L << 13);
  const BaseFlags  Flags::ForceMove           = (1L << 14);
  const BaseFlags  Flags::WarnOnError         = (1L << 15);
  const BaseFlags  Flags::Topology            = (1L << 16);
  const BaseFlags  Flags::GlobalSegment       = (1L << 17);
  const BaseFlags  Flags::AllowTerminal       = (1L << 18);
  const BaseFlags  Flags::AllowLocal          = (1L << 19);
  const BaseFlags  Flags::IgnoreContacts      = (1L << 20);
  const BaseFlags  Flags::Propagate           = (1L << 21);
  const BaseFlags  Flags::Superior            = (1L << 22);
  const BaseFlags  Flags::DoglegOnLeft        = (1L << 23);
  const BaseFlags  Flags::DoglegOnRight       = (1L << 24);
  const BaseFlags  Flags::WithNeighbors       = (1L << 25);
  const BaseFlags  Flags::NoCheckLayer        = (1L << 26);
  const BaseFlags  Flags::HalfSlacken         = (1L << 27);
  const BaseFlags  Flags::NoGCellShrink       = (1L << 28);
  const BaseFlags  Flags::CParanoid           = (1L << 29);
  const BaseFlags  Flags::CheckLowDensity     = (1L << 30);
  const BaseFlags  Flags::CheckLowUpDensity   = (1L << 31);
  const BaseFlags  Flags::NoUpdate            = (1L << 32);
  const BaseFlags  Flags::NorthPath           = (1L << 33);
  const BaseFlags  Flags::UseNonPref          = (1L << 34);


  Flags::~Flags ()
  { }


  string  Flags::asString ( uint32_t mode ) const
  {
    ostringstream s;

    s << ((_flags & (uint64_t)Horizontal ) ? "h" : "-");
    s << ((_flags & (uint64_t)Vertical   ) ? "v" : "-");
    s << ((_flags & (uint64_t)Source     ) ? "S" : "-");
    s << ((_flags & (uint64_t)Target     ) ? "T" : "-");
    s << ((_flags & (uint64_t)Invalidated) ? "i" : "-");

    switch ( mode ) {
      case FlagsFunction:
        s << ((_flags & (uint64_t)Create           ) ? "C" : "-");
        s << ((_flags & (uint64_t)WithPerpands     ) ? "P" : "-");
        s << ((_flags & (uint64_t)WithDoglegs      ) ? "D" : "-");
        s << ((_flags & (uint64_t)WithSelf         ) ? "s" : "-");
        s << ((_flags & (uint64_t)AboveLayer       ) ? "a" : "-");
        s << ((_flags & (uint64_t)BelowLayer       ) ? "b" : "-");
        s << ((_flags & (uint64_t)OpenSession      ) ? "o" : "-");
        s << ((_flags & (uint64_t)Realignate       ) ? "R" : "-");
        s << ((_flags & (uint64_t)NativeConstraints) ? "N" : "-");
        s << ((_flags & (uint64_t)ForceMove        ) ? "m" : "-");
        s << ((_flags & (uint64_t)WarnOnError      ) ? "w" : "-");
        s << ((_flags & (uint64_t)Topology         ) ? "t" : "-");
        s << ((_flags & (uint64_t)GlobalSegment    ) ? "G" : "-");
        s << ((_flags & (uint64_t)AllowTerminal    ) ? "t" : "-");
        s << ((_flags & (uint64_t)AllowLocal       ) ? "l" : "-");
        s << ((_flags & (uint64_t)IgnoreContacts   ) ? "i" : "-");
        s << ((_flags & (uint64_t)Propagate        ) ? "p" : "-");
        s << ((_flags & (uint64_t)Superior         ) ? "S" : "-");
        s << ((_flags & (uint64_t)DoglegOnLeft     ) ? "L" : "-");
        s << ((_flags & (uint64_t)DoglegOnRight    ) ? "R" : "-");
        s << ((_flags & (uint64_t)WithNeighbors    ) ? "N" : "-");
        s << ((_flags & (uint64_t)NoCheckLayer     ) ? "L" : "-");
        s << ((_flags & (uint64_t)HalfSlacken      ) ? "h" : "-");
        s << ((_flags & (uint64_t)NoGCellShrink    ) ? "s" : "-");
        s << ((_flags & (uint64_t)CParanoid        ) ? "p" : "-");
        s << ((_flags & (uint64_t)CheckLowDensity  ) ? "l" : "-");
        s << ((_flags & (uint64_t)CheckLowUpDensity) ? "u" : "-");
        s << ((_flags & (uint64_t)NoUpdate         ) ? "u" : "-");
        break;
    }

    s << " (" << value() << ")";
    return s.str();
  }


  string  Flags::_getTypeName () const
  { return "Anabatic::Flags"; }


  string Flags::_getString () const
  {
    string s = "";
    s += (_flags & (uint64_t)Horizontal   ) ? 'h' : '-';
    s += (_flags & (uint64_t)Vertical     ) ? 'v' : '-';
    s += (_flags & (uint64_t)Source       ) ? 'S' : '-';
    s += (_flags & (uint64_t)Target       ) ? 'T' : '-';
    s += (_flags & (uint64_t)DeviceGCell  ) ? 'd' : '-';
    s += (_flags & (uint64_t)HChannelGCell) ? 'c' : '-';
    s += (_flags & (uint64_t)VChannelGCell) ? 'c' : '-';
    s += (_flags & (uint64_t)HRailGCell   ) ? 'r' : '-';
    s += (_flags & (uint64_t)VRailGCell   ) ? 'r' : '-';
    s += (_flags & (uint64_t)StrutGCell   ) ? 's' : '-';
    s += (_flags & (uint64_t)MatrixGCell  ) ? 'm' : '-';
    s += (_flags & (uint64_t)StdCellRow   ) ? 'S' : '-';
    s += (_flags & (uint64_t)ChannelRow   ) ? 'C' : '-';
    s += ",";
    s += (_flags & (uint64_t)Invalidated  ) ? 'i' : '-';
    s += (_flags & (uint64_t)DestroyGCell ) ? 'D' : '-';
    s += (_flags & (uint64_t)AboveLayer   ) ? 'A' : '-';
    s += (_flags & (uint64_t)BelowLayer   ) ? 'B' : '-';

    return s;
  }


}  // Anabatic namespace.
