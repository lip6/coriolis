// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./alliance/vst/Vst.h"                          |
// +-----------------------------------------------------------------+


#pragma once
#include <string>


namespace Hurricane {
  class Cell;
}


namespace CRL {


  void  vstParser ( const std::string cellPath, Hurricane::Cell* );
  void  vstDriver ( const std::string cellPath, Hurricane::Cell* , unsigned int& saveState);


}
