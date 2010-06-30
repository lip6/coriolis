// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
#ifndef  __COPENACCESS_BRIDGE_LAYER_H__
#define  __COPENACCESS_BRIDGE_LAYER_H__

#include "hurricane/BasicLayer.h"
using namespace Hurricane;

#include "oa/oaDesignDB.h"
using namespace oa;

namespace CRL {

class COpenAccessBridgeLayer {
    public:
        static BasicLayer::Material::Code oaMaterialToBasicLayerType(const oaMaterial& material);
};

}

#endif /* __COPENACCESS_BRIDGE_LAYER_H__ */ 
