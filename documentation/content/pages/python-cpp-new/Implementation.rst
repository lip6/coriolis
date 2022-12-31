.. -*- Mode: rst -*-


2. Implementation
=================

We do not try to provides an interface as sleek as ``pybind11`` that completely
hides the Python/C API. Instead we keep mostly visible the classic structure of
the Python/C API but we provides templates to automate as much as possible the
boring tasks (and code duplication). This way, if we need a very specific
feature at some point, we can still revert back to the pure Python/C API.

The wrapper basically provides support for three kind of operations:

1. Encapsulate C++ object *in* Python ones, done by ``cToPy<>()`` template.
   Template specializations are defined for the ``POD`` and basic ``STL``
   types like ``std::string``.

   To add more easily new specializations, they resides in the top level
   scope (**not** inside ``Isobar``).

2. Decapsulate a C++ object *from* a Python one, done by ``pyToC()``.
   It's implementation is slightly different from the one of ``cToPy<>()``
   in the sense that it is a mix of normal C++ functions and template
   functions. I was having trouble inside the ``callMethod()`` to access
   to templates specialization defined *after* that point, so function be
   it.

   There are two mutually exclusives versions of the ``pyToC<>()`` for
   objects managed through the type manager. One is for value types and
   the other for pointer types.

3. Wrap C/C++ functions & methods inside C-linkage ``PyCFunction``, that is
   ``PyOject* (*)(PyObject*, PyObject*)``. This is done respectively through
   ``callFunction<>()`` and ``callMethod<>()``. ``callMethod<>()`` having
   two variants, one for directly calling the function member, if there is
   no overload and one for calling one given flavor of the overloaded
   function member.

   In addition we provides special variants for Python unary, binary and
   operator functions.

In addition to those user exposed features, we have:

* The ``PyTypeManager`` and it's derived classes to store and share informations
  about all our newly defined ``PyTypeObjects``.

* Various wrapper *classes* to wrap functions & methods. They are not directly
  exposed because the template class intanciation needs the template parameters
  to be explicitely given, wich is tedious. Instead we create them *through*
  a function template call, which will perform for us the template type
  deduction.

We creates only two kind of ``PyObject`` (but many ``PyTypeObject``):

* ``PyOneVoid`` which encapsulate one void pointer to the C++ associated
  object.

  .. code-block:: Python

     extern "C" {
       typedef struct PyOneVoid {
         PyObject_HEAD
	 void* _object1;
       };
     }

* ``PyTwoVoid`` which encapsulate one void pointer to the C++ associated
  object (an iterator) and one another to the ``PyObject`` of the container.

  .. code-block:: Python

     extern "C" {
       typedef struct PyTwoVoid {
         PyObject_HEAD
	 void*     _object1;  // C++ iterator.
	 PyObject* _object2;  // Python wrapped container.
       };
     }

     
.. note:: A ``PyTwoVoid`` can be casted/accessed as a ``PyOneVoid``.


2.1 PyTypeManager
~~~~~~~~~~~~~~~~~

``PyTypeManager`` has two usage:

* Act as a registry of all the created ``PyTypeObject``, and serve as a
  dispatcher for the ``PyTypeObject`` *tp* like methods. 

* Provide a non-template abstract base class for all the derived ``PyTypeObject``.
  As said, it is not a template class but it supplies function member
  template. Derived classes are provided to manage different kind of C++
  classes.

  * :cb:`PyTypeManagerVTrunk<CppT>`
    Is an intermediate between the non-template base class and all the
    templatized others (do **not** use it directly).

  * :cb:`PyTypeManagerNonDBo<CppT>`
    Template for standalone C++ classes that are not derived from ``DBo``.
    For example ``Box`` or ``Parameter``.

  * :cb:`PyTypeManagerDBo<CppT>`
    Template for C++ classes that *are* not derived from ``DBo``.
    For example ``Cell`` or ``Instance``.

  * :cb:`PyTypeManagerDerivedDBo<CppT,BaseT>`
    Template for a ``CppT`` class derived derived from a ``BaseT`` class.
    ``CppT`` doesn'y need to be a direct derived of ``BaseT``. ``BaseT``
    needs to derive from ``DBo``

  * :cb:`PyTypeManagerVector<CppT>`, template for C++ ``std::vector<CppT*>``.

  * :cb:`PyTypeManagerVectorIterator<CppT>`
    Template for C++ ``std::vector<CppT*>::iterator``, automatically created
    from the vector registration.

  * :cb:`PyTypeManagerMap<CppK,CppT>`, template for C++ ``std::map<CppK*,CppT*>``.

  * :cb:`PyTypeManagerMapIterator<CppK,CppT>`
    Template for C++ ``std::vector<CppK*,CppT*>::iterator``, automatically created
    from the map registration.

  * :cb:`PyTypeManagerCollection<,CppT>`, template for C++ ``Hurricane::Collection<CppT*>``.

  * :cb:`PyTypeManagerCollectionIterator<,CppT>`
    Template for C++ ``Hurricane::Locator<CppT*>``, automatically created from
    the collection registration.


