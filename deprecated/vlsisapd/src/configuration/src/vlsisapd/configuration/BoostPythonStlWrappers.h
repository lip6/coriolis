
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2011-2011, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :                                                  |
// |              "./vlsisapd/configuration/BoostPythonStlWrapper.h" |
// +-----------------------------------------------------------------+


#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace Cfg {


// Enumerated type to_python converter.
  template<typename EnumType>
  struct EnumToInt {
      static PyObject* convert ( EnumType& type )
      { return boost::python::incref ( boost::python::object((int)type).ptr() ); }
  };


// STL vector<> wrapper for boost::python.
  template<typename Vector>
  class VectorWrapper {
    public:
      static boost::python::scope  wrap ( const std::string& );
  };


  template<typename Vector>
  boost::python::scope VectorWrapper<Vector>::wrap ( const std::string& className )
  {
    return boost::python::class_< Vector >(className.c_str())
      .def("size"    ,&Vector::size )
      .def("empty"   ,&Vector::empty)
      .def("__iter__", boost::python::iterator<Vector>())
      ;
  }


// STL pair<> wrapper for boost::python.
  template<typename Pair>
  class PairWrapper {
    public:
      typedef typename Pair::first_type   Key;
      typedef typename Pair::second_type  Value;
    public:
      static boost::python::scope  wrap ( const std::string& );
      static boost::python::scope  wrap ( const std::string&
                                        , const std::string& firstAlias
                                        , const std::string& secondAlias
                                        );
  };


  template<typename Pair>
  boost::python::scope PairWrapper<Pair>::wrap ( const std::string& className )
  {
    return boost::python::class_< Pair >(className.c_str(),boost::python::init<const Key&,const Value&>())
      .def_readonly ("first" , &Pair::first )
      .def_readwrite("second", &Pair::second)
      ;
  }


  template<typename Pair>
  boost::python::scope PairWrapper<Pair>::wrap ( const std::string& className
                                               , const std::string& firstAlias
                                               , const std::string& secondAlias
                                               )
  {
    return boost::python::class_< Pair >(className.c_str(),boost::python::init<const Key&,const Value&>())
      .def_readonly ("first"            , &Pair::first )
      .def_readwrite("second"           , &Pair::second)
      .def_readonly (firstAlias .c_str(), &Pair::first )
      .def_readwrite(secondAlias.c_str(), &Pair::second)
      ;
  }
  

// STL map<> wrapper for boost::python.
  template<typename Map>
  class MapWrapper {
    public:
      typedef typename Map::key_type    Key;
      typedef typename Map::mapped_type Value;
    public:
      static Value&                get    ( Map&, const Key& );
      static void                  set    ( Map&, const Key&, const Value& );
      static void                  del    ( Map&, const Key& );
      static bool                  in     ( Map&, const Key& );
      static boost::python::list   keys   ( Map& );
      static boost::python::list   values ( Map& );
      static boost::python::list   items  ( Map& );
      static boost::python::scope  wrap   ( const std::string& );
  };


  inline void  KeyError () { PyErr_SetString(PyExc_KeyError, "Key not found"); }

  
  template<typename Map>
  typename MapWrapper<Map>::Value& MapWrapper<Map>::get ( Map& m, const typename MapWrapper<Map>::Key& k )
  {
    static Value notFound;
    if( m.find(k) != m.end() ) return m[k];
    KeyError ();
    return notFound;
  }


  template<typename Map>
  void  MapWrapper<Map>::set ( Map& m, const MapWrapper<Map>::Key& k, const typename MapWrapper<Map>::Value& v )
  { m[k] = v; }


  template<typename Map>
  void  MapWrapper<Map>::del ( Map& m, const typename MapWrapper<Map>::Key& k)
  {
    if( m.find(k) != m.end() ) m.erase(k);
    else KeyError ();
  }


  template<typename Map>
  bool  MapWrapper<Map>::in ( Map& m, const typename MapWrapper<Map>::Key& k )
  { return m.find(k) != m.end(); }


  template<typename Map>
  boost::python::list  MapWrapper<Map>::keys ( Map& m )
  {
    boost::python::list l;
    for(typename Map::iterator it = m.begin() ; it != m.end() ; ++it )
      l.append ( it->first );
    return l;
  }


  template<typename Map>
  boost::python::list  MapWrapper<Map>::values ( Map& m)
  {
    boost::python::list l;
    for( typename Map::iterator it=m.begin(); it != m.end() ; ++it )
      l.append ( it->second );
    return l;
  }


  template<typename Map>
  boost::python::list  MapWrapper<Map>::items ( Map& m ) {
    boost::python::list l;
    for( typename Map::iterator it=m.begin(); it!=m.end(); ++it )
      l.append( make_tuple(it->first,it->second) );
    return l;
  }


  template<typename Map>
  boost::python::scope MapWrapper<Map>::wrap ( const std::string& className )
  {
    return boost::python::class_< Map >(className.c_str())
      .def("__len__"      , &Map::size )
      .def("clear"        , &Map::clear)
      .def("__getitem__"  , &get       , boost::python::return_value_policy<boost::python::reference_existing_object>())
      .def("__setitem__"  , &set       , boost::python::return_value_policy<boost::python::reference_existing_object>())
      .def("__delitem__"  , &del       )
      .def("__contains__" , &in        )
      .def("has_key"      , &in        )
      .def("keys"         , &keys      )
      .def("values"       , &values    )
      .def("items"        , &items     )
      ;
  }


} // End of Cfg namespace.
