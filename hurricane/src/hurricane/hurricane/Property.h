// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2021, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/Property.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Name.h"
#include "hurricane/Properties.h"
#include "hurricane/DBo.h"
#include "hurricane/Error.h"


namespace Hurricane {


  extern const char* propertyTypeNameError;


// -------------------------------------------------------------------
// Classes  :  template enable/disable Json support.

  struct JsonEnabled  { enum State { enabled=1 }; };
  struct JsonDisabled { enum State { enabled=0 }; };


// -------------------------------------------------------------------
// Class  :  "Hurricane::Property".


  class Property {

    public:
    // Static Method.
      template<typename DerivedProperty>
      static  DerivedProperty* get           ( const DBo* );
      static  Name             staticGetName ();
    // Constructor.
      template<typename DerivedProperty>
      static  DerivedProperty* create        ();
      template<typename DerivedProperty, typename Value>
      static  DerivedProperty* create        ( const Value& );
    // Destructor.
      virtual void             destroy       ();
    // Methods.
      virtual Name             getName       () const = 0;
      virtual void             onCapturedBy  ( DBo* owner ) = 0;
      virtual void             onReleasedBy  ( DBo* owner ) = 0;
    // Hurricane Managment.
      virtual bool             hasJson       () const;
      virtual void             toJson        ( JsonWriter*, const DBo* ) const;
      virtual string           _getTypeName  () const = 0;
      virtual string           _getString    () const;
      virtual Record*          _getRecord    () const;

    private:
      static  Name             _baseName;
    protected:
    // Internal: Constructors & Destructors.
                               Property      ();
      virtual                 ~Property      ();
      virtual void             _postCreate   () {};
      virtual void             _preDestroy   () {};
    private:
                               Property      ( const Property& );
                     Property& operator=     ( const Property& );
  };


  template<typename DerivedProperty>
  DerivedProperty* Property::create ()
  {
    DerivedProperty* property = new DerivedProperty();
    property->_postCreate();
    return property;
  }


  template<typename DerivedProperty, typename Value>
  DerivedProperty* Property::create ( const Value& value )
  {
    DerivedProperty* property = new DerivedProperty(value);
    property->_postCreate();
    return property;
  }


