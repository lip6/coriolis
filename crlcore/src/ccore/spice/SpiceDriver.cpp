
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           S P I C E  / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./spice/SpiceDriver.cpp"                      |
// +-----------------------------------------------------------------+


#include <ctime>
#include <cmath>
#include <cstdio>
#include <cfenv>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include "hurricane/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Polygon.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Cell.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Spice.h"
#include "crlcore/SpiceBit.h"
#include "crlcore/SpiceEntity.h"
using namespace CRL;



namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::Spice".

  bool  Spice::save ( Cell* cell, uint64_t flags )
  {
    ::Spice::Entity* spiceEntity = ::Spice::EntityExtension::create( cell, flags );

    string cellFile = getString(cell->getName()) + ".spi";
    ofstream cellStream ( cellFile );
    spiceEntity->toEntity( cellStream );
    cellStream.close();

    return true;
  }


  void  Spice::clearProperties ()
  {
    ::Spice::EntityExtension::destroyAll();
  }


}  // CRL namespace.
