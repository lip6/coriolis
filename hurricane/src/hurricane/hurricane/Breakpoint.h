
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
// |  C++ Header  :       "./Breakpoint.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __HURRICANE_BREAKPOINT__
# define  __HURRICANE_BREAKPOINT__

#include "hurricane/Commons.h"


namespace Hurricane {


  class Breakpoint {

    public:
      typedef bool ( StopCb_t )( const string& );

    public:
      static  Breakpoint*   get           ();
      static  void          setStopCb     ( StopCb_t* );
      static  bool          stop          ( unsigned int, const string );
      static  void          setStopLevel  ( unsigned int );
      static  unsigned int  getStopLevel  ();
    protected:                            
              bool          _stop         ( unsigned int, const string& );
                                          
    protected:                            
      static  Breakpoint*   _singleton;   
      static  StopCb_t*     _stopCb;      
      static  unsigned int  _stopLevel;   
                                          
    protected:                            
                            Breakpoint    ();
                            Breakpoint    ( const Breakpoint& );
              Breakpoint&   operator=     ( const Breakpoint& );
                           ~Breakpoint    ();
  };


} // End of Hurricane namespace.


# endif // __HURRICANE_BREAKPOINT__
