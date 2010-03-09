
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
// |  C++ Module  :       "./DataNegociate.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <cstdlib>
#include  <sstream>

#include  "kite/DataNegociate.h"


namespace Kite {

  using std::ostringstream;


// -------------------------------------------------------------------
// Class  :  "DataNegociate".


  DataNegociate::DataNegociate ( TrackElement* trackSegment )
    : _routingEvent(NULL)
    , _trackSegment(trackSegment)
    , _cost        (trackSegment)
    , _gcellOrder  ((unsigned int)-1)
    , _state       (RipupPerpandiculars)
    , _stateCount  (1)
    , _leftBorder  (false)
    , _rightBorder (false)
    , _ring        (false)
  //, _z           (RoutingGauge::getLayerDepth(trackSegment->getLayer()))
  { }


  DataNegociate::~DataNegociate ()
  { }


  void  DataNegociate::update ()
  {
    _cost.update ( _trackSegment );
  }


  string  DataNegociate::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(_trackSegment)
               + ">";
  }


  Record* DataNegociate::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_routingEvent",  _routingEvent ) );
    record->add ( getSlot ( "_trackSegment",  _trackSegment ) );
    record->add ( getSlot ( "_cost"        , &_cost         ) );
    record->add ( getSlot ( "_gcellOrder"  ,  _gcellOrder   ) );
  //record->add ( getSlot ( "_z"           ,  _z            ) );
                                     
    return record;
  }


  string  DataNegociate::getStateString ( DataNegociate* data )
  {
    ostringstream s;
    switch ( data->_state ) {
      case RipupPerpandiculars: s << "RipupPerpandiculars"; break;
      case Minimize:            s << "Minimize";            break;
      case DogLeg:              s << "DogLeg";              break;
      case Desalignate:         s << "Desalignate";         break;
      case Slacken:             s << "Slacken";             break;
      case ConflictSolve1:      s << "ConflictSolve1";      break;
      case ConflictSolve2:      s << "ConflictSolve2";      break;
      case LocalVsGlobal:       s << "LocalVsGlobal";       break;
      case MoveUp:              s << "MoveUp";              break;
      case MaximumSlack:        s << "MaximumSlack";        break;
      case Unimplemented:       s << "Unimplemented";       break;
      default:
        s << "Unknown(" << data->_state << ")"; break;
    }
    s << ":" << data->_stateCount;
    return s.str();
  }


} // End of Kite namespace.
