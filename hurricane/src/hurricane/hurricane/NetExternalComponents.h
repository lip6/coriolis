
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
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./NetExternalComponents.h"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_NET_EXTERNAL_COMPONENTS__
#define __HURRICANE_NET_EXTERNAL_COMPONENTS__

#include  "hurricane/Component.h"
#include  "hurricane/Relation.h"


namespace Hurricane {


  class NetExternalComponents {
    public:
      static Components        get         ( const Net* );
      static void              setExternal ( Component* );
      static bool              isExternal  ( Component* );
    protected:
      static StandardRelation* getRelation ( const Net* );
    private:
      static const Name  _name;
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_NET_EXTERNAL_COMPONENTS__
