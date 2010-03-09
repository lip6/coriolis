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