2.2 Highjacking the *tp* methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Functions of a ``PyTypeObject`` like the *tp* methods (``tp_alloc``, ``tp_print``,
``tp_hash``, ...) must have a C-linkage. So we create *one* function per slot that
we want to use, set that *same* function for all the created ``PyTypeObject``, and
perform a dispacth in it. The drawback is that for each access we have to perform
a map lookup. Hope it is fast.

Excerpt from the code:

.. code-block:: C++

   namespace Isobar3 {

     extern "C" {
  
       // Here we have C-linkage.
       extern long  _tpHash ( PyObject* self )
       {
       // Dispatch towards the relevant class, based on ob_type pointer.
         return PyTypeManager::get( Py_TYPE(self)->_getTpHash( self );
       }
  
     }
  
     class PyTypeManager {
       public:
                 void           PyTypeManager::_setupPyType ()
         // Derived classes must implement it as they see fit.
         virtual long           _getTpHash                  ( PyObject* ) = 0;
         template<typename CppT>
	 static  PyTypeManager* _get();
       private:
         PyTypeObject  _typeObject;
     };
  
     void  PyTypeManager::_setupPyType ()
     {
       PyTypeObject* ob_type = _getTypeObject();
       ob_type->tp_name    = _getPyTypeName().c_str();
       ob_type->tp_dealloc = (destructor)&::Isobar3::_tpDeAlloc;
       ob_type->tp_str     = (reprfunc)  &::Isobar3::_tpStr;
       // All Python Type will call the same _tpHash().
       ob_type->tp_hash    = (hashfunc)  &::Isobar3::_tpHash;
       ob_type->tp_compare = (cmpfunc)   &::Isobar3::_getTpCompare;
       ob_type->tp_methods = _getMethods();
       ob_type->tp_getset  = _getGetsets();
     }

   }  // Isobar3 namespace.


2.3 Going From Python to C++
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To convert a C++ object (pointer) into a Python object, a mix of
:cb:`pyToC<>()` templates functions and real functions are supplieds.

Templates functions are used for all types/classes managed through
the ``PyTypeManger``. They come in two flavors:

2. **Value as pointer version** (C++ argment type is ``T*``):
   The encapsulated C++ object is still a pointer,
   but to a *stand-alone* one which has been created for the sole
   purpose of this ``PyObject``. Typical example is the ``Box``.
   Then, we *copy by value* the contents of the pointed object into
   the contents of the pointer argument that we where given.

3. **Pointer version** (C++ argument type is ``T**``):
   The encapsulated C++ object is truly a pointer
   to an element of the data-structure, then we just extract the
   C++ pointer value.

Normal function overload are used for ``POD`` types (``bool``, ``int``,
``long``, ``double``, ...) and basic ``STL`` types (``std::string``, ...).

Specialization for all POD type that can be directly translated into
Python types must be provideds (``bool``, ``int``, ``long``, ``double``,
``std::string``, ...).

Those templates/functions are the ones the ``Isobar::parse_objects()`` recursive
template function call in turn for each ``PyObject*`` argument.

.. note:: ``Hurricane::Name`` are *not* exposed to the Python interface, they
          must be treated as ``std::string``.

.. code-block:: C++

   // Template/Pointer to a value flavor.
   template< typename T
           , typename std::enable_if< !std::is_pointer<T>::value, bool >::type = true >
   inline bool  pyToC ( PyObject* pyArg, T* arg )
   {
     typedef typename std::remove_cv<T>::type  NonConstT;
     Isobar3::PyTypeManager* manager = Isobar3::PyTypeManager::_get<T>();
     if (not manager) {
       std::cerr << "Isobar3::pyToC<>(const T*): Unsupported type." << std::endl;
       return false;
     }
     if (Py_TYPE(pyArg) != manager->_getTypeObject()) return false;
     *(const_cast< NonConstT* >(arg)) = *(( T* )( Isobar3::object1( pyArg )));
     return true;
   }
   
   // Template/Pointer to a pointer flavor.
   template<typename T>
   inline bool  pyToC ( PyObject* pyArg, T** arg )
   {
     Isobar3::PyTypeManager* manager = Isobar3::PyTypeManager::_get<T>();
     if (not manager) {
       std::cerr << "Isobar3::pyToC<T>(T*&): Unsupported type \"" << typeid(T).name() << "\"" << std::endl;
       return false;
     }
     *arg = (T*)( Isobar3::object1( pyArg ));
     return true;
   }
   
   // True function overload for std::string.
   inline bool  pyToC ( PyObject* pyArg, std::string* arg )
   {
     if (not PyUnicode_Check(pyArg)) return false;
     PyObject* pyBytes = PyUnicode_AsASCIIString( pyArg );
     *arg = PyBytes_AsString( pyBytes );
     Py_DECREF( pyBytes );
     return true;
   }
   
   // True function overload for bool.
   inline bool  pyToC ( PyObject* pyArg, bool* arg )
   {
     if (not PyBool_Check(pyArg)) return false;
     *arg = (pyArg == Py_True);
     return true;
   }
   
   // True function overload for int.
   inline bool  pyToC ( PyObject* pyArg, int* arg )
   {
     if (PyLong_Check(pyArg)) { *arg = PyLong_AsLong( pyArg ); }
     else return false;
     return true;
   }



2.4 Going From C++ to Python
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To convert a Python object into a C++ object, a set of
:cb:`cToPy<>` templates functions are supplieds.

We completely disable the partially specialized templates for
objects that are non-POD as the compiler seems to be unable to
choose the fully specialized template in this case (or I still
misunderstood the template resolution mechanism).

In the case of object registered in ``PyTypeManager``, we delegate
the ``PyObject`` creation to the ``PyTypeManager::link()`` template
function, which in turn, can call the right ``PyTypeManagerVTrunk<CppT>::_link()`` 
method.

.. note:: The ``PyTypeManagerVTrunk<CppT>::_link()`` method is the reason
	  **why** we need the intermediate ``PyTypeManagerVTrunk<CppT>``
	  template class. 

.. note:: **Different C++ templates.** You may notice that the two following templates
	  may look like specializations of the same one:

          * ``template<typename CppT> PyObject* cToPy ( CppT  object )``
          * ``template<typename CppT> PyObject* cToPy ( CppT* object )``

	  Which would be illegal (function templates are not allowed to have *partial*
	  specialization), but they are *not*. The two pairs
	  ``(template parameter,function parameter)``, that is ``(CppT,CppT)`` and
	  ``(CppT,CppT*)`` cannot be made to be a specialization of each other.


.. code-block:: C++
  
   // Generic template for values.
   template< typename CppT >
   inline PyObject* cToPy ( CppT object )
   {
     if (not Isobar3::PyTypeManager::hasType<CppT>()) {
       std::string message = "Overload for Isobar3::cToPy< "
                           + Hurricane::demangle(typeid(CppT).name()) + " >() Type not registered in the manager.";
       PyErr_SetString( Isobar3::HurricaneError, message.c_str() );
       return NULL;
     }
     return Isobar3::PyTypeManager::link<CppT>( new CppT (object) );
   }
   
   // Disabled for POD & STL types, pointer flavor.
   template< typename CppT
           , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                     && !std::is_same<CppT,int >::value
                                     && !std::is_same<CppT,std::string>::value
                                     && !std::is_same<CppT,const std::string>::value,bool>::type = true >
   inline PyObject* cToPy ( CppT* object )
   { return Isobar3::PyTypeManager::link<CppT>( object ); }
   
   // Disabled for POD & STL types, const pointer flavor.
   template< typename CppT
           , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                     && !std::is_same<CppT,int >::value
                                     && !std::is_same<CppT,std::string>::value
                                     && !std::is_same<CppT,const std::string>::value,bool>::type = true >
   inline PyObject* cToPy ( const CppT* object )
   { return Isobar3::PyTypeManager::link<CppT>( const_cast<CppT*>( object )); }
   
   // Specialization for booleans.
   template<>
   inline PyObject* cToPy<bool> ( bool b )
   { if (b) Py_RETURN_TRUE; Py_RETURN_FALSE; }
   
   // Specialization for STL std::string.
   template<> inline PyObject* cToPy<      std::string>  (       std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
   template<> inline PyObject* cToPy<const std::string > ( const std::string  s ) { return PyUnicode_FromString( s.c_str() ); }
   template<> inline PyObject* cToPy<const std::string*> ( const std::string* s ) { return PyUnicode_FromString( s->c_str() ); }
   
   // Specialization for POD int.
   template<> inline PyObject* cToPy<      int > (       int  i ) { return PyLong_FromLong(  i ); }
   template<> inline PyObject* cToPy<const int > ( const int  i ) { return PyLong_FromLong(  i ); }
   template<> inline PyObject* cToPy<const int*> ( const int* i ) { return PyLong_FromLong( *i ); }


2.5 Object Methods Wrappers
~~~~~~~~~~~~~~~~~~~~~~~~~~~

One of the more tedious task in exporting a C++ interface towards Python is
to have wrap the C++ functions/methods into C-linkage functions that can be
put into the ``PyMethodDef`` table.

Basically, we want to fit:

* A C++ function or method with a variable number of arguments, each argument
  having it's own type.

  .. code-block:: C++

     class Parameter {
       // ...
       public:
         void  addValue ( std::string s, int v );
       // ...
     };

* Into a ``PyCFunction`` prototype.

  .. code-block:: C++

     extern "C" {
       typedef PyObject* ( *PyCFunction )( PyObject* self, PyObject* args );
     }

  Here, the C++ object is provided through the first argument and the
  functions arguments through a *tuple* in second argument. In Python
  wrappers, the tuple doesn't have any complex structure, it boils down
  to a sequence of ``PyObject*`` (that must match the number of arguments
  of it's C++ function conterpart).

So, the problem is to change a Python tuple which size is only kown at
*runtime* into a list of C/C++ parameters known at *compile time*.

I am not such an expert in template programming so I can find a *generic*
solution able to handle any number of parameters. Instead I did write
a set of templates managing the translation from zero to ten parameters.
I did delay that translation as much as possible so it happens very close
to the C++ function call and the duplicated code needed for each template
is kept to a minimum.

To translate the Python tuple into an ordered list (vector like) of C++
object *of different types*, the obvious choice should have been ``std::tuple<>``,
but I did encouter problems when the functions signature did contains
references. So to manage that I did implement:

* A ``BaseArg`` class and it's template derived ``Arg<T>`` to hold
  one value of a type (more or less like ``std::any<>``).
  The type of the value attribute of ``Arg<T>`` is ``T`` *stripped*
  from reference and constness. This internal type is accessible
  through ``Arg<T>::ValueT``.

* A template list of arguments ``Args<typename... Ts>`` analogous to
  ``std::tuple<>`` which holds a table of ``BaseArg`` to convert all the
  arguments.

* A recursive template converter function ``parse_pyobjects<>``, which is
  called through the ``Args<>::parse()`` function. 

Another challenge is the return type. I distinguish three flavor of
return type:

* Function returning nothing (``void``).
* Function returning a value.
* Function returning a reference to a value.
* Function returning a pointer.

To uniformize the return type we create four templates ``_callMethodReturn<>()`` 
that takes whatever the C++ return type is, and turn it into a ``PyObject*``.
Except for the functions returning ``void``, we call ``cToPy<>()`` to
wrap the value. Given the return type of the method, only one template
will match. But as functions template do not allow partial specialization,
only one must be defined for that method (the one *matching* it's
return type), so we make the template mutually exclusives based on
the ``TR`` type (with the ``std::enable_if<>`` clause).


.. note:: In the various ``_callMethodReturn<>`` we have *two* sets for the
	  method parameters types : ``TArgsF...`` and ``TArgsW...``. This is to
	  allow a wider range of matching in the template as the type of the
	  arguments of the method (``TArgsF...``) may not *exactly* matches the
	  one passed by the wrapper (``TArgsW...``), typically the method has
	  a ``const`` parameter which is non-``const`` in the wrapper.

Here is an excerpt of the code:

.. code-block:: C++
 
   // Flavor for "return by value" (seems to match std::is_object<>)
   template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
           , typename std::enable_if<   !std::is_reference<TR>::value
                                     && !std::is_pointer  <TR>::value
                                     && !std::is_void     <TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
   {
     TR value = (cppObject->*method)( args... );
     return cToPy( value );
   }
   
   // Flavor for "return by reference"
   template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
           , typename std::enable_if<    std::is_reference<TR>::value
                                     && !std::is_pointer  <TR>::value
                                     && !std::is_void     <TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
   {
     TR rvalue = (cppObject->*method)( args... );
     return cToPy( rvalue );
   }
   
   // Flavor for "return by pointer".
   template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
           , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
   {
     TR pvalue = (cppObject->*method)( args... );
     return cToPy( pvalue );
   }
   
   // Flavor for "return void".
   template< typename TC, typename TR, typename... TArgsF, typename... TArgsW
           , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgsF...), TC* cppObject, TArgsW... args )
   {
     (cppObject->*method)( args... );
     Py_RETURN_NONE;
   }
 
   // Make the translation call for a method without arguments.
   template< typename TC, typename TR >
   inline PyObject* _callMethod ( TR(TC::* method)(), TC* cppObject, Args<>& )
   { return _callMethodReturn<TC,TR>( method, cppObject ); }
 
   // Make the translation call for a method one argument.
   template< typename TC, typename TR, typename TA0 >
   inline PyObject* _callMethod ( TR(TC::* method)(TA0), TC* cppObject, Args<TA0>& args )
   { return _callMethodReturn( method, cppObject, as<TA0>( args[0] ) ); }
 
   // Make the translation call for a method two argument.
   template< typename TC, typename TR, typename TA0, typename TA1 >
   PyObject* _callMethod ( TR(TC::* method)(TA0,TA1), TC* cppObject, Args<TA0,TA1>& args )
   { return _callMethodReturn( method, cppObject, as<TA0>( args[0] ), as<TA1>( args[1] ) ); }


The complete work of translating the Python tuple into a ``Args<>`` is done inside
a dedicated template class ``PyWrapper`` and it's ``call()`` method.
C++ exceptions are catched and translated into Python ones.

* ``PyWrapper`` the base class wich handle both C++ and Python exceptions.
  Provides the ``call()`` function which in turn wraps the ``_call()`` that
  must be overloaded in derived classes.

* ``PyFunctionWrapper<>``, template derived class for C/C++ normal functions.

* ``PyMethodWrapper<>``, template derived class for C++ class methods.
  Two flavors are supported, the real method and a function build upon a
  method (first argument beaing the object itself). The later is used when
  we need to desambiguate overloaded functions, we must create one *function*
  per overload.

As a class template cannot guess the template parameters, we wrap them into a
function template which can perform the guess. The ``callMethod<>`` template function.

In the end, what the user can write is simply:

.. code-block:: C++

   static PyObject* PyParameter_addValue ( PyObject* self, PyObject* args )
   { return callMethod("Parameter.addValue",&Parameter::addValue,self,args); }
 
   PyMethodDef PyParameter_Methods[] =
     { { "isFile"  , (PyCFunction)PyParameter_isFile  , METH_NOARGS
                   , "Tells if this parameter (string) holds a file name." }
     , { "addValue", (PyCFunction)PyParameter_addValue, METH_VARARGS
                   , "Add a new value to parameter of enumerated type." }
     // ...
     , {NULL, NULL, 0, NULL}   /* sentinel */
     };


2.6 Case of C++ overloaded functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This apply to both overloaded functions and functions with default arguments values.

In that case, the only solution is to create a set of different functions
with differents arguments to expose all the various signature of the function.
We then create a function wrapper that calls them in decreasing number of
parameters order.

.. note::

   If something goes wrong in a ``callMethod()``, it returns ``NULL`` and
   sets an error exception. If, say, the ``setString3()`` variant fails,
   but ``setString2()`` succeed, it will clear the error and sets ``rvalue``
   to something non-``NULL``.

You may also notice that the signature of an un-overloaded function is that
of a normal function, not a class method, with the object (aka C++ ``this``
passed as the first argument). So ``callMethod()`` and ``PyMethodWrapper``
support both case (through different constructors).

.. code-block:: C++

   static bool  setString1 ( Parameter* self, std::string value )
   { return self->setString(value); }
 
   static bool  setString2 ( Parameter* self, std::string value, unsigned int flags )
   { return self->setString(value,Configuration::getDefaultPriority(),flags); }
 
   static bool  setString3 ( Parameter* self
                           , std::string value
                           , unsigned int flags
                           , Parameter::Priority pri )
   { return self->setString(value,pri,flags); }
 
   static PyObject* PyParameter_setString ( PyObject* self, PyObject* args )
   {
     PyObject*       rvalue = callMethod("Parameter.setString",&setString3,self,args);
     if (not rvalue) rvalue = callMethod("Parameter.setString",&setString2,self,args);
     if (not rvalue) rvalue = callMethod("Parameter.setString",&setString1,self,args);
     return rvalue;
   }
 
   PyMethodDef PyParameter_Methods[] =
     { { "isFile"  , (PyCFunction)PyParameter_isFile   , METH_NOARGS
                   , "Tells if this parameter (string) holds a file name." }
    , { "setString", (PyCFunction)PyParameter_setString, METH_VARARGS
                   , "Set the parameter value as a string." }
     // ...
     , {NULL, NULL, 0, NULL}   /* sentinel */
     };


2.7 Wrapper for ordinary functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The same mechanic as for the object methods has been built for ordinary
functions. The top level wrapper beeing ``callFunction<>()`` ...

.. code-block:: C++

   static PyObject* PyCfg_hasParameter ( PyObject* module, PyObject* args )
   { return callFunction("hasParameter",&hasParameter,args); }
 
   static PyMethodDef PyCfg_Methods[] =
     { { "hasParameter", (PyCFunction)PyCfg_hasParameter, METH_VARARGS
                       , "Tells if a parameter exists already in the DB." }
     // ...
     , {NULL, NULL, 0, NULL}  /* sentinel */
     };


2.8 Object post-create hook
~~~~~~~~~~~~~~~~~~~~~~~~~~~

By defining specialization of the ``pyTypePostModuleinit<>()`` function template,
you can add any post-treatment to a Python type object. Like adding sub-classes
or constants values.

In the following code, we add ``Priority`` as a sub-object of ``Parameter`` then
set some constant values in ``Priority``. This was we emulate the behavior of
the ``Priority`` ``enum``.

.. code-block:: C++

   template<>
   inline void  pyTypePostInit<Cfg::Parameter> ( PyTypeObject* typeObject )
   {
     PyTypeManagerNonDBo<Cfg::Parameter::Priority>::create( (PyObject*)typeObject
                                                          , Cfg::PyParameterPriority_Methods
                                                          , NULL
                                                          , PyTypeManager::NoCppDelete );
   }

   template<>
   inline void  pyTypePostInit<Cfg::Parameter::Priority> ( PyTypeObject* typeObject )
   {
   // Parameter::Priority enum.
     addConstant( typeObject, "UseDefault"        , Cfg::Parameter::UseDefault );
     addConstant( typeObject, "ApplicationBuiltin", Cfg::Parameter::ApplicationBuiltin );
     addConstant( typeObject, "ConfigurationFile" , Cfg::Parameter::ConfigurationFile );
     addConstant( typeObject, "UserFile"          , Cfg::Parameter::UserFile );
     addConstant( typeObject, "CommandLine"       , Cfg::Parameter::CommandLine );
     addConstant( typeObject, "Interactive"       , Cfg::Parameter::Interactive );
   }


