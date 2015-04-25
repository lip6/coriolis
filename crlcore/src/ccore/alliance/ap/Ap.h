// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./alliance/ap/Ap.h"                           |
// +-----------------------------------------------------------------+


#include  <string>
using namespace std;

namespace Hurricane {
  class Cell;
}


#ifndef CRL_AP_H
#define CRL_AP_H


namespace CRL {

  using namespace Hurricane;

// -------------------------------------------------------------------
// functions.

  void  apParser ( const string cellPath, Cell* cell );
  void  apDriver ( const string cellPath, Cell* cell, unsigned int& saveState);

}

#endif  // CRL_AP_H
