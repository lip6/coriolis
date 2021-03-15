
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2010-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyToolBox.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern PyObject* PyToolBox_createPartRing       ( PyObject* module, PyObject* args );
  extern PyObject* PyToolBox_restoreNetsDirection ( PyObject* module, PyObject* args );


}  // End of extern "C".

}  // End of CRL namespace.
