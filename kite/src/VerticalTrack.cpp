
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./VerticalTrack.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "kite/VerticalTrack.h"


namespace Kite {


// -------------------------------------------------------------------
// Class  :  "VerticalTrack".


  VerticalTrack::VerticalTrack ( RoutingPlane* routingPlane, unsigned int index )
    : Track(routingPlane,index)
  { }


  void  VerticalTrack::_postCreate ()
  { }


  VerticalTrack* VerticalTrack::create ( RoutingPlane* routingPlane, unsigned int index )
  {
    VerticalTrack* track = new VerticalTrack ( routingPlane, index );

    track->_postCreate ();

    return track;
  }


  VerticalTrack::~VerticalTrack ()
  { }


  void  VerticalTrack::_preDestroy ()
  { }


  bool          VerticalTrack::isHorizontal () const { return false; }
  bool          VerticalTrack::isVertical   () const { return true; }
  unsigned int  VerticalTrack::getDirection () const { return Constant::Vertical; }


  Point  VerticalTrack::getPosition ( DbU::Unit coordinate ) const
  { return Point ( getAxis(), coordinate ); }


  string  VerticalTrack::_getTypeName () const
  { return "VerticalTrack"; }


  Record* VerticalTrack::_getRecord () const
  {
    Record* record = Track::_getRecord ();
    return record;
  }


} // End of Kite namespace.
