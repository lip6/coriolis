// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-06 17:24:31" - OpenAccessWrapper.cpp
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
using namespace Hurricane;

#include "OpenAccessWrapper.h"

#define assert(cond) if (! (cond) ) throw Error("assertion failed")
//#define assert(cond) 
    

namespace {

#ifdef HAVE_OPENACCESS

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

    oaInstTerm* getInstTerm(oaInst* inst, Plug* plug) {
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
        instTerm = oaInstTerm::create(NULL, inst, term);
        return instTerm;
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
        Library2OALibMap _library2OALibMap;
        Cell2OADesignMap _cell2OADesign4SchematicMap;
        Cell2OADesignMap _cell2OADesign4LayoutMap;
        Instance2OAInstsMap _instance2OAInstMap;
        Layer2OAPhysicalLayerMap _layer2OAPhysicalLayerMap;
        DataBase* _db;
        Technology* _technology;
    public:
        OADriver(const string& path):
            _path(path),
            _oaTech(NULL),
            _library2OALibMap(),
            _cell2OADesign4SchematicMap(),
            _cell2OADesign4LayoutMap(),
            _instance2OAInstMap(),
            _layer2OAPhysicalLayerMap(),
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
        }

        ~OADriver() {
            cerr << "SAVING ALL" << endl;            
            for (Cell2OADesignMap::iterator it = _cell2OADesign4SchematicMap.begin();
                 it != _cell2OADesign4SchematicMap.end();
                 ++it) {
                cerr << it->first << endl;
                oaDesign* design = it->second;
                design->save();
                design->close();
            }
            for (Cell2OADesignMap::iterator it = _cell2OADesign4LayoutMap.begin();
                 it != _cell2OADesign4LayoutMap.end();
                 ++it) {
                cerr << it->first << endl;
                oaDesign* design = it->second;
                design->save();
                design->close();
            }
            _oaTech->save();
            _oaTech->close();
            for (Library2OALibMap::iterator it = _library2OALibMap.begin();
                 it != _library2OALibMap.end();
                 ++it) {
                oaLib* lib = it->second;
                lib->close();
            }
            cerr << "ALL SAVED" << endl;
        }

