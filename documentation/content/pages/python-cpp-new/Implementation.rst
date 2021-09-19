.. -*- Mode: rst -*-


2. Implementation
=================

We do not try to provides an iterface as sleek as ``pybind11`` that completely
hides the Python/C API. Instead we keep mostly visible the classic structure of
the Python/C API but we provides templates to automate as much as possible the
boring tasks (and code duplication). This way, if we need a very specific
feature at some point, we can still revert back to the pure Python/C API.

The key features of our wrapper are:

* The ``PyTypeManager`` and it's derived classes to store and share informations
  about all our newly defined ``PyTypeObjects``.

* The ``callMethod<>()`` and ``callFunction<>()`` function templates to
  automatically generate a wrapper around C++ object methods or functions.

* The ``PyBoject* objectLink<CppT>( CppT* )`` functions templates that provides C++ to Python
  object translation.

* The ``bool pyAs<CppT>( PyObject*, CppT*& )`` functions templates that provides Python to C++
  object translation.

We creates only two kind of ``PyObject`` (but many ``PyTypeObject``):

* ``PyVoidpointer`` which encapsulate one void pointer to the C++ associated
  object.

  .. code-block:: Python

     extern "C" {
       typedef struct PyVoidPointer {
         PyObject_HEAD
	 void* _object;
       };
     }

* ``PyIteratorPointer`` which encapsulate one void pointer to the C++ associated
  iterator and one another to the ``PyObject`` of the container.

  .. code-block:: Python

     extern "C" {
       typedef struct PyVoidPointer {
         PyObject_HEAD
	 void*          _object;     // C++ iterator.
	 PyVoidPointer* _container;  // Python wrapped container.
       };
     }


2.1 PyTypeManager
~~~~~~~~~~~~~~~~~

``PyTypeManager`` has two usage:

* Act as a registry of all the created ``PyTypeObject``, and serve as a
  dispatcher for the ``PyTypeObject`` *tp* like methods. 

* Provide a non-template abstract base class for all the derived ``PyTypeObject``.
  As said, it is not a template class but it supplies function member
  template. Derived classes are provides for different kind of C++
  class.

  * :cb:`PyTypeManagerVTrunk<CppT>`

    Is an intermediate between the non-template base class and all the
    templatized others.

  * :cb:`PyTypeManagerNonDBo<CppT>`

    Template for standalone C++ classes that are not derived from ``DBo``.
    For example ``Box`` or ``Parameter``.

  * :cb:`PyTypeManagerDBo<CppT>`

    Template for C++ classes that *are* not derived from ``DBo``.
    For example ``Cell`` or ``Instance``.

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
perform a dispact in it. The drawback is that for each access we have to perform
a map lookup. Hope it is fast.

Excerpt from the code:

