
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./solstice/RoutingError.h"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __SOLSTICE_ROUTING_ERROR__
#define  __SOLSTICE_ROUTING_ERROR__

#include <string>
#include <set>

#include  <hurricane/DbU.h>
#include  <hurricane/Occurrence.h>
#include  <hurricane/ExtensionGo.h>

namespace Hurricane {
  class Box;
  class Net;
  class Cell;
  class Name;
  class Occurrence;  
}

namespace Solstice {

  using namespace std;
  using namespace Hurricane;

  class RoutingError : public ExtensionGo
  {

  public :
    inline          Cell *                 getCell                ()        const    ;
    inline          Box                    getBoundingBox         ()        const    ;
    inline          int                    getCount               ()        const    ;
    inline          set<Occurrence>*       getAllOccurrences      ()        const    ;
    /**/   virtual  set<Occurrence>*       getErrorOccurrences    ()        const = 0;
    inline          void                   destroy                ();
							            
    //Champs ListBox					            
    /**/   virtual  const Name&            getName                ()        const = 0;
    /**/   virtual  string                 getErrorName           ()        const = 0;
    /**/   virtual  string                 getErrorDescription    ()        const = 0;
 
    //ExtensionGO inherited
    /**/            void                   translate              ( const DbU::Unit& dx,
								    const DbU::Unit& dy );

  protected:
    inline          void                  _postCreate             ();    
    inline          void                  _preDestroy             (); 
    /**/                                   RoutingError           (Cell *  cell,
								   int     count);
    virtual                               ~RoutingError           ();


  public :
    static          const Name            _extensionName;
    
  protected :
    
    /**/            Cell *                _cell;
    /**/            Box                   _area;
    /**/            int                   _count;
    /**/            set<Occurrence>*      _occurrences;
    
  };

  inline  Cell *                 RoutingError::getCell             () const { return _cell;};
  inline  Box                    RoutingError::getBoundingBox      () const { return _area;};
  inline  set<Occurrence>*       RoutingError::getAllOccurrences   () const { return _occurrences;};
  inline  int                    RoutingError::getCount            () const { return _count;};

  inline  void                   RoutingError::_postCreate          ()  { ExtensionGo::_postCreate();};
  inline  void                   RoutingError::_preDestroy          ()  { ExtensionGo::_preDestroy(); _occurrences->clear(); };
  inline  void                   RoutingError::destroy              ()  { _preDestroy(); delete this; };
}  // End of Solstice namespace.

INSPECTOR_P_SUPPORT(Solstice::RoutingError);


#endif   // __SOLSTICE_ROUTING_ERROR__
