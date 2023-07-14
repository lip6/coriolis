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
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Commons.h"                         |
// +-----------------------------------------------------------------+


#pragma once
#define HURRICANE_COMMONS_H

#include <cstdio>
#include <cassert>
#include <cmath>
#include <memory>
#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <array>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>


// +-----------------------------------------------------------------+
// |                        Macros Definition                        |
// +-----------------------------------------------------------------+


namespace Hurricane {

  using namespace std;

  class Slot;


  // +-------------------------------------------------------------+
  // |                shared_ptr<> support for DBo                 |
  // +-------------------------------------------------------------+


  template<typename DboType>
  class DboDestroy {
    public:
      inline void operator() ( DboType* dbo ) { dbo->destroy(); }
  };


  template<typename DboType>
  class dbo_ptr : public std::shared_ptr<DboType> {
    public:
      dbo_ptr ( DboType* dbo ) : std::shared_ptr<DboType>(dbo,DboDestroy<DboType>()) { }
  };




  // +-------------------------------------------------------------+
  // |                   Miscellaneous Utilites                    |
  // +-------------------------------------------------------------+


  inline string _TName ( const string& s ) { return s; }
  inline string _PName ( const string& s ) { return "Hurricane::" + s; }

  template<class Type>
  inline Type abs ( const Type& value ) { return (value<0) ? -value : value; }

         string  demangle ( const char*      symbol );
  inline string  demangle ( string           symbol ) { return demangle(symbol.c_str()); }
  inline string  demangle ( const type_info& info   ) { return demangle(info.name()); }

  template<typename Element>
  inline void  erase_element ( vector<Element*>& v, const Element* e )
  {
    for ( auto ielement = v.begin() ; ielement != v.end() ; ++ielement )
      if (*ielement == e) { v.erase( ielement ); return; }
  }


#if DEPRECATED
// For a complete explanation of this function, please look at :
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm

  inline int  floatCompare ( float a, float b )
  {
    assert ( sizeof(float) == sizeof(int) );

    if ( a == b ) return 0;
    return *(int*)&a - *(int*)&b;
  }

  inline int  floatDifference ( float a, float b, int threshold )
  {
    int difference = floatCompare(a,b);
    if ( abs(difference) < threshold ) return 0;

    return (difference<0) ? -1 : 1;
  }


  inline void  floatRound ( float& value, float precision )
  {
    float rounded = roundf ( value*precision );
    value = rounded / precision;
  }
#endif

  inline float  roundfp ( float value, float precision=100.0 ) { return roundf(value*precision)/precision; }


  template<typename Type> inline void  order ( Type& a, Type& b ) { if (a>b) std::swap(a,b); }

  template<typename Type> inline Type  setInBound ( Type lower, Type upper, Type& value )
  {
    if      (value < lower) value = lower;
    else if (value > upper) value = upper;
    return value;
  }


  string& split ( std::string& );


} // End of Hurricane namespace.


#include  "hurricane/Record.h"


// +-----------------------------------------------------------------+
// |                Functions for Inspector Support                  |
// +-----------------------------------------------------------------+

// Note 1: Theses are specialized templates for "getString<>()" & "getRecord<>()".
// Note 2: we are outside the Hurricane namespace.
// Note 3: thoses templates manage all POD & STL types.


template<typename Data> inline Hurricane::Slot* getSlot ( std::string name, Data );


// -------------------------------------------------------------------
// Inspector Support for  :  "POD types".

// Default match.

template<typename Data> inline std::string  getString ( Data data )
{ return std::string("<type ")
         + Hurricane::demangle(typeid(data).name())
         + std::string(" unsupported by getString()>"); }

// "const &" flavors.

template<> inline std::string  getString<const bool&> ( const bool& b )
{ return (b)?"True":"False" ; }

template<> inline std::string  getString<const int&> ( const int& i )
{ std::ostringstream os (""); os << i; return os.str(); }

