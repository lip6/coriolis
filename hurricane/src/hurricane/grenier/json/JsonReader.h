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

  class Entity;
  class Cell;

// -------------------------------------------------------------------
// Class  :  "JsonObject".

  class JsonStack;
  class JsonArray { };


// -------------------------------------------------------------------
// Class  :  "JsonAttribute".

  class JsonAttribute {
    public:
      inline                   JsonAttribute ( const std::string& key, unsigned long flags );
      virtual                 ~JsonAttribute ();
      inline  std::string      key           () const;
      virtual std::type_index  tid           () const = 0;
      virtual void             toJson        ( JsonWriter*, boost::any object ) const = 0;
      virtual JsonAttribute*   clone         () const = 0;
      inline  void             setFlags      ( unsigned long mask );
      inline  void             resetFlags    ( unsigned long mask );
      inline  bool             issetFlags    ( unsigned long mask ) const;
    private:
      std::string      _key;
      unsigned long    _flags;
  };

  inline              JsonAttribute::JsonAttribute ( const std::string& key, unsigned long flags ): _key(key), _flags(flags) { }
  inline std::string  JsonAttribute::key           () const { return _key; }
  inline void         JsonAttribute::setFlags      ( unsigned long mask ) { _flags |=  mask; }
  inline void         JsonAttribute::resetFlags    ( unsigned long mask ) { _flags &= ~mask; }
  inline bool         JsonAttribute::issetFlags    ( unsigned long mask ) const { return _flags & mask; }


// -------------------------------------------------------------------
// Class  :  "JsonStacked".

  class JsonStacked : public JsonAttribute {
    public:
      inline                   JsonStacked ( const std::string& key );
      virtual                 ~JsonStacked ();
      virtual std::type_index  tid         () const;
      virtual void             toJson      ( JsonWriter*, boost::any object ) const;
      virtual JsonStacked*     clone       () const;
  };

  inline JsonStacked::JsonStacked ( const std::string& key ): JsonAttribute(key,0) { }


// -------------------------------------------------------------------
// Class  :  "JsonAnyAttribute<T,C>".

  template<typename T, typename C>
  class JsonAnyAttribute : public JsonAttribute {
    public:
                                  JsonAnyAttribute ( const std::string&    key
                                                   ,       unsigned long   flags
                                                   ,       T C::*          attr
                                                   );
      virtual                    ~JsonAnyAttribute ();
      virtual std::type_index     tid              () const;
      virtual JsonAnyAttribute*   clone            () const;
      virtual void                toJson           ( JsonWriter*, boost::any object ) const;
    private:
      T C::* _attribute;
  };


  template<typename T, typename C>
  JsonAnyAttribute<T,C>::JsonAnyAttribute ( const std::string&    key
                                          ,       unsigned long   flags
                                          ,       T C::*          attr
                                          )
    : JsonAttribute(key,flags)
    , _attribute   (attr)
  { }

  template<typename T, typename C>
  JsonAnyAttribute<T,C>::~JsonAnyAttribute ()
  { }

  template<typename T, typename C>
  std::type_index  JsonAnyAttribute<T,C>::tid ()
  { return typeid(T); }

  template<typename T, typename C>
  JsonAnyAttribute<T,C>* JsonAnyAttribute<T,C>::clone () const
  { return new JsonAnyAttribute<T,C>( *this ); }


