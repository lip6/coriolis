// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-08-16 13:05:27" - OpenAccessDriver.cpp
// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x


#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"

// Go
#include "hurricane/Instance.h"
#include "hurricane/Component.h"
#include "hurricane/Segment.h"
#include "hurricane/Vertical.h"
#include "hurricane/Pad.h"
// Layer
#include "hurricane/BasicLayer.h"
#include "hurricane/ContactLayer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/TransistorLayer.h"
#include "hurricane/DiffusionLayer.h"
#include "hurricane/RegularLayer.h"

using namespace Hurricane;

#include "OpenAccess.h"
#include "OpenAccessCommon.h"

namespace {
#ifdef HAVE_OPENACCESS
    using namespace CRL_OA;

    /**
       Class to drive OA to Hurricane
    */
    class OADriver {
    private:
        typedef map<const Library*, oaLib*> Library2OALibMap;
        typedef map<const Cell*, oaDesign*> Cell2OADesignMap;
        typedef map<const Cell*, oaCell*> Cell2OACellMap;
        typedef map<const Instance*, oaInst*> Instance2OAInstsMap;
        typedef map<const Layer*, oaLayer*> Layer2OALayerMap;
        typedef map<const ViaLayer*, oaViaDef*> ViaLayer2OAViaDefMap;
        typedef map<const Pad*, oaRect*> Pad2OARectMap;
        typedef map<const Component*, oaPathSeg*> Component2OAPathSegMap;

        string _path;
        oaTech* _oaTech;
        Library2OALibMap _library2OALib;
        Cell2OADesignMap _cell2OADesign4Netlist;
        Cell2OADesignMap _cell2OADesign4Schematic;
        Cell2OADesignMap _cell2OADesign4Symbolic;
        Cell2OADesignMap _cell2OADesign4Layout;
        Cell2OACellMap _cell2OAcell;
        Instance2OAInstsMap _instance2OAInst;
        Layer2OALayerMap _layer2OALayer;
        ViaLayer2OAViaDefMap _viaLayer2OAViaDef;
        Pad2OARectMap _pad2OARect;
        Component2OAPathSegMap _component2OAPathSeg;
        set<int> _layerIDS;
        int _layerID;
        oaLayer* _layerDev;
        oaLayer* _layerPin;
        oaLayer* _layerText;
        oaLayer* _layerWire;
        DataBase* _db;
        Technology* _technology;
    public:
        OADriver(const string& path) :
            _path(path),
            _oaTech(NULL),
            _library2OALib(),
            _cell2OADesign4Netlist(),
            _cell2OADesign4Schematic(),
            _cell2OADesign4Symbolic(),
            _cell2OADesign4Layout(),
            _cell2OAcell(),
            _instance2OAInst(),
            _layer2OALayer(),
            _viaLayer2OAViaDef(),
            _pad2OARect(),
            _component2OAPathSeg(),
            _layerIDS(),
            _layerID(0),
            _layerDev(NULL),
            _layerPin(NULL),
            _layerText(NULL),
            _layerWire(NULL),
            _db(NULL),
            _technology(NULL) {
            _db = DataBase::getDB();
            if (!_db) {
                throw Error("no database");
            }
            _technology = _db->getTechnology();
            if (!_technology) {
                throw Error("no technology");
            }
            oaFuncs::realPath(_path);
            cerr << "realpath: " << _path << endl;
        }

        ~OADriver() {
            cerr << "SAVING ALL" << endl;
            _oaTech->save();
            _oaTech->close();
            oaFuncs::saveOADesignsInMap(_cell2OADesign4Netlist);
            _cell2OADesign4Netlist.clear();
            oaFuncs::saveOADesignsInMap(_cell2OADesign4Schematic);
            _cell2OADesign4Schematic.clear();
            oaFuncs::saveOADesignsInMap(_cell2OADesign4Symbolic);
            _cell2OADesign4Symbolic.clear();
            oaFuncs::saveOADesignsInMap(_cell2OADesign4Layout);
            _cell2OADesign4Layout.clear();
            for (Library2OALibMap::iterator it = _library2OALib.begin();
                 it != _library2OALib.end();
                 ++it) {
                oaLib* lib = it->second;
                lib->close();
            }
            _path.clear();
            cerr << "ALL SAVED" << endl;
        }

