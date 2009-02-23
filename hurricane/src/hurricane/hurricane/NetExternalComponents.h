// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: NetExternalComponents.h
// Authors: C. Alexandre
// ****************************************************************************************************

#ifndef HURRICANE_NET_EXTERNAL_COMPONENTS
#define HURRICANE_NET_EXTERNAL_COMPONENTS

#include "hurricane/Component.h"
#include "hurricane/Relation.h"

namespace Hurricane {

  class NetExternalComponents {
    public:
      static Components        get         ( const Net* );
      static void              setExternal ( Component* );
    protected:
      static StandardRelation* getRelation ( const Net* );
    private:
      static const Name  _name;
  };

} // End of Hurricane namespace.

#endif // HURRICANE_NET_EXTERNAL_COMPONENTS
