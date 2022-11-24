#include <map>

#include <boost/python.hpp>

namespace SPICE {
// since we want to wrap std::map<type, Class*> with cimple pointers (no boost::shared_ptr),
//   we cannot use map_indexing_suite which has not the right return_value_policy.
// std::maps with no points value are wrapped using map_indexing_suite technique.
//
// This templated map_item struct is intended to be used for 'manual' wrapping:
template<class Key, class Val>
struct map_item {
    typedef std::map<Key,Val> Map;

    static Val get(Map & self, const Key idx) {
      if (self.find(idx) == self.end()) {
          PyErr_SetString(PyExc_KeyError,"Map key not found");
          throw_error_already_set();
      }
      return self[idx];
    }

    static void set(Map& self, const Key idx, const Val val) { self[idx]=val; }
    static void del(Map& self, const Key n)                  { self.erase(n); }
    static bool in (Map const& self, const Key n)            { return self.find(n) != self.end(); }

    static list keys(Map const& self) {
        list t;
        for(typename Map::const_iterator it = self.begin() ; it!=self.end() ; ++it)
            t.append(it->first);
        return t;
    }

    static list values(Map const& self) {
        list t;
        for(typename Map::const_iterator it=self.begin(); it!=self.end(); ++it)
            t.append(it->second);
        return t;
    }

    static list items(Map const& self) {
        list t;
        for(typename Map::const_iterator it=self.begin(); it!=self.end(); ++it)
            t.append( make_tuple(it->first, it->second) );
        return t;
    }
};

#define STL_MAP_WRAPPING_PTR(KEY_TYPE, VALUE_TYPE, PYTHON_TYPE_NAME)               \
    class_<std::pair<const KEY_TYPE, VALUE_TYPE> >((std::string(PYTHON_TYPE_NAME)+std::string("DATA")).c_str()) \
        .def_readonly ("key"  , &std::pair<const KEY_TYPE, VALUE_TYPE>::first ) \
        .def_readwrite("value", &std::pair<const KEY_TYPE, VALUE_TYPE>::second) \
    ;                                                                          \
    class_<std::map<KEY_TYPE, VALUE_TYPE> >(PYTHON_TYPE_NAME)                  \
        .def("__len__"     , &std::map<KEY_TYPE, VALUE_TYPE>::size)            \
        .def("__iter__"    , boost::python::iterator<std::map<KEY_TYPE, VALUE_TYPE>, return_internal_reference<> >()) \
        .def("__getitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().get, return_internal_reference<>()) \
        .def("__setitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().set   )         \
        .def("__delitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().del   )         \
        .def("__contains__", &map_item<KEY_TYPE, VALUE_TYPE>().in    )         \
        .def("clear"       , &std::map<KEY_TYPE, VALUE_TYPE>::clear  )         \
        .def("has_key"     , &map_item<KEY_TYPE, VALUE_TYPE>().in    )         \
        .def("keys"        , &map_item<KEY_TYPE, VALUE_TYPE>().keys  )         \
        .def("values"      , &map_item<KEY_TYPE, VALUE_TYPE>().values)         \
        .def("items"       , &map_item<KEY_TYPE, VALUE_TYPE>().items )         \
    ;

#define STL_MAP_WRAPPING(KEY_TYPE, VALUE_TYPE, PYTHON_TYPE_NAME)               \
    class_<std::pair<const KEY_TYPE, VALUE_TYPE> >((std::string(PYTHON_TYPE_NAME)+std::string("DATA")).c_str()) \
        .def_readonly ("key"  , &std::pair<const KEY_TYPE, VALUE_TYPE>::first ) \
        .def_readwrite("value", &std::pair<const KEY_TYPE, VALUE_TYPE>::second) \
    ;                                                                          \
    class_<std::map<KEY_TYPE, VALUE_TYPE> >(PYTHON_TYPE_NAME)                  \
        .def("__len__"     , &std::map<KEY_TYPE, VALUE_TYPE>::size)            \
        .def("__iter__"    , boost::python::iterator<std::map<KEY_TYPE, VALUE_TYPE>, return_internal_reference<> >()) \
        .def("__getitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().get   )         \
        .def("__setitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().set   )         \
        .def("__delitem__" , &map_item<KEY_TYPE, VALUE_TYPE>().del   )         \
        .def("__contains__", &map_item<KEY_TYPE, VALUE_TYPE>().in    )         \
        .def("clear"       , &std::map<KEY_TYPE, VALUE_TYPE>::clear  )         \
        .def("has_key"     , &map_item<KEY_TYPE, VALUE_TYPE>().in    )         \
        .def("keys"        , &map_item<KEY_TYPE, VALUE_TYPE>().keys  )         \
        .def("values"      , &map_item<KEY_TYPE, VALUE_TYPE>().values)         \
        .def("items"       , &map_item<KEY_TYPE, VALUE_TYPE>().items )         \
    ;

} // namespace
