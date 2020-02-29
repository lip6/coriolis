// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Breakpoint.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_BREAKPOINT_H
#define  HURRICANE_BREAKPOINT_H

#include "hurricane/Commons.h"


namespace Hurricane {


  class Breakpoint {

    public:
      typedef bool ( StopCb_t )( const string& );

    public:
      static  bool          simpleStopCb  ( const string& message );
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


} // Hurricane namespace.


#endif // HURRICANE_BREAKPOINT_H
