
#ifndef __OPENACCESSUTILS_H__
#define __OPENACCESSUTILS_H__

#include <iostream>
using namespace std;

#ifdef HAVE_OPENACCESS
#include "oa/oaDesignDB.h"
using namespace oa;

#include "hurricane/Cell.h"
#include "hurricane/BasicLayer.h"
using namespace Hurricane;

#undef assert
#define assert(cond) if (! (cond) ) throw Error("assertion failed : " + string( #cond ) )
//#define assert(cond)

namespace {    

    /**
       giving a oaDesign pointer that should be not NULL
       return the oaString representing its name
    */
    oaString getDesignName(oaDesign* design) {
        cerr << "getDesignName" << endl;
        assert(design);
        oaNativeNS ns;
        oaString libName, cellName, viewName;
        design->getLibName(ns, libName);
        design->getCellName(ns, cellName);
        design->getViewName(ns, viewName);
        oaString designName = "<" + libName + "," + cellName + "," + viewName + ">";
        return designName;
    }

    /**
       giving a oaBlock
       print the connectivity, mainly used for debug purpose ...
       @todo remove when not needed anymore
    */
    void printBlockTerms(oaBlock* block) {
        cerr << "printBlockTerms" << endl;
        assert(block);
        oaNativeNS ns;
        oaDesign* design = block->getDesign();
        cerr << " o Printing " << getDesignName(design) << " terms" << endl;
        oaIter<oaTerm> termIter(block->getTerms());
        while (oaTerm* term = termIter.getNext()) {
            oaString termName;
            term->getName(ns, termName);
            cerr << "  - " << termName << endl;
        }
        cerr << endl;
    }

    /**
       Convert material from Hurricane to OA ...
       @todo verify
    */
    oaMaterial getOAMaterial(const BasicLayer::Material&  material) {
        cerr << "getOAMaterial" << endl;
        switch ( material.getCode() ) {
        case BasicLayer::Material::nWell:    return oacNWellMaterial;
        case BasicLayer::Material::pWell:    return oacPWellMaterial;
        case BasicLayer::Material::nImplant: return oacNImplantMaterial;
        case BasicLayer::Material::pImplant: return oacPImplantMaterial;
        case BasicLayer::Material::active:   return oacOtherMaterial;//is it OK?
        case BasicLayer::Material::poly:     return oacPolyMaterial;
        case BasicLayer::Material::cut:      return oacCutMaterial;
        case BasicLayer::Material::metal:    return oacMetalMaterial;
        case BasicLayer::Material::blockage: 
            //there is no blockage type but a specific oaLayerBlockage class
            return oacOtherMaterial;
        case BasicLayer::Material::other:    return oacOtherMaterial;
        default:
            throw Error("Unrecognized material");
        }
    }

    /**
       Convertion helper for Net convertion ...
       @todo verify
    */
    oaTermType getOATermType(const Net::Direction& direction) {
        cerr << "getOATermType" << endl;
        switch (direction) {
        case Net::Direction::IN:
            return oacInputTermType;
        case Net::Direction::OUT:
            return oacOutputTermType;
        case Net::Direction::INOUT:
            return oacInputOutputTermType;
        case Net::Direction::TRISTATE:
            return oacTristateTermType;
        case Net::Direction::UNDEFINED:
            return oacUnusedTermType;// is it OK ?
        default:
            throw Error("Unrecognized direction");
        }
    }

    /**
       Convertion helper for Net convertion ...
       @todo verify
    */
    oaSigType getOASigType(const Net::Type& type) {
        cerr << "getOASigType" << endl;
        switch (type.getCode()) {
        case Net::Type::LOGICAL:
            return oacSignalSigType;
        case Net::Type::CLOCK:
            return oacClockSigType;
        case Net::Type::POWER:
            return oacPowerSigType;
        case Net::Type::GROUND:
            return oacGroundSigType;
        case Net::Type::UNDEFINED:
            return oacAnalogSigType;// is it OK ?
        default:
            throw Error("Unrecognized net type");
        }
    }

