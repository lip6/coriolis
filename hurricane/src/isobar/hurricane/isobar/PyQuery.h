
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
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
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyQuery.h"                  |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_PYQUERY_H
#define  HURRICANE_PYQUERY_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Query.h"

namespace  Isobar {

  using namespace Hurricane;


#if WORK_IN_PROGRESS
  template< typename CppObject, int TableSize >
  class MethodDefTable {
    public:
                                   MethodDefTable ();
                   PyMethodDef*    getMethods     ();
             const PyMethodDef&    operator[]     ( size_t );
                   MethodDefTable& addMethod      ( const char* fname, PyCFunction, int flags, const char* fdoc );
                   MethodDefTable& operator()     ( const char* fname, PyCFunction, int flags, const char* fdoc );
      template< typename R, typename... Ps >
                   MethodDefTable& addMethod      ( const char* fname, PyCFunction, int flags, const char* fdoc, R(CppObject::* mfp)(Ps...) );
      template< typename R, typename... Ps >
                   MethodDefTable& operator()     ( const char* fname, PyCFunction, int flags, const char* fdoc, R(CppObject::* mfp)(Ps...) );
      inline       size_t          size           () const;
    private:
      size_t       _size;
      PyMethodDef  _pyMethods [TableSize];
      void*        _cppMethods[TableSize];
  };


  template< typename CppObject, int TableSize >
  size_t  MethodDefTable<CppObject,TableSize>::size () const
  { return _size; }


  template< typename CppObject, int TableSize >
  MethodDefTable<CppObject,TableSize>::MethodDefTable ()
    : _size     (0)
  //, _pyMethods(new PyMethodDef[TableSize+1])
  {
    for ( size_t i=0 ; i<TableSize+1 ; ++i ) {
      _pyMethods [i].ml_name  = NULL;
      _pyMethods [i].ml_meth  = NULL;
      _pyMethods [i].ml_flags = 0;
      _pyMethods [i].ml_doc   = NULL;
      _cppMethods[i]          = NULL;
    }
  }


  template< typename CppObject, int TableSize >
  PyMethodDef* MethodDefTable<CppObject,TableSize>::getMethods ()
  { return _pyMethods; }


  template< typename CppObject, int TableSize >
  const PyMethodDef& MethodDefTable<CppObject,TableSize>::operator[] ( size_t i )
  { return _pyMethods[i]; }


  template< typename CppObject, int TableSize >
  MethodDefTable<CppObject,TableSize>& MethodDefTable<CppObject,TableSize>::addMethod ( const char* fname, PyCFunction fp, int flags, const char* fdoc )
  {
    _pyMethods [_size].ml_name  = fname;
    _pyMethods [_size].ml_meth  = fp;
    _pyMethods [_size].ml_flags = flags;
    _pyMethods [_size].ml_doc   = fdoc;
    _cppMethods[_size]          = NULL;
    ++_size;

    return *this;
  }


  template< typename CppObject, int TableSize >
  template< typename R, typename... Ps >
  MethodDefTable<CppObject,TableSize>& MethodDefTable<CppObject,TableSize>::addMethod
  ( const char* fname, PyCFunction fp, int flags, const char* fdoc, R(CppObject::* mfp)(Ps...) )
  {
    _pyMethods[_size].ml_name   = fname;
    _pyMethods[_size].ml_meth   = fp;
    _pyMethods[_size].ml_flags  = flags;
    _pyMethods[_size].ml_doc    = fdoc;
    _cppMethods[_size]          = (void*)mfp;
    ++_size;

    return *this;
  }


  template< typename CppObject, int TableSize >
  MethodDefTable<CppObject,TableSize>& MethodDefTable<CppObject,TableSize>::operator() ( const char* fname, PyCFunction fp, int flags, const char* fdoc )
  { return addMethod( fname, fp, flags, fdoc ); }


  template< typename CppObject, int TableSize >
  template< typename R, typename... Ps >
  MethodDefTable<CppObject,TableSize>& MethodDefTable<CppObject,TableSize>::operator()
    ( const char* fname, PyCFunction fp, int flags, const char* fdoc, R(CppObject::* mfp)(Ps...) )
  { return addMethod( fname, fp, flags, fdoc, mfp ); }
#endif


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyQuery".

  typedef struct {
      PyObject_HEAD
      Query* _object;
  } PyQuery;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeQuery;
  extern  PyMethodDef   PyQuery_Methods[];

  extern  PyObject* PyQuery_Link           ( Hurricane::Query* object );
  extern  void      PyQuery_LinkPyType     ();
  extern  void      PyQuery_postModuleInit ();

#define IsPyQuery(v)    ( (v)->ob_type == &PyTypeQuery )
#define PYQUERY(v)      ( (PyQuery*)(v) )
#define PYQUERY_O(v)    ( PYQUERY(v)->_object )


}  // extern "C".

}  // Isobar namespace.
 
#endif
