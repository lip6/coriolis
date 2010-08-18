
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Row.cpp"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Row.h"


namespace Bookshelf {


  void  Row::writeToStream ( std::ostream& o )
  {
    o << "CoreRow Horizontal\n";
    o << "  Coordinate    :" << std::setw(10) << std::right << _coordinate << "\n";
    o << "  Height        :" << std::setw(10) << std::right << _height << "\n";
    o << "  Sitewidth     :" << std::setw(10) << std::right << _sitewidth << "\n";
    o << "  Sitespacing   :" << std::setw(10) << std::right << _sitespacing << "\n";

    o << "  Siteorient    : ";
    if      ( _siteorient == Orientation::Disabled ) o << " N";
    else if ( _siteorient == Orientation::N        ) o << " N";
    else if ( _siteorient == Orientation::E        ) o << " E";
    else if ( _siteorient == Orientation::S        ) o << " S";
    else if ( _siteorient == Orientation::W        ) o << " W";
    else if ( _siteorient == Orientation::FN       ) o << " FN";
    else if ( _siteorient == Orientation::FE       ) o << " FE";
    else if ( _siteorient == Orientation::FS       ) o << " FS";
    else if ( _siteorient == Orientation::FW       ) o << " FW";
    o << "\n";

    o << "  Sitesymmetry  : ";
    if      ( _sitesymmetry == Symmetry::Disabled ) o << " N";
    else if ( _sitesymmetry == Symmetry::R90      ) o << " R90";
    else if ( _sitesymmetry == Symmetry::X        ) o << " X";
    else if ( _sitesymmetry == Symmetry::Y        ) o << " Y";
    o << "\n";

    o << "  SubrowOrigin  :" << std::setw(10) << std::right << _subrowOrigin;
    o <<  " Numsites :"      << std::setw(10) << std::right << _numsites << "\n";

    o << "End" << std::endl;
  }


}  // End of Bookshelf namespace.
