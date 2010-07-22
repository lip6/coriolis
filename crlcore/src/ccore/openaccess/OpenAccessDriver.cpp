// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-22 15:38:03" - OpenAccessWrapper.cpp
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

#include "OpenAccessDriver.h"
#include "OpenAccessUtils.h"

namespace {

#ifdef HAVE_OPENACCESS

    /**
       Class to drive OA to Hurricane
    */
    class OADriver {
    private:
        typedef map<const Library*, oaLib*> Library2OALibMap;
        typedef map<const Cell*, oaDesign*> Cell2OADesignMap;
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

            // 1) create or open library
            oaNativeNS ns;
            const char* strNameLib = getString(library->getName()).c_str();
            string strPathLib = _path + '/' + strNameLib;
            oaScalarName scNameLib(ns, strNameLib);
            cerr << "Library for Lib name="
                 << strNameLib << " to filesystem path=" << strPathLib << endl;
            cerr << "oaLib::find" << endl;
            oaLib *lib = oaLib::find(scNameLib);
            if (!lib) {
                if (oaLib::exists(strPathLib.c_str())){
                    cerr << "oaLib::open" << endl;
                    lib = oaLib::open(scNameLib, strPathLib.c_str());
                }else{
                    cerr << "create directory for library" << endl;
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

#if 0
            // 2) for each cell convert them too : if it's a standard cell library for example
            for_each_cell(c ,library->getCells()){
                getOADesignForCell(c);
                end_for;
            }

            // 3) also convert each contained library if any
            for_each_library(l ,library->getLibraries()){
                getOALibForLibrary(l);
                end_for;
            }
#endif
            // 4) create, update library list file
            try{
                cerr << "Overwriting cds.lib file begin" << endl;
                string cdsPath  = strPathLib + "/cds.lib";
                oaLibDefList* ldl = oaLibDefList::get( cdsPath.c_str(), 'a');
                assert(ldl);
                assert(ldl->isValid());
                if(!oaLibDef::find(ldl, scNameLib))
                    oaLibDef::create(ldl, scNameLib, strPathLib.c_str());
                ldl->save();
                ldl->destroy();//claim memory
                ldl = NULL;
                cerr << "Overwrited cds.lib file end" << endl;
            }catch(oaException& e){
                cerr << "ERROR cds: " << e.getMsg() << endl;
                exit(-2);
            }

            return lib;
        }

        /**
           convert oaLayer from a Layer ...
        */
        oaLayer* getOALayerFromLayer(Layer* layer,oaTech* theOATech) {
            cerr << "getOALayerFromLayer" << endl;
            assert(layer);
            Layer2OAPhysicalLayerMap::iterator it = _layer2OAPhysicalLayer.find(layer);
            if (it != _layer2OAPhysicalLayer.end()) {
                return it->second;
            }
            assert(theOATech);

            // 1) get or create layer
            oaString layerName = getString(layer->getName()).c_str();
            oaPhysicalLayer* aOALayer = NULL;
            aOALayer = oaPhysicalLayer::find(theOATech, layerName, true);
            if(aOALayer){
                _layer2OAPhysicalLayer[layer] = aOALayer;
                return aOALayer;
            }
            BasicLayer* bLayer = dynamic_cast<BasicLayer*>(layer);
            if(bLayer)
                aOALayer = oaPhysicalLayer::create(theOATech, layerName, _layerID++,getOAMaterial(bLayer->getMaterial()));
            else
                aOALayer = oaPhysicalLayer::create(theOATech, layerName, _layerID++);
            assert(aOALayer);

            _layer2OAPhysicalLayer[layer] = aOALayer;

            //create and add layer constraint for Layer specific manufacturing rules
            cerr << " o    get value for constraint" << endl;
            long minSize = Hurricane::DbU::getDb(layer->getMinimalSize());
            long minSpace = Hurricane::DbU::getDb(layer->getMinimalSpacing());
            long pitch = Hurricane::DbU::getDb(layer->getPitch());

#if 0
            cerr << " o    create constraint for min size : " << pitch << endl;
            oaLayerConstraint* cMinSize = NULL;
            try{
                cMinSize = oaLayerConstraint::create(aOALayer->getNumber(),
                                                     oaLayerConstraintDef::get(oacMinSize),
                                                     oaIntValue::create(theOATech,500))
                    }catch(oaException& e){
                cerr << "ERROR oaLayer: " << e.getMsg() << endl;
                exit(-2);
            }catch(...){
                cerr << "ERROR oaLayer: [UNKNOWN]" << endl;
                exit(-1);
            }
            assert(cMinSize);
            cerr << " o    create constraint for min space" << endl;
            oaLayerConstraint* cMinSpace = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                     oaLayerConstraintDef::get(oacMinSpacing),
                                                                     oaIntValue::create(theOATech->getLib(),minSpace));
            assert(cMinSpace);
            cerr << " o    create constraint for pitchH" << endl;
            oaLayerConstraint* cPitchH = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacHorizontalRouteGridPitch),
                                                                   oaIntValue::create(theOATech->getLib(),pitch));
            assert(cPitchH);

