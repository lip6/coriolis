// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./hurricane/isobar/PythonAttributes.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include <Python.h>
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/DBo.h"


namespace Isobar {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::PrivateProperty;
  class PyHolderProperty;


  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyAttributesHolder".

    typedef struct {
        PyObject_HEAD
        PyObject* dict;
    } PyAttributesHolder;

    
    extern PyTypeObject  PyTypeAttributesHolder;
    
  } // extern "C".


// -------------------------------------------------------------------
// Class  :  "Isobar::PyHolderProperty".

  class PyHolderProperty : public Hurricane::PrivateProperty {
      friend class PythonAttributes;
    public:
      typedef Hurricane::PrivateProperty  Super;
      static  Name  _name;
    public:
      static  PyHolderProperty*  create          ( DBo* );
      static  Name                getPropertyName ();
      virtual Name                getName         () const;
      inline  PyAttributesHolder* getHolder       ();
      inline  int32_t             getDictSize     ();
              int32_t             delattr         ( std::string );
      virtual void                onReleasedBy    ( DBo* owner );
      virtual std::string         _getTypeName    () const;
      virtual std::string         _getString      () const;
      virtual Record*             _getRecord      () const;
    protected:                    
      virtual void                _preDestroy     ();
    protected:
    // Attributes.
      PyAttributesHolder* _holder;
      
    protected:
    // Constructor.
      inline  PyHolderProperty ();
  };


  inline  PyHolderProperty::PyHolderProperty ()
    : PrivateProperty(), _holder(NULL)
  { }


  inline PyAttributesHolder* PyHolderProperty::getHolder   () { return _holder; }
  inline int32_t             PyHolderProperty::getDictSize () { return PyDict_Size( _holder->dict ); }


// -------------------------------------------------------------------
// Class  :  "Isobar::PythonAttributes".

  class PythonAttributes {
    public:
      static        PyAttributesHolder* get             ( const DBo* );
      static        int32_t             delattr         ( DBo*, std::string );
      static        void                enable          ( DBo* );
      static        void                disable         ( DBo* );
      static        void                disableAll      ();
      static        void                disableAll      ( std::string );
      static        int32_t             count           ();
  };


}  // Isobar Namespace.


INSPECTOR_P_SUPPORT(Isobar::PyHolderProperty);