template<> inline std::string  getString<const long&> ( const long& l )
{ std::ostringstream os (""); os << l; return os.str(); }

template<> inline std::string  getString<const unsigned int&>  ( const unsigned int& u )
{ std::ostringstream os (""); os << u; return os.str(); }

template<> inline std::string  getString<const unsigned long&> ( const unsigned long& ul )
{ std::ostringstream os (""); os << ul; return os.str(); }

template<> inline std::string  getString<const unsigned long long&> ( const unsigned long long& ull )
{ std::ostringstream os (""); os << ull; return os.str(); }

template<> inline std::string  getString<const unsigned short int&> ( const unsigned short int& us )
{ std::ostringstream os (""); os << us; return os.str(); }

template<> inline std::string  getString<const float&> ( const float& f )
{ std::ostringstream os (""); os << f; return os.str(); }

template<> inline std::string  getString<const double&> ( const double& d )
{ std::ostringstream os; os << d; return os.str(); }

template<> inline std::string  getString<const std::string&> ( const std::string& s )
{ return s; }

// "const *" flavors.

template<> inline std::string  getString<const bool*> ( const bool* b )
{ return (*b)?"True":"False" ; }

template<> inline std::string  getString<const char*> ( const char* c )
{ return c; }

template<> inline std::string  getString<const int*> ( const int* i )
{ std::ostringstream os (""); os << *i; return os.str(); }

template<> inline std::string  getString<const long*> ( const long* l )
{ std::ostringstream os (""); os << *l; return os.str(); }

template<> inline std::string  getString<const unsigned int*>  ( const unsigned int* u )
{ std::ostringstream os (""); os << *u; return os.str(); }

template<> inline std::string  getString<const unsigned long*> ( const unsigned long* ul )
{ std::ostringstream os (""); os << *ul; return os.str(); }

template<> inline std::string  getString<const unsigned long long*> ( const unsigned long long* ull )
{ std::ostringstream os (""); os << *ull; return os.str(); }

template<> inline std::string  getString<const unsigned short int*> ( const unsigned short int* us )
{ std::ostringstream os (""); os << *us; return os.str(); }

template<> inline std::string  getString<const float*> ( const float* f )
{ std::ostringstream os (""); os << *f; return os.str(); }

template<> inline std::string  getString<const double*> ( const double* d )
{ std::ostringstream os; os << *d; return os.str(); }

template<> inline std::string  getString<const void*> ( const void* p )
{ std::ostringstream os ("0x"); os << std::hex << p; return os.str(); }

template<> inline std::string  getString<const std::string*> ( const std::string* s )
{ return *s; }


// "*" flavors.

template<> inline std::string  getString<bool*> ( bool* b )
{ return (*b)?"True":"False" ; }

template<> inline std::string  getString<char*> ( char* c )
{ return c; }

template<> inline std::string  getString<int*> ( int* i )
{ std::ostringstream os (""); os << *i; return os.str(); }

template<> inline std::string  getString<long*> ( long* l )
{ std::ostringstream os (""); os << *l; return os.str(); }

template<> inline std::string  getString<unsigned int*>  ( unsigned int* u )
{ std::ostringstream os (""); os << *u; return os.str(); }

template<> inline std::string  getString<unsigned long*> ( unsigned long* ul )
{ std::ostringstream os (""); os << *ul; return os.str(); }

template<> inline std::string  getString<unsigned long long*> ( unsigned long long* ull )
{ std::ostringstream os (""); os << *ull; return os.str(); }

template<> inline std::string  getString<unsigned short int*> ( unsigned short int* us )
{ std::ostringstream os (""); os << *us; return os.str(); }

template<> inline std::string  getString<float*> ( float* f )
{ std::ostringstream os (""); os << *f; return os.str(); }

template<> inline std::string  getString<double*> ( double* d )
{ std::ostringstream os; os << *d; return os.str(); }

template<> inline std::string  getString<void*> ( void* p )
{ std::ostringstream os ("0x"); os << std::hex << p; return os.str(); }

