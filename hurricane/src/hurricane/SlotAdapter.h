

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




# ifndef  __SLOT_ADAPTER__
#   define  __SLOT_ADAPTER__


#   ifndef  __HURRICANE_COMMONS__
#     error "SlotAdapter.h musn't be included alone, please uses Commons.h."
#   endif



#   include  <sstream>



namespace Hurricane {


  using namespace std;




// -------------------------------------------------------------------
// Template Functions  :  "Proxy...()".

template<typename Type>
  inline string  ProxyTypeName ( const Type* object ) { return object->_GetTypeName(); }

template<typename Type>
  inline string  ProxyString   ( const Type* object ) { return object->_GetString(); }

template<typename Type>
  inline Record* ProxyRecord   ( const Type* object ) { return object->_GetRecord(); }
  



// -------------------------------------------------------------------
// SlotAdapter Type Identification by Template.


template<typename T>
  class IsNestedSlotAdapter {
    public:
      enum { True=0, False=1 };
  };


template<typename T>
  class IsNestedSlotAdapter<const T> {
    public:
      enum { True=0, False=1 };
  };




// -------------------------------------------------------------------
// Class  :  "SlotAdapter".

  class SlotAdapter {
  
    // Destructor.
    public:
      virtual ~SlotAdapter () {};
  
    // Slot Management.
    public:
             virtual string  _GetTypeName () const = 0;
             virtual string  _GetString   () const { return "SlotAdapter::_GetString()"; };
             virtual Record* _GetRecord   () const { return NULL; };
      inline virtual Slot*   _GetSlot     ( const string& name ) const;
  
  };




// -------------------------------------------------------------------
// Class  :  "NestedSlotAdapter".

  class NestedSlotAdapter : public SlotAdapter {
  
    // Destructor.
    public:
      virtual ~NestedSlotAdapter () {};
  
    // Slot Management.
    public:
      inline virtual Slot* _GetSlot ( const string& name ) const;
  
  };




# define  SetNestedSlotAdapter(T)                                                       \
  namespace Hurricane {                                                                 \
    template<>                                                                          \
      class IsNestedSlotAdapter<const T> {                                              \
        protected:                                                                      \
          const T* t;                                                                   \
        public:                                                                         \
          const NestedSlotAdapter* _CheckInheritance () const                           \
                                   { return static_cast<const NestedSlotAdapter*>(t); } \
        public:                                                                         \
          enum { True=1, False=0 };                                                     \
      };                                                                                \
  }




// -------------------------------------------------------------------
// Templates Class  :  "PointerSlotAdapter<Type>".

template<typename Type>
  class PointerSlotAdapter : public SlotAdapter {

    // Attributes.
    protected:
      Type* _object;
  
    // Constructors.
    public:
      PointerSlotAdapter ( Type* o ) : SlotAdapter(), _object(o) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};
  
    // Slot Management.
    public:
      virtual string  _GetTypeName () const { return ProxyTypeName(_object); };
      virtual string  _GetString   () const { return ProxyString  (_object); };
      virtual Record* _GetRecord   () const { return ProxyRecord  (_object); };
  
  };




// -------------------------------------------------------------------
// Templates Class  :  "ValueSlotAdapter<Type>".

template<typename Type>
  class ValueSlotAdapter : public SlotAdapter {

    // Attributes.
    protected:
      Type  _object;
  
    // Constructors.
    public:
      ValueSlotAdapter ( Type o ) : SlotAdapter(), _object(o) {};
  
    // Destructor.
    public:
      virtual ~ValueSlotAdapter () {};
  
    // Slot Management.
    public:
      virtual string  _GetTypeName () const { return ProxyTypeName(&_object); };
      virtual string  _GetString   () const { return ProxyString  (&_object); };
      virtual Record* _GetRecord   () const { return ProxyRecord  (&_object); };
  
  };




// -------------------------------------------------------------------
// Class  :  "Proxy...<const string*>".

template<>
  inline string  ProxyTypeName<string> ( const string* object ) { return "<PointerSlotAdapter<string>>"; }

