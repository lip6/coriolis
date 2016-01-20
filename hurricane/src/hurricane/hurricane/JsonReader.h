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
// |  C++ Header  :  "./hurricane/JsonReader.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_JSON_READER_H
#define  HURRICANE_JSON_READER_H

#ifndef HURRICANE_SLOT_H
#error "JsonWriter.h must be included through Commons.h"
#endif

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <set>
#include <map>
#include <string>
#include <functional>
#include <boost/any.hpp>


namespace Hurricane {

  class DBo;
  class Hook;
  class Entity;
  class Cell;
  class Component;


// -------------------------------------------------------------------
// Class  :  "JsonObject".

  class JsonStack;
  class JsonArray { };


// -------------------------------------------------------------------
// Class  :  "JsonAttribute".

  class JsonAttribute {
    public:
      inline                   JsonAttribute ( const std::string& key, std::type_index tid );
      inline  std::string      key           () const;
      inline  std::type_index  tid           () const;
    private:
      std::string      _key;
      std::type_index  _tid;
  };

  inline                   JsonAttribute::JsonAttribute ( const std::string& key, std::type_index tid ) : _key(key), _tid(tid) { }
  inline  std::string      JsonAttribute::key           () const { return _key; }
  inline  std::type_index  JsonAttribute::tid           () const { return _tid; }


// -------------------------------------------------------------------
// Class  :  "JsonObject".

  class JsonObject {
    public:
                                                  JsonObject    ( unsigned long flags );
                           virtual               ~JsonObject    ();
                           virtual bool           isDummy       () const;
                           virtual std::string    getTypeName   () const = 0;
                           inline  std::string    getStackName  () const;
                                   bool           check         ( JsonStack&, std::string fname ) const;
                                   void           print         ( std::ostream& ) const;
                                   bool           has           ( const std::string& key ) const;
                                   void           add           ( const std::string& key, std::type_index tid );
                                   void           remove        ( const std::string& key );
      template<typename T> inline  T              get           ( JsonStack&, const std::string& key ) const;
                           inline  void           copyAttrs     ( const JsonObject*, bool reset=false );
                           inline  void           clear         ();
                           inline  std::string    getName       () const;
                           inline  void           setName       ( const std::string& );
      template<typename T> inline  T&             getObject     () const; 
      template<typename T> inline  void           setObject     ( T ) ; 
                           inline  bool           isBound       () const;
                           virtual JsonObject*    clone         ( unsigned long flags ) const = 0;
                           virtual void           toData        ( JsonStack& );
                                   unsigned int   presetId      ( JsonStack& );
      template<typename T> inline  void           update        ( JsonStack&, T );
                           inline  JsonObject*    setFlags      ( unsigned long mask );
                           inline  JsonObject*    resetFlags    ( unsigned long mask );
                           inline  bool           issetFlags    ( unsigned long mask ) const;
    protected:
      unsigned long               _flags;
      std::string                 _name;
      std::vector<JsonAttribute>  _stackeds;
      std::vector<JsonAttribute>  _attributes;
      std::vector<JsonAttribute>  _collections;
      boost::any                  _object;
  };


  inline bool         JsonObject::isBound    () const { return not _object.empty(); }
  inline std::string  JsonObject::getName    () const { return _name; }
  inline void         JsonObject::setName    ( const string& name ) { _name=name; }
  inline JsonObject*  JsonObject::setFlags   ( unsigned long mask ) { _flags |= mask; return this; }
  inline JsonObject*  JsonObject::resetFlags ( unsigned long mask ) { _flags &= ~mask; return this; }
  inline bool         JsonObject::issetFlags ( unsigned long mask ) const { return _flags & mask; }

  inline std::string  JsonObject::getStackName () const
  { return (_name.empty()) ? std::string(".")+getTypeName(): _name; }

  template<typename T> inline T& JsonObject::getObject () const
  { return boost::any_cast<T>(_object); }

  template<typename T> inline void  JsonObject::setObject ( T t )
  { _object = t; }

  inline void  JsonObject::copyAttrs ( const JsonObject* other, bool reset )
  {
    if (reset) _attributes.clear();
    for ( size_t i=0 ; i<other->_attributes.size() ; ++i )
      _attributes.push_back( other->_attributes[i] );
  }

  void  JsonObject::clear ()
  {
    _stackeds.clear();
    _attributes.clear();
    _collections.clear();
    boost::any emptyAny;
    _object.swap( emptyAny );
  }


// -------------------------------------------------------------------
// Class  :  "JsonKey".

  class JsonKey : public JsonObject {
    public:
      inline               JsonKey     ( const std::string& );
      virtual std::string  getTypeName () const;
      virtual JsonKey*     clone       ( unsigned long ) const;
    private:
      std::string  _key;
  };


// -------------------------------------------------------------------
// Class  :  "JsonDummy".

  class JsonDummy : public JsonObject {
    public:
      inline               JsonDummy     ();
      virtual bool         isDummy       () const;
      virtual std::string  getTypeName   () const;
              void         setTypeName   ( const std::string& name );
      virtual JsonDummy*   clone         ( unsigned long ) const;
    private:
      std::string  _typename;
  };


} // Hurricane namespace.


