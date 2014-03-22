// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   S m u r f  -  A   D e m o   T o o l E n g i n e               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./smurf/SmurfEngine.h"                     |
// +-----------------------------------------------------------------+


#ifndef  SMURF_SMURF_ENGINE_H
#define  SMURF_SMURF_ENGINE_H

#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
}


#include "crlcore/ToolEngine.h"
#include "smurf/Configuration.h"


namespace Smurf {

  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Record;


// -------------------------------------------------------------------
// Class  :  "Smurf::SmurfEngine".

  class SmurfEngine : public CRL::ToolEngine {

    public:
      static  const Name&      staticGetName    ();
      static  SmurfEngine*   create           ( Cell* );
      static  SmurfEngine*   get              ( const Cell* );
    public:                                     
      virtual Configuration*   getConfiguration ();
      virtual const Name&      getName          () const;
              void             place            ( unsigned int slowMotion=0 );
      virtual Record*          _getRecord       () const;
      virtual std::string      _getString       () const;
      virtual std::string      _getTypeName     () const;

    private:
    // Attributes.
      static Name                   _toolName;
    protected:
             Configuration*         _configuration;

    protected:
    // Constructors & Destructors.
                             SmurfEngine ( Cell* );
      virtual               ~SmurfEngine ();
      virtual void           _postCreate   ();
      virtual void           _preDestroy   ();
    private:
                             SmurfEngine ( const SmurfEngine& );
              SmurfEngine& operator=     ( const SmurfEngine& );
  };


// Inline Functions.


// Variables.
  extern const char* missingSmurf;


}  // Smurf namespace.


INSPECTOR_P_SUPPORT(Smurf::SmurfEngine);


#endif  // SMURF_SMURF_ENGINE_H
