// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2016, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/TStream.h"                         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_TSTREAM_H
#define  HURRICANE_TSTREAM_H

#include "hurricane/Tabulation.h"


// -------------------------------------------------------------------
// Class  :  "::cdebug()".
//
// Wrapper around the STL ostream which to print debugging messages.

class tstream : public std::ostream {
  public:
    inline int       getMinLevel () const;
    inline int       getMaxLevel () const;
    inline int       setMinLevel ( int );
    inline int       setMaxLevel ( int );
    inline int       getLevel    () const;
    inline int       setLevel    ( int );
    inline bool      enabled     () const;
    inline bool      enabled     ( int ) const;
    inline tstream&  log         ( int level, int count=0 );
    inline tstream&  tabw        ( int level, int count );
    inline           tstream     ( std::ostream & );
  private:
    inline tstream&  _tab        ();
    inline tstream&  _tabw       ( int count );
  public:
  // Overload for formatted outputs.
  //template<typename T> inline tstream& operator<< ( T  t );
    template<typename T> inline tstream& operator<< ( T* t );
    template<typename T> inline tstream& operator<< ( const T  t );
    template<typename T> inline tstream& operator<< ( const T* t );
                         inline tstream& put        ( char c );
                         inline tstream& flush      ();
  // Overload for manipulators.
                         inline tstream &operator<< ( std::ostream &(*pf)(std::ostream &) );
  private:
    int                    _minLevel;
    int                    _maxLevel;
    int                    _level;
    Hurricane::Tabulation  _tabulation;
};


inline tstream::tstream ( std::ostream& s )
  : std::ostream(s.rdbuf())
  , _minLevel(0)
  , _maxLevel(0)
  , _level(0)
  , _tabulation("  ")
{ }  

inline int      tstream::getMinLevel () const { return _minLevel; }
inline int      tstream::getMaxLevel () const { return _maxLevel; }
inline int      tstream::setMinLevel ( int l ) { int pl=_minLevel; _minLevel=l; return pl; }
inline int      tstream::setMaxLevel ( int l ) { int pl=_maxLevel; _maxLevel=l; return pl; }
inline int      tstream::getLevel    () const { return _level; }
inline int      tstream::setLevel    ( int l ) { int pl=_level; _level=l; return pl; }
inline bool     tstream::enabled     () const { return (_level >= _minLevel) and (_level < _maxLevel); }
inline bool     tstream::enabled     ( int l ) const { return (l >= _minLevel) and (l < _maxLevel); }
inline tstream& tstream::tabw        ( int level, int count ) { setLevel(level); return _tabw(count); }
inline tstream& tstream::put         ( char c ) { if (enabled()) static_cast<std::ostream*>(this)->put(c); return *this; }  
inline tstream& tstream::flush       () { if (enabled()) static_cast<std::ostream*>(this)->flush(); return *this; }  
inline tstream& tstream::operator<<  ( std::ostream& (*pf)(std::ostream&) ) { if (enabled()) (*pf)(*this); return *this; }


inline tstream&  tstream::_tab  () { if (enabled()) (*this) << _tabulation; return *this; }  
inline tstream&  tstream::_tabw ( int count )
{
  if (enabled()) {
    if      (count > 0) while(count--) _tabulation++;
    else if (count < 0) while(count++) _tabulation--;
  }
  return *this;
}

inline tstream& tstream::log ( int level, int count )
{ setLevel(level); _tab(); return _tabw(count); }

// For POD Types.
// template<typename T>
// inline tstream& tstream::operator<< ( T& t )
// { if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

template<typename T>
inline tstream& tstream::operator<< ( T* t )
{ if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

template<typename T>
inline tstream& tstream::operator<< ( const T t )
{ if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

template<typename T>
inline tstream& tstream::operator<< ( const T* t )
{ if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

// For STL Types.
inline tstream& operator<< ( tstream& o, const std::string& s )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << s; } return o; };


extern tstream  cdebug;


#define  cdebug_log(level,indent)   if (cdebug.enabled(level)) cdebug.log(level,indent)
#define  cdebug_tabw(level,indent)  cdebug.tabw(level,indent)

#endif  // HURRICANE_TSTREAM_H