namespace std {

  template<>
  struct less<Hurricane::JsonObject*> {
      inline bool operator() ( const Hurricane::JsonObject* lhs, const Hurricane::JsonObject* rhs )
      { return lhs->getTypeName() < rhs->getTypeName(); }
  };

} // std namespace.


namespace Hurricane {

// -------------------------------------------------------------------
// Class  :  "JsonTypes".

  class JsonTypes {
    public:
      static void         initialize    ();
      static void         registerType  ( JsonObject* ); 
      static JsonObject*  find          ( const std::string& tname );
    private:
                          JsonTypes     ();
                         ~JsonTypes     ();
                          JsonTypes     ( const JsonTypes& );
             void         _registerType ( JsonObject* ); 
             JsonObject*  _find         ( const std::string& tname );
    private:
      static JsonTypes*             _jsonTypes;
             std::set<JsonObject*>  _jsonObjects;
  };


// -------------------------------------------------------------------
// Class  :  "HookKey".

  class HookKey {
    public:
      inline                HookKey ( unsigned int id, const std::string& tname );
      inline  unsigned int  id      () const;
      inline  std::string   tname   () const;
    private:
      unsigned int  _id;
      std::string   _tname;
  };


  inline                HookKey::HookKey ( unsigned int id, const std::string& tname ) : _id(id), _tname(tname) { }
  inline  unsigned int  HookKey::id      () const { return _id; }
  inline  std::string   HookKey::tname   () const { return _tname; }

  inline bool operator< ( const HookKey& lhs, const HookKey& rhs )
  {
    if (lhs.id() != rhs.id()) return lhs.id() < rhs.id();
    return lhs.tname() < rhs.tname();
  }


// -------------------------------------------------------------------
// Class  :  "HookElement".

  class HookElement {
    public:
      enum Flags { OpenRingStart = (1<<0)
                 , ClosedRing    = (1<<1)
                 };
    public:
      inline                HookElement ( Hook*, unsigned long flags=0 );
      inline Hook*          hook        () const;
      inline HookElement*   next        () const;
      inline void           setHook     ( Hook* );
      inline void           setNext     ( HookElement* );
      inline unsigned long  flags       () const;
      inline HookElement&   setFlags    ( unsigned long mask );
      inline HookElement&   resetFlags  ( unsigned long mask );
      inline bool           issetFlags  ( unsigned long mask ) const;
    private:
      Hook*          _hook;
      HookElement*   _next;
      unsigned long  _flags;
  };


  inline                HookElement::HookElement ( Hook* hook, unsigned long flags ) : _hook(hook), _next(NULL), _flags(flags) { }
  inline Hook*          HookElement::hook        () const { return _hook; }
  inline HookElement*   HookElement::next        () const { return _next; }
  inline void           HookElement::setHook     ( Hook* hook ) { _hook = hook; }
  inline void           HookElement::setNext     ( HookElement* element ) { _next = element; }
  inline unsigned long  HookElement::flags       () const { return _flags; }
  inline HookElement&   HookElement::setFlags    ( unsigned long mask ) { _flags |=  mask; return *this; }
  inline HookElement&   HookElement::resetFlags  ( unsigned long mask ) { _flags &= ~mask; return *this; }
  inline bool           HookElement::issetFlags  ( unsigned long mask ) const { return _flags & mask; }


  typedef  map<HookKey,HookElement>  HookLut;


// -------------------------------------------------------------------
// Class  :  "JsonStack".

  class JsonStack {
    public:
      typedef  std::pair<std::string, boost::any>  Element;
    public:
      static                      bool           hookFromString ( std::string s, unsigned int& id, std::string& tname );
    public:
                           inline                JsonStack      ();
                           inline size_t         size           () const;
      template<typename T> inline void           push_back      ( const std::string&, T ); 
                           inline void           pop_back       ( size_t count=1 ); 
                           inline int            rhas           ( const std::string& ) const; 
      template<typename T> inline T              as             ( const std::string& ) const; 
      template<typename T> inline T              as             ( int ) const; 
                           inline void           push_back_dbo  ( DBo* );
                           inline void           pop_back_dbo   ();
                           inline DBo*           back_dbo       () const;
      template<typename T> inline T              getEntity      ( unsigned int ) const;
                                  void           addEntity      ( unsigned int jsonId, Entity* );
                                  void           addHookLink    ( Hook*, unsigned int jsonId, const std::string& jsonNext );
                                  Hook*          getHook        ( unsigned int jsonId, const std::string& tname ) const;
                                  bool           checkRings     () const;
                                  void           buildRings     () const;
                           inline void           clearHookLinks ();
                                  void           print          ( std::ostream& ) const;
                           inline JsonStack*     setFlags       ( unsigned long mask );
                           inline JsonStack*     resetFlags     ( unsigned long mask );
                           inline bool           issetFlags     ( unsigned long mask ) const;
                           inline const Element& operator[]     ( int index ) const;
    private:
      unsigned long                   _flags;
      vector<Element>                 _stack;
      vector<DBo*>                    _dbos;
      std::map<unsigned int,Entity*>  _entities;
      HookLut                         _hooks;
  };


