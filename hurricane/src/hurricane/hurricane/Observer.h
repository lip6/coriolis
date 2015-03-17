//  -*- mode: C++; explicit-buffer-name: "Observer.h<hurricane>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Observer.h"                        |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_OBSERVER_H
#define  HURRICANE_OBSERVER_H

#include <vector>
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Classes  :  "BaseObserver" & "Observer".

  class BaseObserver {
    public:
      virtual void  notify ( unsigned int flags );
      template< typename T >
      inline  T*    as     ();
  };


  template< typename T>
  inline T* BaseObserver::as () { return dynamic_cast<T*>(this); }


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
      inline                              Observable     ();
      inline  std::vector<BaseObserver*>  getObservers   ();
              void                        addObserver    ( BaseObserver* );
              void                        removeObserver ( BaseObserver* );
      inline  void                        notify         ( unsigned int flags );
    private:
                                          Observable     ( const Observable& );
    private:
      std::vector<BaseObserver*>  _observers;
  };


  inline  Observable::Observable ()
    : _observers()
  { }


  inline  std::vector<BaseObserver*>  Observable::getObservers ()
  { return _observers; }


  inline void  Observable::notify ( unsigned int flags )
  {
    for ( auto iobserver : _observers ) {
      iobserver->notify( flags );
    }
  }


} // Hurricane namespace.

#endif
