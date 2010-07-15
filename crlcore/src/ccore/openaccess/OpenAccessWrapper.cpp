// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-15 17:35:58" - OpenAccessWrapper.cpp
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

#ifdef HAVE_OPENACCESS
#include "oa/oaDesignDB.h"
using namespace oa;
#endif

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Segment.h"
#include "hurricane/Pad.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Slice.h"
using namespace Hurricane;

#include "OpenAccessWrapper.h"

#undef assert
#define assert(cond) if (! (cond) ) throw Error("assertion failed : " + string( #cond ) )
//#define assert(cond)


namespace {

#ifdef HAVE_OPENACCESS

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
        case BasicLayer::Material::blockage: return oacOtherMaterial;//is it OK?
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

    void getOATransformFromTransformation(oaTransform& transform, const Transformation& transformation) {
        cerr << "getOATransformFromTransformation" << endl;
        transform.set(transformation.getTx(),
                      transformation.getTy(),
                      getOAOrientFromOrientation(transformation.getOrientation()));
    }

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

    class OADriver {
    private:
        typedef map<const Library*, oaLib*> Library2OALibMap;
        typedef map<const Cell*, oaDesign*> Cell2OADesignMap;
        typedef pair<oaModInst*, oaInst*> OAInstPair;
        typedef map<Instance*, oaInst*> Instance2OAInstsMap;
        typedef map<Layer*, oaPhysicalLayer*> Layer2OAPhysicalLayerMap;

        string _path;
        oaTech* _oaTech;
        Library2OALibMap _library2OALib;
        Cell2OADesignMap _cell2OADesign4Netlist;
        Cell2OADesignMap _cell2OADesign4Schematic;
        Cell2OADesignMap _cell2OADesign4Symbolic;
        Cell2OADesignMap _cell2OADesign4Layout;
        Instance2OAInstsMap _instance2OAInst;
        Layer2OAPhysicalLayerMap _layer2OAPhysicalLayer;
        DataBase* _db;
        Technology* _technology;
        int _layerID;
    public:
        OADriver(const string& path):
            _path(path),
            _oaTech(NULL),
            _library2OALib(),
            _cell2OADesign4Netlist(),
            _cell2OADesign4Schematic(),
            _cell2OADesign4Symbolic(),
            _cell2OADesign4Layout(),
            _instance2OAInst(),
            _layer2OAPhysicalLayer(),
            _db(NULL),
            _technology(NULL),
            _layerID(0) {
            _db = DataBase::getDB();
            if (!_db) {
                throw Error("no database");
            }
            _technology = _db->getTechnology();
            if (!_technology) {
                throw Error("no technology");
            }
        }

        ~OADriver() {
            cerr << "SAVING ALL" << endl;
            _oaTech->save();
            _oaTech->close();
            saveDesignsInMap(_cell2OADesign4Netlist);
            saveDesignsInMap(_cell2OADesign4Schematic);
            saveDesignsInMap(_cell2OADesign4Symbolic);
            saveDesignsInMap(_cell2OADesign4Layout);
            for (Library2OALibMap::iterator it = _library2OALib.begin();
                 it != _library2OALib.end();
                 ++it) {
                oaLib* lib = it->second;
                lib->close();
            }
            cerr << "ALL SAVED" << endl;
        }

        /**
           Create an empty oaLib from a Library
           no cells are added in this oaLib
           all sub Library are also converted.
        */
        oaLib* getOALibForLibrary(const Library* library) {
            cerr << "getOALibForLibrary" << endl;
            assert(library);
            Library2OALibMap::iterator it = _library2OALib.find(library);
            if (it != _library2OALib.end()) {
                return it->second;
            }
            oaNativeNS ns;
            const char* strNameLib = getString(library->getName()).c_str();
            if (!strcmp(strNameLib, "."))
                strNameLib = "worklib";
            string strPathLib = _path + '/' + strNameLib;
            cerr << "Creating new Library for Lib name="
                 << strNameLib << " to filesystem path=" << strPathLib << endl;
            oaScalarName scNameLib(ns, strNameLib);
            cerr << "Creating new Library for Lib name="
                 << strNameLib << " to filesystem path=" << strPathLib << endl;

            cerr << "oaLib::find" << endl;
            oaLib *lib = oaLib::find(scNameLib);
            if (!lib) {
                if (oaLib::exists(strPathLib.c_str())){
                    cerr << "oaLib::open" << endl;
                    lib = oaLib::open(scNameLib, strPathLib.c_str());
                }else{
                    string cmd = "mkdir -p "+strPathLib;
                    system(cmd.c_str());
                }
                if(!lib){
                    cerr << "oaLib::create" << endl;
                    lib = oaLib::create(scNameLib, strPathLib.c_str());
                }
            }

            assert(lib);
            assert(lib->isValid());

            _library2OALib[library] = lib;

            for_each_library(l ,library->getLibraries()){
                getOALibForLibrary(l);
                end_for;
            }
/*
  cerr << "Overwriting cds.lib file" << endl;
  oaLibDefList *ldl = oaLibDefList::get( "cds.lib", 'a' );
  assert(ldl);
  oaLibDef::create( ldl, scNameLib, strPathLib.c_str() );
  ldl->save();
  ldl->destroy();
  ldl = NULL;
  cerr << "Overwrited cds.lib file" << endl;
*/

            return lib;
        }

