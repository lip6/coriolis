
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: PyUpdateSession.cpp,v 1.12 2006/07/19 14:00:05 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyUpdateSession.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "PyUpdateSession.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


// x=================================================================x
// |           "PyUpdateSession" Python Module Code Part             |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |             "PyUpdateSession" General Methods               |
  // x-------------------------------------------------------------x


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUpdateSession_open ()"

  extern PyObject* PyUpdateSession_open ( PyObject* module ) {
    trace << "PyUpdateSession_open()" << endl;

    HTRY
    UpdateSession::open ();
    HCATCH

    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyUpdateSession_close()"

  extern PyObject* PyUpdateSession_close( PyObject* module )
  {
    trace << "PyUpdateSession_close()" << endl;

    HTRY
    UpdateSession::close();
    HCATCH

    Py_RETURN_NONE;
  }

#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyUpdateSession" Shared Library Code Part             |
// x=================================================================x


#endif


}  // End of extern "C".




}  // End of Isobar namespace.
 
