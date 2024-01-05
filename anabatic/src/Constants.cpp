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


#include "hurricane/Error.h"
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::BaseFlags;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "Anabatic::Flags".

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
  const BaseFlags  Flags::DisableCanonize     = (1L << 10);
// Flags for NetDatas objects states only.                      
  const BaseFlags  Flags::GlobalFixed         = (1L <<  5);
  const BaseFlags  Flags::GlobalEstimated     = (1L <<  6);
  const BaseFlags  Flags::GlobalRouted        = (1L <<  7);
  const BaseFlags  Flags::DetailRouted        = (1L <<  8);
  const BaseFlags  Flags::ExcludeRoute        = (1L <<  9);
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
  const BaseFlags  Flags::NoCheckGCell        = (1L << 27);
  const BaseFlags  Flags::HalfSlacken         = (1L << 28);
  const BaseFlags  Flags::NoGCellShrink       = (1L << 29);
  const BaseFlags  Flags::CParanoid           = (1L << 30);
  const BaseFlags  Flags::CheckLowDensity     = (1L << 31);
  const BaseFlags  Flags::CheckLowUpDensity   = (1L << 32);
  const BaseFlags  Flags::NoUpdate            = (1L << 33);
  const BaseFlags  Flags::NorthPath           = (1L << 34);
  const BaseFlags  Flags::UseNonPref          = (1L << 35);
  const BaseFlags  Flags::Force               = (1L << 36);
  const BaseFlags  Flags::LayerCapOnly        = (1L << 37);
  const BaseFlags  Flags::NoMinLength         = (1L << 38);
  const BaseFlags  Flags::NoSegExt            = (1L << 39);
  const BaseFlags  Flags::NullLength          = (1L << 40);
  const BaseFlags  Flags::OnVSmall            = (1L << 41);
  const BaseFlags  Flags::Unbreakable         = (1L << 42);
  const BaseFlags  Flags::ForOffgrid          = (1L << 43);


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
    s += (_flags & (uint64_t)HRailGCell   ) ? 'H' : '-';
    s += (_flags & (uint64_t)VRailGCell   ) ? 'V' : '-';
    s += (_flags & (uint64_t)StrutGCell   ) ? 'S' : '-';
    s += (_flags & (uint64_t)MatrixGCell  ) ? 'M' : '-';
    s += (_flags & (uint64_t)StdCellRow   ) ? 'R' : '-';
    s += (_flags & (uint64_t)ChannelRow   ) ? 'C' : '-';
    s += (_flags & (uint64_t)GoStraight   ) ? 'g' : '-';
    s += ",";
    s += (_flags & (uint64_t)Invalidated  ) ? 'i' : '-';
    s += (_flags & (uint64_t)DestroyGCell ) ? 'D' : '-';
    s += (_flags & (uint64_t)AboveLayer   ) ? 'A' : '-';
    s += (_flags & (uint64_t)BelowLayer   ) ? 'B' : '-';

    return s;
  }


// -------------------------------------------------------------------
// Class  :  "Anabatic::StyleFlags".

  
  const BaseFlags  StyleFlags::NoStyle   =  0;
  const BaseFlags  StyleFlags::HV        = (1L <<  0);
  const BaseFlags  StyleFlags::VH        = (1L <<  1);
  const BaseFlags  StyleFlags::OTH       = (1L <<  2);
  const BaseFlags  StyleFlags::Channel   = (1L <<  3);
  const BaseFlags  StyleFlags::Hybrid    = (1L <<  4);
  const BaseFlags  StyleFlags::M1Offgrid = (1L <<  5);


  StyleFlags::~StyleFlags ()
  { }


  StyleFlags  StyleFlags::toFlag ( std::string textFlag )
  {
    if (textFlag == "HV")        return HV;
    if (textFlag == "VH")        return VH;
    if (textFlag == "OTH")       return OTH;
    if (textFlag == "Channel")   return Channel;
    if (textFlag == "Hybrid")    return Hybrid;
    if (textFlag == "NoStyle")   return NoStyle;
    if (textFlag == "M1Offgrid") return M1Offgrid;
    std::cerr << Error( "StyleFlags::toFlag(): Unknown flag value \"%s\"", textFlag.c_str() ) << std::endl;
    return NoStyle;
  }


  StyleFlags  StyleFlags::from ( std::string textFlags )
  {
    size_t start = 0;
    size_t stop  = textFlags.find( '|' );
    while ( stop != string::npos ) {
      *this |= toFlag( textFlags.substr( start, stop-start-1 ));
      start = stop + 1;
      stop  = textFlags.find( '|', start );
    }
    *this |= toFlag( textFlags.substr( stop+1 ));
    return *this;
  }


  string  StyleFlags::asString () const
  {
    ostringstream s;

    if (_flags & (uint64_t)HV)        { s << (s.tellp() ? "|" : "") << "HV"; }
    if (_flags & (uint64_t)VH)        { s << (s.tellp() ? "|" : "") << "VH"; }
    if (_flags & (uint64_t)OTH)       { s << (s.tellp() ? "|" : "") << "OTH"; }
    if (_flags & (uint64_t)Channel)   { s << (s.tellp() ? "|" : "") << "Channel"; }
    if (_flags & (uint64_t)Hybrid )   { s << (s.tellp() ? "|" : "") << "Hybrid"; }
    if (_flags & (uint64_t)M1Offgrid) { s << (s.tellp() ? "|" : "") << "M1Offgrid"; }
    s << " (0x" << hex << _flags << ")";
    return s.str();
  }


  string  StyleFlags::_getTypeName () const
  { return "Anabatic::StyleFlags"; }


  string StyleFlags::_getString () const
  { return asString(); }
  

}  // Anabatic namespace.
