
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
// ===================================================================
//
// $Id: PyNet.cpp,v 1.27 2008/02/07 17:09:41 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyNet.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyNet.h"
# include  "PyName.h"
# include  "PyCell.h" 
# include  "PyPoint.h" 
# include  "PyPlugLocator.h" 
# include  "PySegmentLocator.h" 
# include  "PyComponentLocator.h" 
# include  "PyPinLocator.h" 

# include  "NetExternalComponents.h"
USING_NAMESPACE_HURRICANE


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT           _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Net,net,function)

# define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );


// x=================================================================x
// |                 "PyNet" Python Module Code Part                 |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNet" Local Functions                     |
  // x-------------------------------------------------------------x

  
  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsType ()"
  
  static Net::Type  PyInt_AsType ( PyObject* object )
  {
    switch ( PyInt_AsLong(object) ) {
      case Net::Type::UNDEFINED : return ( Net::Type(Net::Type::UNDEFINED) );
      case Net::Type::LOGICAL   : return ( Net::Type(Net::Type::LOGICAL) );
      case Net::Type::CLOCK     : return ( Net::Type(Net::Type::CLOCK) );
      case Net::Type::POWER     : return ( Net::Type(Net::Type::POWER) );
      case Net::Type::GROUND    : return ( Net::Type(Net::Type::GROUND) );
    }

    return ( Net::Type(Net::Type::UNDEFINED) );
  }



  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsDirection ()"

  static Net::Direction  PyInt_AsDirection ( PyObject* object )
  {
    switch ( PyInt_AsLong(object) ) {
      case Net::Direction::UNDEFINED      : return ( Net::Direction(Net::Direction::UNDEFINED) );
      case Net::Direction::IN             : return ( Net::Direction(Net::Direction::IN) );
      case Net::Direction::OUT            : return ( Net::Direction(Net::Direction::OUT) );
      case Net::Direction::INOUT          : return ( Net::Direction(Net::Direction::INOUT) );
      case Net::Direction::TRISTATE       : return ( Net::Direction(Net::Direction::TRISTATE) );
    }

    return ( Net::Direction(Net::Direction::UNDEFINED) );
  }




  // x-------------------------------------------------------------x
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x

  extern void  NetLoadConstants ( PyObject* dictionnary )
  {
    PyObject* constant;

    LOAD_CONSTANT ( Net::Type::UNDEFINED        , "TypeUNDEFINED" )
    LOAD_CONSTANT ( Net::Type::LOGICAL          , "TypeLOGICAL" )
    LOAD_CONSTANT ( Net::Type::CLOCK            , "TypeCLOCK" )
    LOAD_CONSTANT ( Net::Type::POWER            , "TypePOWER" )
    LOAD_CONSTANT ( Net::Type::GROUND           , "TypeGROUND" )
    LOAD_CONSTANT ( Net::Direction::UNDEFINED   , "DirectionUNDEFINED" )
    LOAD_CONSTANT ( Net::Direction::IN          , "DirectionIN" )
    LOAD_CONSTANT ( Net::Direction::OUT         , "DirectionOUT" )
    LOAD_CONSTANT ( Net::Direction::INOUT       , "DirectionINOUT" )
    LOAD_CONSTANT ( Net::Direction::TRISTATE    , "DirectionTRISTATE" )
  }



  // x-------------------------------------------------------------x
  // |                "PyNet" Attribute Methods                    |
  // x-------------------------------------------------------------x



  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyNet_GetX,GetX,PyNet,Net)
  DirectGetLongAttribute(PyNet_GetY,GetY,PyNet,Net)  
  
  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyNet_IsGlobal  ,IsGlobal  ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsExternal,IsExternal,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsLogical ,IsLogical ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsClock   ,IsClock   ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsGround  ,IsGround  ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsPower   ,IsPower   ,PyNet,Net)
  DirectGetBoolAttribute(PyNet_IsSupply  ,IsSupply  ,PyNet,Net)

  GetBoundStateAttribute(PyNet_IsPyBound            ,PyNet,Net)

  // Standart Delete (Attribute).
  DBoDeleteAttribute(PyNet_Delete,PyNet)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetName ()"

  static PyObject* PyNet_GetName ( PyNet *self )
  {
    trace << "PyNet_GetName ()" << endl;
    
    METHOD_HEAD ( "Net.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }
  
    trace_in ();
    trace << "new PyName [" << hex << pyName << "]" << endl;
    trace_out ();

    HTRY

    pyName->_object = new Name ( net->GetName() );

    HCATCH
    
    return ( (PyObject*)pyName ); 
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetType ()"

  static PyObject* PyNet_GetType ( PyNet *self )
  {
    trace << "PyNet_GetType ()" << endl;
    
    METHOD_HEAD ( "Net.GetType()" )

    return ( (PyObject*)Py_BuildValue("i",(long)net->GetType().GetCode()) );
  }

  

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetDirection ()"

  static PyObject* PyNet_GetDirection ( PyNet *self )
  {
    trace << "PyNet_GetDirection ()" << endl;
   
    METHOD_HEAD ( "Net.GetDirection()" )

    return ( (PyObject*)Py_BuildValue("i",(long)net->GetDirection().GetCode()) );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetPlugsLocator ()"

  static PyObject* PyNet_GetPlugsLocator ( PyNet *self )
  {
    trace << "PyNet_GetPlugsLocator ()" << endl;

    METHOD_HEAD ( "Net.GetPlugsLocator()" )

    Plugs plugs = net->GetPlugs ();

    PyPlugLocator* pyPlugLocator = PyObject_NEW ( PyPlugLocator, &PyTypePlugLocator );
    if (pyPlugLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyPlugLocator [" << hex << pyPlugLocator << "]" << endl;
    trace_out ();

    HTRY

    pyPlugLocator->_object = plugs.GetLocator ();

    HCATCH

    return ( (PyObject*)pyPlugLocator );
  }

    
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetSegmentsLocator ()"

  static PyObject* PyNet_GetSegmentsLocator ( PyNet *self )
  {
    trace << "PyNet_GetSegmentsLocator ()" << endl;

    METHOD_HEAD ( "Net.GetSegmentsLocator()" )

    Segments segments = net->GetSegments ();

    PySegmentLocator* pySegmentLocator = PyObject_NEW ( PySegmentLocator, &PyTypeSegmentLocator );
    if (pySegmentLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PySegmentLocator [" << hex << pySegmentLocator << "]" << endl;
    trace_out ();

    HTRY

    pySegmentLocator->_object = segments.GetLocator ();

    HCATCH

    return ( (PyObject*)pySegmentLocator );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetPinsLocator ()"

  static PyObject* PyNet_GetPinsLocator ( PyNet *self )
  {
    trace << "PyNet_GetPinsLocator ()" << endl;

    METHOD_HEAD ( "Net.GetPinsLocator()" )

    Pins pins = net->GetPins ();

    PyPinLocator* pyPinLocator = PyObject_NEW ( PyPinLocator, &PyTypePinLocator );
    if (pyPinLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyPinLocator [" << hex << pyPinLocator << "]" << endl;
    trace_out ();

    HTRY

    pyPinLocator->_object = pins.GetLocator ();

    HCATCH

    return ( (PyObject*)pyPinLocator );
  }
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_GetExternalComponentsLocator ()"

  static PyObject* PyNet_GetExternalComponentsLocator ( PyNet *self )
  {
    trace << "PyNet_GetExternalComponentsLocator ()" << endl;

    METHOD_HEAD ( "Net.GetExternalcomponentsLocator()" )

    Components externalComponents = GetExternalComponents(net);

    PyComponentLocator* pyExternalComponentsLocator = PyObject_NEW ( PyComponentLocator, &PyTypeComponentLocator );
    if (pyExternalComponentsLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyExternalComponentsLocator [" << hex << pyExternalComponentsLocator << "]" << endl;
    trace_out ();

    HTRY

    pyExternalComponentsLocator->_object = externalComponents.GetLocator ();

    HCATCH

    return ( (PyObject*)pyExternalComponentsLocator );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetName ()"

  static PyObject* PyNet_SetName ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetName()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetName()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetName", args, NET_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->SetName ( *PYNAME_O(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetGlobal ()"

  static PyObject* PyNet_SetGlobal ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetGlobal()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetGlobal()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetGlobal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->SetGlobal ( PyInt_AsLong(arg0) != 0 );

    HCATCH

    Py_RETURN_NONE;
  }


  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetExternal ()"

  static PyObject* PyNet_SetExternal ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetExternal()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetExternal()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetExternal", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->SetExternal ( PyInt_AsLong(arg0) != 0 );

    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetType ()"

  static PyObject* PyNet_SetType ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetType()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetType()" )
    
    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetType", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
    
    net->SetType ( PyInt_AsType(arg0) );

    HCATCH
    
    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetDirection ()"

  static PyObject* PyNet_SetDirection ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetDirection()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetDirection()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetDirection", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->SetDirection ( PyInt_AsDirection(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_SetPosition ()"

  static PyObject* PyNet_SetPosition ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_SetPosition()" << endl;

    HTRY

    METHOD_HEAD ( "Net.SetPosition()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.SetPosition", args, POINT_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->SetPosition ( *PYPOINT_O(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_Merge ()"

  static PyObject* PyNet_Merge ( PyNet *self, PyObject* args )
  {
    trace << "PyNet_Merge()" << endl;

    HTRY

    METHOD_HEAD ( "Net.Merge()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Net.Merge", args, NET_ARG, (PyObject**)&arg0 ) ) return ( NULL );

    net->Merge ( PYNET_O(arg0) );

    HCATCH
    
    Py_RETURN_NONE;
  }






  // ---------------------------------------------------------------
  // PyNet Attribute Method table.

  PyMethodDef PyNet_Methods[] =
    { { "GetName"              , (PyCFunction)PyNet_GetName                  , METH_NOARGS , "Returns the net name." }
    //, { "GetArity"             , (PyCFunction)PyNet_GetArity                 , METH_NOARGS , "Returns the signal arity." }
    , { "GetType"              , (PyCFunction)PyNet_GetType                  , METH_NOARGS
                               , "Returns the signal type (by default set to UNDEFINED)." }
    , { "GetDirection"         , (PyCFunction)PyNet_GetDirection             , METH_NOARGS
                               , "Returns the signal direction (by default set to UNDEFINED)." }
    //, { "GetPosition"          , (PyCFunction)PyNet_GetPosition   , METH_NOARGS
    //                           , "Returns the X,Y position of the net. This position is used for computing the location of the plugs (on slave instances calling the cell owning this net) having that net as master." }
    , { "GetX"                 , (PyCFunction)PyNet_GetX                     , METH_NOARGS , "Returns net abscissa." }
    , { "GetY"                 , (PyCFunction)PyNet_GetY                     , METH_NOARGS , "Returns net ordinate." }
    //, { "GetRubbers"           , (PyCFunction)PyNet_GetRubbers               , METH_NOARGS , "Returns the collection of net's rubbers." }
    //, { "GetComponents"        , (PyCFunction)PyNet_GetComponents            , METH_NOARGS , "Returns the collection of net's components." }
    , { "GetExternalComponents", (PyCFunction)PyNet_GetExternalComponentsLocator , METH_NOARGS , "Returns the collection of net's external components. (only for an external net)" }
    , { "GetPlugsLocator"      , (PyCFunction)PyNet_GetPlugsLocator          , METH_NOARGS , "Returns the collection of net's plugs." }
    //, { "GetContacts"          , (PyCFunction)PyNet_GetContacts              , METH_NOARGS , "Returns the collection of net's contacts." }
    , { "GetPinsLocator"        , (PyCFunction)PyNet_GetPinsLocator            , METH_NOARGS , "Returns the collection of net's pins." }
    , { "GetSegmentsLocator"    , (PyCFunction)PyNet_GetSegmentsLocator        , METH_NOARGS , "Returns the collection of net's segments." }
    //, { "GetSegments"          , (PyCFunction)PyNet_GetSegments              , METH_NOARGS , "Returns the collection of net's segments." }
    //, { "GetVerticals"         , (PyCFunction)PyNet_GetVerticals             , METH_NOARGS , "Returns the collection of net's vertical segments." }
    //, { "GetHorizontals"       , (PyCFunction)PyNet_GetHorizontals           , METH_NOARGS , "Returns the collection of net's horizontal segments." }
    //, { "GetPads"              , (PyCFunction)PyNet_GetPads                  , METH_NOARGS , "Returns the collection of net's pads." }
    //, { "GetSavePlugs"         , (PyCFunction)PyNet_GetSavePlugs             , METH_NOARGS , "Returns the collection of plugs which have this net as master." }
    //, { "GetConnectedSavePlugs", (PyCFunction)PyNet_GetConnectedSavePlugs    , METH_NOARGS
    //                           , "Returns the collection of connected plugs which have this net as master." }
    //, { "GetUnconnectedSavePlugs", (PyCFunction)PyNet_GetUnconnectedSavePlugs, METH_NOARGS
    //                             , "Returns the collection of unconnected plugs which have this net as master." }
    , { "IsGlobal"             , (PyCFunction)PyNet_IsGlobal                 , METH_NOARGS, "return true if the net is global" }
    , { "IsExternal"           , (PyCFunction)PyNet_IsExternal               , METH_NOARGS, "return true if the the net is external." }
    , { "IsLogical"            , (PyCFunction)PyNet_IsLogical                , METH_NOARGS, "return true if the net is logical ." }
    , { "IsClock"              , (PyCFunction)PyNet_IsClock                  , METH_NOARGS, "return true if the net is a clock" }
    , { "IsPower"              , (PyCFunction)PyNet_IsPower                  , METH_NOARGS, "return true if the net is a power" }
    , { "IsGround"             , (PyCFunction)PyNet_IsGround                 , METH_NOARGS, "return true if the net is a ground" }
    , { "IsSupply"             , (PyCFunction)PyNet_IsSupply                 , METH_NOARGS, "return true if the net is a supply" }
    , { "IsBound"              , (PyCFunction)PyNet_IsPyBound                , METH_NOARGS, "return true if the net is bounded to the hurricane net" }
    , { "SetName"              , (PyCFunction)PyNet_SetName                  , METH_VARARGS, "Allows to change net name." }
    //, { "SetArity"             , (PyCFunction)PyNet_SetArity                 , METH_VARARGS, "Sets the signal arity to <arity>." }
    , { "SetGlobal"            , (PyCFunction)PyNet_SetGlobal                , METH_VARARGS, "Set the net global." }
    , { "SetExternal"          , (PyCFunction)PyNet_SetExternal              , METH_VARARGS, "Set the net external." }
    , { "SetType"              , (PyCFunction)PyNet_SetType                  , METH_VARARGS, "Set the type of the net." }
    , { "SetDirection"         , (PyCFunction)PyNet_SetDirection             , METH_VARARGS, "Set the direction of the net." }
    , { "SetPosition"          , (PyCFunction)PyNet_SetPosition              , METH_VARARGS, "Set the X,Y location of the net." }
    //, { "Materialize"          , (PyCFunction)PyNet_Materialize              , METH_NOARGS
    //                           , "Materializes all the rubbers and components of a net." }
    //, { "Unmaterialize"        , (PyCFunction)PyNet_Unmaterialize            , METH_NOARGS
    //                           , "De-materializes all the rubbers and components of a net." }
    , { "Merge"                , (PyCFunction)PyNet_Merge                    , METH_VARARGS
                               , "Merges the net <net> to the net <this> which keeps its characteristics (arity, global, external and direction)." }
    , { "Delete"               , (PyCFunction)PyNet_Delete                   , METH_NOARGS
                               , "Delete associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyNet" Object Methods                     |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNet_new ()"

  static PyObject* PyNet_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyNet_new()" << endl;

    Net*      net = NULL;
    PyObject* arg0;
    PyObject* arg1;
    
    if ( ! ParseTwoArg ( "Net.new", args, CELL_NAME_ARG, &arg0, &arg1 ) ) return ( NULL );

    HTRY
    net = Net::Create ( PYCELL_O(arg0), *PYNAME_O(arg1) );
    HCATCH

    return PyNet_Link ( net );
  }

  DBoDeleteMethod(Net)
  PyTypeObjectLinkPyType(Net)
  PyTypeObjectConstructor(Net)


#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyNet" Shared Library Code Part                 |
// x=================================================================x




  // Link/Creation Method.
  DBoLinkCreateMethod(PyNet_Link,PyNet,PyTypeNet,Net)




  // ---------------------------------------------------------------
  // PyNet Object Definitions.

  PyTypeInheritedObjectDefinitions(Net, Entity)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