// -------------------------------------------------------------------
// Class  :  "JsonStrAttribute<T,C>".

  template<typename T, typename C>
  class JsonStrAttribute : public JsonAttribute {
    public:
                                  JsonStrAttribute ( const std::string&    key
                                                   ,       unsigned long   flags
                                                   ,       T C::*          attr
                                                   );
      virtual                    ~JsonStrAttribute ();
      virtual std::type_index     tid              () const;
      virtual JsonStrAttribute*   clone            () const;
      virtual void                toJson           ( JsonWriter*, boost::any object ) const;
    private:
      T C::* _attribute;
  };


  template<typename T, typename C>
  JsonStrAttribute<T,C>::JsonStrAttribute ( const std::string&    key
                                          ,       unsigned long   flags
                                          ,       T C::*          attr
                                          )
    : JsonAttribute(key,flags)
    , _attribute   (attr)
  { }
                                 
  template<typename T, typename C>
  JsonStrAttribute<T,C>::~JsonStrAttribute ()
  { }

  template<typename T, typename C>
  std::type_index  JsonStrAttribute<T,C>::tid ()
  { return typeid(T); }

  template<typename T, typename C>
  JsonStrAttribute<T,C>* JsonStrAttribute<T,C>::clone () const
  { return new JsonStrAttribute<T,C>( *this ); }

  template<typename T, typename C>
  void  JsonStrAttribute<T,C>::toJson ( JsonWriter* w, boost::any object ) const
  { jsonWrite( w, key(), getString( boost::any_cast<C*>(object)->*_attribute ) ); }


// -------------------------------------------------------------------
// Class  :  "JsonFtrrAttribute<T&,C,R>".

  template<typename T, typename C, typename R >
  class JsonFtrrAttribute : public JsonAttribute {
    public:
                                  JsonFtrrAttribute ( const std::string&         key
                                                    ,       unsigned long        flags
                                                    ,       T C::*               attr
                                                    ,       std::function<R(T&)> tr
                                                    );
      virtual                    ~JsonFtrrAttribute ();
      virtual std::type_index     tid               () const;
      virtual JsonFtrrAttribute*  clone             () const;
      virtual void                toJson            ( JsonWriter*, boost::any object ) const;
    private:
      T C::*                _attribute;
      std::function<R(T&)>  _tr;
  };


  template<typename T, typename C, typename R>
  JsonFtrrAttribute<T,C,R>::JsonFtrrAttribute ( const std::string&         key
                                              ,       unsigned long        flags
                                              ,       T C::*               attr
                                              ,       std::function<R(T&)> tr
                                              )
    : JsonAttribute(key,flags)
    , _attribute   (attr)
    , _tr          (tr)
  { }
                                 
  template<typename T, typename C, typename R>
  JsonFtrrAttribute<T,C,R>::~JsonFtrrAttribute ()
  { }

  template<typename T, typename C, typename R>
  std::type_index  JsonFtrrAttribute<T,C,R>::tid ()
  { return typeid(T); }

  template<typename T, typename C, typename R>
  JsonFtrrAttribute<T,C,R>* JsonFtrrAttribute<T,C,R>::clone () const
  { return new JsonFtrrAttribute<T,C,R>( *this ); }

  template<typename T, typename C, typename R>
  void  JsonFtrrAttribute<T,C,R>::toJson ( JsonWriter* w, boost::any object ) const
  { jsonWrite( w, key(), _tr( boost::any_cast<C*>(object)->*_attribute ) ); }


// -------------------------------------------------------------------
// Class  :  "JsonFtrpAttribute<T,C,R>".

  template<typename T, typename C, typename R >
  class JsonFtrpAttribute : public JsonAttribute {
    public:
                                  JsonFtrpAttribute ( const std::string&         key
                                                    ,       unsigned long        flags
                                                    ,       T* C::*              attr
                                                    ,       std::function<R(T*)> tr
                                                    );
      virtual                    ~JsonFtrpAttribute ();
      virtual std::type_index     tid               () const;
      virtual JsonFtrpAttribute*  clone             () const;
      virtual void                toJson            ( JsonWriter*, boost::any object ) const;
    private:
      T* C::*               _attribute;
      std::function<R(T*)>  _tr;
  };


  template<typename T, typename C, typename R>
  JsonFtrpAttribute<T,C,R>::JsonFtrpAttribute ( const std::string&       key
                                            ,       unsigned long        flags
                                            ,       T* C::*              attr
                                            ,       std::function<R(T*)> tr
                                            )
    : JsonAttribute(key,flags)
    , _attribute   (attr)
    , _tr          (tr)
  { }
                                 
  template<typename T, typename C, typename R>
  JsonFtrpAttribute<T,C,R>::~JsonFtrpAttribute ()
  { }

  template<typename T, typename C, typename R>
  std::type_index  JsonFtrpAttribute<T,C,R>::tid ()
  { return typeid(T); }

  template<typename T, typename C, typename R>
  JsonFtrpAttribute<T,C,R>* JsonFtrpAttribute<T,C,R>::clone () const
  { return new JsonFtrpAttribute<T,C,R>( *this ); }

  template<typename T, typename C, typename R>
  void  JsonFtrpAttribute<T,C,R>::toJson ( JsonWriter* w, boost::any object ) const
  { jsonWrite( w, key(), _tr( boost::any_cast<C*>(object)->*_attribute ) ); }


