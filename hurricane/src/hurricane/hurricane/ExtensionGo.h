
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
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ExtensionGo.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_EXTENSION_GO__
#define  __HURRICANE_EXTENSION_GO__

#include  "hurricane/Name.h"
#include  "hurricane/Go.h"
#include  "hurricane/ExtensionGos.h"


namespace Hurricane {


  class ExtensionGo : public Go {

    public:
    // Methods.
      virtual void        materialize   ();
      virtual void        unmaterialize ();
      virtual const Name& getName       () const = 0;
      virtual Cell*       getCell       () const;
    // Hurricane Managment.
      virtual string      _getTypeName  () const;
      virtual string      _getString    () const;
      virtual Record*     _getRecord    () const;

    protected:
    // Internal: Attributes.
              Cell*       _cell;

    protected:
    // Internal: Constructor.
                          ExtensionGo   ( Cell* );
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_EXTENSION_GO__


