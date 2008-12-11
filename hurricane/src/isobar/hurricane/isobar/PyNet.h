// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./PyNet.h"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





#ifndef __PYNET__
#define __PYNET__


#include "hurricane/isobar/PyEntity.h"

#include "hurricane/Net.h"


namespace  Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNet".

  typedef struct {
      PyEntity  _baseObject;
  } PyNet;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeNet;
  extern  PyMethodDef   PyNet_Methods[];

  extern  PyObject* PyNet_Link(Hurricane::Net* object);
  extern  void      NetLoadConstants(PyObject* dictionnary);
  extern  void      PyNet_LinkPyType();
  extern  void      PyNet_Constructor();



#define IsPyNet(v) ((v)->ob_type == &PyTypeNet)
#define PYNET(v)   ((PyNet*)(v))
#define PYNET_O(v) (dynamic_cast<Net*>(PYNET(v)->_baseObject._object))


}  // End of extern "C".




}  // End of Isobar namespace.
 



#endif
