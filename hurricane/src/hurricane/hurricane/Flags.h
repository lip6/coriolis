// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/BaseFlags.h"                       |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_BASE_FLAGS_H
#define  HURRICANE_BASE_FLAGS_H

#include <string>
#include "hurricane/Commons.h"


namespace Hurricane {


  class BaseFlags {
    public:
    // Methods.
      inline               BaseFlags             ( unsigned int flags=0 );
      virtual             ~BaseFlags             ();
      inline  bool         zero                  () const;
      inline  BaseFlags&   set                   ( BaseFlags, bool state=true );
      inline  BaseFlags&   reset                 ( BaseFlags );
      inline  bool         isset                 ( BaseFlags ) const;
      inline  bool         contains              ( BaseFlags ) const;
      inline  bool         intersect             ( BaseFlags ) const;
      inline  BaseFlags    nthbit                ( unsigned int ) const;
      inline  BaseFlags    operator compl        () const;
      inline  BaseFlags    operator bitand       ( BaseFlags ) const;
      inline  BaseFlags    operator bitor        ( BaseFlags ) const;
      inline  BaseFlags    operator xor          ( BaseFlags ) const;
      inline  BaseFlags    operator bitand       ( unsigned int ) const;
      inline  BaseFlags    operator bitor        ( unsigned int ) const;
      inline  BaseFlags    operator xor          ( unsigned int ) const;
      inline  BaseFlags    lshift                ( unsigned int ) const;
      inline  BaseFlags    rshift                ( unsigned int ) const;
      inline  BaseFlags&   operator |=           ( BaseFlags );
      inline  BaseFlags&   operator &=           ( BaseFlags );
      inline  bool         operator ==           ( BaseFlags ) const;
      inline  bool         operator !=           ( BaseFlags ) const;
      inline  bool         operator <            ( BaseFlags ) const;
      inline  bool         operator >            ( BaseFlags ) const;
      inline  BaseFlags&   operator |=           ( unsigned int );
      inline  BaseFlags&   operator &=           ( unsigned int );
      inline  bool         operator ==           ( unsigned int ) const;
      inline  bool         operator !=           ( unsigned int ) const;
      inline  bool         operator <            ( unsigned int ) const;
      inline  bool         operator >            ( unsigned int ) const;
      inline               operator unsigned int () const;
    // Hurricane Managment.
      virtual std::string  _getTypeName          () const;
      virtual std::string  _getString            () const;
      inline  Record*      _getRecord            () const;
    protected:
    // Internal: Attributes.
             unsigned int _flags;
  };


// Inline Functions.
  inline            BaseFlags::BaseFlags       ( unsigned int flags )          : _flags(flags) { }
  inline bool       BaseFlags::zero            () const                        { return _flags == 0; }
  inline BaseFlags& BaseFlags::reset           ( BaseFlags flags )             { _flags &= ~flags._flags; return *this; }
  inline bool       BaseFlags::isset           ( BaseFlags flags ) const       { return _flags & flags._flags; }
  inline bool       BaseFlags::contains        ( BaseFlags flags ) const       { return (_flags & flags._flags) && !(~_flags & flags._flags); }
  inline bool       BaseFlags::intersect       ( BaseFlags flags ) const       { return _flags & flags._flags; }
  inline BaseFlags  BaseFlags::operator compl  () const                        { return ~_flags; }
  inline BaseFlags  BaseFlags::operator bitand ( BaseFlags flags ) const       { return _flags & flags._flags; }
  inline BaseFlags  BaseFlags::operator bitor  ( BaseFlags flags ) const       { return _flags | flags._flags; }
  inline BaseFlags  BaseFlags::operator xor    ( BaseFlags flags ) const       { return _flags ^ flags._flags; }
  inline BaseFlags  BaseFlags::operator bitand ( unsigned int flags ) const    { return _flags & flags; }
  inline BaseFlags  BaseFlags::operator bitor  ( unsigned int flags ) const    { return _flags | flags; }
  inline BaseFlags  BaseFlags::operator xor    ( unsigned int flags ) const    { return _flags ^ flags; }
  inline BaseFlags  BaseFlags::lshift          ( unsigned int s ) const        { return _flags << s; }
  inline BaseFlags  BaseFlags::rshift          ( unsigned int s ) const        { return _flags >> s; }
  inline BaseFlags& BaseFlags::operator |=     ( BaseFlags flags )             { _flags |= flags._flags; return *this; } 
  inline BaseFlags& BaseFlags::operator &=     ( BaseFlags flags )             { _flags &= flags._flags; return *this; } 
  inline bool       BaseFlags::operator ==     ( BaseFlags flags ) const       { return _flags == flags._flags; } 
  inline bool       BaseFlags::operator !=     ( BaseFlags flags ) const       { return _flags != flags._flags; } 
  inline bool       BaseFlags::operator <      ( BaseFlags flags ) const       { return _flags <  flags._flags; } 
  inline bool       BaseFlags::operator >      ( BaseFlags flags ) const       { return _flags >  flags._flags; } 
  inline BaseFlags& BaseFlags::operator |=     ( unsigned int flags )          { _flags |= flags; return *this; } 
  inline BaseFlags& BaseFlags::operator &=     ( unsigned int flags )          { _flags &= flags; return *this; } 
  inline bool       BaseFlags::operator ==     ( unsigned int flags ) const    { return _flags == flags; } 
  inline bool       BaseFlags::operator !=     ( unsigned int flags ) const    { return _flags != flags; } 
  inline bool       BaseFlags::operator <      ( unsigned int flags ) const    { return _flags <  flags; } 
  inline bool       BaseFlags::operator >      ( unsigned int flags ) const    { return _flags >  flags; } 
  inline            BaseFlags::operator unsigned int () const { return _flags; }

  inline BaseFlags& BaseFlags::set ( BaseFlags flags, bool state )
  {
    if (state) _flags |=  flags._flags;
    else       _flags &= ~flags._flags;
    return *this;
  }

  inline BaseFlags  BaseFlags::nthbit ( unsigned int nth ) const
  { 
    unsigned int select = 1;
    for ( ; select ; select=select<<1 ) {
      if ( _flags & select ) nth--;
      if ( !nth ) break;
    }
    return select;
  }

  inline Record* BaseFlags::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot("_flags", &_flags) );
    return record;
  }


} // Hurricane namespace.

# endif  // HURRICANE_BASE_FLAGS_H
