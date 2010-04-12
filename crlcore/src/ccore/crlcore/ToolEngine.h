
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ToolEngine.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef __CRL_TOOL_ENGINE__
#define __CRL_TOOL_ENGINE__


#include  <string>
#include  <vector>

#include  "hurricane/Commons.h"
#include  "hurricane/DBo.h"
#include  "hurricane/Slot.h"

namespace Hurricane {
  class Name;
  class Cell;
}

#include  "crlcore/ToolEngines.h"


namespace CRL {


  using std::string;
  using std::vector;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Cell;
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEngine".

  
  class ToolEngine : public DBo {
    public:
    // Static Methods.
      static  ToolEngines  get                                 ( const Cell* cell );
      static  ToolEngine*  get                                 ( const Cell* cell, const Name& name );
      static  void         destroyAll                          ();
    // Methods.
      virtual const Name&  getName                             () const = 0;
      inline  Cell*        getCell                             () const;
              bool         placementModificationFlagHasChanged ();
              bool         routingModificationFlagHasChanged   ();
      inline  void         setInRelationDestroy                ( bool );
      virtual string       _getTypeName                        () const;
      virtual string       _getString                          () const;
      virtual Record*      _getRecord                          () const;

    protected:
    // Internal: Attributes
              Cell*        _cell;
    private:
              unsigned int _placementModificationFlag;
              unsigned int _routingModificationFlag;
              bool         _inRelationDestroy;

    // Internal: Constructors & Destructors.
    protected:
                           ToolEngine                          ( Cell* cell );
      virtual void         _postCreate                         ();
      virtual void         _preDestroy                         ();
    // Internal: Methods.
              void         grabPlacementModificationFlag       ();
              void         getPlacementModificationFlag        ();
              void         grabRoutingModificationFlag         ();
              void         getRoutingModificationFlag          ();
  };


// -------------------------------------------------------------------
// Inline Functions.


  inline  Cell* ToolEngine::getCell              () const { return _cell; }
  inline  void  ToolEngine::setInRelationDestroy ( bool state ) { _inRelationDestroy = state; }


} // End of CRL namespace.


#endif // __CRL_TOOL_ENGINE__
