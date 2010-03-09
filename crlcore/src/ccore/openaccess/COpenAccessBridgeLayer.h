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
