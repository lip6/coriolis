# ifndef  __VECTORTOLIST__
# define  __VECTORTOLIST__

#include "hurricane/isobar/PyPoint.h"

namespace  Isobar {

  using namespace Hurricane;
  using Hurricane::Point;


  template< typename CppType >
  inline bool ListToVector ( PyObject* list, PyTypeObject* itemType, std::vector<CppType>& v )
  {
    if (not PyList_Check(list)) return false;

    int length = PyList_Size( list );
    for ( int i=0 ; i<length ; ++i ) {
      PyObject* item = PyList_GetItem( list, i );
      if ((item)->ob_type != itemType) {
        string message =  "Polygon: Item at position " + getString(i) + "has wrong type.";
        PyErr_SetString( ConstructorError, message.c_str() );
        return false;
      }
      v.push_back( *PYPOINT_O(item) );
    }
    return true;
  }


  inline PyObject* VectorToList ( const std::vector<Point>& v )
  {
    PyObject* pyList = PyList_New( v.size() );

    for ( size_t i=0 ; i<v.size() ; ++i ) {
      PyPoint* pyPoint = PyObject_NEW( PyPoint, &PyTypePoint );
      if (pyPoint == NULL) { return NULL; }

      HTRY
        pyPoint->_object = new Point ( v[i] );
      HCATCH
      PyList_SetItem( pyList, i, (PyObject*)pyPoint );
    }

    return pyList;
  }
}

# endif //__VECTORTOLIST__