template<>
  inline string  ProxyString<string>   ( const string* object ) { return *object; }

template<>
  inline Record* ProxyRecord<string>   ( const string* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "PointerSlotAdapter<const char*>".

template<>
  inline string  ProxyTypeName<char> ( const char* object ) { return "<PointerSlotAdapter<char*>>"; }

template<>
  inline string  ProxyString<char>   ( const char* object ) { return object; }

template<>
  inline Record* ProxyRecord<char>   ( const char* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "ValueSlotAdapter<const char>".

template<>
  class ValueSlotAdapter<const char> : public SlotAdapter {
      
    // Attributes.
    protected:
      const char  _c;
  
    // Constructor.
    public:
      ValueSlotAdapter ( const char c ) : _c(c) {};
  
    // Destructor.
    public:
      virtual ~ValueSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName () const { return "<ValueSlotAdapter<const char>>"; };
      virtual string  _GetString   () const { return string(1,_c); };
  };




// -------------------------------------------------------------------
// Class  :  "ValueSlotAdapter<char>".

template<>
  class ValueSlotAdapter<char> : public SlotAdapter {
      
    // Attributes.
    protected:
      const char  _c;
  
    // Constructor.
    public:
      ValueSlotAdapter ( const char c ) : _c(c) {};
  
    // Destructor.
    public:
      virtual ~ValueSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName () const { return "<ValueSlotAdapter<const char>>"; };
      virtual string  _GetString   () const { return string(1,_c); };
  };




// -------------------------------------------------------------------
// Class  :  "Proxy...<const bool*>".

template<>
  inline string  ProxyTypeName<bool> ( const bool* object ) { return "<PointerSlotAdapter<bool>>"; }

template<>
  inline string  ProxyString<bool>   ( const bool* object ) { return (*object)?"True":"False" ; }

template<>
  inline Record* ProxyRecord<bool>   ( const bool* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const void*>".

template<>
  inline string  ProxyTypeName<void> ( const void* object ) { return "<PointerSlotAdapter<void>>"; }

template<>
  inline string  ProxyString<void>   ( const void* object )
                                     {
                                       static ostringstream os;
                                       os.str ( "0x" );
                                       os << hex << object;
                                       return os.str();
                                     }

template<>
  inline Record* ProxyRecord<void>   ( const void* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const int*>".

template<>
  inline string  ProxyTypeName<int> ( const int* object ) { return "<PointerSlotAdapter<int>>"; }

template<>
  inline string  ProxyString<int>   ( const int* object )
                                    {
                                      static ostringstream os;
                                      os.str ( "" );
                                      os << *object;
                                      return os.str();
                                    }

template<>
  inline Record* ProxyRecord<int>   ( const int* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const long*>".

template<>
  inline string  ProxyTypeName<long> ( const long* object ) { return "<PointerSlotAdapter<long>>"; }

template<>
  inline string  ProxyString<long>   ( const long* object )
                                     {
                                       static ostringstream os;
                                       os.str ( "" );
                                       os << *object;
                                       return os.str();
                                     }

template<>
  inline Record* ProxyRecord<long>   ( const long* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const unsigned int*>".

template<>
  inline string  ProxyTypeName<unsigned int> ( const unsigned int* object ) { return "<PointerSlotAdapter<unsigned int>>"; }

template<>
  inline string  ProxyString<unsigned int>   ( const unsigned int* object )
                                             {
                                               static ostringstream os;
                                               os.str ( "" );
                                               os << *object;
                                               return os.str();
                                             }

template<>
  inline Record* ProxyRecord<unsigned int>   ( const unsigned int* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const unsigned long*>".

template<>
  inline string  ProxyTypeName<unsigned long> ( const unsigned long* object ) { return "<PointerSlotAdapter<unsigned long>>"; }

template<>
  inline string  ProxyString<unsigned long>   ( const unsigned long* object )
                                              {
                                                static ostringstream os;
                                                os.str ( "" );
                                                os << *object;
                                                return os.str();
                                              }

template<>
  inline Record* ProxyRecord<unsigned long>   ( const unsigned long* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const unsigned long long*>".

template<>
  inline string  ProxyTypeName<unsigned long long> ( const unsigned long long* object ) { return "<PointerSlotAdapter<unsigned long long>>"; }

template<>
  inline string  ProxyString<unsigned long long>   ( const unsigned long long* object )
                                                   {
                                                     static ostringstream os;
                                                     os.str ( "" );
                                                     os << *object;
                                                     return os.str();
                                                   }

template<>
  inline Record* ProxyRecord<unsigned long long>   ( const unsigned long long* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const unsigned short int*>".

template<>
  inline string  ProxyTypeName<unsigned short int> ( const unsigned short int* object ) { return "<PointerSlotAdapter<unsigned long long>>"; }

template<>
  inline string  ProxyString<unsigned short int>   ( const unsigned short int* object )
                                                   {
                                                     static ostringstream os;
                                                     os.str ( "" );
                                                     os << *object;
                                                     return os.str();
                                                   }

template<>
  inline Record* ProxyRecord<unsigned short int>   ( const unsigned short int* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const float*>".

template<>
  inline string  ProxyTypeName<float> ( const float* object ) { return "<PointerSlotAdapter<float>>"; }

template<>
  inline string  ProxyString<float>   ( const float* object )
                                      {
                                        static ostringstream os;
                                        os.str ( "" );
                                        os << *object;
                                        return os.str();
                                      }

template<>
  inline Record* ProxyRecord<float>   ( const float* object ) { return NULL; }




// -------------------------------------------------------------------
// Class  :  "Proxy...<const double*>".

template<>
  inline string  ProxyTypeName<double> ( const double* object ) { return "<PointerSlotAdapter<double>>"; }

template<>
  inline string  ProxyString<double>   ( const double* object )
                                       {
                                         static ostringstream os;
                                         os.str ( "" );
                                         os << *object;
                                         return os.str();
                                       }

template<>
  inline Record* ProxyRecord<double>   ( const double* object ) { return NULL; }




} // End of Hurricane namespace.




// x-----------------------------------------------------------------x
// |             Generic Functions for Slot Managment                |
// x-----------------------------------------------------------------x

// Note 1: we are outside the Hurricane namespace.
// Note 2: thoses templates manage all types.


template<typename T>
  inline Hurricane::Record* GetRecord ( T* t )
  {
    if ( !t ) return NULL;
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((const void*)t))->_GetRecord();

    return Hurricane::PointerSlotAdapter<T>(t)._GetRecord();
  }


template<typename T>
  inline Hurricane::Record* GetRecord ( T t )
  {
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((const void*)&t))->_GetRecord();

    return Hurricane::ValueSlotAdapter<T>(t)._GetRecord();
  }


template<typename T>
  inline string  GetString ( T* t )
  {
    if ( !t ) return "NULL";
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((const void*)t))->_GetString();

    return Hurricane::PointerSlotAdapter<T>(t)._GetString();
  }


template<typename T>
  inline string  GetString ( T t )
  {
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((void*)&t))->_GetString();

    return Hurricane::ValueSlotAdapter<T>(t)._GetString();
  }


template<typename T>
  inline Hurricane::Slot*  GetSlot ( const string& name, T* t )
  {
    if ( !t ) return GetSlot ( name, "NULL pointer" );
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((const void*)t))->_GetSlot(name);

    return (new Hurricane::PointerSlotAdapter<T>(t))->_GetSlot(name);
  }


template<typename T>
  inline Hurricane::Slot*  GetSlot ( const string& name, T t )
  {
    if ( Hurricane::IsNestedSlotAdapter<T>::True )
      return ((const Hurricane::SlotAdapter*)((const void*)&t))->_GetSlot(name);

    return (new Hurricane::ValueSlotAdapter<T>(t))->_GetSlot(name);
  }


# define  PointerIOStreamSupport(Type)                                                 \
  inline ostream& operator<< ( ostream& stream, const Type* t )                        \
  {                                                                                    \
    if ( !t ) return stream << "NULL";                                                 \
    if ( Hurricane::IsNestedSlotAdapter<Type>::True )                                  \
      return stream << "&" << ((const Hurricane::SlotAdapter*)((const void*)t))->_GetString(); \
                                                                                       \
    return stream << "&" << Hurricane::PointerSlotAdapter<const Type>(t)._GetString(); \
  }


# define  ValueIOStreamSupport(Type)                                                      \
  inline ostream& operator<< ( ostream& stream, const Type& t )                           \
  {                                                                                       \
    if ( Hurricane::IsNestedSlotAdapter<Type>::True )                                     \
      return stream << ((const Hurricane::SlotAdapter*)((const void*)&t))->_GetString();  \
                                                                                          \
    return stream << Hurricane::ValueSlotAdapter<const Type>(t)._GetString();             \
  }



# include  "Record.h"
# include  "Slot.h"


namespace Hurricane {


  inline Slot* SlotAdapter::_GetSlot ( const string& name ) const
  {
    return new ValueSlot(name,this);
  };


  inline Slot* NestedSlotAdapter::_GetSlot ( const string& name ) const
  {
    return new PointerSlot(name,this);
  };




// Re-Entering Hurricane namespace, but now we can make use of
// GetString(), GetRecord() & GetSlot() for containers templates.


// -------------------------------------------------------------------
// Class  :  "PointerSlotAdapter<const vector<Element>>".

template<typename Element>
  class PointerSlotAdapter<const vector<Element> > : public SlotAdapter {
      
    // Attributes.
    protected:
      const vector<Element>* _v;
  
    // Constructor.
    public:
      PointerSlotAdapter ( const vector<Element>* v ) : SlotAdapter(), _v(v) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName     () const { return "<PointerSlotAdapter<const vector<Element>>"; };
      virtual string  _GetString       () const;
      virtual Record* _GetRecord       () const;
  };


template<typename Element>
  inline string  PointerSlotAdapter<const vector<Element> >::_GetString () const
  {
    string name = "vector<Element>:";
    return name + GetString(_v->size());
  }


template<typename Element>
  inline Record* PointerSlotAdapter<const vector<Element> >::_GetRecord () const
  {
    Record* record = NULL;
    if ( !_v->empty() ) {
      record = new Record ( "vector<Element>" );
      unsigned n = 1;
      typename vector<Element>::const_iterator iterator = _v->begin();
      while ( iterator != _v->end() ) {
        record->Add ( GetSlot(GetString(n++), *iterator) );
        ++iterator;
      }
    }
    return record;
  }




// -------------------------------------------------------------------
// Class  :  "PointerSlotAdapter<const list<Element>>".

template<typename Element>
  class PointerSlotAdapter<const list<Element> > : public SlotAdapter {
      
    // Attributes.
    protected:
      const list<Element>* _l;
  
    // Constructor.
    public:
      PointerSlotAdapter ( const list<Element>* l ) : SlotAdapter(), _l(l) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName     () const { return "<PointerSlotAdapter<const list<Element>>"; };
      virtual string  _GetString       () const;
      virtual Record* _GetRecord       () const;
  };


template<typename Element>
  inline string  PointerSlotAdapter<const list<Element> >::_GetString () const
  {
    string name = "list<Element>:";
    return name + GetString(_l->size());
  }


template<typename Element>
  inline Record* PointerSlotAdapter<const list<Element> >::_GetRecord () const
  {
    Record* record = NULL;
    if ( !_l->empty() ) {
      record = new Record ( "list<Element>" );
      unsigned n = 1;
      typename list<Element>::const_iterator iterator = _l->begin();
      while ( iterator != _l->end() ) {
        record->Add ( GetSlot(GetString(n++), *iterator) );
        ++iterator;
      }
    }
    return record;
  }




// -------------------------------------------------------------------
// Class  :  "PointerSlotAdapter<const map<Element>>.

template<typename Key, typename Element, typename Compare>
  class PointerSlotAdapter<const map<Key,Element,Compare> > : public SlotAdapter {
      
    // Attributes.
    protected:
      const map<Key,Element,Compare>* _m;
  
    // Constructor.
    public:
      PointerSlotAdapter ( const map<Key,Element,Compare>* m ) : _m(m) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName     () const { return "<PointerSlotAdapter<const map<Element>>"; };
      virtual string  _GetString       () const;
      virtual Record* _GetRecord       () const;
  };


template<typename Key, typename Element, typename Compare>
  inline string  PointerSlotAdapter<const map<Key,Element,Compare> >::_GetString () const
  {
    string name = "map<Element>:";
    return name + GetString(_m->size());
  }


template<typename Key, typename Element, typename Compare>
  inline Record* PointerSlotAdapter<const map <Key,Element,Compare> >::_GetRecord () const
  {
    Record* record = NULL;
    if ( !_m->empty() ) {
      record = new Record ( "map<Element>" );
      typename map<Key,Element,Compare>::const_iterator iterator = _m->begin();
      while ( iterator != _m->end() ) {
        record->Add ( GetSlot(GetString(iterator->first), iterator->second) );
        ++iterator;
      }
    }
    return record;
  }




// -------------------------------------------------------------------
// Class  :  "SlotAdapterSet".

template<typename Element, typename Compare>
  class PointerSlotAdapter<const set<Element,Compare> > : public SlotAdapter {
      
    // Attributes.
    protected:
      const set<Element,Compare>* _s;
  
    // Constructor.
    public:
      PointerSlotAdapter ( const set<Element,Compare>* m ) : _s(m) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName     () const { return "<PointerSlotAdapter<const set<Element>>"; };
      virtual string  _GetString       () const;
      virtual Record* _GetRecord       () const;
  };


template<typename Element, typename Compare>
  inline string  PointerSlotAdapter<const set<Element,Compare> >::_GetString () const
  {
    string name = "set<Element>:";
    return name + GetString(_s->size());
  }


template<typename Element, typename Compare>
  inline Record* PointerSlotAdapter<const set<Element,Compare> >::_GetRecord () const
  {
    Record* record = NULL;
    if ( !_s->empty() ) {
      record = new Record ( "set<Element>" );
      unsigned n = 1;
      typename set<Element,Compare>::const_iterator iterator = _s->begin();
      while ( iterator != _s->end() ) {
        record->Add ( GetSlot(GetString(n++), *iterator) );
        ++iterator;
      }
    }
    return record;
  }




// -------------------------------------------------------------------
// Class  :  "SlotAdapterMultiSet".

template<typename Element, typename Compare>
  class PointerSlotAdapter<const multiset<Element,Compare> > : public SlotAdapter {
      
    // Attributes.
    protected:
      const multiset<Element,Compare>* _s;
  
    // Constructor.
    public:
      PointerSlotAdapter ( const multiset<Element,Compare>* m ) : _s(m) {};
  
    // Destructor.
    public:
      virtual ~PointerSlotAdapter () {};

    // Slot Management.
    public:
      virtual string  _GetTypeName     () const { return "<PointerSlotAdapter<const multiset<Element>>"; };
      virtual string  _GetString       () const;
      virtual Record* _GetRecord       () const;
  };


template<typename Element, typename Compare>
  inline string  PointerSlotAdapter<const multiset<Element,Compare> >::_GetString () const
  {
    string name = "multiset<Element>:";
    return name + GetString(_s->size());
  }


template<typename Element, typename Compare>
  inline Record* PointerSlotAdapter<const multiset<Element,Compare> >::_GetRecord () const
  {
    Record* record = NULL;
    if ( !_s->empty() ) {
      record = new Record ( "multiset<Element>" );
      unsigned n = 1;
      typename multiset<Element,Compare>::const_iterator iterator = _s->begin();
      while ( iterator != _s->end() ) {
        record->Add ( GetSlot(GetString(n++), *iterator) );
        ++iterator;
      }
    }
    return record;
  }




}  // End of Hurricane namespace.




# endif
