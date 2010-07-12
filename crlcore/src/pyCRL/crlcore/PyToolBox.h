
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyToolBox.h"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __PY_CRL_TOOLBOX__
#define __PY_CRL_TOOLBOX__

#include "hurricane/isobar/PyHurricane.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern PyObject* PyToolBox_createPartRing ( PyObject* module, PyObject* args );


}  // End of extern "C".


}  // End of CRL namespace.


#endif  // __PY_CRL_TOOLBOX__