        /**
           create a oaLib from a Library
        */
        oaLib* toOALib(const Library* library, bool recursive=false) {
            cerr << "toOALib" << endl;
            assert(library);
            Library2OALibMap::iterator it = _library2OALib.find(library);
            if (it != _library2OALib.end())
                return it->second;

            // 1) create or open library
            cerr << "lib path : " << _path << endl;
            pair<oaScalarName,string> infos = oaFuncs::libInfos(_path,
                                                                getString(library->getName()));
            oaLib *lib = oaFuncs::openOALib(infos);
            _library2OALib[library] = lib;

            if(recursive){
                // 2) for each cell convert them too : if it's a standard cell library for example
                for_each_cell(c ,library->getCells()){
                    toOACell(c);
                    end_for;
                }

                // 3) also convert each contained library if any
                for_each_library(l ,library->getLibraries()){
                    toOALib(l);
                    end_for;
                }
            }

            // 4) create, update library list file
            oaFuncs::createCDS(infos,_path);
            infos.second.clear();

            return lib;
        }

        /**
           handle layerID i.e: get extractNumber in Hurricane world if
           possible ...
        */
        int generateLayerID(const BasicLayer* bLayer){
            // the layer number is unique to a particular layer
            cerr << "generateLayerID -> ";
            int numLayer = _layerID;
            if(bLayer){
                numLayer = bLayer->getExtractNumber();
                if(_layerIDS.find(numLayer) == _layerIDS.end()){
                    cerr << "getExtractNumber " << numLayer << endl;
                    _layerIDS.insert(numLayer);
                    return numLayer;
                }
            }

            cerr << " while(...) ";
            set<int>::iterator it;
            while((it = _layerIDS.find(_layerID)) != _layerIDS.end()){
                numLayer = ++_layerID;
            }
            cerr << numLayer << endl;
            _layerIDS.insert(numLayer);
            return numLayer;
        }