template<> inline std::string  getString<std::string*> ( std::string* s )
{ return *s; }


// "by value" flavors.

template<> inline std::string  getString<bool> ( bool b )
{ return (b)?"True":"False" ; }

template<> inline std::string  getString<char> ( char c )
{ return std::string(1,c); }

template<> inline std::string  getString<int> ( int i )
{ std::ostringstream os (""); os << i; return os.str(); }

template<> inline std::string  getString<long> ( long l )
{ std::ostringstream os (""); os << l; return os.str(); }

template<> inline std::string  getString<unsigned int>  ( unsigned int u )
{ std::ostringstream os (""); os << u; return os.str(); }

template<> inline std::string  getString<unsigned long> ( unsigned long ul )
{ std::ostringstream os (""); os << ul; return os.str(); }

template<> inline std::string  getString<unsigned long long> ( unsigned long long ull )
{ std::ostringstream os (""); os << ull; return os.str(); }

template<> inline std::string  getString<unsigned short int> ( unsigned short int us )
{ std::ostringstream os (""); os << us; return os.str(); }

template<> inline std::string  getString<float> ( float f )
{ std::ostringstream os (""); os << f; return os.str(); }

template<> inline std::string  getString<double> ( double d )
{ std::ostringstream os; os << d; return os.str(); }

template<> inline std::string  getString<std::string> ( std::string s )
{ return  s; }


