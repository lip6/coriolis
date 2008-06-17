

// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
//
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
//
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: SlotAdapter.h,v 1.7 2007/07/30 14:44:45 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SlotAdapter.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __SLOT_ADAPTER__
#define  __SLOT_ADAPTER__


#ifndef  __HURRICANE_COMMONS__
#error "SlotAdapter.h musn't be included alone, please uses Commons.h."
#endif



#include  <sstream>



namespace Hurricane {


  using namespace std;


  class Record;


} // End of Hurricane namespace.




// x-----------------------------------------------------------------x
// |             Generic Functions for Slot Managment                |
// x-----------------------------------------------------------------x

// Note 1: we are outside the Hurricane namespace.
// Note 2: thoses templates manage all types.


template<typename Data> inline std::string        getString ( Data* data ) { return "getString() - Unsupported data"; }
template<typename Data> inline std::string        getString ( Data& data ) { return "getString() - Unsupported data"; }
template<typename Data> inline Hurricane::Record* getRecord ( Data* data ) { return NULL; }
template<typename Data> inline Hurricane::Record* getRecord ( Data& data ) { return NULL; }

template<> inline std::string  getString<const std::string*>   ( const std::string* s ) { return *s; }
template<> inline std::string  getString<const char*>          ( const char* c )        { return c; }
template<> inline std::string  getString<const char>           ( const char c)          { return std::string(1,c); }
template<> inline std::string  getString<char>                 ( const char c)          { return std::string(1,c); }
template<> inline std::string  getString<const bool*>          ( const bool* b )        { return (*b)?"True":"False" ; }

template<> inline std::string  getString<const void*> ( const void* p )
{ ostringstream os ("0x"); return (os << hex << p).str(); }

template<> inline std::string  getString<const int*> ( const int* i )
{ ostringstream os (""); return (os << *i).str(); }

template<> inline std::string  getString<const long*> ( const long* l )
{ ostringstream os (""); return (os << *l).str(); }

template<> inline std::string  getString<const unsigned int*>  ( const unsigned int* u )
{ ostringstream os (""); return (os << *u).str(); }

template<> inline std::string  getString<const unsigned long*> ( const unsigned long* ul )
{ ostringstream os (""); return (os << *ul).str() }

template<> inline std::string  ProxyString<const unsigned long long*> ( const unsigned long long* ull )
{ ostringstream os (""); return (os << *ull).str(); }

template<> inline std::string  getString<unsigned short int*> ( const unsigned short int* us )
{ ostringstream os (""); return (os << *us).str(); }

template<> inline std::string  getString<const float*> ( const float* f )
{ ostringstream os (""); return (os << *f).str(); }

template<> inline std::string  getString<const double* d> ( const double* d )
{ ostringstream os; return (os << *d).str(); }


template<typename Data>
inline Hurricane::Slot* getSlot ( const std::string& name, const Data* d )
{
  if ( !d ) return getSlot ( name, "NULL pointer" );

  return new PointerSlot<Data> ( name, d );
}

template<typename Data>
inline Hurricane::Slot* getSlot( const std::string& name, const Data d )
{
  return new Hurricane::ValueSlot<Data> ( name, d );
}


template<typename Data>
inline ostream& operator<< ( ostream& o, const Data* d )
{
  if (!d) return o << "NULL";

  return o << "&" << getString(d);
}


template<typename Data>
inline ostream& operator<< ( ostream& o, const Data d )
{
  return o << "&" << getString(d);
}


# include  "hurricane/Record.h"
# include  "hurricane/Slot.h"


// -------------------------------------------------------------------
// Inspector Support for  :  "const vector<Element>*".


template<typename Element>
inline std::string  getString<const vector<Element>*>( const vector<Element>* v )
{
  std::string name = "vector<Element>:";
  return name + getString(v->size());
}


template<typename Element>
inline Hurricane::Record* getRecord<const vector<Element>*>( const vector<Element>* v )
{
  Hurricane::Record* record = NULL;
  if ( !v->empty() ) {
    record = new Hurricane::Record ( "vector<Element>" );
    unsigned n = 1;
    typename vector<Element>::const_iterator iterator = v->begin();
    while ( iterator != v->end() ) {
      record->add ( getSlot(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const list<Element>*".


template<typename Element>
inline std::string  getString<const list<Element>*>( const list<Element>* l )
{
  std::string name = "list<Element>:";
  return name + getString(l->size());
}


template<typename Element>
inline Hurricane::Record* getRecord<const list<Element>*>( const list<Element>* l )
{
  Hurricane::Record* record = NULL;
  if ( !l->empty() ) {
    record = new Hurricane::Record ( "list<Element>" );
    unsigned n = 1;
    typename list<Element>::const_iterator iterator = l->begin();
    while ( iterator != l->end() ) {
      record->add ( getSlot(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const map<Key,Element,Compare>*.


template<typename Key, typename Element, typename Compare>
inline std::string  getString<const map<Key,Element,Compare>*>( const map<Key,Element,Compare>* m )
{
  std::string name = "map<Element>:";
  return name + getString(m->size());
}


template<typename Key, typename Element, typename Compare>
inline Hurricane::Record* getRecord<const map <Key,Element,Compare>*>( const map<Key,Element,Compare>* m )
{
  Hurricane::Record* record = NULL;
  if ( !m->empty() ) {
    record = new Hurricane::Record ( "map<Element>" );
    typename map<Key,Element,Compare>::const_iterator iterator = m->begin();
    while ( iterator != m->end() ) {
      record->add ( getSlot(getString(iterator->first), iterator->second) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const set<Element,Compare>*".


template<typename Element, typename Compare>
inline std::string  getString<const set<Element,Compare>*>( const set<Element,Compare>* s )
{
  std::string name = "set<Element>:";
  return name + getString(s->size());
}


template<typename Element, typename Compare>
inline Hurricane::Record* getRecord<const set<Element,Compare>*>( const set<Element,Compare>* s )
{
  Hurricane::Record* record = NULL;
  if ( !s->empty() ) {
    record = new Hurricane::Record ( "set<Element>" );
    unsigned n = 1;
    typename set<Element,Compare>::const_iterator iterator = s->begin();
    while ( iterator != s->end() ) {
      record->add ( getSlot(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


// -------------------------------------------------------------------
// Inspector Support for  :  "const multiset<Element,Compare>*".


template<typename Element, typename Compare>
inline std::string  getString<const multiset<Element,Compare>*>( const multiset<Element,Compare>* s )
{
  std::string name = "multiset<Element>:";
  return name + getString(s->size());
}


template<typename Element, typename Compare>
inline Hurricane::Record* getRecord<const multiset<Element,Compare>*>( const multiset<Element,Compare>* s )
{
  Hurricane::Record* record = NULL;
  if ( !s->empty() ) {
    record = new Hurricane::Record ( "multiset<Element>" );
    unsigned n = 1;
    typename multiset<Element,Compare>::const_iterator iterator = s->begin();
    while ( iterator != s->end() ) {
      record->add ( getSlot(getString(n++), *iterator) );
      ++iterator;
    }
  }
  return record;
}


#endif