    /**
       Convertion helper ...
     */
    oaOrient getOAOrientFromOrientation(const Transformation::Orientation& orientation) {
        cerr << "getOAOrientFromOrientation" << endl;
        switch (orientation) {
        case Transformation::Orientation::ID:
            return oacR0;
        case Transformation::Orientation::R1:
            return oacR90;
        case Transformation::Orientation::R2:
            return oacR180;
        case Transformation::Orientation::R3:
            return oacR270;
        case Transformation::Orientation::MX:
            return oacMX;
        case Transformation::Orientation::XR:
            return oacMXR90;
        case Transformation::Orientation::MY:
            return oacMY;
        case Transformation::Orientation::YR:
            return oacMYR90;
        default:
            throw Error("Unrecognized orientation");
        }
    }

    /**
       Convertion helper ...
     */
    void getOATransformFromTransformation(oaTransform& transform, const Transformation& transformation) {
        cerr << "getOATransformFromTransformation" << endl;
        transform.set(transformation.getTx(),
                      transformation.getTy(),
                      getOAOrientFromOrientation(transformation.getOrientation()));
    }

    /**
       Convertion helper ...
     */
    void getOABoxForBox(oaBox& box, const Box& hbox) {
        cerr << "getOABoxForBox" << endl;
        box.set(hbox.getXMin(), hbox.getYMin(), hbox.getXMax(), hbox.getYMax());
    }

    /**
       Create InstTerm representing connection of nets ...
       always return a non NULL value
    */
    oaInstTerm* getInstTerm(oaInst* inst, Plug* plug,oaNet* net) {
        cerr << "getInstTerm" << endl;
        assert(inst);
        assert(plug);
        oaNativeNS ns;
        oaScalarName scPlugName(ns, getString(plug->getMasterNet()->getName()).c_str());
        oaName instTermName(scPlugName);
        oaInstTerm* instTerm = oaInstTerm::find(inst, instTermName);
        if (instTerm) {
            return instTerm;
        }
        oaDesign* design = inst->getMaster();
        assert(design);
        oaBlock* masterBlock = design->getTopBlock();
        oaTerm* term = oaTerm::find(masterBlock, instTermName);
        assert(term);
        cerr << "looking for " << plug->getName() << endl;
        printBlockTerms(masterBlock);
        cerr << "oaInstTerm::create" << endl;
        instTerm = oaInstTerm::create(net, inst, term);
        assert(instTerm);
        return instTerm;
    }

    /**
       save design stored in a map
    */
    void saveDesignsInMap(map<const Cell*, oaDesign*> cell2OAdesign){
        for (map<const Cell*, oaDesign*>::iterator it = cell2OAdesign.begin();
             it != cell2OAdesign.end();
             ++it) {
            cerr << it->first << endl;
            oaDesign* design = it->second;
            design->save();
            design->close();
        }
    }

    /**
       print the oaLayera in a oaTech ...
    */
    void printOALayers(oaTech* theOATech){
        cerr << "printOALayers" << endl;
        assert(theOATech);
        oaIter<oaLayer> lIter(theOATech->getLayers());
        while(oaLayer* l = lIter.getNext()){
            oaString layerName;
            l->getName(layerName);
            cerr << " o created oaLayer " << layerName << endl;
        }
    }

    /**
       @todo complete,verify ...
     */
    BasicLayer::Material::Code oaMaterialToBasicLayerType(const oaMaterial& material) {
        switch(material) {
        case oacNWellMaterial:
            return BasicLayer::Material::nWell;
        case oacPWellMaterial:
            return BasicLayer::Material::pWell;
        case oacNImplantMaterial:
            return BasicLayer::Material::nImplant;
        case oacPImplantMaterial:
            return BasicLayer::Material::pImplant;
        case oacPolyMaterial:
            return BasicLayer::Material::poly;
        case oacCutMaterial:
            return BasicLayer::Material::cut;            
        case oacMetalMaterial:
        case oacContactlessMetalMaterial:
            return BasicLayer::Material::metal;
        default:
            return BasicLayer::Material::other;
        }
    }


}//end anonymous namespace

#endif//HAVE_OPENACCESS

#endif//__OPENACCESSUTILS_H__
