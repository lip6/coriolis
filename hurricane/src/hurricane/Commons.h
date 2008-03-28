

// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id: Commons.h,v 1.19 2007/07/29 15:24:52 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Commons.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __HURRICANE_COMMONS__
#define  __HURRICANE_COMMONS__


#include <stdio.h>
#include <assert.h>

#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>




// x-----------------------------------------------------------------x
// |                        Macros Definition                        |
// x-----------------------------------------------------------------x

#define is_a (bool)dynamic_cast


using namespace std;


namespace Hurricane {

  using namespace std;


  // ---------------------------------------------------------------
  // Forward Declarations.

  class Slot;
  class Record;




  // x-------------------------------------------------------------x
  // |                 Tracing/Debugging Utilites                  |
  // x-------------------------------------------------------------x


  bool          in_trace    ();
  void          trace_on    ();
  void          trace_off   ();
  void          trace_in    ();
  void          trace_out   ();
  bool          inltrace    ( unsigned int level );
  unsigned int  ltracelevel ( unsigned int level );
  void          ltracein    ( unsigned int level, unsigned int count=1 );
  void          ltraceout   ( unsigned int level, unsigned int count=1 );

#define        trace            if (in_trace()     ) cerr << tab
#define        ltrace(level)    if (inltrace(level)) cerr << tab




  // x-------------------------------------------------------------x
  // |                   Miscellaneous Utilites                    |
  // x-------------------------------------------------------------x


  inline string _TName ( const string& s ) { return s; }
  inline string _PName ( const string& s ) { return "Hurricane::" + s; }

  template<class Type>
    inline Type abs ( const Type& value ) { return (value<0) ? -value : value; }

         string  Demangle ( const char*      symbol );
  inline string  Demangle ( const type_info& info   ) { return Demangle(info.name()); }




} // End of Hurricane namespace.




// x-----------------------------------------------------------------x
// |                   Generic Functions Definition                  |
// x-----------------------------------------------------------------x


using namespace std;




// x-----------------------------------------------------------------x
// |             getString() Overloads for POD/STL types             |
// x-----------------------------------------------------------------x

// Note: we are outside the Hurricane namespace.


#include "SlotAdapter.h"




// x-----------------------------------------------------------------x
// |               Scan() Overloads for POD/STL types                |
// x-----------------------------------------------------------------x

// Note: we are outside the Hurricane namespace.


bool Scan ( const string& s, int&            i         );
bool Scan ( const string& s, unsigned&       u         );
bool Scan ( const string& s, unsigned short& u         );
bool Scan ( const string& s, double&         d         );
bool Scan ( const string& s, string&         pattern   );
bool Scan ( const string& s, unsigned short& redValue
                           , unsigned short& greenValue
                           , unsigned short& blueValue );



// x-----------------------------------------------------------------x
// |              Record & Tabulation Types Definitions              |
// x-----------------------------------------------------------------x

// Note: Record & Tabulation are not templates, so they can be defined
//       early.

#include "Tabulation.h"




#endif