.. code-block:: C++

   namespace Isobar3 {

     extern "C" {
  
       // Here we have C-linkage.
       extern long  _tpHash ( PyObject* self )
       {
       // Dispatch towards the relevant class, based on ob_type pointer.
         return PyTypeManager::get( self->ob_type )->_getTpHash( asVPtr(self) );
       }
  
     }
  
     class PyTypeManager {
       public:
                 void           PyTypeManager::_setupPyType ()
         // Derived classes must implement it as they see fit.
         virtual long           _getTpHash                  ( PyVoidPointer* ) = 0;
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

To convert a C++ object (pointer) into a Python object, a set of
:cb:`pyAs<>` templates functions are supplieds.

Specialization for all POD type that can be directly translated into
Python types must be provideds (``bool``, ``int``, ``long``, ``double``,
``std::string``, ...).

Partial specialization for type availables through the ``PyTypeManager``
is supplied. It checks the manager for the type's existence.

.. code-block:: C++

   // Most generic template, should *not* be used.
   // So issue an error message and report a failed conversion.
   template<typename T>
   inline bool  pyAs ( PyObject* pyArg, T& arg )
   {
     std::cerr << "Isobar3.pyAs<T>(): Unsupported type \""
               << demangle(typeid(T).name()) << "\"" << endl;
     return false;
   }
   
   // Template specialization for POD type "int".
   template<>
   inline bool  pyAs ( PyObject* pyArg, int& arg )
   {
     if      (PyInt_Check (pyArg)) { arg = PyInt_AsLong ( pyArg ); }
     else if (PyLong_Check(pyArg)) { arg = PyLong_AsLong( pyArg ); }
     else return false;
     return true;
   }

   // Template partial specialization for any type registered in
   // the PyTypeManager.
   template<typename T>
   inline bool  pyAs ( PyObject* pyArg, T*& arg )
   {
     PyTypeManager* manager = PyTypeManager::_get<T>();
     if (not manager) {
       std::cerr << "Isobar3.pyAs<T>(): Unsupported type \""
                 << typeid(T).name() << "\"" << endl;
       return false;
     }
     arg = (T*)( asVPtr( pyArg )->_object );
     return true;
   }
   
   template<typename T>
   inline bool  pyAs ( PyVoidPointer* pyArg, T*& arg )
   { return pyAs( (PyObject*)pyArg, arg ); }


2.4 Going From C++ to Python
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To convert a Python object into a C++ object, a set of
:cb:`objectLink<>` templates functions are supplieds.

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


.. code-block:: C++

   // The most generic template, should *not* be used. So raise a Python exception.
   template< typename CppT >
   inline PyObject* objectLink ( CppT object )
   {
     std::string message = "Overload for Isobar3::objectLink< "
                         + demangle(typeid(CppT).name()) + " >() is missing.";
     PyErr_SetString( HurricaneError, message.c_str() );
     return NULL;
   }
 
   // Disable this template if "CppT" is an already supported type.
   template< typename CppT
           , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                     && !std::is_same<CppT,int >::value
                                     && !std::is_same<CppT,std::string>::value
                                     && !std::is_same<CppT,const std::string>::value
				     ,bool>::type = true >
   inline PyObject* objectLink ( CppT* object )
   { return PyTypeManager::link<CppT>( object ); }
 
   // Disable this template if "CppT" is an already supported type.
   template< typename CppT
           , typename std::enable_if<   !std::is_same<CppT,bool>::value
                                     && !std::is_same<CppT,int >::value
                                     && !std::is_same<CppT,std::string>::value
                                     && !std::is_same<CppT,const std::string>::value
				     ,bool>::type = true >
   inline PyObject* objectLink ( const CppT* object )
   { return PyTypeManager::link<CppT>( const_cast<CppT*>( object )); }

   // Specializations for POD type "int" .
   template<> inline PyObject* objectLink<int > ( int  i )
   { return PyInt_FromLong(  i ); }

   template<> inline PyObject* objectLink<const int > ( const int  i )
   { return PyInt_FromLong(  i ); }

   template<> inline PyObject* objectLink<const int*> ( const int* i )
   { return PyInt_FromLong( *i ); }


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

So, the problem is to change a tuple which size is only kown at *runtime*
into a list of C/C++ parameters known at *compile time*.

I am not such an expert in template programming so I can find a *generic*
solution able to handle any number of parameters. Instead I did write
a set of templates managing the translation from zero to ten parameters.
I did delay that translation as much as possible so it happens very close
to the C++ function call and the duplicated code needed for each template
is kept to a minimum.

To translate the Python tuple into an ordered list (vector like) of C++
object *of different types*, the obvious choice is C++ ``std::tuple<>``.

Another challenge is the return type. I distinguish three flavor of
return type:

* Function returning nothing (``void``).
* Function returning a value.
* Function returning a reference to a value.
* Function returning a pointer.

To uniformize the return type we create four templates ``_callMethodReturn<>()`` 
that takes whatever the C++ return type is, and turn it into a ``PyObject*``.
Except for the functions returning ``void``, we call ``objectLink<>()`` to
wrap the value.

Here is an excerpt of the code:

.. code-block:: C++

   // Flavor for "return by pointer".
   template< typename TC, typename TR, typename... TArgs
           , typename std::enable_if<std::is_pointer<TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
   {
     TR pvalue = (cppObject->*method)( args... );
     return objectLink( pvalue );
   }
 
   // Flavor for "return void".
   template< typename TC, typename TR, typename... TArgs
           , typename std::enable_if<std::is_void<TR>::value,bool>::type = true >
   inline PyObject* _callMethodReturn ( TR(TC::* method)(TArgs...), TC* cppObject, TArgs... args )
   {
     (cppObject->*method)( args... );
     Py_RETURN_NONE;
   }

   // Function without argument.
   template< typename TC, typename TR >
   inline PyObject* _callMethod ( TR(TC::* method)(), TC* cppObject, std::tuple<> )
   { return _callMethodReturn<TC,TR>( method, cppObject ); }
   
   // Function with one argument.
   template< typename TC, typename TR, typename TA0 >
   inline PyObject* _callMethod ( TR(TC::* method)(TA0), TC* cppObject, std::tuple<TA0> args )
   { return _callMethodReturn( method, cppObject, std::get<0>(args) ); }
   
   // Function with two arguments.
   template< typename TC, typename TR, typename TA0, typename TA1 >
   PyObject* _callMethod ( TR(TC::* method)(TA0,TA1), TC* cppObject, std::tuple<TA0,TA1> args )
   { return _callMethodReturn( method, cppObject, std::get<0>(args), std::get<1>(args) ); }


The complete work of translating the Python tuple into a ``std::tuple<>`` and error
handling is done with a dedicated template class ``PyMethodWrapper`` and it's ``call()``
method.

As a class template cannot guess the template parameters, we wrap them into a
function template which can perform the guess. The ``callMethod<>`` template function.

In the end, what the user can write is simply:

.. code-block:: C++

   static PyObject* PyParameter_addValue ( PyVoidPointer* self, PyObject* args )
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

This apply to both overloaded functions and functions with arguments values.

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
 
   static PyObject* PyParameter_setString ( PyVoidPointer* self, PyObject* args )
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
   inline void  pyTypePostModuleInit<Cfg::Parameter> ( PyTypeObject* typeObject )
   {
     PyTypeManagerNonDBo<Cfg::Parameter::Priority>::create( (PyObject*)typeObject
                                                          , Cfg::PyParameterPriority_Methods
                                                          , NULL
                                                          , PyTypeManager::NoCppDelete );
   }

   template<>
   inline void  pyTypePostModuleInit<Cfg::Parameter::Priority> ( PyTypeObject* typeObject )
   {
   // Parameter::Priority enum.
     addConstant( typeObject, "UseDefault"        , Cfg::Parameter::UseDefault );
     addConstant( typeObject, "ApplicationBuiltin", Cfg::Parameter::ApplicationBuiltin );
     addConstant( typeObject, "ConfigurationFile" , Cfg::Parameter::ConfigurationFile );
     addConstant( typeObject, "UserFile"          , Cfg::Parameter::UserFile );
     addConstant( typeObject, "CommandLine"       , Cfg::Parameter::CommandLine );
     addConstant( typeObject, "Interactive"       , Cfg::Parameter::Interactive );
   }