        /**
           Convert material from Hurricane to OA ...
           @todo verify
        */
        static oaMaterial toOAMaterial(const BasicLayer::Material&  material) {
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
           helper function
           technology MUST exist first
         */
        oaLayerNum toOALayerNum(const Layer* hLayer){
            assert(hLayer);
            oaLayer* layer = toOALayer(hLayer,_oaTech);
            assert(layer);
            return layer->getNumber();
        }
        
        /**
           convert oaLayer from a Layer ...
        */
        oaLayer* toOALayer(const Layer* layer,oaTech* theOATech) {
            assert(layer);
            cerr << "toOALayer " << getString(layer->getName()) << endl;
            Layer2OALayerMap::iterator it = _layer2OALayer.find(layer);
            if (it != _layer2OALayer.end())
                return it->second;
            assert(theOATech);

            // 1) get or create layer
            oaString layerName = getString(layer->getName()).c_str();
            oaLayer* aOALayer = NULL;
            aOALayer = oaPhysicalLayer::find(theOATech, layerName, true);
            if(aOALayer){
                _layer2OALayer[layer] = aOALayer;
                _layerIDS.insert(aOALayer->getNumber());
                return aOALayer;
            }
            const BasicLayer* bLayer = dynamic_cast<const BasicLayer*>(layer);
            if(!bLayer){
                cerr << "NOT A BASICLAYER " << layerName;
                const ViaLayer* vLayer = dynamic_cast<const ViaLayer*>(layer);
                if(vLayer)
                    cerr << " but is a ViaLayer" << endl;
                const ContactLayer* cLayer = dynamic_cast<const ContactLayer*>(layer);
                if(cLayer)
                    cerr << " but is a ContactLayer" << endl;
                const DiffusionLayer* dLayer = dynamic_cast<const DiffusionLayer*>(layer);
                if(dLayer)
                    cerr << " but is a DiffusionLayer" << endl;
                const TransistorLayer* tLayer = dynamic_cast<const TransistorLayer*>(layer);
                if(tLayer)
                    cerr << " but is a TransistorLayer" << endl;
                const RegularLayer* rLayer = dynamic_cast<const RegularLayer*>(layer);
                if(rLayer)
                    cerr << " but is a RegularLayer" << endl;
            }
            aOALayer = oaPhysicalLayer::create(theOATech, layerName, generateLayerID(bLayer),
                                               bLayer ? toOAMaterial(bLayer->getMaterial())
                                               : oaMaterial(oacOtherMaterial));
            assert(aOALayer);
            
            _layer2OALayer[layer] = aOALayer;

            //create and add layer constraint for Layer specific manufacturing rules
            cerr << " o    get value for constraint" << endl;
            long minSize = Hurricane::DbU::getDb(layer->getMinimalSize());
            long minSpace = Hurricane::DbU::getDb(layer->getMinimalSpacing());
            long pitch = Hurricane::DbU::getDb(layer->getPitch());
            oaLayerConstraint* cMinSize = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                    oaLayerConstraintDef::get(oacMinWidth),
                                                                    oaIntValue::create(theOATech,minSize));
            oaLayerConstraint* cMinSpace = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                     oaLayerConstraintDef::get(oacMinSpacing),
                                                                     oaIntValue::create(theOATech,minSpace));
            oaLayerConstraint* cPitchH = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacHorizontalRouteGridPitch),
                                                                   oaIntValue::create(theOATech,pitch));
            oaLayerConstraint* cPitchV = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacVerticalRouteGridPitch),
                                                                   oaIntValue::create(theOATech,pitch));
            return aOALayer;
        }

    /**
       create a oaTech from a Hurricane::Technology in it's Library
       also create the oaLib corresponding to the Hurricane::Library
       containing the Hurricane::Technology
       @todo complete with technology info for layers
       @see toOALib
    */
        oaTech* toOATech(const Technology* technology,const Library* lib) {
            cerr << "createOATechForTechnology" << endl;
            assert(technology);

            // 1) get or create Library for the techno
            assert(lib);
            oaLib* techOAlib = toOALib(lib);
            assert(techOAlib);

            // 2) get or create oaTech container
            bool created = false;
            cerr << "oaTech::find" << endl;
            oaTech* theOATech = oaTech::find(techOAlib);
            if(!theOATech){
                if (oaTech::exists(techOAlib)){
                    cerr << "oaTech::open" << endl;
                    theOATech = oaTech::open(techOAlib,'a');
                }
                if(!theOATech){
                    cerr << "oaTech::create" << endl;
                    theOATech = oaTech::create(techOAlib);
                    created = true;
                }
            }
            assert(techOAlib);
            if(created){
                theOATech->setDefaultManufacturingGrid(10);
                theOATech->setDBUPerUU(oaViewType::get(oacMaskLayout), 2000);

                //create and add foundry constraint group for General manufacturing rules
                //and add oaSimpleConstraintType too
                assert(theOATech);
                theOATech->getDefaultConstraintGroup();
                oaConstraintGroup *cgFoundry = theOATech->getFoundryRules();

                /*
                  add the constraint group LEFDefaultRouteSpec for oa2lef
                */
            }

            // get or create physical layer
            //first convert basicLayers and use the getExtractNumber
            for_each_basic_layer(layer, technology->getBasicLayers()) {
                toOALayer(layer,theOATech);
                end_for;
            }
            //then convert all other layers unsing a generated ID
            for_each_layer(layer, technology->getLayers()) {
                toOALayer(layer,theOATech);
                end_for;
            }
            cerr << "test" << endl;
            try{
                //create or find "utility" layers following :
                _layerDev = oaLayer::find(theOATech,"device");
                if(!_layerDev)
                    _layerDev = oaPhysicalLayer::create(theOATech, "device", generateLayerID(NULL));
                assert(_layerDev);
                _layerIDS.insert(_layerDev->getNumber());

                _layerText = oaLayer::find(theOATech,"text");
                if(!_layerText)
                    _layerText = oaPhysicalLayer::create(theOATech, "text", generateLayerID(NULL));
                assert(_layerText);
                _layerIDS.insert(_layerText->getNumber());

                _layerPin = oaLayer::find(theOATech,"pin");
                if(!_layerPin)
                    _layerPin = oaPhysicalLayer::create(theOATech, "pin", generateLayerID(NULL));
                assert(_layerPin);
                _layerIDS.insert(_layerPin->getNumber());

                _layerWire = oaLayer::find(theOATech,"wire");
                if(!_layerWire)
                    _layerWire = oaPhysicalLayer::create(theOATech, "wire", generateLayerID(NULL));
                assert(_layerWire);
                _layerIDS.insert(_layerWire->getNumber());
            }catch(oaException&e ){
                cerr << "OA:" << e.getMsg() << endl;
                exit(-2);
            }catch(std::exception&e ){
                cerr << "STD:" << e.what() << endl;
                exit(-1);
            }
            if(created)
                oaFuncs::printOALayers(theOATech);

            return theOATech;
        }

    /**
       convert the orientation of a transformation
    */
    static oaOrient toOAOrient(const Transformation::Orientation& orientation) {
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
       create a oaTransform for instanciation of cells
    */
    static oaTransform toOATransform(const Transformation& transformation) {
        oaTransform transform;
        transform.set(transformation.getTx(),
                      transformation.getTy(),
                      toOAOrient(transformation.getOrientation()));
        return transform;
    }

    /**
       convert Hurricane::Instance to oaInst ...
    */
    oaInst* toOAInst(Instance* instance,oaBlock* topBlock) {
        cerr << "toOAInst " << instance << endl;
        assert(instance);
        Instance2OAInstsMap::iterator it = _instance2OAInst.find(instance);
        if (it != _instance2OAInst.end()) {
            return it->second;
        }
        assert(topBlock);

        // 1) get the master cell for the instance
        Cell* masterCell = instance->getMasterCell();
        assert(masterCell);
        oaDesign* masterDesign = toOADesign(masterCell);
        assert(masterDesign);

        oaNativeNS ns;
        oaScalarName scMasterName;
        masterDesign->getCellName(scMasterName);
        oaString strMasterName;
        scMasterName.get(strMasterName);
        oaScalarName scInstName(ns, getString(instance->getName()).c_str());


        oaScalarInst* blockInst = oaScalarInst::find(topBlock,
                                                     scInstName);
        if(!blockInst){
            oaTransform transform = toOATransform(instance->getTransformation());
            blockInst = oaScalarInst::create(topBlock, masterDesign, scInstName, transform);
        }
        _instance2OAInst[instance] = blockInst;
        return blockInst;
    }

    /**
       convert Hurricane::plug to oaInstTerm
       and add it to if connected.
       always return a non NULL value
    */
    oaInstTerm* toOAInstTerm(Plug* plug,oaNet* net){
        cerr << "toOAInstTerm " << plug << endl;
        assert(plug);
        Instance* instance = plug->getInstance();
        Instance2OAInstsMap::iterator it = _instance2OAInst.find(instance);
        assert(it != _instance2OAInst.end());
        oaInst* blockInst = it->second;

        oaName instTermName(toOAName( getString(plug->getMasterNet()->getName()) ));
        oaInstTerm* instTerm = oaInstTerm::find(blockInst, instTermName);
        if (instTerm)
            return instTerm;

        oaDesign* design = blockInst->getMaster();
        assert(design);
        oaBlock* masterBlock = design->getTopBlock();
        oaTerm* term = oaTerm::find(masterBlock, instTermName);
        assert(term);

        cerr << "looking for " << plug->getName() << endl;
        oaFuncs::printOABlockTerms(masterBlock);
        cerr << "oaInstTerm::create" << endl;
        instTerm = oaInstTerm::create(net, blockInst, term);

        assert(instTerm);
        return instTerm;
    }

    static oaName toOAName(const Name& n){
        oaNativeNS ns;
        oaScalarName scN(ns, getString(n).c_str());
        return oaName(scN);
    }

    /**
       convert to OAPoint
    */
    static oaPoint toOAPoint(const Point& p) {
        oaPoint point;
        point.set(p.getX(), p.getY());
        return point;
    }

    /**
       convert to OABox
    */
    static oaBox toOABox(const Box& b) {
        oaBox box;
        box.set(b.getXMin(), b.getYMin(), b.getXMax(), b.getYMax());
        return box;
    }

        

    /**
       Used to convert Pad
    */
    oaRect* toOARect(Pad* pad,oaNet* blockNet){
        cerr << "toOARect" << endl;
        assert(pad);
        assert(blockNet);

        Pad2OARectMap::iterator it = _pad2OARect.find(pad);
        if (it != _pad2OARect.end())
            return it->second;
        
        oaRect* rect = oaRect::create(blockNet->getBlock(),
                                      toOALayerNum( pad->getLayer() ),
                                      oacDrawingPurposeType,
                                      toOABox(pad->getBoundingBox()) );
        return rect;
    }

    /**
       Used to convert Segment
    */
    oaPathSeg* toOAPathSeg(Segment* segment,oaNet* blockNet){
        cerr << "toOAPathSeg" << endl;
        assert(segment);
        assert(blockNet);

        Component2OAPathSegMap::iterator it = _component2OAPathSeg.find(segment);
        if (it != _component2OAPathSeg.end())
            return it->second;

        oaSegStyle style(segment->getWidth(), oacTruncateEndStyle, oacTruncateEndStyle);
        oaPathSeg* res = oaPathSeg::create(blockNet->getBlock(),
                                           toOALayerNum( segment->getLayer() ),
                                           oacDrawingPurposeType,
                                           toOAPoint(segment->getSourcePosition()),
                                           toOAPoint(segment->getTargetPosition()),
                                           style);
        return res;
    }

    /**
       Used to convert Pin
    */
    oaPin* toOAPin(Pin* hpin,oaNet* blockNet){
        cerr << "toOAPin" << endl;
        assert(hpin);
        assert(blockNet);
        oaName pinName;
        blockNet->getName(pinName);
        oaString sPinName;
        pinName.get(sPinName);
        oaTerm* term = oaTerm::find(blockNet->getBlock(), pinName);
        assert(term);
        oaPin* pin = oaPin::find(term, sPinName);
        if(!pin)
            pin = oaPin::create(term);

        return pin;
    }

    /**
       used to convert Via
       @todo implement me ...
    */
    oaVia* toOAVia(Contact* contact,oaNet* blockNet){
        //TODO
#if 0
        // Get via layers from the technology database    oaPhysicalLayer *poly = oaPhysicalLayer::find(tech, "Poly");
        ViaLayer::ViaLayer
            oaPhysicalLayer *imp1 = oaPhysicalLayer::find(_oaTech, "Nimp");
        oaPhysicalLayer *imp2 = oaPhysicalLayer::find(_oaTech, "Pimp");
        oaPhysicalLayer *metal1 = oaPhysicalLayer::find(tech, "Metal1");
        oaPhysicalLayer *cut = oaPhysicalLayer::find(tech, "Via1");


        // Set up the oaStdViaDef parameters
        oaViaParam stdViaDefParam;
        stdViaDefParam.setCutLayer(cut->getNumber());
        stdViaDefParam.setCutWidth(1000);
        stdViaDefParam.setCutHeight(1000);
        stdViaDefParam.setCutSpacing(100);
        stdViaDefParam.setCutColumns(4);


        // Create the oaStdViaDef    oaStdViaDef *myStdViaDef =
        oaStdViaDef::create(tech, oaString("myStdViaDef"), poly, metal1,
                            stdViaDefParam, imp1, imp2);


        // Create the oaStdViaDef
        oaTransform zeroTrans(oaPoint(0, 0), oacR0);
        oaStdVia* via = oaStdVia::create(blk, myStdViaDef, zeroTrans);

        return via;
#endif
        return NULL;
    }

    /**
       Convertion helper for Net convertion ...
       @todo verify
    */
    static oaTermType toOATermType(const Net::Direction& direction) {
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
    static oaSigType toOASigType(const Net::Type& type) {
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
       convert Hurricane::Net to oaNet
       always return a non NULL value
    */
    oaNet* toOANet(Net* net,oaBlock* topBlock) {
        cerr << "toOANet " << net << endl;
        assert(net);
        oaNativeNS ns;
        oaScalarName scNetName(ns, getString(net->getName()).c_str());
        oaScalarNet* blockNet = NULL;
        blockNet = oaScalarNet::find(topBlock, scNetName);
        if(blockNet)
            return blockNet;
        assert(!blockNet);
        blockNet = oaScalarNet::create(topBlock, scNetName, toOASigType(net->getType()));
        assert(blockNet);
        oaScalarTerm::create(blockNet, scNetName, toOATermType(net->getDirection()));
        blockNet->setGlobal(net->isGlobal());
        blockNet->scalarize();//ensure we can add shape ..

        //logical part
        cerr << " o transformation of plugs" << endl;
        for_each_plug(plug, net->getPlugs()) {
            oaInstTerm* term = toOAInstTerm(plug, blockNet);
            term->addToNet(blockNet);
            end_for;
        }
        cerr << " o transformation of contacts" << endl;
        for_each_contact(contact, net->getContacts()) {
            Pin* hPin = dynamic_cast<Pin*>(contact);
            oaPin* pin = NULL;
            if(hPin)
                pin = toOAPin(hPin, blockNet);
#if 0
            oaVia* via = toOAVia(contact, blockNet);
            via->addToNet(blockNet);
#endif
            end_for;
        }
        cerr << " o transformation of pads" << endl;
        for_each_pad(pad, net->getPads()){
            oaRect* rect = toOARect(pad, blockNet);
            rect->addToNet(blockNet);
            end_for;
        }
        cerr << " o transformation of segments" << endl;
        for_each_segment(component, net->getSegments()) {
            oaPathSeg* shape = toOAPathSeg(component,blockNet);
            shape->addToNet(blockNet);
            end_for;
        }
        return blockNet;
    }

    /**
       Add netlist view to previous view ...
    */
    oaDesign* addNetlist(const Cell* cell,oaDesign* previous) {
        cerr << "createNetlist " << cell << endl;
        assert(cell);
        assert(previous);
        Cell2OADesignMap::iterator it = _cell2OADesign4Netlist.find(cell);
        if (it != _cell2OADesign4Netlist.end()) {
            return it->second;
        }

        // 1) get the lib containing the cell
        oaNativeNS ns;
        oaLib* lib = toOALib(cell->getLibrary());
        assert(lib);

        // 2) create a netlist CellView of the cell
        oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
        oaScalarName scNameView(ns, "netlist");
        oaScalarName scNameLib;
        lib->getName(scNameLib);
        cerr << "oaDesign::open for netlist view" << endl;
        oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacNetlist), 'a');
        _cell2OADesign4Netlist[cell] = designCellView;

        // get module or embed previous module
        oaModule* topMod = NULL;
        topMod = designCellView->getTopModule();
        if(!topMod){
            topMod = oaModule::embed(designCellView, previous);
            designCellView->setTopModule(topMod);
        }
        oaBlock *topBlock = designCellView->getTopBlock();
        assert(topBlock);

        return designCellView;
    }

    /**
       Add symbol view to previous view ...
    */
    oaDesign* addSymbol(const Cell* cell,oaDesign* previous) {
        cerr << "addSymbol" << cell << endl;
        assert(cell);
        assert(previous);
        Cell2OADesignMap::iterator it = _cell2OADesign4Symbolic.find(cell);
        if (it != _cell2OADesign4Symbolic.end()) {
            return it->second;
        }
        oaNativeNS ns;
        oaLib* lib = toOALib(cell->getLibrary());
        assert(lib);
        oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
        oaScalarName scNameView(ns, "symbolic");
        oaScalarName scNameLib;
        lib->getName(scNameLib);

        // create a symbolic CellView of the cell
        oaViewType* vType = oaViewType::get(oacSchematicSymbol);
        cerr << "oaDesign::open for symbolic view" << endl;
        oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, vType, 'a');
        _cell2OADesign4Symbolic[cell] = designCellView;

        // get module or embed previous module
        oaModule* topMod = NULL;
        topMod = designCellView->getTopModule();
        if(!topMod){
            topMod = oaModule::embed(designCellView, previous);
            designCellView->setTopModule(topMod);
        }
        oaBlock *topBlock = designCellView->getTopBlock();
        assert(topBlock);

        return designCellView;
    }

    /**
       Add schematic view to previous view ...
    */
    oaDesign* addSchematic(const Cell* cell,oaDesign* previous) {
        cerr << "addSchematic" << cell << endl;
        assert(cell);
        assert(previous);
        Cell2OADesignMap::iterator it = _cell2OADesign4Schematic.find(cell);
        if (it != _cell2OADesign4Schematic.end())
            return it->second;

        oaNativeNS ns;
        oaLib* lib = toOALib(cell->getLibrary());
        assert(lib);
        oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
        oaScalarName scNameView(ns, "schematic");
        oaScalarName scNameLib;
        lib->getName(scNameLib);

        // create a schematic CellView of the cell
        oaViewType* vType = oaViewType::get(oacSchematic);
        cerr << "oaDesign::open for schematic view" << endl;
        oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, vType, 'a');
        _cell2OADesign4Schematic[cell] = designCellView;

        // get module or embed previous module
        oaModule* topMod = NULL;
        topMod = designCellView->getTopModule();
        if(!topMod){
            topMod = oaModule::embed(designCellView, previous);
            designCellView->setTopModule(topMod);
        }
        oaBlock *topBlock = designCellView->getTopBlock();
        assert(topBlock);

        return designCellView;
    }

    /**
       Create initial oaDesign from the cell and save it as a layout (abstract) view
    */
    oaDesign* addLayout(const Cell* cell) {
        cerr << "addLayout" << cell << endl;
        assert(cell);
        Cell2OADesignMap::iterator it = _cell2OADesign4Layout.find(cell);
        if (it != _cell2OADesign4Layout.end()) {
            return it->second;
        }

        oaNativeNS ns;
        oaLib* lib = toOALib(cell->getLibrary());
        assert(lib);
        oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
        oaScalarName scNameView(ns, "layout");
        oaScalarName scNameLib;
        lib->getName(scNameLib);

        // create a layout CellView of the cell
        oaViewType* vType = oaViewType::get(oacMaskLayout);
        cerr << "oaDesign::open for layout view" << endl;
        oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, vType, 'a');
        _cell2OADesign4Layout[cell] = designCellView;

        // create oaBlock singleton where we will do all the work
        oaBlock* topBlock = designCellView->getTopBlock();
        if(!topBlock){
            cerr << "oaBlock::create for netlist view" << endl;
            topBlock = oaBlock::create(designCellView);
        }
        assert(topBlock);

        // 4) convert each instance, net to OA
        cerr << "transformation of instances" << endl;
        for_each_instance(instance, cell->getInstances()){
            toOAInst(instance,topBlock);
            end_for;
        }
        cerr << "transformation of nets" << endl;
        for_each_net(net, cell->getNets()){
            toOANet(net,topBlock);
            end_for;
        }

        //get and update boundingBox and set abutment box
        Box bBox = cell->getBoundingBox();
        cerr << "Hurricane bounding box" << bBox << " in cell " << cell  << endl;
        Box aBox = cell->getAbutmentBox();
        cerr << "Hurricane abutment box" << aBox << " in cell " << cell  << endl;

        // create abutment in oa
        if(!aBox.isEmpty())
            if(!oaSnapBoundary::find(topBlock))
                oaSnapBoundary::create(topBlock, toOABox(aBox));

        oaBox boundingBox = toOABox(bBox);
        topBlock->getBBox(boundingBox);

        return designCellView;
    }

    /**
       Convert a Cell to OA designs ...
    */
    oaDesign* toOADesign(const Cell* cell) {
        cerr << "toOADesign " << cell << endl;
        assert(cell);

        // 1) get technology
        if(!_oaTech)
            _oaTech = toOATech(_technology,cell->getLibrary());

        // 2) create OA structure ...
        oaDesign* layoutView = addLayout(cell);
        assert(layoutView);

        oaCell* c1 = toOACell(layoutView);
        assert(c1);

        oaDesign* netlistView = addNetlist(cell,layoutView);
        assert(netlistView);

        oaCell* c2 = toOACell(netlistView);
        assert(c2);

        oaDesign* symbolicView = addSymbol(cell,netlistView);
        assert(symbolicView);

        oaCell* c3 = toOACell(symbolicView);
        assert(c3);

        oaDesign* schematicView = addSchematic(cell,symbolicView);
        assert(schematicView);

        oaCell* c4 = toOACell(schematicView);
        assert(c4);

        //3) we check it's the same oaCell for all designs
        assert(c1 == c2 && c2 == c3 && c3 == c4);

        return netlistView;
    }

    /**
       given a oaDesign get the oaCell corresponding
    */
    static oaCell* toOACell(oaDesign* design){
        assert(design);
        oaScalarName cellName;
        design->getCellName(cellName);
        oaLib* lib = design->getLib();
        oaBoolean gotAccess = false;
        gotAccess = lib->getAccess(oacReadLibAccess);
        oaCell* cell = oaCell::find(lib,cellName);
        if(gotAccess)
            lib->releaseAccess();
        assert(cell);
        return cell;
    }

    oaCell* toOACell(const Cell* cell) {
        Cell2OACellMap::iterator it = _cell2OAcell.find(cell);
        if (it != _cell2OAcell.end())
            return it->second;

        oaCell* c1 = toOACell( toOADesign(cell) );
        _cell2OAcell[cell] = c1;

        return c1;
    }
};//OADriver class
#endif
}//namespace CRL_OA

namespace CRL {
    void OpenAccess::oaDriver(const string& path, Cell* cell) {
#ifdef HAVE_OPENACCESS
        oaCell* convertedCell = NULL;
        assert(cell);
        cell->materialize();
        //for the moment a driver for hurricaneAMS
        //save the Cell only and all used Cell
        cerr << "Saving " << cell << " in " << path << endl;
        try {
            oaDesignInit(oacAPIMajorRevNumber,
                         oacAPIMinorRevNumber,
                         oacDataModelRevNumber);

            OADriver oaDriver(path);
            convertedCell = oaDriver.toOACell(cell);
        }catch (oaException  &e) {
            cerr << "OA::ERROR => " << e.getMsg() << endl;
            exit(1);
        }catch(std::exception& e){
            cerr << "STD::ERROR => " << e.what() << endl;
            exit(2);
        }
#else
        cerr << "\nDummy OpenAccess driver call for " << path << endl;
#endif
        return;
    }
}