  inline JsonStack::JsonStack ()
    : _flags(0), _stack(), _dbos(), _entities(), _hooks()
  { }

  template<typename T> inline void  JsonStack::push_back ( const std::string& key, T t )
  {
    ltrace(51) << "JsonStack::push_back(T) key:" << key << " value:" << t
               << " (" << demangle(typeid(T)) << ")." << endl;
    _stack.push_back(std::make_pair(key,boost::any(t)));
  }

  inline void  JsonStack::pop_back ( size_t count ) 
  {
    while (count--) {
      if (_stack.empty()) {
        std::cerr << "[ERROR] JsonStack::pop_back(): Stack is empty, but "
                  << (count+1) << " elements remains to pop." << std::endl;
        break;
      }
      ltrace(51) << "| _stack.pop_back() \"" << _stack.back().first
                 << "\", size:" << _stack.size() << ", dbos:" << _dbos.size() << endl;
      _stack.pop_back();
    }
  }

  inline const JsonStack::Element& JsonStack::operator[] ( int index ) const
  {
    if (index < 0) return _stack[_stack.size()+index];
    return _stack[index];
  }

  inline int  JsonStack::rhas ( const std::string& key ) const 
  {
    if (_stack.empty()) return 0;

    int i = _stack.size()-1;
    do {
      if (_stack[i].first == key) {
        ltrace(51) << "JsonStack::rhas(): key \"" << key << "\" found at index:"
                   << (i-(int)_stack.size()) << " (i:" << i << ") "
                   << "(" << demangle(_stack[i].second.type().name()) << ")."
                   << endl;
        return i-(int)_stack.size();
      }
      if (i == 0) break;
      --i;
    } while ( true );

    ltrace(51) << "JsonStack::rhas(): key \"" << key << "\" not found (returning index: 0)." << endl;
    return 0;
  }

  template<typename T> inline T JsonStack::as ( const std::string& key ) const
  {
    if (not _stack.empty()) {
      int i = _stack.size()-1;
      do {
        if (_stack[i].first == key) {
          ltrace(51) << "JsonStack::as<T>() k:" << key
                     << " value:" << demangle(_stack[i].second.type().name()) << std::endl;
          return boost::any_cast<T>( _stack[i].second );
        }
        if (i == 0) break;
        --i;
      } while ( true );

      std::cerr << "[ERROR] JsonStack::as<T>(key): No element with key \""
                << key << "\" in stack." << std::endl;
    } else {
      std::cerr << "[ERROR] JsonStack::as<T>(key): Stack is empty while searching for key \""
                << key << "\"." << std::endl;
    }

    return T();
  }

  template<typename T> inline T JsonStack::as ( int index ) const
  {
    size_t i = (index >= 0) ? index : (_stack.size()+index);
    return boost::any_cast<T>( _stack[i].second );
  }


  template<typename T> inline T  JsonStack::getEntity ( unsigned int id ) const
  {
    std::map<unsigned int,Entity*>::const_iterator it = _entities.find(id);
    if (it == _entities.end()) return NULL;
    return dynamic_cast<T>((*it).second);
  }
  
  inline void       JsonStack::push_back_dbo  ( DBo* dbo ) { _dbos.push_back(dbo); }
  inline void       JsonStack::pop_back_dbo   () { _dbos.pop_back(); }
  inline DBo*       JsonStack::back_dbo       () const { return (_dbos.empty()) ? NULL : _dbos.back(); }
  inline void       JsonStack::clearHookLinks () { _hooks.clear(); }
  inline size_t     JsonStack::size           () const { return _stack.size(); }
  inline JsonStack* JsonStack::setFlags       ( unsigned long mask ) { _flags |= mask; return this; }
  inline JsonStack* JsonStack::resetFlags     ( unsigned long mask ) { _flags &= ~mask; return this; }
  inline bool       JsonStack::issetFlags     ( unsigned long mask ) const { return _flags & mask; }


  template<typename T>
  T  JsonObject::get ( JsonStack& stack, const std::string& key ) const
  {
    int index = stack.rhas(key);
    if (index == 0) return T();;
    if (stack[index].second.type() == typeid(void*)) return T();

    return stack.as<T>( index );
  }

  template<typename T>
  inline void  JsonObject::update ( JsonStack& stack, T hobject )
  {
    ltrace(51) << "JsonObject::update<T>()" << endl;
    stack.pop_back( _attributes.size() );
    stack.push_back<T>( getStackName(), hobject );
    setObject<T>( hobject );
  }


// -------------------------------------------------------------------
// Function  :  Json Parsers.

  Cell* jsonCellParse ( std::string filename );
  Cell* jsonBlobParse ( std::string filename );


}  // Hurricane namespace.

#endif  // HURRICANE_JSON_READER_H