            cerr << " o    create constraint for pitchV" << endl;
            oaLayerConstraint* cPitchV = oaLayerConstraint::create(aOALayer->getNumber(),
                                                                   oaLayerConstraintDef::get(oacVerticalRouteGridPitch),
                                                                   oaIntValue::create(theOATech->getLib(),pitch));
            assert(cPitchV);
#endif
            if(bLayer){
                unsigned gdsIInumber = bLayer->getExtractNumber();
            }
            return aOALayer;
        }

        /**
           create a oaTech from a Hurricane::Technology in it's Library
           also create the oaLib corresponding to the Hurricane::Library
           containing the Hurricane::Technology
           @todo complete with technology info for layers
           @see getOALibForLibrary
        */
        oaTech* getOATechForTechnology(const Technology* technology,const Library* lib) {
            cerr << "createOATechForTechnology" << endl;
            assert(technology);

            // 1) get or create Library for the techno
            assert(lib);
            oaLib* techOAlib = getOALibForLibrary(lib);
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
                oaConstraintGroup *cgFoundry = theOATech->getFoundryRules();

                /*
                  add the constraint group LEFDefaultRouteSpec for oa2lef 
                */

                //TODO: add this layer
                //first create "utility" layers
                //strLayerDev ( "device" );
                //strLayerText( "text"   );
                //strLayerPin ( "pin"    );
                //strLayerWire( "wire"   );
            }
            
            // get or create physical layer

            for_each_layer(layer, technology->getLayers()) {
                getOALayerFromLayer(layer,theOATech);
                end_for;
            }
            printOALayers(theOATech);

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

            // 1) get the master cell for the instance
            Cell* masterCell = instance->getMasterCell();
            assert(masterCell);
            oaDesign* masterDesign = getOADesignForCell(masterCell);
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

#if 0
            oaLayerNum   layer;
            oaPurposeNum purpose;
            getLayerPurpose(block->getDesign(), "pin", "drawing", layer, purpose);
            oaPinFig *fig = oaPolygon::create(block, layer, purpose,  points);
#endif
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
            oaScalarNet* blockNet = NULL;
            blockNet = oaScalarNet::find(topBlock, scNetName);
            if(blockNet)
                return blockNet;
            assert(!blockNet);
            blockNet = oaScalarNet::create(topBlock, scNetName, getOASigType(net->getType()));
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
        void getOARectFromSlice(Slice* slice,oaBlock* topBlock){
            cerr << "getOARectFromSlice" << endl;
            assert(slice);
            assert(topBlock);
            for_each_component(component, slice->getComponents()) {
                oaRect* rect = getOARectFromComponent(component,topBlock);
                end_for;
            }
        }

        /**
           Create initial oaDesign from the cell and save it as a netlist view
        */
        oaDesign* createOAasNetlist(const Cell* cell) {
            cerr << "createNetlist " << cell << endl;
            assert(cell);
            Cell2OADesignMap::iterator it = _cell2OADesign4Netlist.find(cell);
            if (it != _cell2OADesign4Netlist.end()) {
                return it->second;
            }

            // 1) get the lib containing the cell
            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);

            // 2) create a netlist CellView of the cell
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameView(ns, "netlist");
            oaScalarName scNameLib;
            lib->getName(scNameLib);
            cerr << "oaDesign::open for netlist view" << endl;
            oaDesign* designCellView = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacNetlist), 'a');
            _cell2OADesign4Netlist[cell] = designCellView;

            // 3) create oaBlock singleton where we will do all the work
            oaBlock* topBlock = designCellView->getTopBlock();
            if(!topBlock){
                cerr << "oaBlock::create for netlist view" << endl;
                topBlock = oaBlock::create(designCellView);
            }
            assert(topBlock);

            // 4) convert each OA object
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
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
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
            if (it != _cell2OADesign4Schematic.end()) {
                return it->second;
            }

            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
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
           Add layout view to previous view ...
        */
        oaDesign* addLayout(const Cell* cell,oaDesign* previous) {
            cerr << "addLayout" << cell << endl;
            assert(cell);
            assert(previous);
            Cell2OADesignMap::iterator it = _cell2OADesign4Layout.find(cell);
            if (it != _cell2OADesign4Layout.end()) {
                return it->second;
            }

            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
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
           Convert a Cell to OA designs ...
        */
        oaDesign* getOADesignForCell(const Cell* cell) {
            cerr << "getOADesignForCell " << cell << endl;
            assert(cell);

            // 1) get technology
            if(!_oaTech)
                _oaTech = getOATechForTechnology(_technology,cell->getLibrary());

            // 2) create OA structure ...
            oaDesign* netlistView = createOAasNetlist(cell);
            assert(netlistView);
            oaDesign* symbolicView = addSymbol(cell,netlistView);
            assert(symbolicView);
            oaDesign* schematicView = addSchematic(cell,symbolicView);
            assert(schematicView);
            oaDesign* layoutView = addLayout(cell,schematicView);
            assert(layoutView);

            return netlistView;
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
    }
}