template<typename Data> inline Hurricane::Record* getRecord ( Data data )
{
//std::cerr << "::getRecord(Data) Data=" << Hurricane::demangle(typeid(data).name()) << std::endl;
  return NULL;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::pair<T,U>&".

template<typename T, typename U>
inline std::string  getString ( const std::pair<T,U>& p )
{
  return "const std::pair<T,U>";
}


template<typename T, typename U>
inline Hurricane::Record* getRecord ( const std::pair<T,U>& p )
{
  Hurricane::Record* record = NULL;
  record = new Hurricane::Record ( "const std::pair<T,U>" );
  record->add( getSlot<const T>(std::string("first" ), &p.first ) );
  record->add( getSlot<const U>(std::string("second"), &p.second) );
  return record;
}


template<typename T, typename U>
inline std::string  getString ( std::pair<T,U>& p )
{
  return "std::pair<T,U>";
}


template<typename T, typename U>
inline Hurricane::Record* getRecord ( std::pair<T,U>& p )
{
  Hurricane::Record* record = NULL;
  record = new Hurricane::Record ( "std::pair<T,U>" );
  record->add( getSlot<T>(std::string("first" ), &p.first ) );
  record->add( getSlot<U>(std::string("second"), &p.second) );
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::array<Element>*".


template<typename Element,size_t N>
inline std::string  getString ( std::array<Element,N>* v )
{
  std::string name = "const std::array<Element,N>:";
  return name + getString<size_t>(v->size());
}


template<typename Element,size_t N>
inline Hurricane::Record* getRecord ( std::array<Element,N>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "std::array<Element,N>" );
    unsigned n = 0;
    typename std::array<Element,N>::iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot<Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


template<typename Element,size_t N>
inline std::string  getString ( const std::array<Element,N>* v )
{
  std::string name = "const std::array<Element,N>:";
  return name + getString<size_t>(v->size());
}


template<typename Element,size_t N>
inline Hurricane::Record* getRecord ( const std::array<Element,N>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "const std::array<Element,N>" );
    unsigned n = 0;
    typename std::array<Element,N>::const_iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot<const Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


template<typename Element,size_t N>
inline std::string  getString ( std::array<Element,N>& v )
{
  std::string name = "std::array<Element,N>&:";
  return name + getString<size_t>(v.size());
}


template<typename Element,size_t N>
inline Hurricane::Record* getRecord ( std::array<Element,N>& v )
{
  Hurricane::Record* record = NULL;
  if (not v.empty()) {
    record = new Hurricane::Record ( "std::array<Element,N>&" );
    unsigned n = 0;
    for ( auto element : v )
      record->add( getSlot<Element>(getString(n++), element) );
  }
  return record;
}


template<typename Element,size_t N>
inline std::string  getString ( const std::array<Element,N>& v )
{
  std::string name = "const std::array<Element,N>&:";
  return name + getString<size_t>(v.size());
}


template<typename Element,size_t N>
inline Hurricane::Record* getRecord ( const std::array<Element,N>& v )
{
  Hurricane::Record* record = NULL;
  if (not v.empty()) {
    record = new Hurricane::Record ( "const std::array<Element,N>&" );
    unsigned n = 0;
    for ( auto element : v )
      record->add( getSlot<Element>(getString(n++), element) );
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "std::vector<Element>*".


template<typename Element>
inline std::string  getString ( std::vector<Element>* v )
{
  std::string name = "std::vector<Element>*:";
  return name + getString<size_t>(v->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( std::vector<Element>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "std::vector<Element>*" );
    unsigned n = 0;
    typename std::vector<Element>::iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot<const Element*>(getString(n++), &(*iterator)) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "std::vector<Element*>*".


template<typename Element>
inline std::string  getString ( std::vector<Element*>* v )
{
  std::string name = "std::vector<Element*>*:";
  return name + getString<size_t>(v->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( std::vector<Element*>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "std::vector<Element*>*" );
    unsigned n = 0;
    typename std::vector<Element*>::iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot<Element*>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const std::vector<Element>*".


template<typename Element>
inline std::string  getString ( const std::vector<Element>* v )
{
  std::string name = "const std::vector<Element>*:";
  return name + getString<size_t>(v->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( const std::vector<Element>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "const std::vector<Element>*" );
    unsigned n = 0;
    typename std::vector<Element>::const_iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot<const Element*>(getString(n++), &(*iterator)) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const std::vector<Element*>*".


template<typename Element>
inline std::string  getString ( const std::vector<Element*>* v )
{
  std::string name = "const std::vector<Element*>*:";
  return name + getString<size_t>(v->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( const std::vector<Element*>* v )
{
  Hurricane::Record* record = NULL;
  if (not v->empty()) {
    record = new Hurricane::Record ( "const std::vector<Element*>*" );
    size_t n = 0;
    typename std::vector<Element*>::const_iterator iterator = v->begin();
    while (iterator != v->end()) {
      record->add ( getSlot<const Element*>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const std::list<Element>*".


template<typename Element>
inline std::string  getString ( const std::list<Element>* l )
{
  std::string name = "const std::list<Element>*:";
  return name + getString<size_t>(l->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( const std::list<Element>* l )
{
  Hurricane::Record* record = NULL;
  if ( !l->empty() ) {
    record = new Hurricane::Record ( "const std::list<Element>" );
    unsigned n = 1;
    typename std::list<Element>::const_iterator iterator = l->begin();
    while ( iterator != l->end() ) {
      record->add ( getSlot<const Element*>(getString(n++), &(*iterator)) );
      ++iterator;
    }
  }
  return record;
}


template<typename Element>
inline std::string  getString ( std::list<Element>* l )
{
  std::string name = "std::list<Element>*:";
  return name + getString<size_t>(l->size());
}


template<typename Element>
inline Hurricane::Record* getRecord ( std::list<Element>* l )
{
  Hurricane::Record* record = NULL;
  if ( !l->empty() ) {
    record = new Hurricane::Record ( "std::list<Element>" );
    unsigned n = 1;
    typename std::list<Element>::iterator iterator = l->begin();
    while ( iterator != l->end() ) {
      record->add ( getSlot<const Element*>(getString(n++), &(*iterator)) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::map<Key,Element,Compare>*.


template<typename Key, typename Element>
inline std::string  getString ( std::map<Key,Element>* m )
{
  std::string name = "std::map<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element>
inline Hurricane::Record* getRecord ( std::map<Key,Element>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "std::map<Element>" );
    typename std::map<Key,Element>::iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


template<typename Key, typename Element>
inline std::string  getString ( const std::map<Key,Element>* m )
{
  std::string name = "const std::map<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element>
inline Hurricane::Record* getRecord ( const std::map<Key,Element>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "const std::map<Element>" );
    typename std::map<Key,Element>::const_iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<const Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::map<Key,Element,Compare>*.


template<typename Key, typename Element, typename Compare>
inline std::string  getString ( std::map<Key,Element,Compare>* m )
{
  std::string name = "std::map<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element, typename Compare>
inline Hurricane::Record* getRecord ( std::map<Key,Element,Compare>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "std::map<Element>" );
    typename std::map<Key,Element,Compare>::iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


template<typename Key, typename Element, typename Compare>
inline std::string  getString ( const std::map<Key,Element,Compare>* m )
{
  std::string name = "const std::map<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element, typename Compare>
inline Hurricane::Record* getRecord ( const std::map<Key,Element,Compare>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "const std::map<Element>" );
    typename std::map<Key,Element,Compare>::const_iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<const Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const std::multimap<Key,Element,Compare>*".


template<typename Key, typename Element, typename Compare>
inline std::string  getString ( const std::multimap<Key,Element,Compare>* m )
{
  std::string name = "const std::multimap<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element, typename Compare>
inline Hurricane::Record* getRecord ( const std::multimap<Key,Element,Compare>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "const std::multimap<Element>" );
    typename std::multimap<Key,Element,Compare>::const_iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<const Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


template<typename Key, typename Element, typename Compare>
inline std::string  getString ( std::multimap<Key,Element,Compare>* m )
{
  std::string name = "std::multimap<Element>:";
  return name + getString<size_t>(m->size());
}


template<typename Key, typename Element, typename Compare>
inline Hurricane::Record* getRecord ( std::multimap<Key,Element,Compare>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "std::multimap<Element>" );
    typename std::multimap<Key,Element,Compare>::iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot<Element>(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::set<Element,Compare>*".


template<typename Element, typename Compare>
inline std::string  getString ( const std::set<Element,Compare>* s )
{
  std::string name = "const std::set<Element>:";
  return name + getString<size_t>(s->size());
}


template<typename Element, typename Compare>
inline Hurricane::Record* getRecord ( const std::set<Element,Compare>* s )
{
  Hurricane::Record* record = NULL;
  if ( !s->empty() ) {
    record = new Hurricane::Record ( "const std::set<Element>" );
    unsigned n = 1;
    typename std::set<Element,Compare>::const_iterator iterator = s->begin();
    while ( iterator != s->end() ) {
      record->add ( getSlot<const Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


template< typename Element, typename Compare, typename Allocator >
inline std::string  getString ( std::set<Element,Compare,Allocator>* s )
{
  std::string name = "std::set<Element>:";
  return name + getString<size_t>(s->size());
}


template< typename Element, typename Compare, typename Allocator >
inline Hurricane::Record* getRecord ( std::set<Element,Compare,Allocator>* s )
{
  Hurricane::Record* record = NULL;
  if (not s->empty()) {
    record = new Hurricane::Record ( "std::set<Element>" );
    unsigned n = 1;
    typename std::set<Element,Compare,Allocator>::iterator iterator = s->begin();
    while ( iterator != s->end() ) {
      record->add( getSlot<Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}

// -------------------------------------------------------------------
// Inspector Support for  :  "[const] std::set<Element,Compare>&".


template<typename Element, typename Compare>
inline std::string  getString ( const std::set<Element,Compare>& s )
{
  std::string name = "const std::set<Element>:";
  return name + getString<size_t>(s.size());
}


template<typename Element, typename Compare>
inline Hurricane::Record* getRecord ( const std::set<Element,Compare>& s )
{
  Hurricane::Record* record = NULL;
  if ( !s.empty() ) {
    record = new Hurricane::Record ( "const std::set<Element>" );
    unsigned n = 1;
    typename std::set<Element,Compare>::const_iterator iterator = s.begin();
    while ( iterator != s.end() ) {
      record->add ( getSlot<Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}

// -------------------------------------------------------------------
// Inspector Support for  :  "const std::multiset<Element,Compare>*".


template<typename Element, typename Compare>
inline std::string  getString ( const std::multiset<Element,Compare>* s )
{
  std::string name = "std::multiset<Element>:";
  return name + getString<size_t>(s->size());
}


template<typename Element, typename Compare>
inline Hurricane::Record* getRecord ( const std::multiset<Element,Compare>* s )
{
  Hurricane::Record* record = NULL;
  if ( !s->empty() ) {
    record = new Hurricane::Record ( "std::multiset<Element>" );
    unsigned n = 1;
    typename std::multiset<Element,Compare>::const_iterator iterator = s->begin();
    while ( iterator != s->end() ) {
      record->add ( getSlot<Element>(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


# define GETSTRING_POINTER_SUPPORT(Data) \
  template<> inline std::string getString<Data*>( Data* data )             \
  {                                                                        \
    if (!data) return "NULL [" #Data "]";                                  \
    return data->_getString();                                             \
  }                                                                        \
                                                                           \
  template<> inline std::string getString<const Data*>( const Data* data ) \
  { if (!data) return "NULL [const " #Data "]"; return data->_getString(); }


# define IOSTREAM_POINTER_SUPPORT(Data) \
  inline std::ostream& operator<< ( std::ostream& o, Data* d )       \
  {                                                                  \
    if (!d) return o << "NULL [" #Data "]";                          \
    return o << "&" << getString<const Data*>(d);                    \
  }                                                                  \
  inline std::ostream& operator<< ( std::ostream& o, const Data* d ) \
  {                                                                  \
    if (!d) return o << "NULL [const " #Data "]";                    \
    return o << "&" << getString<const Data*>(d);                    \
  }                                                                  \


# define GETRECORD_POINTER_SUPPORT(Data) \
  template<> inline Hurricane::Record* getRecord<Data*>( Data* data )             \
  { if (!data) return NULL; return data->_getRecord(); }                          \
                                                                                  \
  template<> inline Hurricane::Record* getRecord<const Data*>( const Data* data ) \
  { if (!data) return NULL; return data->_getRecord(); }


# define GETSTRING_REFERENCE_SUPPORT(Data) \
  template<> inline std::string getString<Data&>( Data& data )             \
  { return data._getString(); }                                            \
                                                                           \
  template<> inline std::string getString<const Data&>( const Data& data ) \
  { return data._getString(); }


# define IOSTREAM_REFERENCE_SUPPORT(Data) \
  inline std::ostream& operator<< ( std::ostream& o, Data& d )             \
  { return o << getString<Data&>(d); }                                     \
                                                                           \
  inline std::ostream& operator<< ( std::ostream& o, const Data& d )       \
  { return o << getString<const Data&>(d); }                               \
                                                                           \

# define GETRECORD_REFERENCE_SUPPORT(Data) \
  template<> inline Hurricane::Record* getRecord<Data&>( Data& data )             \
  { return data._getRecord(); }                                                   \
                                                                                  \
  template<> inline Hurricane::Record* getRecord<const Data&>( const Data& data ) \
  { return data._getRecord(); }


# define GETSTRING_VALUE_SUPPORT(Data) \
  template<> inline std::string getString<Data>( Data data )  \
  { return data._getString(); }


# define IOSTREAM_VALUE_SUPPORT(Data) \
  inline std::ostream& operator<< ( std::ostream& o, Data d )   \
  { return o << getString<Data>(d); }


# define GETRECORD_VALUE_SUPPORT(Data) \
  template<> inline Hurricane::Record* getRecord<Data>( Data data ) \
  { return data._getRecord(); }


# define INSPECTOR_P_SUPPORT(Data)   \
  GETRECORD_POINTER_SUPPORT(Data)    \
  GETSTRING_POINTER_SUPPORT(Data)    \
  IOSTREAM_POINTER_SUPPORT(Data)


# define INSPECTOR_R_SUPPORT(Data)   \
  GETRECORD_REFERENCE_SUPPORT(Data)  \
  GETSTRING_REFERENCE_SUPPORT(Data)  \
  IOSTREAM_REFERENCE_SUPPORT(Data)


# define INSPECTOR_PR_SUPPORT(Data)  \
  GETSTRING_POINTER_SUPPORT(Data)    \
  GETSTRING_REFERENCE_SUPPORT(Data)  \
  GETSTRING_VALUE_SUPPORT(Data)      \
  IOSTREAM_POINTER_SUPPORT(Data)     \
  IOSTREAM_REFERENCE_SUPPORT(Data)   \
  GETRECORD_POINTER_SUPPORT(Data)    \
  GETRECORD_REFERENCE_SUPPORT(Data)


# define INSPECTOR_PV_SUPPORT(Data)  \
  GETSTRING_POINTER_SUPPORT(Data)    \
  GETSTRING_VALUE_SUPPORT(Data)      \
  IOSTREAM_POINTER_SUPPORT(Data)     \
  IOSTREAM_VALUE_SUPPORT(Data)       \
  GETRECORD_POINTER_SUPPORT(Data)    \
  GETRECORD_VALUE_SUPPORT(Data)


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
    inline tstream&  put         ( char c );
    inline tstream&  flush       ();
  private:
    inline tstream&  _tab        ();
    inline tstream&  _tabw       ( int count );
  public:
  // Overload for manipulators.
    inline tstream&  operator<<  ( std::ostream& (*pf)(std::ostream &) );
  private:
    int                    _minLevel;
    int                    _maxLevel;
    int                    _level;
    Hurricane::Tabulation  _tabulation;
};


inline tstream::tstream ( std::ostream& s )
  : std::ostream(s.rdbuf())
  , _minLevel  (100000)
  , _maxLevel  (0)
  , _level     (0)
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

// For STL Types.
inline tstream& operator<< ( tstream& o, const std::string s )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << s; } return o; };

// For POD Types.
// template<typename T>
// inline tstream& operator<< ( tstream& o, T& t )
// { if (o.enabled()) { static_cast<std::ostream&>(o) << getString<T&>(t); } return o; };

template<typename T>
inline tstream& operator<< ( tstream& o, T* t )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << getString<T*>(t); } return o; };

// template<typename T>
// inline tstream& operator<< ( tstream& o, const T& t )
// { if (o.enabled()) { static_cast<std::ostream&>(o) << getString<const T&>(t); } return o; };

template<typename T>
inline tstream& operator<< ( tstream& o, T t )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << getString<T>(t); } return o; };

template<typename T>
inline tstream& operator<< ( tstream& o, const T* t )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << getString<const T*>(t); } return o; };

template<>
inline tstream& operator<< ( tstream& o, std::ios_base& (*pf)(std::ios_base&) )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << pf; } return o; };

struct _Tsetw { int n_; };
inline _Tsetw tsetw ( int n ) { return { n }; }

struct _Tsetf { int n_; };
inline _Tsetf tsetf ( int n ) { return { n }; }

template<>
inline tstream& operator<< ( tstream& o, _Tsetw manip )
{ if (o.enabled()) { static_cast<std::ostream&>(o) << std::setw(manip.n_); } return o; }

extern tstream  cdebug;


#define  cdebug_log(level,indent)   if (cdebug.enabled(level)) cdebug.log(level,indent)
#define  cdebug_tabw(level,indent)  cdebug.tabw(level,indent)


// x-----------------------------------------------------------------x
// |            Classes Neededs in All Hurricane Modules             |
// x-----------------------------------------------------------------x

#include "hurricane/Slot.h"
#include "hurricane/Initializer.h"
#include "hurricane/JsonWriter.h"
#include "hurricane/JsonObject.h"
