// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GraphicToolEngine.cpp"                  |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Warning.h"
#include "crlcore/GraphicToolEngine.h"


namespace CRL {

  using namespace std;
  using Hurricane::Warning;
    

// -------------------------------------------------------------------
// Class  :  "CRL::GraphicTool".


  GraphicTool::GraphicTool ()
    : _drawGoMap()
  { }


  GraphicTool::~GraphicTool ()
  { }


  void  GraphicTool::addDrawGo ( const Name&                    name
                               , CellWidget::InitExtensionGo_t* initFunction
                               , CellWidget::DrawExtensionGo_t* drawFunction
                               )
  {
    map<Name,DrawGoFunctions>::iterator idraw = _drawGoMap.find ( name );
    if ( idraw != _drawGoMap.end() )
      cerr << Warning("Overwriting %s DrawGo functions.",getString(name).c_str()) << endl;

    _drawGoMap.insert ( make_pair(name,DrawGoFunctions(initFunction,drawFunction)) );
  }


  GraphicTool::DrawGoFunctions* GraphicTool::getDrawGo ( const Name& name )
  {
    map<Name,DrawGoFunctions>::iterator idraw = _drawGoMap.find ( name );
    if ( idraw != _drawGoMap.end() )
      return &idraw->second;

    return NULL;
  }
  


} // End of CRL namespace.
