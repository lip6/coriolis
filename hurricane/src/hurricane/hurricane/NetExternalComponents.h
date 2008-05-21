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

namespace Hurricane {

Components getExternalComponents(const Net* net);

void setExternal(Component* component);


} // End of Hurricane namespace.

#endif // HURRICANE_NET_EXTERNAL_COMPONENTS
