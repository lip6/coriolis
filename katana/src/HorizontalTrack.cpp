
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HorizontalTrack.cpp"                    |
// +-----------------------------------------------------------------+


#include "katana/HorizontalTrack.h"


namespace Katana {


// -------------------------------------------------------------------
// Class  :  "HorizontalTrack".


  HorizontalTrack::HorizontalTrack ( RoutingPlane* routingPlane, unsigned int index )
    : Track(routingPlane,index)
  { }


  void  HorizontalTrack::_postCreate ()
  { }


  HorizontalTrack* HorizontalTrack::create ( RoutingPlane* routingPlane, unsigned int index )
  {
    HorizontalTrack* track = new HorizontalTrack ( routingPlane, index );

    track->_postCreate ();
    return track;
  }


  HorizontalTrack::~HorizontalTrack ()
  { }


  void  HorizontalTrack::_preDestroy ()
  { }


  bool   HorizontalTrack::isHorizontal () const { return true; }
  bool   HorizontalTrack::isVertical   () const { return false; }
  Flags  HorizontalTrack::getDirection () const { return Flags::Horizontal; }


  Point  HorizontalTrack::getPosition ( DbU::Unit coordinate ) const
  {
    return Point ( coordinate, getAxis() );
  }


  string  HorizontalTrack::_getTypeName () const
  { return "HorizontalTrack"; }


  Record* HorizontalTrack::_getRecord () const
  {
    Record* record = Track::_getRecord ();
    return record;
  }


} // Katana namespace.
