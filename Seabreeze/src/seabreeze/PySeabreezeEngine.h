#ifndef PY_SEABREEZE_ENGINE_H
#define PY_SEABREEZE_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "seabreeze/SeabreezeEngine.h"


namespace  Seabreeze {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySeabreezeEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PySeabreezeEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PySeabreeze.ccp".

  extern  PyTypeObject  PyTypeSeabreezeEngine;
  extern  PyMethodDef   PySeabreezeEngine_Methods[];

  extern  PyObject* PySeabreezeEngine_Link           ( Seabreeze::SeabreezeEngine* );
  extern  void      PySeabreezeEngine_LinkPyType     ();
  extern  void      PySeabreezeEngine_postModuleInit ();


#define IsPySeabreezeEngine(v)  ( (v)->ob_type == &PyTypeSeabreezeEngine )
#define PYSEABREEZEENGINE(v)    ( (PySeabreezeEngine*)(v) )
#define PYSEABREEZEENGINE_O(v)  ( PYSEABREEZEENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Tutorial namespace.

#endif  // PY_SEABREEZE_ENGINE_H