        oaLib* getOALibForLibrary(const Library* library) {
            cerr << "getOALibForLibrary" << endl;
            Library2OALibMap::iterator it = _library2OALibMap.find(library);
            if (it != _library2OALibMap.end()) {
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

            oaLib *lib = oaLib::find(scNameLib);
            if (!lib) {
                if (oaLib::exists(strPathLib.c_str())){
                    lib = oaLib::open(scNameLib, strPathLib.c_str());
                }
//            string cmd = "rm -rf "+strPathLib+"; mkdir -p "+strPathLib;
//            system(cmd.c_str());
//            oaLib* lib = oaLib::create(scNameLib, strPathLib.c_str(), oacSharedLibMode, "oaDMFileSys");
                if(!lib)
                    lib = oaLib::create(scNameLib, strPathLib.c_str());
            }

            assert(lib);
            assert(lib->isValid());

            if(!_oaTech)
                _oaTech = createOATechForTechnology(_technology,lib);
            
            assert(_oaTech);

            _library2OALibMap[library] = lib;
            return lib;
        }

        /**
           @todo complete with technology info for layers
        */
        oaTech* createOATechForTechnology(const Technology* technology,oaLib* lib) {
            cerr << "createOATechForTechnology" << endl;
            oaTech* theOATech = oaTech::create(lib);
            theOATech->setDefaultManufacturingGrid(10);
            theOATech->setDBUPerUU(oaViewType::get(oacMaskLayout), 1000);
            
            int layerID=0;
            for_each_layer(layer, technology->getLayers()) {
                oaString layerName = getString(layer->getName()).c_str();
                oaPhysicalLayer* aOALayer = NULL;                
                BasicLayer* bLayer = dynamic_cast<BasicLayer*>(layer);
                if(bLayer)
                    aOALayer = oaPhysicalLayer::create(theOATech, layerName, layerID++,getOAMaterial(bLayer->getMaterial()));
                else
                    aOALayer = oaPhysicalLayer::create(theOATech, layerName, layerID++);                
                assert(aOALayer);

                _layer2OAPhysicalLayerMap[layer] = aOALayer;
                DbU::Unit minSize =  layer->getMinimalSize();
                DbU::Unit minSpace = layer->getMinimalSpacing();
                DbU::Unit pitch = layer->getPitch();
                end_for;
            }
            
            for_each_basic_layer(basicLayer, technology->getBasicLayers()) {
                Layer2OAPhysicalLayerMap::iterator it = _layer2OAPhysicalLayerMap.find((Layer*)(basicLayer));
                if (it != _layer2OAPhysicalLayerMap.end()) {
                    oaPhysicalLayer* oaLayer = it->second;
                    unsigned gdsIInumber = basicLayer->getExtractNumber();
                }
                end_for;
            }
            
            assert(theOATech != NULL);
            oaIter<oaLayer> lIter(theOATech->getLayers());
            while(oaLayer* l = lIter.getNext()){
                oaString layerName;
                l->getName(layerName);
                cerr << " o created oaLayer " << layerName << endl;
            }
            return theOATech;
        }

        oaDesign* getOADesignForCell(const Cell* cell) {
            cerr << "getOADesignForCell " << cell << endl;
            assert(cell);
            Cell2OADesignMap::iterator it = _cell2OADesign4SchematicMap.find(cell);
            if (it != _cell2OADesign4SchematicMap.end()) {
                return it->second;
            }

            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            assert(lib);
            assert(_oaTech);
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str());
            oaScalarName scNameViewSchematic(ns, "schematic");
            oaScalarName scNameViewLayout(ns, "layout");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            oaDesign* design4Schematic = oaDesign::open(scNameLib, scNameDesign, scNameViewSchematic, oaViewType::get(oacSchematic), 'w');
            _cell2OADesign4SchematicMap[cell] = design4Schematic;
            oaBlock* designBlock4Layout = oaBlock::create(design4Schematic);

            cerr << "transformation of instances" << endl;
            for_each_instance(instance, cell->getInstances()) {
                Cell* masterCell = instance->getMasterCell();
                oaDesign* masterDesign = getOADesignForCell(masterCell);
                oaScalarName scMasterName;
                masterDesign->getCellName(scMasterName);
                oaString strMasterName;
                scMasterName.get(strMasterName);
                oaScalarName scInstName(ns, getString(instance->getName()).c_str());

                oaTransform transform;
                getOATransformFromTransformation(transform, instance->getTransformation());
                oaScalarInst* blockInst = oaScalarInst::create(designBlock4Layout, masterDesign, scInstName, transform);
                _instance2OAInstMap[instance] = blockInst;
                end_for;
            }
            cerr << "transformation of nets" << endl;
            for_each_net(net, cell->getNets()) {
                oaScalarName scNetName(ns, getString(net->getName()).c_str());
                oaScalarNet* blockNet = oaScalarNet::create(designBlock4Layout, scNetName, getOASigType(net->getType()));
                if (net->isExternal()) {
                    oaTerm* term = oaTerm::create(blockNet, scNetName, getOATermType(net->getDirection()));
                    oaPin* pin = oaPin::create(term);
                    Components externalComponents = NetExternalComponents::get(net);
                    for_each_component(component, externalComponents) {
                        oaBox box;
                        getOABoxForBox(box, component->getBoundingBox());
                        oaRect* rect = oaRect::create(designBlock4Layout, 0,
                                                      oaPurpose::get(_oaTech, oacDrawingPurposeType)->getNumber(),
                                                      box);
                        rect->addToPin(pin);
                        end_for;
                    }
                }
                cerr << " o transformation of plugs" << endl;
                for_each_plug(plug, net->getPlugs()) {
                    Instance* instance = plug->getInstance();
                    Instance2OAInstsMap::iterator it = _instance2OAInstMap.find(instance);
                    assert(it != _instance2OAInstMap.end());
                    oaInst* blockInst = it->second;
                    oaInstTerm* instTerm = getInstTerm(blockInst, plug);
                    assert(instTerm);
                    instTerm->addToNet(blockNet);
                    end_for;
                }
                end_for;
            }
            // create a Layout View of the design 
            oaDesign* design4Layout = oaDesign::open(scNameLib, scNameDesign, scNameViewLayout, oaViewType::get(oacMaskLayout), 'w');
            _cell2OADesign4LayoutMap[cell] = design4Layout;
            if(design4Layout == design4Schematic)
                cerr << "design4Layout == design4Schematic" << endl;
            else
                cerr << "design4Layout != design4Schematic" << endl;
            return design4Schematic;
        }
    };
#endif
}

namespace CRL {
    void OpenAccessWrapper::oaDriver(const string& path, Cell* cell) {
#ifdef HAVE_OPENACCESS
        //for the moment a driver for hurricaneAMS
        //save the Cell and all used Cells
        cerr << "Saving " << cell << " in " << path << endl;
        try {
            oaDesignInit(
                oacAPIMajorRevNumber,
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
