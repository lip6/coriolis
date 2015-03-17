// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/Observer.h"                        |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_OBSERVER_H
#define  KATABATIC_OBSERVER_H

#include  "hurricane/Error.h"


namespace Katabatic {


// -------------------------------------------------------------------
// Classes  :  "BaseObserver" & "Observer".

  class BaseObserver {
    public:
      virtual void  notify ( unsigned int flags );
  };


  template< typename T>
  class Observer : public BaseObserver {
    public:
      inline     Observer ( const T* owner );
      inline  T* getOwner () const;
    private:
                 Observer ( const Observer& );
    private:
      static int  _ownerOffset;
  };


  template< typename T>
  int  Observer<T>::_ownerOffset = -1;

  template< typename T>
  inline Observer<T>::Observer ( const T* owner )
    : BaseObserver()
  {
    if (owner == NULL)
      throw Hurricane::Error( "Observer::Observer(), attempt to create with NULL owner." );

    if (_ownerOffset < 0)
      _ownerOffset = (unsigned long)this - (unsigned long)owner;
  }


  template< typename T>
  inline T* Observer<T>::getOwner () const { return reinterpret_cast<T*>((unsigned long)this - _ownerOffset); }


// -------------------------------------------------------------------
// Class  :  "Observable".

  class Observable {
    public:
      inline        Observable     ();
      template< typename T >
      inline  T*    getObserver    ();
      inline  void  addObserver    ( BaseObserver* );
      inline  void  removeObserver ( BaseObserver* );
      inline  void  notify         ( unsigned int flags );
    private:
                    Observable     ( const Observable& );
    private:
      BaseObserver* _observer;
  };


  inline  Observable::Observable ()
    : _observer(NULL)
  { }


  template< typename T >
  T* Observable::getObserver ()
  {
    if (_observer) return (dynamic_cast< Observer<T>* >( _observer ))->getOwner();
    return NULL;
  }

  inline  void  Observable::addObserver ( BaseObserver* observer )
  {
    if (_observer)
      throw Hurricane::Error( "Observable::addObserver(), trying to add a second observer which is unsupported." );
    _observer = observer;
  }

  inline  void  Observable::removeObserver ( BaseObserver* observer )
  {
    if (_observer != observer)
      throw Hurricane::Error( "Observable::removeObserver(), trying to remove wrong observer." );
    _observer = NULL;
  }


  inline  void  Observable::notify ( unsigned int flags )
  {
    if (_observer) _observer->notify( flags );
  }


} // Katabatic namespace.

#endif
