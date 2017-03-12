// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
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
  using Hurricane::Net;
  using Hurricane::NetRoutingState;
  using Hurricane::NetRoutingExtension;
  using Anabatic::AutoSegment;


  class DataSymmetric {
    public:
      static DataSymmetric* create         ( Net* );
    public:
      inline bool           isValid        () const;
      inline bool           isSymVertical  () const;
      inline Net*           getNet         () const;
      inline DbU::Unit      getSymAxis     () const;
      inline Net*           getSymNet      () const;
      inline Point&         getSymmetrical ( Point& ) const;
             AutoSegment*   getSymmetrical ( AutoSegment* ) const;
             void           addSymmetrical ( AutoSegment* );
      inline void           addReference   ( AutoSegment* );
      inline void           setValid       ( bool );
             bool           checkPairing   ();
             void           print          ( std::ostream& ) const;
             Record*        _getRecord     () const;
             std::string    _getString     () const;
    private:                
                            DataSymmetric  ( Net* );
    private:
      bool                                       _valid;
      Net*                                       _net;
      Net*                                       _symNet;
      NetRoutingState*                           _state;
      std::vector< std::array<AutoSegment*,2> >  _paireds;
      size_t                                     _symIndex;
  };


  inline bool       DataSymmetric::isValid       () const { return _valid; }
  inline bool       DataSymmetric::isSymVertical () const { return _state->isSymVertical(); }
  inline Net*       DataSymmetric::getNet        () const { return _net; }
  inline DbU::Unit  DataSymmetric::getSymAxis    () const { return _state->getSymAxis(); }
  inline Net*       DataSymmetric::getSymNet     () const { return _symNet; }
  inline void       DataSymmetric::addReference  ( AutoSegment* segment ) { _paireds.push_back( {segment,NULL} ); }
  inline void       DataSymmetric::setValid      ( bool state ) { _valid = state; }

  inline Point& DataSymmetric::getSymmetrical ( Point& point ) const
  {
    if (_state->isSymVertical()) point.setX( 2*getSymAxis() - point.getX() ); 
    else                         point.setY( 2*getSymAxis() - point.getY() ); 
    return point;
  }


}  // Katana namespace.

#endif  // KATANA_DATA_SYMMETRIC_H


INSPECTOR_P_SUPPORT(Katana::DataSymmetric);
