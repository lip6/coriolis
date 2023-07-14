//  -*- mode: C++; explicit-buffer-name: "Observer.h<hurricane>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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


#pragma  once
#include <vector>
#include <array>
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Classes  :  "BaseObserver" & "Observer".

  class BaseObserver {
    public:
      virtual void  notify ( unsigned int flags );
  };


  template< typename OwnerT>
  class Observer : public BaseObserver {
    public:
      inline            Observer  ( const OwnerT* owner );
      inline  OwnerT*   getOwner  () const;
      virtual void      notify    ( unsigned int flags );
    private:
                       Observer  ( const Observer& );
             Observer& operator= ( const Observer& );
    private:
      static int  _ownerOffset;
  };


  template< typename OwnerT>
  int  Observer<OwnerT>::_ownerOffset = -1;

  template< typename OwnerT>
  inline Observer<OwnerT>::Observer ( const OwnerT* owner )
    : BaseObserver()
  {
    if (owner == NULL)
      throw Error( "Observer::Observer(), attempt to create with NULL owner." );

    if (_ownerOffset < 0) _ownerOffset = (unsigned long)this - (unsigned long)owner;
  }


  template< typename OwnerT>
  inline OwnerT* Observer<OwnerT>::getOwner () const
  { return reinterpret_cast<OwnerT*>((unsigned long)this - _ownerOffset); }

  template< typename OwnerT>
  void  Observer<OwnerT>::notify ( unsigned int flags )
  { OwnerT::notify( getOwner(), flags ); }


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
    for ( BaseObserver* iobserver : _observers ) {
      if (iobserver) iobserver->notify( flags );
    }
  }


// -------------------------------------------------------------------
// Class  :  "StaticObservable".

  template<size_t slotsNb>
  class StaticObservable {
    public:
      inline            StaticObservable ();
      inline  const     std::array<BaseObserver*,slotsNb>&
                        getObservers     () const;
      inline  void      setObserver      ( size_t slot, BaseObserver* );
      inline  void      notify           ( unsigned int flags );
      template<typename OwnerT>
      inline  OwnerT*   getObserver      ( size_t slot ) const;
    private:
                        StaticObservable ( const StaticObservable& );
      StaticObservable& operator=        ( const StaticObservable& );
    private:
      std::array<BaseObserver*,slotsNb>  _observers;
  };


  template<size_t slotsNb>
  inline StaticObservable<slotsNb>::StaticObservable ()
    : _observers()
  { }


  template<size_t slotsNb>
  inline const std::array<BaseObserver*,slotsNb>& StaticObservable<slotsNb>::getObservers () const
  { return _observers; }


  template<size_t slotsNb>
  inline void  StaticObservable<slotsNb>::notify ( unsigned int flags )
  {
    for ( BaseObserver* iobserver : _observers ) {
      if (iobserver) iobserver->notify( flags );
    }
  }


  template<size_t slotsNb>
  inline void  StaticObservable<slotsNb>::setObserver ( size_t slot, BaseObserver* observer )
  {
    if (slot < _observers.size()) _observers[slot] = observer;
    else
      throw Error( "StaticObserver::setObserver(): Trying to access out of range slot %u (size:%u)."
                 , slot, _observers.size()
                 );
  }


  template<size_t slotsNb>
  template<typename OwnerT>
  inline OwnerT* StaticObservable<slotsNb>::getObserver ( size_t slot ) const
  {
    if ( (slot >= _observers.size()) or not _observers[slot]) return NULL;
    return static_cast< Observer<OwnerT>* >(_observers[slot])->getOwner();
  }



} // Hurricane namespace.
