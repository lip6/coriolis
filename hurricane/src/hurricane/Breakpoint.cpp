
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Breakpoint.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "hurricane/Breakpoint.h"


namespace {


  using namespace std;
  using namespace Hurricane;


  bool  simpleStopCb ( const string& message )
  {
    cerr << "[STOP] " << message << endl;

    char answer = '?';
    while ( answer == '?' ) {
      cerr << "       Type <y> to continue, <n> to abort: (y) ";
      cerr.flush ();

      cin >> answer;
      switch ( answer ) {
        case 'Y':
        case 'y': answer = 'y'; break;
        case 'N':
        case 'n': answer = 'n'; break;
        default:  answer = '?';
      }
    }
    return (answer == 'y');
  }


} // End of anonymous namespace.


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Breakpoint".


  Breakpoint*            Breakpoint::_singleton = NULL;
  Breakpoint::StopCb_t*  Breakpoint::_stopCb    = NULL;
  unsigned int           Breakpoint::_stopLevel = 0;


  Breakpoint::Breakpoint ()
  { }


  Breakpoint::~Breakpoint ()
  {
    delete _singleton;
  }


  Breakpoint* Breakpoint::get ()
  {
    if ( !_singleton ) {
      _singleton = new Breakpoint ();
      if ( !_stopCb )
        _singleton->setStopCb ( simpleStopCb );
    }

    return _singleton;
  }


  void  Breakpoint::setStopCb ( Breakpoint::StopCb_t* cb )
  {
    cerr << "Breakpoint::setStopCb() - " << (void*)cb << endl;
    _stopCb = cb;
  }


  bool  Breakpoint::stop ( unsigned int level, const string message )
  {
    return get()->_stop ( level, message );
  }


  void  Breakpoint::setStopLevel ( unsigned int level )
  {
    _stopLevel = level;
  }


  unsigned int  Breakpoint::getStopLevel ()
  {
    return _stopLevel;
  }


  bool  Breakpoint::_stop ( unsigned int level, const string& message )
  {
    if ( _stopCb && ( level <= _stopLevel ) )
      return _stopCb ( message );

    return false;
  }




} // End of Hurricane namespace.
