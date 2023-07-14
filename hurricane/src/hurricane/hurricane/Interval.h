// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2023, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Rémy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Interval.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/DbU.h"

namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Interval".


  class Interval {
    public:
      class CompareByMin {
        public:
          inline bool  operator() ( const Interval& rhs, const Interval& lhs ) const;
          inline bool  operator() ( const Interval* rhs, const Interval* lhs ) const;
      };
      class CompareByMinMax {
        public:
          inline bool  operator() ( const Interval& rhs, const Interval& lhs ) const;
          inline bool  operator() ( const Interval* rhs, const Interval* lhs ) const;
      };
    public:
                              Interval        ( bool makeEmpty=true );
                              Interval        ( const DbU::Unit& );
                              Interval        ( const DbU::Unit& v1, const DbU::Unit& v2 );
                              Interval        ( const Interval& );
    public:
                   Interval&  operator=       ( const Interval& );
                   bool       operator==      ( const Interval& ) const;
                   bool       operator!=      ( const Interval& ) const;
    public:
      inline const DbU::Unit& getVMin         () const;
      inline const DbU::Unit& getVMax         () const;
      inline       DbU::Unit& getVMin         ();
      inline       DbU::Unit& getVMax         ();
      inline       DbU::Unit  getCenter       () const;
      inline       DbU::Unit  getSize         () const;
      inline       DbU::Unit  getHalfSize     () const;
                   Interval   getUnion        ( const Interval& ) const;
                   Interval   getIntersection ( const Interval& ) const;
      inline       bool       isEmpty         () const;
      inline       bool       isFull          () const;
      inline       bool       isPonctual      () const;
                   bool       contains        ( const DbU::Unit& ) const;
                   bool       contains        ( const Interval& ) const;
                   bool       intersect       ( const Interval& , bool strict=false ) const;
                   bool       inferior        ( const Interval& , bool strict=true ) const;
                   bool       superior        ( const Interval& , bool strict=true ) const;
                   bool       isConstrainedBy ( const Interval& ) const;
    public:
                   Interval&  makeEmpty       ();
                   Interval&  shrinkVMin      ( const DbU::Unit& vMin );
                   Interval&  shrinkVMax      ( const DbU::Unit& vMax );
                   Interval&  inflate         ( const DbU::Unit& dv );
                   Interval&  inflate         ( const DbU::Unit& dvMin, const DbU::Unit& dvMax );
                   Interval&  merge           ( const DbU::Unit& );
                   Interval&  merge           ( const Interval& );
                   Interval&  intersection    ( const DbU::Unit& vMin, const DbU::Unit& vMax );
                   Interval&  intersection    ( const Interval& );
                   Interval&  translate       ( const DbU::Unit& );
    public:
      inline       string     _getTypeName    () const;
                   string     _getString      () const;
                   Record*    _getRecord      () const;
    private:
      DbU::Unit _vMin;
      DbU::Unit _vMax;
  };


  inline const DbU::Unit& Interval::getVMin      () const {return _vMin;};
  inline const DbU::Unit& Interval::getVMax      () const {return _vMax;};
  inline       DbU::Unit& Interval::getVMin      () {return _vMin;};
  inline       DbU::Unit& Interval::getVMax      () {return _vMax;};
  inline       DbU::Unit  Interval::getCenter    () const {return ((_vMin + _vMax) / 2);};
  inline       DbU::Unit  Interval::getHalfSize  () const {return (getSize() / 2);};
  inline       bool       Interval::isEmpty      () const { return (_vMax < _vMin);};
  inline       bool       Interval::isFull       () const { return (_vMin == DbU::Min) and (_vMax == DbU::Max); };
  inline       bool       Interval::isPonctual   () const { return (_vMax == _vMin);};
  inline       string     Interval::_getTypeName () const { return _TName("Interval"); };

  inline  DbU::Unit  Interval::getSize () const
  {
    if (isEmpty()) return 0;
    if (isFull ()) return DbU::Max;
    return _vMax - _vMin;
  }


  inline bool  Interval::CompareByMin::operator() ( const Interval& lhs, const Interval& rhs ) const
  { return lhs.getVMin() < rhs.getVMin(); }

  
  inline bool  Interval::CompareByMin::operator() ( const Interval* lhs, const Interval* rhs ) const
  { return lhs->getVMin() < rhs->getVMin(); }


  inline bool  Interval::CompareByMinMax::operator() ( const Interval& lhs, const Interval& rhs ) const
  {
    if (lhs.getVMin() != rhs.getVMin()) return lhs.getVMin() < rhs.getVMin();
    return lhs.getVMax() < rhs.getVMax();
  }

  
  inline bool  Interval::CompareByMinMax::operator() ( const Interval* lhs, const Interval* rhs ) const
  {
    if (lhs->getVMin() != rhs->getVMin()) return lhs->getVMin() < rhs->getVMin();
    return lhs->getVMax() < rhs->getVMax();
  }


} // Hurricane namespace.


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const Hurricane::Interval* interval )
{
  w->key( key );
  w->startArray();
  w->write( &interval->getVMin() );
  w->write( &interval->getVMax() );
  w->endArray();
}


INSPECTOR_PR_SUPPORT(Hurricane::Interval);
