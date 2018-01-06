// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/ToolEngine.h"                        |
// +-----------------------------------------------------------------+



#ifndef CRL_TOOL_ENGINE_H
#define CRL_TOOL_ENGINE_H

#include <string>
#include <vector>
#include "hurricane/Commons.h"
#include "hurricane/Timer.h"
#include "hurricane/DBo.h"
#include "hurricane/Slot.h"

namespace Hurricane {
  class Name;
  class Cell;
}

#include  "crlcore/ToolEngines.h"


namespace CRL {

  using std::string;
  using std::vector;
  using Hurricane::Timer;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Cell;
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEngine".

  class ToolEngine : public DBo {
    public:
      typedef  DBo  Super;
    public:
      static        ToolEngines  get                                 ( const Cell* cell );
      static        ToolEngine*  get                                 ( const Cell* cell, const Name& name );
      static        void         destroyAll                          ();
      static        bool         inDestroyAll                        ();
    public:
      virtual const Name&        getName                             () const = 0;
      inline        Cell*        getCell                             () const;
                    bool         placementModificationFlagHasChanged ();
                    bool         routingModificationFlagHasChanged   ();
      inline        void         setInRelationDestroy                ( bool );
      inline  const Timer&       getTimer                            () const;
                    void         startMeasures                       ();
                    void         stopMeasures                        ();
                    void         suspendMeasures                     ();
                    void         resumeMeasures                      ();
                    void         printMeasures                       () const;
      virtual       string       _getTypeName                        () const;
      virtual       string       _getString                          () const;
      virtual       Record*      _getRecord                          () const;
    private:
      static        bool         _inDestroyAll;
    protected:
                    Cell*        _cell;
    private:
                    unsigned int _placementModificationFlag;
                    unsigned int _routingModificationFlag;
                    bool         _inRelationDestroy;
                    Timer        _timer;
    protected:
                                 ToolEngine                          ( Cell* cell );
      virtual       void         _postCreate                         ();
      virtual       void         _preDestroy                         ();
    protected:      
                    void         grabPlacementModificationFlag       ();
                    void         getPlacementModificationFlag        ();
                    void         grabRoutingModificationFlag         ();
                    void         getRoutingModificationFlag          ();
  };


// -------------------------------------------------------------------
// Inline Functions.


  inline       Cell*  ToolEngine::getCell              () const { return _cell; }
  inline       void   ToolEngine::setInRelationDestroy ( bool state ) { _inRelationDestroy = state; }
  inline const Timer& ToolEngine::getTimer             () const { return _timer; }


} // CRL namespace.

#endif // CRL_TOOL_ENGINE_H
