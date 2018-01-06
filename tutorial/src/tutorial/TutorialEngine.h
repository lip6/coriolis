// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./tutorial/TutorialEngine.h"                   |
// +-----------------------------------------------------------------+


#ifndef  TUTORIAL_TUTORIAL_ENGINE_H
#define  TUTORIAL_TUTORIAL_ENGINE_H

#include <string>
#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellViewer;
}

#include "crlcore/ToolEngine.h"


namespace Tutorial {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


// -------------------------------------------------------------------
// Class  :  "Tutorial::TutorialEngine".

  class TutorialEngine : public ToolEngine {
    public:
      typedef  ToolEngine  Super;
    public:
      static  const Name&     staticGetName   ();
      static  TutorialEngine* create          ( Cell* );
      static  TutorialEngine* get             ( const Cell* );
    public:                                                      
      inline  CellViewer*     getViewer       () const;
      inline  ToolEngine*     base            ();
      virtual const Name&     getName         () const;
      inline  void            setViewer       ( CellViewer* );
              Cell*           runDemoPart1    ();
              void            runDemoPart2    ();
      virtual Record*         _getRecord      () const;
      virtual std::string     _getString      () const;
      virtual std::string     _getTypeName    () const;
    protected:
    // Constructors & Destructors.
                              TutorialEngine  ( Cell* );
      virtual                ~TutorialEngine  ();
      virtual void            _postCreate     ();
      virtual void            _preDestroy     ();
    private:
                              TutorialEngine  ( const TutorialEngine& );
              TutorialEngine& operator=       ( const TutorialEngine& );
    private:                          
    // Attributes.                    
      static  Name        _toolName;
    protected:                              
              CellViewer* _viewer;
  };


// Inline Functions.
  inline  CellViewer* TutorialEngine::getViewer () const { return _viewer; }
  inline  ToolEngine* TutorialEngine::base      () { return static_cast<ToolEngine*>(this); }
  inline  void        TutorialEngine::setViewer ( CellViewer* viewer ) { _viewer=viewer; }


}  // Tutorial namespace.


INSPECTOR_P_SUPPORT(Tutorial::TutorialEngine);

#endif  // TUTORIAL_TUTORIAL_ENGINE_H
