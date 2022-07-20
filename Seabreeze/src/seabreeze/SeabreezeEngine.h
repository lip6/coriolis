// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./seabreeze/SeabreezeEngine.h"                 |
// +-----------------------------------------------------------------+

#pragma  once
#include <string>
#include <iostream>

#include "hurricane/Name.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/RoutingPad.h"

namespace Hurricane {
  class Net;
  class Cell;
}

#include "crlcore/ToolEngine.h"
#include "seabreeze/Configuration.h"

namespace Seabreeze {
  
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using Hurricane::RoutingPad;
  using CRL::ToolEngine;


//----------------------------------------------------------
// Class : "Seabreeze::SeabreezeEngine"

  class SeabreezeEngine : public ToolEngine {
    public :
      typedef ToolEngine Super;
    public :
      static  const Name&          staticGetName    ();
      static  SeabreezeEngine*     create           ( Cell* );
      static  SeabreezeEngine*     get              ( const Cell* );
    public :                                        
      inline  CellViewer*          getViewer        () const;
      inline  ToolEngine*          base             ();
      virtual const Name&          getName          () const;
      inline  const Configuration* getConfiguration () const;
      inline  double               getRct           () const;
      inline  double               getRsm           () const;
      inline  double               getCsm           () const;
      inline  void                 setViewer        ( CellViewer* );
      virtual Record*              _getRecord       () const;
      virtual std::string          _getString       () const;
      virtual std::string          _getTypeName     () const;
      virtual void                 buildElmore      ( Net* net );
    protected :                                 
                                   SeabreezeEngine  ( Cell* );
      virtual                     ~SeabreezeEngine  ();
      virtual void                 _postCreate      ();
      virtual void                 _preDestroy      ();
    private :                                       
                                   SeabreezeEngine  ( const SeabreezeEngine& ) = delete;
              SeabreezeEngine&     operator=        ( const SeabreezeEngine& ) = delete;
    private :
    // Attributes.
      static  Name           _toolName;
    protected :
              Configuration* _configuration;
              CellViewer*    _viewer;
  };


// Inline Functions.
  inline       ToolEngine*    SeabreezeEngine::base             () { return static_cast<ToolEngine*>(this); }
  inline const Configuration* SeabreezeEngine::getConfiguration () const { return _configuration; }
  inline       double         SeabreezeEngine::getRct           () const { return getConfiguration()->getRct(); }
  inline       double         SeabreezeEngine::getRsm           () const { return getConfiguration()->getRsm(); }
  inline       double         SeabreezeEngine::getCsm           () const { return getConfiguration()->getCsm(); }
  inline       CellViewer*    SeabreezeEngine::getViewer        () const { return _viewer; }
  inline       void           SeabreezeEngine::setViewer        ( CellViewer* viewer ) { _viewer = viewer; }

} // Seabreeze namespace.

INSPECTOR_P_SUPPORT(Seabreeze::SeabreezeEngine);