// -------------------------------------------------------------------
// Class  :  "JsonMethAttribute<C,R>".

  template<typename C, typename R >
  class JsonMethAttribute : public JsonAttribute {
    public:
                                  JsonMethAttribute ( const std::string&   key
                                                    ,       unsigned long  flags
                                                    ,       R (C::* meth )() const
                                                    );
      virtual                    ~JsonMethAttribute ();
      virtual std::type_index     tid               () const;
      virtual JsonMethAttribute*  clone             () const;
      virtual void                toJson            ( JsonWriter*, boost::any object ) const;
    private:
      R (C::* _meth)() const;
  };


  template<typename C, typename R>
  JsonMethAttribute<C,R>::JsonMethAttribute ( const std::string&   key
                                            ,       unsigned long  flags
                                            ,       R (C::* meth)() const
                                            )
    : JsonAttribute(key,flags)
    , _meth        (meth)
  { }
                                 
  template<typename C, typename R>
  JsonMethAttribute<C,R>::~JsonMethAttribute ()
  { }

  template<typename C, typename R>
  std::type_index  JsonMethAttribute<C,R>::tid ()
  { return typeid(R); }

  template<typename C, typename R>
  JsonMethAttribute<C,R>* JsonMethAttribute<C,R>::clone () const
  { return new JsonMethAttribute<C,R>( *this ); }

  template<typename C, typename R>
  void  JsonMethAttribute<C,R>::toJson ( JsonWriter* w, boost::any object ) const
  { jsonWrite( w, key(), (boost::any_cast<C*>(object)->*_meth)() ); }


// -------------------------------------------------------------------
// Class  :  "JsonAnyAttribute<T,long int>".

#define JSON_POD_ATTRIBUTE(pod_t) \
  template<typename C>            \
  class JsonAnyAttribute<pod_t,C> : public JsonAttribute {                      \
    public:                                                                     \
                                  JsonAnyAttribute ( const std::string&   key   \
                                                   ,       unsigned long  flags \
                                                   ,       pod_t C::*     attr  \
                                                   );                           \
      virtual                    ~JsonAnyAttribute ();                          \
      virtual std::type_index     tid              () const;                    \
      virtual JsonAnyAttribute*   clone            () const;                    \
      virtual void                toJson           ( JsonWriter*, boost::any object ) const; \
    private:                                                                    \
      pod_t C::* _attribute;                                                    \
  };                                                                            \
                                                                                \
                                                                                \
  template<typename C>                                                          \
  JsonAnyAttribute<pod_t,C>::JsonAnyAttribute ( const std::string& key          \
                                              ,     unsigned long  flags        \
                                              ,     pod_t C::*     attr         \
                                              )                                 \
    : JsonAttribute(key,flags)                                                  \
    , _attribute   (attr)                                                       \
  { }                                                                           \
                                                                                \
  template<typename C>                                                          \
  JsonAnyAttribute<pod_t,C>::~JsonAnyAttribute ()                               \
  { }                                                                           \
                                                                                \
  template<typename C>                                                          \
  std::type_index  JsonMethAttribute<pod_t,C>::tid ()                           \
  { return typeid(pod_t); }                                                     \
                                                                                \
  template<typename C>                                                          \
  JsonAnyAttribute<pod_t,C>* JsonAnyAttribute<pod_t,C>::clone () const          \
  { return new JsonAnyAttribute<pod_t,C>( *this ); }                            \
                                                                                \
  template<typename C>                                                          \
  void  JsonAnyAttribute<pod_t,C>::toJson ( JsonWriter* w, boost::any object ) const \
  { jsonWrite( w, key(), boost::any_cast<C*>(object)->*_attribute ); }


