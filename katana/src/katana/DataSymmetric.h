// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./katana/DataSymmetric.h"                     |
// +-----------------------------------------------------------------+


#ifndef  KATANA_DATA_SYMMETRIC_H
#define  KATANA_DATA_SYMMETRIC_H

#include <string>
#include <vector>
#include <array>
#include "hurricane/Net.h"
#include "hurricane/NetRoutingProperty.h"

namespace Hurricane {
  class Record;
}

namespace Anabatic {
  class AutoSegment;
}


namespace Katana {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Interval;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Anabatic::AutoSegment;


  class DataSymmetric {
    public:
      typedef std::vector< std::array<AutoSegment*,2> >  Paireds;
    public:
      static DataSymmetric* create         ( Net* );
    public:
      inline bool           isValid        () const;
      inline bool           isSymVertical  () const;
      inline Net*           getNet         () const;
      inline DbU::Unit      getSymAxis     () const;
      inline Net*           getSymNet      () const;
      inline const Paireds& getPaireds     () const;
      inline DbU::Unit      getSymmetrical ( DbU::Unit ) const;
      inline Point          getSymmetrical ( const Point& ) const;
      inline Interval       getSymmetrical ( const Interval& ) const;
             AutoSegment*   getSymmetrical ( AutoSegment* ) const;
             void           addSymmetrical ( AutoSegment* );
      inline void           addReference   ( AutoSegment* );
      inline void           setValid       ( bool );
      inline void           setSymAxis     ( DbU::Unit );
             bool           checkPairing   ();
             void           print          ( std::ostream& ) const;
             Record*        _getRecord     () const;
             std::string    _getString     () const;
    private:                
                            DataSymmetric  ( Net* );
    private:
      bool              _valid;
      Net*              _net;
      Net*              _symNet;
      NetRoutingState*  _state;
      Paireds           _paireds;
      size_t            _symIndex;
  };


  inline bool                          DataSymmetric::isValid       () const { return _valid; }
  inline bool                          DataSymmetric::isSymVertical () const { return _state->isSymVertical(); }
  inline const DataSymmetric::Paireds& DataSymmetric::getPaireds    () const { return _paireds; }
  inline Net*                          DataSymmetric::getNet        () const { return _net; }
  inline DbU::Unit                     DataSymmetric::getSymAxis    () const { return _state->getSymAxis(); }
  inline void                          DataSymmetric::setSymAxis    ( DbU::Unit axis ) { _state->setSymAxis(axis); }
  inline Net*                          DataSymmetric::getSymNet     () const { return _symNet; }
  inline void                          DataSymmetric::addReference  ( AutoSegment* segment ) { _paireds.push_back( {{segment,NULL}} ); }
  inline void                          DataSymmetric::setValid      ( bool state ) { _valid = state; }

  inline DbU::Unit  DataSymmetric::getSymmetrical ( DbU::Unit pos ) const
  { return 2*getSymAxis() - pos; }

  inline Point  DataSymmetric::getSymmetrical ( const Point& point ) const
  {
    Point symPoint ( point );
    if (_state->isSymVertical()) symPoint.setX( 2*getSymAxis() - point.getX() ); 
    else                         symPoint.setY( 2*getSymAxis() - point.getY() ); 
    return symPoint;
  }

  inline Interval  DataSymmetric::getSymmetrical ( const Interval& interval ) const
  {
    DbU::Unit delta = 0;
    DbU::Unit vmin  = interval.getVMin();
    DbU::Unit vmax  = interval.getVMax();

    if ( (vmin != DbU::Min) and (vmin != DbU::Max) ) delta = -1;
    if ( (vmin != DbU::Min) and (vmin != DbU::Max) ) vmin  = 2*getSymAxis() - vmin + delta;
    if ( (vmax != DbU::Max) and (vmax != DbU::Max) ) vmax  = 2*getSymAxis() - vmax + delta;

    return Interval( vmin, vmax );
  }


  typedef  std::map<Net*,DataSymmetric*,Entity::CompareById>  DataSymmetricMap;

}  // Katana namespace.

#endif  // KATANA_DATA_SYMMETRIC_H


INSPECTOR_P_SUPPORT(Katana::DataSymmetric);
