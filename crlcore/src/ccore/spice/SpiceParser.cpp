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
// |  C++ Module  :   "./spice/SpiceParser.cpp"                      |
// +-----------------------------------------------------------------+


#include <ctime>
#include <cstdio>
#include <string>
#include <bitset>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include "hurricane/configuration/Configuration.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Spice.h"
using namespace CRL;


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::Spice".

  bool  Spice::load ( Library* library, string spicePath )
  {
  //DebugSession::open( 101, 110 );
    UpdateSession::open();

    cerr << Error( "Spice::load(): SPICE parser is not implemented yet.\n"
                 "        \"%s\"."
                 , spicePath.c_str()
                 ) << endl;

    UpdateSession::close();
  //DebugSession::close();

    return false;
  }


}  // CRL namespace.
