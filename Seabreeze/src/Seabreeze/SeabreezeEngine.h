#ifndef  SEABREEZE_SEABREEZE_ENGINE_H
#define  SEABREEZE_SEABREEZE_ENGINE_H

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
#include "Seabreeze.h"

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
      static  const Name&      staticGetName   ();
      static  SeabreezeEngine* create          ( Cell* );
      static  SeabreezeEngine* get             ( const Cell* );
    public :
      inline  CellViewer*      getViewer       () const;
      inline  ToolEngine*      base            ();
      virtual const Name&      getName         () const;
      inline  void             setViewer       ( CellViewer* );
      virtual Record*          _getRecord      () const;
      virtual std::string      _getString      () const;
      virtual std::string      _getTypeName    () const;
      virtual void             runTool         ( Net* net );
    protected :
                               SeabreezeEngine ( Cell* );
      virtual                 ~SeabreezeEngine ();
      virtual void             _postCreate     ();
      virtual void             _preDestroy     ();
    private :
                               SeabreezeEngine ( const SeabreezeEngine& );
              SeabreezeEngine& operator=       ( const SeabreezeEngine& );
    private :
    // Attributes.
      static  Name           _toolName;
    protected :
              CellViewer* _viewer;
  };

// Inline Functions.
  inline CellViewer*    SeabreezeEngine::getViewer () const { return _viewer; }
  inline ToolEngine*    SeabreezeEngine::base      () { return static_cast<ToolEngine*>(this); }
  inline void           SeabreezeEngine::setViewer ( CellViewer* viewer ) { _viewer = viewer; }
} // Seabreeze namespace.

INSPECTOR_P_SUPPORT(Seabreeze::SeabreezeEngine);

#endif // SEABREEZE_SEABREEZE_ENGINE_H
