// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/TramontanaEngine.h"              |
// +-----------------------------------------------------------------+


#pragma  once
#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellViewer;
}
#include "crlcore/ToolEngine.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TramontanaEngine".

  class TramontanaEngine : public ToolEngine {
    public:
      typedef  ToolEngine  Super;
    public:
      static  const Name&              staticGetName              ();
      static  TramontanaEngine*        create                     ( Cell* );
      static  TramontanaEngine*        get                        ( const Cell* );
    public:                                                      
              const Name&              getName                    () const;
      inline  const std::set<Equipotential*,DBo::CompareById>
                                       getEquipotentials () const;
      inline        void               setViewer                  ( CellViewer* );
      inline        CellViewer*        getViewer                  ();
              void                     extract                    ();
              void                     consolidate                ();
              void                     showEquipotentials         () const;
              void                     add                        ( Equipotential* );
      virtual Record*                  _getRecord                 () const;
      virtual std::string              _getString                 () const;
      virtual std::string              _getTypeName               () const;
    private:                          
    // Attributes.                    
      static  Name                     _toolName;
    private:                         
              CellViewer*              _viewer;
              std::set<Equipotential*,DBo::CompareById>  _equipotentials;
    protected:
    // Constructors & Destructors.
                                TramontanaEngine ( Cell* );
      virtual                  ~TramontanaEngine ();
      virtual void              _postCreate      ();
      virtual void              _preDestroy      ();
    private:
                                TramontanaEngine ( const TramontanaEngine& ) = delete;
              TramontanaEngine& operator=        ( const TramontanaEngine& ) = delete;
  };

  
  inline void        TramontanaEngine::setViewer ( CellViewer* viewer ) { _viewer=viewer; }
  inline CellViewer* TramontanaEngine::getViewer () { return _viewer; }
  inline const std::set<Equipotential*,DBo::CompareById>
                     TramontanaEngine::getEquipotentials () const { return _equipotentials; }


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::TramontanaEngine);
