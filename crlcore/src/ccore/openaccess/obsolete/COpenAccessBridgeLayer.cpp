// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
#include "COpenAccessBridgeLayer.h"

namespace CRL {

BasicLayer::Material::Code COpenAccessBridgeLayer::oaMaterialToBasicLayerType(const oaMaterial& material) {
    switch(material) {
        case oacMetalMaterial:
        case oacContactlessMetalMaterial:
            return BasicLayer::Material::metal;
        default:
            return BasicLayer::Material::other;
    }
}

}
