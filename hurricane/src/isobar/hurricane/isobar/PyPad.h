#ifndef __PYPAD_H
#define __PYPAD_H


#include "hurricane/isobar/PyComponent.h"

#include "hurricane/Pad.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPad".

  typedef struct {
      PyComponent  _baseObject;
  } PyPad;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypePad;
  extern  PyMethodDef   PyPad_Methods[];

  extern  PyObject* PyPad_create     ( PyObject* module, PyObject* args );
  extern  PyObject* PyPad_Link       ( Hurricane::Pad* object );
  extern  void      PyPad_LinkPyType ();


#define IsPyPad(v)    ( (v)->ob_type == &PyTypePad )
#define PYPAD(v)      ( (PyPad*)(v) )
#define PYPAD_O(v)    ( PYPAD(v)->_baseObject._baseObject._object )


}  // End of extern "C".




}  // End of Isobar namespace.
 



#endif /* __PYPAD_H */