// Build a specialized JsonAnyAttribute<T,C> for all POD types.
  JSON_POD_ATTRIBUTE(bool)
  JSON_POD_ATTRIBUTE(int)
  JSON_POD_ATTRIBUTE(long)
  JSON_POD_ATTRIBUTE(unsigned int)
  JSON_POD_ATTRIBUTE(unsigned long)


// -------------------------------------------------------------------
// Class  :  "JsonObject".

  class JsonObject {
    public:
      enum Flags { IsJsonObject     = (1<<0)
                 , UsePlugReference = (1<<1)
                 , UseReference     = (1<<3)
                 };
    public:
                                                  JsonObject    ();
                                                  JsonObject    ( const JsonObject& );
                           virtual               ~JsonObject    ();
                           virtual std::string    getTypeName   () const = 0;
                           inline  std::string    getStackName  () const;
                           virtual bool           isJsonObject  () const;
                                   bool           check         ( JsonStack&, string fname ) const;
                                   void           print         ( std::ostream& ) const;
                                   bool           has           ( const std::string& key ) const;
      template<typename T>
      inline T     get    ( JsonStack&, const std::string& key ) const;
             void  add    ( const std::string& key,                           unsigned long flags );
      template<typename T, typename C>                          
             void  add    ( const std::string& key, T  C::* attr,             unsigned long flags );
      template<typename T, typename C, typename R>                          
             void  add    ( const std::string& key, T  C::* attr, R(*tr)(T&), unsigned long flags );
      template<typename T, typename C, typename R>                          
             void  add    ( const std::string& key, T* C::* attr, R(*tr)(T*), unsigned long flags );
      template<typename C, typename R>                          
             void  add    ( const std::string& key, R(C::*  meth)() const,    unsigned long flags );
      template<typename T, typename C>                          
             void  addS   ( const std::string& key, T  C::* attr,             unsigned long flags );
             void  remove ( const std::string& key );
      template<typename T, typename C>                          
                                   T              access        ( const std::string& key );
                           inline  void           clear         ();
                           inline  std::string    getName       () const;
                           inline  void           setName       ( const string& );
      template<typename T> inline  T&             getObject     () const; 
      template<typename T> inline  void           setObject     ( T& ) ; 
                           inline  bool           isCreated     () const;
                           virtual JsonObject*    clone         () const = 0;
      template<typename C>         void           toJson        ( JsonWriter*, C* object ) const;
                           virtual void           toData        ( JsonStack& );
      template<typename T> inline  void           update        ( JsonStack&, T );
                           inline  void           setFlags      ( unsigned long mask );
                           inline  void           resetFlags    ( unsigned long mask );
                           inline  bool           issetFlags    ( unsigned long mask ) const;
    protected:
                           inline  bool           _checkKey     ( const std::string& key ) const;
    protected:
      unsigned long                _flags;
      std::string                  _name;
      std::vector<JsonAttribute*>  _stackeds;
      std::vector<JsonAttribute*>  _attributes;
      std::vector<JsonAttribute*>  _collections;
      boost::any                   _object;
  };


  inline bool         JsonObject::isCreated  () const { return not _object.empty(); }
  inline std::string  JsonObject::getName    () const { return _name; }
  inline void         JsonObject::setName    ( const string& name ) { _name=name; }
  inline void         JsonObject::setFlags   ( unsigned long mask ) { _flags |=  mask; }
  inline void         JsonObject::resetFlags ( unsigned long mask ) { _flags &= ~mask; }
  inline bool         JsonObject::issetFlags ( unsigned long mask ) const { return _flags & mask; }

  inline std::string  JsonObject::getStackName () const
  { return (_name.empty()) ? std::string(".")+getTypeName(): _name; }

  template<typename T> inline T& JsonObject::getObject () const
  { return boost::any_cast<T>(_object); }

  template<typename T> inline void  JsonObject::setObject ( T& t )
  { _object = t; }


  bool  JsonObject::checkKey ( const std::string& key ) const
  {
    if (key.empty()) {
      cerr << "[ERROR] JsonObject::add(): Attempt to add attribute with an empty name, ignored."
           << endl;
      return false;
    }
    if (has(key)) {
      cerr << "[ERROR] JsonObject::add(): Attempt to add attribute \"" << key << "\" twice, cancelled."
           << endl;
      return false;
    }
    return true;
  }


  void  JsonObject::add ( const std::string& key )
  {
    if ( _checkKey(key) and (key[0] == '.') ) {
      _stackeds.push_back( new JsonStacked(key) );
      return;
    }
    cerr << "[ERROR] JsonObject::add(): Stack requirement key \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename T, typename C>
  void  JsonObject::add( const std::string& key, T C::* attr, unsigned long flags )
  {
    if (not _checkKey(key)) return;
    switch ( key[0] ) {
      case '_': _attributes.push_back( new JsonAnyAttribute<T,C>(key,flags,attr) );
      case '+': _attributes.push_back( new JsonAnyAttribute<T,C>(key,flags,attr) );
      return;
    }
    cerr << "[ERROR] JsonObject::add(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename T, typename C, typename R>
  void  JsonObject::add( const std::string& key, T C::* attr, R(*tr)(T&), unsigned long flags )
  {
    if (not _checkKey(key)) return;
    switch ( key[0] ) {
      case '_': _attributes.push_back( new JsonFtrrAttribute<T,C,R>(key,flags,attr,std::function<R(T&)>(tr)) );
      case '+': _attributes.push_back( new JsonFtrrAttribute<T,C,R>(key,flags,attr,std::function<R(T&)>(tr)) );
      return;
    }
    cerr << "[ERROR] JsonObject::add(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename T, typename C, typename R>
  void  JsonObject::add( const std::string& key, T* C::* attr, R(*tr)(T*), unsigned long flags )
  {
    if (not _checkKey(key)) return;
    switch ( key[0] ) {
      case '_': _attributes.push_back( new JsonFtrpAttribute<T,C,R>(key,flags,attr,std::function<R(T*)>(tr)) );
      case '+': _attributes.push_back( new JsonFtrpAttribute<T,C,R>(key,flags,attr,std::function<R(T*)>(tr)) );
      return;
    }
    cerr << "[ERROR] JsonObject::add(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename C, typename R>
  void  JsonObject::add( const std::string& key, R(C::*meth)() const, unsigned long flags )
  {
    if (not _checkKey(key)) return;
    switch ( key[0] ) {
      case '_': _attributes.push_back( new JsonMethAttribute<C,R>(key,flags,meth) );
      case '+': _attributes.push_back( new JsonMethAttribute<C,R>(key,flags,meth) );
      return;
    }
    cerr << "[ERROR] JsonObject::add(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename T, typename C>
  void  JsonObject::addS( const std::string& key, T C::* attr, unsigned int flags )
  {
    if (not _checkKey(key)) return;
    switch ( key[0] ) {
      case '_': _attributes.push_back( new JsonStrAttribute<T,C>(key,flags,attr) );
      return;
    }
    cerr << "[ERROR] JsonObject::addS(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  template<typename C> void  JsonObject::toJson ( JsonWriter* w, C* object ) const
  {
    if (isJsonObject()) {
      w->startObject();
      jsonWrite( w, "@typename", getTypeName() );
      for ( JsonAttribute* attribute  : _attributes  ) attribute ->toJson( w, object );
      for ( JsonAttribute* collection : _collections ) collection->toJson( w, object );
      w->endObject();
    }
  }


  inline void  JsonObject::clear ()
  {
    for ( JsonAttribute* attribute : _stackeds    ) delete attribute;
    for ( JsonAttribute* attribute : _attributes  ) delete attribute;
    for ( JsonAttribute* attribute : _collections ) delete attribute;

    _stackeds   .clear();
    _attributes .clear();
    _collections.clear();
  }


  class JsonKey : public JsonObject {
    public:
      inline               JsonKey     ( const std::string& );
      virtual std::string  getTypeName () const;
      virtual JsonKey*     clone       () const;
    private:
      std::string  _key;
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
// Class  :  "JsonStack".

  class JsonStack {
    public:
      typedef  std::pair<std::string, boost::any>  Element;
    public:
                           inline                JsonStack   ();
                           inline size_t         size        () const;
      template<typename T> inline void           push_back   ( const std::string&, T ); 
                           inline void           pop_back    ( size_t count=1 ); 
                           inline int            rhas        ( const std::string& ) const; 
      template<typename T> inline T              as          ( const std::string& ) const; 
      template<typename T> inline T              as          ( int ) const; 
      template<typename T> inline T              getEntity   ( unsigned int ) const;
                                  void           addEntity   ( unsigned int jsonId, Entity* );
                                  void           print       ( std::ostream& ) const;
                           inline JsonStack*     setFlags    ( unsigned long mask );
                           inline JsonStack*     resetFlags  ( unsigned long mask );
                           inline bool           issetFlags  ( unsigned long mask ) const;
                           inline const Element& operator[]  ( int index ) const;
    private:
      unsigned long                   _flags;
      vector<Element>                 _stack;
      std::map<unsigned int,Entity*>  _entities;
  };


  inline JsonStack::JsonStack ()
    : _flags(0), _stack(), _entities()
  { }

  template<typename T> inline void  JsonStack::push_back ( const std::string& key, T t ) {
    ltrace(51) << "JsonStack::push_back() key:" << key << " t:" << t
               << " (" << demangle(typeid(T)) << ")." << endl;
    _stack.push_back(std::make_pair(key,boost::any(t)));
  }

  inline void  JsonStack::pop_back ( size_t count ) 
  { while (count--) {
      if (_stack.empty()) {
        std::cerr << "[ERROR] JsonStack::pop_back(): Stack is empty, but "
                  << (count+1) << " elements remains to pop." << std::endl;
        break;
      }
      ltrace(51) << "| _stack.pop_back() " << _stack.back().first << endl;
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
                   << (i-(int)_stack.size()) << " (i:" << i << ")." << endl;
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
                     << " t:" << _stack[i].second.type().name() << std::endl;
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

  template<typename T> inline T  JsonStack::getEntity ( unsigned int id ) const {
    std::map<unsigned int,Entity*>::const_iterator it = _entities.find(id);
    if (it == _entities.end()) return NULL;
    return dynamic_cast<T>((*it).second);
  }
  
  inline size_t     JsonStack::size       () const { return _stack.size(); }
  inline JsonStack* JsonStack::setFlags   ( unsigned long mask ) { _flags |= mask; return this; }
  inline JsonStack* JsonStack::resetFlags ( unsigned long mask ) { _flags &= ~mask; return this; }
  inline bool       JsonStack::issetFlags ( unsigned long mask ) const { return _flags & mask; }


  template<typename T>
  T  JsonObject::get ( JsonStack& stack, const std::string& key ) const
  {
    int index = stack.rhas(key);
    if (index == 0) return T();;

    return stack.as<T>( index );
  }

  template<typename T> inline void  JsonObject::update ( JsonStack& stack, T hobject )
  {
    stack.pop_back( _attributes.size() );
    stack.push_back<T>( getStackName(), hobject );
    setObject<T>( hobject );
  }


// -------------------------------------------------------------------
// Function  :  Json Cell parser.

  Cell* jsonCellParse ( std::string filename );


}  // Hurricane namespace.


template<typename C>
inline void  jsonWrite ( JsonWriter* w, const std::string& key, const C* object, unsigned long flags )
{
  w->key( key );
  Hurricane::JsonObject* jobject = C::getJsonObject(flags);
  jobject->toJson( w, object );
  delete jobject;
}


namespace Hurricane {

// Delayed "void JsonAnyAttribute<T,C>::toJson()" template definition, because
// it needs the jsonWrite<> template specialisation for "const T*", which can
// be only declared after "JsonObject".

  template<typename T, typename C>
  void  JsonAnyAttribute<T,C>::toJson ( JsonWriter* w, boost::any object ) const
  { jsonWrite<T>( w, key(), &(boost::any_cast<C*>(object)->*_attribute) ); }

}  // Hurricane namespace.

#endif  // HURRICANE_JSON_READER_H