        /**
           convert oaLayer from a Layer ...
        */
        oaLayer* getOALayerFromLayer(Layer* layer,oaTech* theOATech) {
            assert(layer);
            oaString layerName = getString(layer->getName()).c_str();
            oaPhysicalLayer* aOALayer = NULL;
            BasicLayer* bLayer = dynamic_cast<BasicLayer*>(layer);
            if(bLayer)
                aOALayer = oaPhysicalLayer::create(theOATech, layerName, _layerID++,getOAMaterial(bLayer->getMaterial()));
            else
                aOALayer = oaPhysicalLayer::create(theOATech, layerName, _layerID++);
            assert(aOALayer);

            _layer2OAPhysicalLayer[layer] = aOALayer;

            //TODO: add oaConstraint and oaConstraintGroup defined by technology
            DbU::Unit minSize =  layer->getMinimalSize();
            oaLayerConstraint* cMinSize = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                    oaLayerConstraintDef::get(oacMinSize),
                                                                    oaIntValue::create(aOALayer,minSize));
            DbU::Unit minSpace = layer->getMinimalSpacing();
            oaLayerConstraint* cMinSpace = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                     oaLayerConstraintDef::get(oacMinSpacing),
                                                                     oaIntValue::create(aOALayer,minSpace));
            DbU::Unit pitch = layer->getPitch();
            oaLayerConstraint* cPitchH = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacHorizontalRouteGridPitch),
                                                                   oaIntValue::create(aOALayer,pitch));
            oaLayerConstraint* cPitchV = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacVerticalRouteGridPitch),
                                                                   oaIntValue::create(aOALayer,pitch));
            if(bLayer){
                unsigned gdsIInumber = bLayer->getExtractNumber();
            }

            return aOALayer;
        }

        /**
           print the oaLayera in a oaTech ...
        */
        void printOALayers(oaTech* theOATech){
            assert(theOATech);
            oaIter<oaLayer> lIter(theOATech->getLayers());
            while(oaLayer* l = lIter.getNext()){
                oaString layerName;
                l->getName(layerName);
                cerr << " o created oaLayer " << layerName << endl;
            }
        }

        /**
           create a oaTech from a Hurricane::Technology in it's Library
           also create the oaLib corresponding to the Hurricane::Library
           containing the Hurricane::Technology
           @todo complete with technology info for layers
           @see getOALibForLibrary
        */
        oaTech* getOATechForTechnology(const Technology* technology) {
            cerr << "createOATechForTechnology" << endl;
            assert(technology);
            if(_oaTech != NULL)
                return _oaTech;
            else
                cerr << "working in createOATechForTechnology" << endl;

            //get or create Library for the techno
            DataBase* db = technology->getDataBase();
            assert(db);
            Library* lib  = db->getRootLibrary();
            assert(lib);
            oaLib* techOAlib = getOALibForLibrary(lib);
            assert(techOAlib);

            cerr << "oaTech::find" << endl;
            oaTech* theOATech = oaTech::find(techOAlib);
            if(!theOATech){
                cerr << "oaTech::open" << endl;
                theOATech = oaTech::open(techOAlib,'a');
                if(!theOATech){
                    cerr << "oaTech::create" << endl;
                    theOATech = oaTech::create(techOAlib);
                }else{
                    _oaTech = theOATech;
                    return theOATech;
                }
            }else{
                _oaTech = theOATech;
                return theOATech;
            }

            assert(techOAlib);
            theOATech->setDefaultManufacturingGrid(10);
            theOATech->setDBUPerUU(oaViewType::get(oacMaskLayout), 1000);

            //TODO: add this layer 
            //first create "utility" layers
            //strLayerDev ( "device" );
            //strLayerText( "text"   );
            //strLayerPin ( "pin"    );
            //strLayerCut1( "cut1"   );
            //strLayerWire( "wire"   );

            //create physical layer
            for_each_layer(layer, technology->getLayers()) {
                getOALayerFromLayer(layer,theOATech);
                end_for;
            }

            printOALayers(theOATech);

            _oaTech = theOATech;
            return theOATech;
        }

        /**
           convert Hurricane::Instance to oaInst ...
        */
        oaInst* getOAInstForInstance(Instance* instance,oaBlock* topBlock) {
            cerr << "getOAInstForInstance " << instance << endl;
            assert(instance);
            Instance2OAInstsMap::iterator it = _instance2OAInst.find(instance);
            if (it != _instance2OAInst.end()) {
                return it->second;
            }
            assert(topBlock);
            Cell* masterCell = instance->getMasterCell();
            assert(masterCell);
            oaDesign* masterDesign = createNetlist(masterCell);
            assert(masterDesign);


            oaNativeNS ns;
            oaScalarName scMasterName;
            masterDesign->getCellName(scMasterName);
            oaString strMasterName;
            scMasterName.get(strMasterName);
            oaScalarName scInstName(ns, getString(instance->getName()).c_str());

            oaTransform transform;
            getOATransformFromTransformation(transform, instance->getTransformation());
            oaScalarInst* blockInst = oaScalarInst::create(topBlock, masterDesign, scInstName, transform);
            _instance2OAInst[instance] = blockInst;
            return blockInst;
        }

        /**
           convert Hurricane::plug to oaInstTerm
           and add it to if connected.
           always return a non NULL value
        */
        oaInstTerm* getOAInstTermFromPlug(Plug* plug,oaNet* net){
            cerr << "getOAInstTermFromPlug " << plug << endl;
            assert(plug);
            Instance* instance = plug->getInstance();
            Instance2OAInstsMap::iterator it = _instance2OAInst.find(instance);
            assert(it != _instance2OAInst.end());
            oaInst* blockInst = it->second;
            oaInstTerm* instTerm = getInstTerm(blockInst, plug,net);
            assert(instTerm);
            return instTerm;
        }

        oaPin* getOAPinFromNet(Net* net,oaNet* blockNet){
            cerr << "getOAPinFromNet" << endl;
            assert(net);
            assert(net->isExternal());
            assert(blockNet);
            oaNativeNS ns;
            oaScalarName scNetName(ns, getString(net->getName()).c_str());
            oaTerm* term = oaTerm::create(blockNet, scNetName, getOATermType(net->getDirection()));
            assert(term);
            oaPin* pin = oaPin::create(term);
            assert(pin);

/*            oaLayerNum   layer;
              oaPurposeNum purpose;
              getLayerPurpose(block->getDesign(), "pin", "drawing", layer, purpose);
              oaPinFig *fig = oaPolygon::create(block, layer, purpose,  points);
*/
            return pin;
        }

        oaRect* getOARectFromComponent(Component* component,oaBlock* topBlock){
            cerr << "getOARectFromComponent" << endl;
            assert(component);
            assert(topBlock);
            oaBox box;
            getOABoxForBox(box, component->getBoundingBox());
            Layer* layer = (Layer*) component->getLayer();
            assert(layer);
            oaPhysicalLayer* physLayer = NULL;
            Layer2OAPhysicalLayerMap::iterator it = _layer2OAPhysicalLayer.find(layer);
            if (it != _layer2OAPhysicalLayer.end()) {
                physLayer = it->second;
            }
            assert(physLayer);
            oaLayerNum layerNum = physLayer->getNumber();
            oaRect* rect = oaRect::create(topBlock, layerNum,
                                          oaPurpose::get(_oaTech, oacDrawingPurposeType)->getNumber(),
                                          box);
            return rect;
        }

        /**
           convert Hurricane::Net to oaNet only at
           the logical abstraction point of view
           always return a non NULL value
        */
        oaNet* getOANetFromNet(Net* net,oaBlock* topBlock) {
            cerr << "getOANetFromNet " << net << endl;
            assert(net);
            oaNativeNS ns;
            oaScalarName scNetName(ns, getString(net->getName()).c_str());
            oaScalarNet* blockNet = oaScalarNet::create(topBlock, scNetName, getOASigType(net->getType()));
            assert(blockNet);
            if (net->isExternal()) {
                oaPin* pin = getOAPinFromNet(net,blockNet);
                Components externalComponents = NetExternalComponents::get(net);
                for_each_component(component, externalComponents) {
                    oaRect* rect = getOARectFromComponent(component,topBlock);
                    rect->addToPin(pin);
                    end_for;
                }
            }
            cerr << " o transformation of plugs" << endl;
            for_each_plug(plug, net->getPlugs()) {
                getOAInstTermFromPlug(plug,blockNet);
                end_for;
            }
            return blockNet;
        }

        /**
           create oaRect for slice ...
        */
        oaRect* getOARectFromSlice(Slice* slice,oaBlock* topBlock){
            for_each_component(component, slice->getComponents()) {
                oaRect* rect = getOARectFromComponent(component,topBlock);
                end_for;
            }
        }

        /**
           Create oaDesign for netlist Cell View from the cell
        */
        oaDesign* createNetlist(const Cell* cell) {
            cerr << "createNetlist " << cell << endl;
            assert(cell);
            Cell2OADesignMap::iterator it = _cell2OADesign4Netlist.find(cell);
            if (it != _cell2OADesign4Netlist.end()) {
                return it->second;
            }

            getOATechForTechnology(_technology);
            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameView(ns, "netlist");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            // create a netlist CellView of the cell
            cerr << "oaDesign::open for netlist view" << endl;
            oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacNetlist), 'w');
            _cell2OADesign4Netlist[cell] = designCellView;
            cerr << "oaBlock::create for netlist view" << endl;
            oaBlock* topBlock = oaBlock::create(designCellView);
            assert(topBlock);

            cerr << "transformation of slices" << endl;
            for_each_slice(slice, cell->getSlices()){
                getOARectFromSlice(slice,topBlock);
                end_for;
            }
            cerr << "transformation of instances" << endl;
            for_each_instance(instance, cell->getInstances()){
                getOAInstForInstance(instance,topBlock);
                end_for;
            }
            cerr << "transformation of nets" << endl;
            for_each_net(net, cell->getNets()){
                getOANetFromNet(net,topBlock);
                end_for;
            }
            return designCellView;
        }

        oaDesign* addSchematic(const Cell* cell,oaDesign* previous) {
            cerr << "addSchematic" << cell << endl;
            assert(cell);
            assert(previous);
            Cell2OADesignMap::iterator it = _cell2OADesign4Schematic.find(cell);
            if (it != _cell2OADesign4Schematic.end()) {
                return it->second;
            }

            getOATechForTechnology(_technology);
            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameView(ns, "schematic");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            // create a schematic CellView of the cell
            cerr << "oaDesign::open for schematic view" << endl;
            oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacSchematic), 'w');
            _cell2OADesign4Schematic[cell] = designCellView;

            return designCellView;
        }

        oaDesign* addSymbol(const Cell* cell,oaDesign* previous) {
            cerr << "addSymbol" << cell << endl;
            assert(cell);
            assert(previous);
            Cell2OADesignMap::iterator it = _cell2OADesign4Symbolic.find(cell);
            if (it != _cell2OADesign4Symbolic.end()) {
                return it->second;
            }

            getOATechForTechnology(_technology);
            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameView(ns, "symbolic");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            // create a symbolic CellView of the cell
            cerr << "oaDesign::open for symbolic view" << endl;
            oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacSchematicSymbol), 'w');
            _cell2OADesign4Symbolic[cell] = designCellView;

            return designCellView;
        }


        /**
           from genPhysical in OAGear
         */
        double findArea(oaBlock *block) {
            double totalArea = 0.0;
            oaIter<oaInst> instIter(block->getInsts());
            while (oaInst *i = instIter.getNext()) {
                oaDesign *master = i->getMaster();
                if (!master) {
                    oaString nameString;
                    i->getName(oaNativeNS(), nameString);
                    std::cerr << "Warning: black box found, assuming zero area:"
                              << nameString << std::endl;
                } else {
                    oaBlock *masterBlock = master->getTopBlock();  
                    assert(masterBlock);
                    oaPRBoundary *bound = oaPRBoundary::find(masterBlock);
                    assert(bound);
                    oaBox bbox;
                    bound->getBBox(bbox);
                    oaUInt8 cellArea = static_cast<oaUInt8>(bbox.getWidth()) *
                        static_cast<oaUInt8>(bbox.getHeight());
                    oaTech *tech = master->getTech();
                    assert(tech);
                    totalArea += tech->dbuToUUArea(oaViewType::get(oacMaskLayout), cellArea);   
                }
            }
            return totalArea;
        }        

        /** 
            from genPhysical in OAGear
        */
        oaSiteDef * findSiteDef(oaTech *tech){                
            oaSiteDef *r = 0;
            oaIter<oaSiteDef> siteIter(tech->getSiteDefs());
            while (oaSiteDef *i = siteIter.getNext()) {
                if (i->getSiteDefType() == oacCoreSiteDefType) {
                    if (!r || r->getWidth() > i->getWidth()) {
                        r = i;
                    }
                }
            }
            return r;
        }

        /**
           from genPhysical in OAGear
         */
        oaLayer * findMetal3(oaTech *tech){
            oaIter<oaLayer> oaLayerIter(tech->getLayers());
            while (oaLayer *i = oaLayerIter.getNext()) {
                if (i->getType() != oacPhysicalLayerType) {
                    continue;
                }
                oaMaterial m = (static_cast<oaPhysicalLayer *>(i))->getMaterial();
                if (m != oacMetalMaterial) {
                    continue;
                }
                int metalLayersBelow = 0;
                oaPhysicalLayer *belowLayer = (static_cast<oaPhysicalLayer *>(i))
                    ->getLayerBelow(oacMetalMaterial);
                while (belowLayer) {
                    metalLayersBelow++;
                    belowLayer =
                        belowLayer->getLayerBelow(oacMetalMaterial);
                }
                if (metalLayersBelow == 2) {
                    return i;
                }
            }
            return 0;
        }

        /**
           from genPhysical main in OAGear 
         */
        void genPhysical(oaDesign* physCV,oaDesign* netlistCV){
            double utilization = 0.70;
            
            // merge logically equivalent nets in original netlist view
            oaBlock *topNetlistBlock = netlistCV->getTopBlock();
            assert(topNetlistBlock);
            oaIter<oaNet> netIter(topNetlistBlock->getNets(
                                      oacNetIterSingleBit | oacNetIterNotImplicit));
            while (oaNet *n = netIter.getNext()) {
                oaBitNet *bn = static_cast<oaBitNet *>(n);
                oaBitNet *en = bn->getPreferredEquivalent();
                if (en != bn) {
                    en->merge(bn);
                }
            }
            
            // embed netlist into physical view
            oaModule *topMod = oaModule::embed(physCV, netlistCV);
            physCV->setTopModule(topMod);
            oaBlock *topBlock = physCV->getTopBlock();
            assert(topBlock);
            double totalArea = findArea(topBlock);
            if (totalArea == 0.0) {   
                std::cerr << "ERROR : Design has zero area" << std::endl;
                return ;
            }
            unsigned int numTerms = topBlock->getTerms().getCount();
            if (!numTerms) {
                std::cerr << "ERROR : Design has no terms" << std::endl;
                return ;
            }
            oaSiteDef *mySiteDef = findSiteDef(_oaTech);
            assert(mySiteDef);
            // compute die width and height
            double width = sqrt(totalArea / utilization);
            oaInt4 dbWidth = _oaTech->uuToDBUDistance(oaViewType::get(oacMaskLayout), width);
            oaInt4 numSites = dbWidth / mySiteDef->getWidth() + 1;
            dbWidth = numSites * mySiteDef->getWidth();
            double height = sqrt(totalArea / utilization);
            oaInt4 dbHeight = _oaTech->uuToDBUDistance(oaViewType::get(oacMaskLayout), height);
            oaInt4 numRows = dbHeight / mySiteDef->getHeight() + 1;
            dbHeight = numRows * mySiteDef->getHeight();
            oaConstraintGroup *cg = oaConstraintGroup::find(_oaTech, "LEFDefaultRouteSpec");
            assert(cg);
            // generate pins for terms and assign to boundary
            double jump = (static_cast<double>(dbWidth) + dbHeight)
                * 2.0 / numTerms;
            jump = static_cast<int>(jump / mySiteDef->getWidth());
            assert(jump >= 1.0);
            jump *= mySiteDef->getWidth();
            double pos = 0.0;
            std::vector<double> posVector(numTerms);
            for (unsigned int i = 0; i < numTerms; ++i) {
                posVector[i] = pos;
                pos += jump;
            }
            std::random_shuffle(posVector.begin(), posVector.end());
            // find the route layer - Metal3
            oaLayer *routeLayer = findMetal3(_oaTech);
            assert(routeLayer);
            oaLayerNum routeLayerNum(routeLayer->getNumber());
            // get route width constraint for metal 3 to compute size of pins
            oaConstraint *cons = oaLayerConstraint::find(cg, routeLayerNum,
                                                         oaLayerConstraintDef::get(oacMinWidth));
            oaValue *val = cons->getValue();
            oaInt4 m3Width = static_cast<oaIntValue *>(val)->get();
            oaIter<oaTerm> termIter(topBlock->getTerms());
            while (oaTerm *i = termIter.getNext()) {
                pos = posVector.back();
                double x, y;
                if (pos < dbWidth) {
                    x = pos;
                    y = 0.0;
                } else if (pos < dbWidth + dbHeight) {
                    x = dbWidth;
                    y = pos - dbWidth;
                } else if (pos < 2.0 * dbWidth + dbHeight) {
                    x = 2.0 * dbWidth + dbHeight - pos;
                    y = dbHeight;
                } else {
                    x = 0.0;
                    y = 2.0 * dbWidth + 2.0 * dbHeight - pos;
                }
                oaInt4 ux = static_cast<oaInt4>(x);
                oaInt4 uy = static_cast<oaInt4>(y);
                oaBox pinBox(ux - m3Width / 2, uy - m3Width / 2,
                             ux + m3Width / 2, uy + m3Width / 2);
                oaRect *pinRect = oaRect::create(topBlock, routeLayerNum,
                                                 oavPurposeNumberDrawing, pinBox);
                assert(pinRect);
                oaPin *pin = oaPin::create(i);
                assert(pin);
                pinRect->addToPin(pin);
                pin->setPlacementStatus(oacFixedPlacementStatus);
                posVector.pop_back();
            }
            oaPointArray prBoundPoints(4);
            oaPoint p0(0, 0);
            prBoundPoints.append(p0);
            oaPoint p1(0, dbHeight);
            prBoundPoints.append(p1);
            oaPoint p2(dbWidth, dbHeight);
            prBoundPoints.append(p2);
            oaPoint p3(dbWidth, 0);
            prBoundPoints.append(p3);
            oaPRBoundary *boundary = oaPRBoundary::create(topBlock, prBoundPoints);
            oaBox cBox(p0, p2);
            oaCoreBoxSpec coreSpec(cBox);
        }

        oaDesign* addLayout(const Cell* cell,oaDesign* previous) {
            cerr << "addLayout" << cell << endl;
            assert(cell);
            assert(previous);
            Cell2OADesignMap::iterator it = _cell2OADesign4Layout.find(cell);
            if (it != _cell2OADesign4Layout.end()) {
                return it->second;
            }

            getOATechForTechnology(_technology);
            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameView(ns, "layout");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            // create a layout CellView of the cell
            cerr << "oaDesign::open for layout view" << endl;
            oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacMaskLayout), 'w');
            _cell2OADesign4Layout[cell] = designCellView;


