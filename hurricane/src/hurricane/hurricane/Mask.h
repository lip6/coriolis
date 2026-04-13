// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Mask.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include <iomanip>
#include <sstream>
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/math/tools/precision.hpp"
#include "hurricane/Commons.h"


namespace Hurricane {


  template<typename IntType>
  class Mask {
    public:
      static        Mask     FFFF;
    // Methods.
             inline          Mask             ();
             inline explicit Mask             ( int mask );
             inline          Mask             ( IntType mask );
             inline bool     zero             () const;
             inline Mask&    set              ( const Mask mask );
             inline Mask&    unset            ( const Mask mask );
             inline bool     isSet            ( const Mask mask ) const;
             inline bool     contains         ( const Mask mask ) const;
             inline bool     intersect        ( const Mask mask ) const;
             inline Mask     nthbit           ( unsigned int ) const;
             inline Mask     operator compl   () const;
             inline Mask     operator bitand  ( const Mask mask ) const;
             inline Mask     operator bitor   ( const Mask mask ) const;
             inline Mask     operator xor     ( const Mask mask ) const;
             inline Mask     lshift           ( unsigned int ) const;
             inline Mask     rshift           ( unsigned int ) const;
             inline Mask&    operator +=      ( unsigned int );
             inline Mask&    operator |=      ( const Mask mask );
             inline Mask&    operator &=      ( const Mask mask );
             inline bool     operator not     () const;
             inline bool     operator and     ( const Mask mask ) const;
             inline bool     operator or      ( const Mask mask ) const;
             inline bool     operator ==      ( const Mask mask ) const;
             inline bool     operator !=      ( const Mask mask ) const;
             inline bool     operator <       ( const Mask mask ) const;
             inline bool     operator >       ( const Mask mask ) const;
             inline          operator IntType () const;
             inline          operator bool    () const;
      static inline Mask     fromString       ( std::string );
    // Hurricane Managment.  
             inline string   _getTypeName     () const;
             inline string   _getString       () const;
             inline Record*  _getRecord       () const;
    protected:
    // Internal: Attributes.
      static size_t  _width;
             IntType _mask;
  };


  template<typename IntType>
  Mask<IntType>  Mask<IntType>::FFFF = boost::math::tools::max_value<IntType>();


  template<typename IntType>
  size_t  Mask<IntType>::_width = std::numeric_limits<IntType>::digits / 4;


// Inline Functions.
  template<typename IntType>
  inline Mask<IntType>::Mask () : _mask(0) { }

  template<typename IntType>
  inline Mask<IntType>::Mask ( int mask ) : _mask(mask) { }

  template<typename IntType>
  inline Mask<IntType>::Mask ( IntType mask ) : _mask(mask) { }

  template<typename IntType>
  inline bool  Mask<IntType>::zero () const
  { return _mask == 0; }

  template<typename IntType>
  inline  Mask<IntType>::operator bool () const
  { return _mask != 0; }

  template<typename IntType>
  inline Mask<IntType>& Mask<IntType>::set ( const Mask<IntType> mask )
  { _mask |=  mask._mask; return *this; }

  template<typename IntType>
  inline Mask<IntType>& Mask<IntType>::unset ( const Mask<IntType> mask )
  { _mask &= ~mask._mask; return *this; }

  template<typename IntType>
  inline bool  Mask<IntType>::isSet ( const Mask<IntType> mask ) const
  { return (_mask & mask._mask) != 0; }

  template<typename IntType>
  inline bool  Mask<IntType>::contains ( const Mask<IntType> mask ) const
  { return (_mask & mask._mask) && !(~_mask & mask._mask); }

  template<typename IntType>
  inline bool  Mask<IntType>::intersect ( const Mask<IntType> mask ) const
  { return (_mask & mask._mask) != 0; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::nthbit ( unsigned int nth ) const
  { 
    ++nth;
    IntType select = 1;
    for ( ; select ; select=select<<1 ) {
      if (_mask & select) nth--;
      if (not nth) break;
    }
    return select;
  }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::operator compl () const
  { return ~_mask; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::operator bitand ( const Mask<IntType> mask ) const
  { return _mask & mask._mask; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::operator bitor ( const Mask<IntType> mask ) const
  { return _mask | mask._mask; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::operator xor ( const Mask<IntType> mask ) const
  { return _mask ^ mask._mask; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::lshift ( unsigned int s ) const
  { return _mask << s; }

  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::rshift ( unsigned int s ) const
  { return _mask >> s; }

  template<typename IntType>
  inline Mask<IntType>& Mask<IntType>::operator += ( unsigned int v )
  { _mask += v; return *this; } 

  template<typename IntType>
  inline Mask<IntType>& Mask<IntType>::operator |= ( const Mask<IntType> mask )
  { _mask |= mask._mask; return *this; } 

  template<typename IntType>
  inline Mask<IntType>& Mask<IntType>::operator &= ( const Mask<IntType> mask )
  { _mask &= mask._mask; return *this; } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator not () const
  { return (_mask == 0); } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator and ( const Mask<IntType> mask ) const
  { return (_mask != 0) and (mask._mask != 0); } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator or ( const Mask<IntType> mask ) const
  { return (_mask != 0) or (mask._mask != 0); } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator == ( const Mask<IntType> mask ) const
  { return _mask == mask._mask; } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator != ( const Mask<IntType> mask ) const
  { return _mask != mask._mask; } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator < ( const Mask<IntType> mask ) const
  { return _mask < mask._mask; } 

  template<typename IntType>
  inline bool  Mask<IntType>::operator > ( const Mask<IntType> mask ) const
  { return _mask > mask._mask; } 

  template<typename IntType>
  inline  Mask<IntType>::operator IntType () const { return _mask; }

  template<typename IntType>
  inline string  Mask<IntType>::_getTypeName     () const { return _TName("Mask"); }

  template<typename IntType>
  inline string  Mask<IntType>::_getString () const
  {
    std::ostringstream formatted;
    formatted << "0x" << std::hex << std::setw(_width) << std::setfill('0') << _mask;
    return formatted.str();
  }

  template<typename IntType>
  inline Record* Mask<IntType>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add(getSlot("Mask", &_mask));
    return record;
  }


  template<typename IntType>
  inline Mask<IntType>  Mask<IntType>::fromString ( std::string s )
  {
    IntType       value;
    istringstream iss ( s );

    iss >> hex >> value;
    Mask<IntType> mask ( value );
    

#if NO_ISTRINGSTREAM
    if ( (s.size() > 2) and (s.size() <= _width+2) and (s.substr(0,2) == "0x") ) {
      for ( size_t i=2 ; i<s.size() ; ++i ) {
        mask.lshift( 4 );

        switch ( s[i] ) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            mask += (unsigned int)(s[i] - '0'); break;
          case 'a':
          case 'b':
          case 'c':
          case 'd':
          case 'e':
          case 'f':
            mask += (unsigned int)(s[i] - 'a')+10; break;
          case 'A':
          case 'B':
          case 'C':
          case 'D':
          case 'E':
          case 'F':
            mask += (unsigned int)(s[i] - 'A')+10; break;
          default:
            break;
        }
      }
    }
#endif

    return mask;
  }


} // Hurricane namespace.
