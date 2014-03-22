// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/EtesianEngine.h"                     |
// +-----------------------------------------------------------------+


#ifndef  ETESIAN_ETESIAN_ENGINE_H
#define  ETESIAN_ETESIAN_ENGINE_H

#include <iostream>
#include <unordered_map>

//#include "coloquinte/circuit_graph.hxx"
namespace Coloquinte {
  struct circuit;
}

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
}

#include "crlcore/ToolEngine.h"
#include "etesian/Configuration.h"


namespace Etesian {

  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Record;


// -------------------------------------------------------------------
// Class  :  "Etesian::EtesianEngine".

  class EtesianEngine : public CRL::ToolEngine {

    public:
      static  const Name&      staticGetName    ();
      static  EtesianEngine*   create           ( Cell* );
      static  EtesianEngine*   get              ( const Cell* );
    public:                                     
      virtual Configuration*   getConfiguration ();
      virtual const Name&      getName          () const;
              void             place            ( unsigned int slowMotion=0 );
              void             _updatePlacement ();
      virtual Record*          _getRecord       () const;
      virtual std::string      _getString       () const;
      virtual std::string      _getTypeName     () const;

    private:
    // Attributes.
      static Name                                     _toolName;
    protected:
             Configuration*                           _configuration;
             Coloquinte::circuit*                     _circuit;
             std::unordered_map<string,unsigned int>  _cellsToIds;

    protected:
    // Constructors & Destructors.
                             EtesianEngine ( Cell* );
      virtual               ~EtesianEngine ();
      virtual void           _postCreate   ();
      virtual void           _preDestroy   ();
    private:
                             EtesianEngine ( const EtesianEngine& );
              EtesianEngine& operator=     ( const EtesianEngine& );
  };


// Inline Functions.


// Variables.
  extern const char* missingEtesian;


}  // Etesian namespace.


INSPECTOR_P_SUPPORT(Etesian::EtesianEngine);


#endif  // ETESIAN_ETESIAN_ENGINE_H