//            genPhysical(designCellView,previous);

            return designCellView;
        }

        oaDesign* getOADesignForCell(const Cell* cell) {
            cerr << "getOADesignForCell " << cell << endl;
            assert(cell);

            oaDesign* netlistView = createNetlist(cell);
            assert(netlistView);

            oaDesign* symbolicView = addSymbol(cell,netlistView);
            assert(symbolicView);

            oaDesign* schematicView = addSchematic(cell,netlistView);
            assert(schematicView);

            oaDesign* layoutView = addLayout(cell,schematicView);
            assert(layoutView);

            return layoutView;
        }
    };
#endif
}


namespace CRL {
    void OpenAccessWrapper::oaDriver(const string& path, Cell* cell) {
#ifdef HAVE_OPENACCESS
        //for the moment a driver for hurricaneAMS
        //save the Cell only and all used Cells
        cerr << "Saving " << cell << " in " << path << endl;
        try {
            oaDesignInit(oacAPIMajorRevNumber,
                         oacAPIMinorRevNumber,
                         oacDataModelRevNumber);

            OADriver oaDriver(path);
            oaDesign* design = oaDriver.getOADesignForCell(cell);
            if(design)
                cerr << "DONE ->" << getDesignName(design) << endl;
        } catch (oaException  &excp) {
            cerr << "ERROR: " << excp.getMsg() << endl;
            exit(1);
        }
#else
        cerr << "\nDummy OpenAccess driver call for " << path << endl;
#endif
    }
}
