
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyNetExternalComponents.h"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





# ifndef  __PY_NET_EXTERNAL_COMPONENTS__
#   define  __PY_NET_EXTERNAL_COMPONENTS__


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/NetExternalComponents.h"


namespace  Isobar {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyNetExternalComponents".

  typedef struct {
      PyObject_HEAD
  } PyNetExternalComponents;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeNetExternalComponents;
  extern PyMethodDef   PyNetExternalComponents_Methods[];

  extern void  PyNetExternalComponents_LinkPyType  ();


#define IsPyNetExternalComponents(v)   ( (v)->ob_type == &PyTypeNetExternalComponents )
#define PYNETEXTERNALCOMPONENTS(v)     ( (PyNetExternalComponents*)(v) )
#define PYNETEXTERNALCOMPONENTS_O(v)   ( PY_NET_EXTERNAL_COMPONENTS(v)->_object )


}  // End of extern "C".



}  // End of Isobar namespace.
 



# endif
