// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universié 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./foehn/FoehnEngine.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <vector>
namespace Hurricane {
  class Instance;
  class CellViewer;
}
#include "hurricane/Plug.h"
#include "crlcore/ToolEngine.h"
#include "foehn/Configuration.h"
#include "foehn/Dag.h"


namespace Foehn {

  using std::string;
  using std::vector;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DBo;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Plug;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


// -------------------------------------------------------------------
// Class  :  "Foehn::FoehnEngine".


  class FoehnEngine : public ToolEngine {
    public:
      typedef ToolEngine  Super;
    public:
      static        FoehnEngine*          create                ( Cell* );
      static        FoehnEngine*          get                   ( const Cell* );
      static  const Name&                 staticGetName         ();
      virtual const Name&                 getName               () const;
      inline        Configuration&        getConfiguration      ();
                    Dag*                  getDag                ( std::string label ) const;
                    Dag*                  newDag                ( std::string label );
                    void                  clear                 ();
      inline        CellViewer*           getViewer             () const;
      inline        void                  setViewer             ( CellViewer* );
    // Inspector support.                                       
      virtual       Record*               _getRecord            () const;
      virtual       string                _getString            () const;
      virtual       string                _getTypeName          () const;
    protected:                                                     
                                          FoehnEngine           ( Cell* );
      virtual                         ~   FoehnEngine           ();
      virtual       void                  _postCreate           ();
      virtual       void                  _preDestroy           ();
    private:                                                        
                                          FoehnEngine           ( const FoehnEngine& ) = delete;
                    FoehnEngine&          operator=             ( const FoehnEngine& ) = delete;
    private:
      static Name                    _toolName;
             CellViewer*             _viewer;
             Configuration           _configuration;
             std::vector<Dag*>       _dags;
  };

  
  inline Configuration& FoehnEngine::getConfiguration () { return _configuration; }
  inline CellViewer*    FoehnEngine::getViewer        () const { return _viewer; }
  inline void           FoehnEngine::setViewer        ( CellViewer* viewer ) { _viewer = viewer; }


}  // Foehn namespace.


INSPECTOR_P_SUPPORT(Foehn::FoehnEngine);
