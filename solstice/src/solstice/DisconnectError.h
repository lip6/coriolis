
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
// |  C++ Header  :       "./solstice/DisconnectError.h"             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __SOLSTICE_DISCONNECT_ERROR__
#define  __SOLSTICE_DISCONNECT_ERROR__

#include <solstice/RoutingError.h>

namespace Solstice {

  

  class DisconnectError : public RoutingError 
  {
    
  public :
    inline static  DisconnectError*       create                 (Cell*              cell, 
								  const Occurrence&  occurrence,
								  int                count);

  public :
    //Champs ListBox

    inline virtual const Name&            getName                () const ;
    inline virtual string                 getErrorName           () const ;
    /**/   virtual string                 getErrorDescription    () const ;
    inline virtual set<Occurrence>*       getErrorOccurrences    () const ;


  private : 
    /**/                                   DisconnectError        (Cell*              cell, 
								   const Occurrence&  occurrence,
								   int                count);
    /**/    virtual                       ~DisconnectError        ()       ;
  private : 
    /**/            Occurrence            _occurrence;


  };

  //inlines 
  
  inline         set<Occurrence>*       DisconnectError::getErrorOccurrences     () const {return _occurrences;};
  inline         const Name&            DisconnectError::getName                 () const {return _extensionName;} ;
  inline         string                 DisconnectError::getErrorName            () const {return "Deconnexion";};
  inline         DisconnectError*       DisconnectError::create                  (Cell* cell,  const Occurrence&  occurrence, int count) { DisconnectError* de = new DisconnectError (cell,occurrence,count); de->_postCreate(); return de;};
 


}  // End of Solstice namespace.

#endif   // __SOLSTICE_DISCONNECT_ERROR__