  template<typename DerivedProperty>
  DerivedProperty* Property::get ( const DBo* object )
  {
    Property*        property1 = object->getProperty ( DerivedProperty::staticGetName() );
    DerivedProperty* property2 = dynamic_cast<DerivedProperty*> ( property1 );
    
    if ( property1 && !property2 )
      throw Error ( propertyTypeNameError
                  , getString(DerivedProperty::staticGetName()).c_str()
                  , getString(object).c_str() );

    return property2;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::PrivateProperty".


  class PrivateProperty : public Property {

    public:
    // Methods.
      inline  DBo*    getOwner        () const;
      virtual void    onCapturedBy    ( DBo* owner );
      virtual void    onReleasedBy    ( DBo* owner );
      virtual void    onNotOwned      ();
      virtual string  _getString      () const;
      virtual Record* _getRecord      () const;

    private:
    // Internal: Attributes.
              DBo*    _owner;
    protected:
    // Internal: Constructor & Destructors.
                      PrivateProperty ();
      virtual void    _preDestroy     ();
  };


// Inline Functions.
  inline  DBo* PrivateProperty::getOwner () const { return _owner; };


// -------------------------------------------------------------------
// Template Class  :  "Hurricane::StandardPrivateProperty".


  template<typename Value, typename JsonState=JsonDisabled>
  class StandardPrivateProperty : public PrivateProperty {
    public:
      static  Name                     staticGetName  ();
      static  Value*                   staticGetValue ( const DBo* );
      static  StandardPrivateProperty* get            ( const DBo*, bool create=false );
    // Constructors.
      static  StandardPrivateProperty* create         ();
      static  StandardPrivateProperty* create         ( const Value& );
    // Methods.
      virtual Name                     getName        () const;
              Value&                   getValue       () const;
              void                     setValue       ( const Value& );
      virtual bool                     hasJson        () const;
      virtual void                     toJson         ( JsonWriter*, const DBo* ) const;
      virtual string                   _getTypeName   () const;
      virtual string                   _getString     () const;
      virtual Record*                  _getRecord     () const;
    private:
    // Internal: Attributes.
      static  Name                     _name;
      static  DBo*                     _owner;
      static  StandardPrivateProperty* _cache;
      mutable Value                    _value;
    protected:
    // Internal: Constructor.
      StandardPrivateProperty ();
      StandardPrivateProperty ( const Value& );
    public:
      class JsonProperty : public JsonObject {
        public:
          static  void          initialize   ();
                                JsonProperty ( unsigned long flags );
          virtual string        getTypeName  () const;
          virtual JsonProperty* clone        ( unsigned long ) const;
          virtual void          toData       ( JsonStack& ); 
      };
  };


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>::JsonProperty::JsonProperty ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode)
      cerr << "Registering JsonProperty" << endl;
    add( "_value", typeid(Value) );
  }


  template<typename Value, typename JsonState>
  string  StandardPrivateProperty<Value,JsonState>::JsonProperty::getTypeName () const
  { return getString(StandardPrivateProperty<Value,JsonState>::staticGetName()); }


  template<typename Value, typename JsonState>
  void  StandardPrivateProperty<Value,JsonState>::JsonProperty::initialize ()
  { JsonTypes::registerType( new JsonProperty (JsonWriter::RegisterMode) ); }


  template<typename Value, typename JsonState>
  typename StandardPrivateProperty<Value,JsonState>::JsonProperty*
  StandardPrivateProperty<Value,JsonState>::JsonProperty::clone ( unsigned long flags ) const
  { return new JsonProperty ( flags ); }


  template<typename Value, typename JsonState>
  void StandardPrivateProperty<Value,JsonState>::JsonProperty::toData ( JsonStack& stack )
  {
    check( stack, "JsonProperty::toData" );

    DBo*   dbo   = stack.back_dbo();
    Value  value = get<string>(stack,"_value");
    StandardPrivateProperty<Value,JsonState>* property
                 = StandardPrivateProperty<Value,JsonState>::create(value);
    if (dbo) dbo->put( property );
    
    update( stack, property );
  }


  template<typename Value, typename JsonState>
  DBo* StandardPrivateProperty<Value,JsonState>::_owner = NULL;


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>* StandardPrivateProperty<Value,JsonState>::_cache = NULL;


  template<typename Value, typename JsonState>
  Name  StandardPrivateProperty<Value,JsonState>::staticGetName ()
  {
    return _name;
  }


  template<typename Value, typename JsonState>
  Value* StandardPrivateProperty<Value,JsonState>::staticGetValue ( const DBo* object )
  {
    if ( ( object == _owner ) || get(object) ) return _cache->getValue();
    return NULL;
  }


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>* StandardPrivateProperty<Value,JsonState>::create ()
  {
    _cache = new StandardPrivateProperty<Value>();
    _cache->_postCreate();
    return _cache;
  }


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>* StandardPrivateProperty<Value,JsonState>::create ( const Value& value )
  {
    _cache = new StandardPrivateProperty<Value>(value);
    _cache->_postCreate();
    return _cache;
  }


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>* StandardPrivateProperty<Value,JsonState>::get ( const DBo* object, bool create )
  {
    if ( object == _owner ) return _cache;

    Property* property = object->getProperty ( StandardPrivateProperty<Value>::staticGetName() );
    _cache   = dynamic_cast<StandardPrivateProperty<Value>*> ( property );
    
    if ( !_cache ) {
      if ( property )
        throw Error ( propertyTypeNameError
                    , getString(StandardPrivateProperty<Value>::staticGetName()).c_str()
                    , getString(object).c_str() );
      else if ( create )
        const_cast<DBo*>(object)->put ( StandardPrivateProperty<Value>::create() );
    }

    return _cache;
  }
  

  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>::StandardPrivateProperty ()
    : PrivateProperty(), _value()
  { }


  template<typename Value, typename JsonState>
  StandardPrivateProperty<Value,JsonState>::StandardPrivateProperty ( const Value& value )
    : PrivateProperty(), _value(value)
  { }


  template<typename Value, typename JsonState>
  Name StandardPrivateProperty<Value,JsonState>::getName() const
  {
    return staticGetName();
  }


  template<typename Value, typename JsonState>
  Value& StandardPrivateProperty<Value,JsonState>::getValue () const
  {
    return _value;
  }


  template<typename Value, typename JsonState>
  void StandardPrivateProperty<Value,JsonState>::setValue ( const Value& value )
  {
    _value = value;
  }


  template<typename Value, typename JsonState>
  bool  StandardPrivateProperty<Value,JsonState>::hasJson () const
  {
    return JsonState::enabled;
  }


  template<typename Value, typename JsonState>
  void  StandardPrivateProperty<Value,JsonState>::toJson ( JsonWriter* w, const DBo* ) const
  {
    w->startObject();
    std::string tname = getString(staticGetName());
    jsonWrite( w, "@typename", tname  );
    jsonWrite( w, "_value", _value );
    w->endObject();
  }


  template<typename Value, typename JsonState>
  string  StandardPrivateProperty<Value,JsonState>::_getTypeName () const
  {
    return _TName("StandardPrivateProperty");
  }

  template<typename Value, typename JsonState>
  string  StandardPrivateProperty<Value,JsonState>::_getString () const
  {
    string s = PrivateProperty::_getString();
    s.insert(s.length() - 1, " " + getString<Value&>(_value));
    return s;
  }

  template<typename Value, typename JsonState>
  Record* StandardPrivateProperty<Value,JsonState>::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if (record) {
      record->add ( getSlot("_name"       , staticGetName())    );
      record->add ( getSlot("_value"      ,&_value)             );
      record->add ( getSlot("JSON support", JsonState::enabled) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::SharedProperty".


  class SharedProperty : public Property {
    private:
      class Orphaned {
        public:
          inline Orphaned ( SharedProperty* );
        public:
          SharedProperty* _property;
          unsigned int    _refcount;
          unsigned int    _count;
      };
    public:
      typedef set<DBo*,DBo::CompareById>  DBoSet;
      typedef map<string,Orphaned>        OrphanedMap;
    public:
      static  const OrphanedMap& getOrphaneds   ();
      static  SharedProperty*    getOrphaned    ( const string& );
      static  void               addOrphaned    ( const string&, SharedProperty* );
      static  void               refOrphaned    ( const string& );
      static  void               countOrphaned  ( const string&, unsigned int );
      static  void               removeOrphaned ( const string& );
      static  void               clearOrphaneds ();
    public:                      
      inline  DBos               getOwners      () const;
      virtual void               onCapturedBy   ( DBo* owner );
      virtual void               onReleasedBy   ( DBo* owner );
      virtual void               onNotOwned     ();
              void               _erase         ( DBo* owner );
      inline  DBoSet&            _getOwnerSet   ();
      virtual string             _getString     () const;
      virtual Record*            _getRecord     () const;
    private:
      static  OrphanedMap  _orphaneds;
    private:
              DBoSet       _ownerSet;
    protected:
                         SharedProperty ();
      virtual void       _preDestroy    ();
  };


// Inline Functions.
  inline  SharedProperty::Orphaned::Orphaned ( SharedProperty* property )
    : _property(property), _refcount(0), _count(0)
  { }

  inline DBos                     SharedProperty::getOwners    () const { return getCollection(_ownerSet); }
  inline SharedProperty::DBoSet&  SharedProperty::_getOwnerSet () { return _ownerSet; }


// -------------------------------------------------------------------
// Template Class  :  "Hurricane::StandardSharedProperty".


  template<typename Value> class StandardSharedProperty : public SharedProperty {

    public:
      static  Name                    staticGetName  ();
      static  Value*                  staticGetValue ( const DBo* );
      static  StandardSharedProperty* get            ( const DBo*, bool create=false );
    // Constructors.
      static  StandardSharedProperty* create         ();
      static  StandardSharedProperty* create         ( const Value& );
    // Methods.
      virtual Name                    getName        () const;
              Value&                  getValue       () const;
              void                    setValue       ( const Value& );
      virtual string                  _getTypeName   () const;
      virtual string                  _getString     () const;
      virtual Record*                 _getRecord     () const;

    private:
    // Internal: Attributes.
      static  Name                    _name;
      static  DBo*                    _owner;
      static  StandardSharedProperty* _cache;
      mutable Value                   _value;

    protected:
    // Internal: Constructor.
      StandardSharedProperty ();
      StandardSharedProperty ( const Value& );
  };


// Template function members.
  template<typename Value>
  DBo* StandardSharedProperty<Value>::_owner = NULL;


  template<typename Value>
  StandardSharedProperty<Value>* StandardSharedProperty<Value>::_cache = NULL;


  template<typename Value>
  Name  StandardSharedProperty<Value>::staticGetName ()
  {
    return _name;
  }


  template<typename Value>
  Value* StandardSharedProperty<Value>::staticGetValue ( const DBo* object )
  {
    if ( ( object == _owner ) || get(object) ) return _cache->getValue();
    return NULL;
  }


  template<typename Value>
  StandardSharedProperty<Value>* StandardSharedProperty<Value>::create ()
  {
    _cache = new StandardSharedProperty<Value>();
    _cache->_postCreate();
    return _cache;
  }


  template<typename Value>
  StandardSharedProperty<Value>* StandardSharedProperty<Value>::create ( const Value& value )
  {
    _cache = new StandardPrivateProperty<Value>(value);
    _cache->_postCreate();
    return _cache;
  }


  template<typename Value>
  StandardSharedProperty<Value>* StandardSharedProperty<Value>::get ( const DBo* object, bool create )
  {
    if ( _owner == object ) return _cache;

    Property* property = object->getProperty ( StandardSharedProperty<Value>::staticGetName() );
    _cache = dynamic_cast<StandardSharedProperty<Value>*> ( property );
    
    if ( !_cache ) {
      if ( property )
        throw Error ( propertyTypeNameError
                    , getString(StandardSharedProperty<Value>::staticGetName()).c_str()
                    , getString(object).c_str() );
      else if ( create )
        const_cast<DBo*>(object)->put ( StandardSharedProperty<Value>::create() );
    }

    return _cache;
  }


  template<typename Value>
  StandardSharedProperty<Value>::StandardSharedProperty ()
    : SharedProperty(), _value()
  { }


  template<typename Value>
  StandardSharedProperty<Value>::StandardSharedProperty ( const Value& value )
    : SharedProperty(), _value(value)
  { }


  template<typename Value>
  Name  StandardSharedProperty<Value>::getName() const
  {
    return staticGetName();
  }


  template<typename Value>
  Value& StandardSharedProperty<Value>::getValue() const
  {
    return _value;
  }


  template<typename Value>
  void  StandardSharedProperty<Value>::setValue(const Value& value)
  {
    _value = value;
  }


  template<typename Value>
  string  StandardSharedProperty<Value>::_getTypeName() const
  {
    return _TName("StandardSharedProperty");
  }


  template<typename Value>
  string  StandardSharedProperty<Value>::_getString() const
  {
    string s = SharedProperty::_getString();
    s.insert(s.length() - 1, " " + getString(_value));
    return s;
  }


  template<typename Value>
  Record* StandardSharedProperty<Value>::_getRecord() const
  {
    Record* record = SharedProperty::_getRecord();
    if (record) {
      record->add ( getSlot("Name" ,  staticGetName()) );
      record->add ( getSlot("Value", &_value)          );
    }
    return record;
  }


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Property);
