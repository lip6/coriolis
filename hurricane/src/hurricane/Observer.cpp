// -*- mode: C++; explicit-buffer-name: "Observer.cpp<hurricane>" -*-
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
// |  C++ Module  :  "./Observer.cpp"                                |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Observer.h"


namespace Hurricane {

  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Classes  :  "BaseObserver" & "Observer".


  void  BaseObserver::notify ( unsigned int flags )
  { }


// -------------------------------------------------------------------
// Class  :  "Observable".


  void  Observable::addObserver ( BaseObserver* observer )
  {
    for ( auto iobserver : _observers ) {
      if (iobserver == observer) {
        cerr << Error( "Observable::addObserver(), trying to add twice the same observer." ) << endl;;
        return;
      }
    }
    _observers.push_back( observer );
  }


  void  Observable::removeObserver ( BaseObserver* observer )
  { 
    vector<BaseObserver*>::iterator iobserver=_observers.begin();
    for ( ; iobserver!=_observers.end() ; ++iobserver ) {
      if (*iobserver == observer) {
        _observers.erase( iobserver );
        return;
      }
    }

    cerr << Error( "Observable::removeObserver(), No such observer." ) << endl;;
  }


} // Hurricane namespace.
