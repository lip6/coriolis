// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyParameter.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/PyCapacitorParameter.h"
#include "hurricane/analog/PyChoiceParameter.h"
#include "hurricane/analog/PyFormFactorParameter.h"
#include "hurricane/analog/PyMCheckBoxParameter.h"
#include "hurricane/analog/PySpinBoxParameter.h"
#include "hurricane/analog/PyStepParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Parameter,parameter,function)


// +=================================================================+
// |             "PyParameter" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |              "PyParameter" Attribute Methods                |
  // +-------------------------------------------------------------+


  DirectGetStringAttribute(PyParameter_getName ,getName ,PyParameter,Parameter)
  DirectGetIntAttribute   (PyParameter_getIndex,getIndex,PyParameter,Parameter)
  DirectSetIntAttribute   (PyParameter_setIndex,setIndex,PyParameter,Parameter)


  PyMethodDef PyParameter_Methods[] =
    { { "getName"             , (PyCFunction)PyParameter_getName             , METH_NOARGS
                              , "Returns the parameter's name." }
    , { "getIndex"            , (PyCFunction)PyParameter_getIndex            , METH_NOARGS
                              , "Self explanatory." }
    , { "setIndex"            , (PyCFunction)PyParameter_setIndex            , METH_VARARGS
                              , "Self explanatory." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyParameter Type Methods.


  PythonOnlyDeleteMethod(Parameter)
  PyTypeObjectLinkPyType(Parameter)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyParameter" Shared Library Code Part               |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(Parameter)

  PyTypeRootObjectDefinitions(Parameter)


  extern  void  PyParameter_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeParameter.tp_dict,Parameter::STEP  ,"STEP"  );
    LoadObjectConstant(PyTypeParameter.tp_dict,Parameter::CHOICE,"CHOICE");
    LoadObjectConstant(PyTypeParameter.tp_dict,Parameter::SPIN  ,"SPIN"  );
    LoadObjectConstant(PyTypeParameter.tp_dict,Parameter::MCHECK,"MCHECK");
  }


  extern PyObject* PyParameter_LinkDerived ( Parameter* object )
  {
    if (object == NULL) Py_RETURN_NONE;

    CapacitorParameter* capacitorParameter = dynamic_cast<CapacitorParameter*>(object);
    if (capacitorParameter) return PyCapacitorParameter_Link(capacitorParameter);

    ChoiceParameter* choiceParameter = dynamic_cast<ChoiceParameter*>(object);
    if (choiceParameter) return PyChoiceParameter_Link(choiceParameter);

    FormFactorParameter* ffParameter = dynamic_cast<FormFactorParameter*>(object);
    if (ffParameter) return PyFormFactorParameter_Link(ffParameter);

    MCheckBoxParameter* mCheckBoxParameter = dynamic_cast<MCheckBoxParameter*>(object);
    if (mCheckBoxParameter) return PyMCheckBoxParameter_Link(mCheckBoxParameter);

    SpinBoxParameter* spinBoxParameter = dynamic_cast<SpinBoxParameter*>(object);
    if (spinBoxParameter) return PySpinBoxParameter_Link(spinBoxParameter);

    StepParameter* stepParameter = dynamic_cast<StepParameter*>(object);
    if (stepParameter) return PyStepParameter_Link(stepParameter);

    Py_RETURN_NONE;
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace. 
