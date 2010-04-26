// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
#ifdef HAVE_OPENACCESS
#include "oa/oaDesignDB.h"
using namespace oa;
#endif

#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Segment.h"
#include "hurricane/Pad.h"
using namespace Hurricane;

#include "OpenAccessWrapper.h"

namespace {

#ifdef HAVE_OPENACCESS

#if 0
oaModInstTerm* getModInstTerm(oaModInst* modInst, Plug* plug) {
    oaNativeNS ns;
    oaScalarName scPlugName(ns, getString(plug->getName()).c_str());
    oaName instTermName(scPlugName);
    oaModInstTerm* instTerm = oaModInstTerm::find(modInst, instTermName);
    if (instTerm) {
        return instTerm;
    }
    oaModule* masterModule = modInst->getMasterModule();
    oaModTerm* modTerm = oaModTerm::find(masterModule, instTermName);
    assert(modTerm);
    oaModNet* modNet = oaModNet::find(masterModule, instTermName);
    assert(modNet);
    instTerm = oaModInstTerm::create(modNet, modInst, instTermName);
    return instTerm;
}
#endif

oaString getDesignName(oaDesign* design) {
    oaNativeNS ns;
    oaString libName, cellName, viewName;
    design->getLibName(ns, libName);
    design->getCellName(ns, cellName);
    design->getViewName(ns, viewName);
    oaString designName = "<" + libName + "," + cellName + "," + viewName + ">";
    return designName;
}

void printBlockTerms(oaBlock* block) {
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

oaTermType getOATermType(const Net::Direction& direction) {
    switch (direction) {
        case Net::Direction::IN:
            return oacInputTermType;
        case Net::Direction::OUT:
            return oacOutputTermType;
        case Net::Direction::INOUT:
            return oacInputOutputTermType;
        case Net::Direction::TRISTATE:
            return oacTristateTermType;
        default:
            throw Error("unrecognized direction");
    }
}

oaOrient getOAOrientFromOrientation(const Transformation::Orientation& orientation) {
    cerr << orientation << endl;
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
    transform.set(transformation.getTx(),
            transformation.getTy(),
            getOAOrientFromOrientation(transformation.getOrientation()));
}

void getOABoxForBox(oaBox& box, const Box& hbox) {
    box.set(hbox.getXMin(), hbox.getYMin(), hbox.getXMax(), hbox.getYMax());
}

oaInstTerm* getInstTerm(oaInst* inst, Plug* plug) {
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
    if (!term) {
        cerr << "looking for " << plug->getName() << endl;
        printBlockTerms(masterBlock);
    }
    assert(term);
    instTerm = oaInstTerm::create(NULL, inst, term);
    return instTerm;
}

class OADriver {
    private:
        typedef map<const Library*, oaLib*> Library2OALibMap;
        typedef map<const Cell*, oaDesign*> Cell2OADesignMap;
        typedef pair<oaModInst*, oaInst*> OAInstPair;
        typedef map<Instance*, oaInst*> Instance2OAInstsMap;

        string _path;
        oaTech* _oaTech;
        oaBlock* _oaBlock;
        Library2OALibMap _library2OALibMap;
        Cell2OADesignMap _cell2OADesignMap;
        Instance2OAInstsMap _instance2OAInstMap;

    public:

        OADriver(const string& path):
            _path(path),
            _oaTech(NULL),
            _oaBlock(NULL),
            _library2OALibMap(),
            _cell2OADesignMap(),
            _instance2OAInstMap() {
                DataBase* db = DataBase::getDB();
                if (!db) {
                    throw Error("no database");
                }
                Technology* technology = db->getTechnology();
                //createOATechForTechnology(technology);
            }

        ~OADriver() {
            cerr << "SAVING ALL" << endl;

#if 0
            oaNativeNS ns;
            string fullPath = _path + "/lib.defs";
            oaScalarName scPath(ns, fullPath.c_str());
            oaString strPath;
            scPath.get(strPath);
            oaLibDefList* libDefList = oaLibDefList::get(strPath, 'w');
#endif

            for (Cell2OADesignMap::iterator it = _cell2OADesignMap.begin();
                    it != _cell2OADesignMap.end();
                    ++it) {
                cerr << it->first << endl;
                oaDesign* design = it->second;
                design->save();
                design->close();
            }

            for (Library2OALibMap::iterator it = _library2OALibMap.begin();
                    it != _library2OALibMap.end();
                    ++it) {
                oaLib* lib = it->second;
#if 0
                oaScalarName libName;
                lib->getName(libName);
                oaString libPath;
                lib->getFullPath(libPath);
                oaLibDef* libDef = oaLibDef::create(libDefList, libName, libPath);
#endif
                lib->close();
            }

     //       libDefList->save();
        }


        oaLib* getOALibForLibrary(const Library* library) {
            Library2OALibMap::iterator it = _library2OALibMap.find(library);
            if (it != _library2OALibMap.end()) {
                return it->second;
            }
            oaNativeNS ns;
            const char* strNameLib = getString(library->getName()).c_str();
            if (!strcmp(strNameLib, ".")) {
                strNameLib = "worklib";
            }
            string strPathLib = _path + '/' + strNameLib;
            //cerr << "Creating new Library for Lib name="
            //    << strNameLib << " to filesystem path=" << strPathLib << endl;
            oaScalarName scNameLib(ns, strNameLib);
            //for the moment absolute path
            //strPathLib = "/dsk/l1/oatest/test";
            cerr << "Creating new Library for Lib name="
                << strNameLib << " to filesystem path=" << strPathLib << endl;
            oaLib* lib = oaLib::create(scNameLib, strPathLib.c_str(), oacSharedLibMode, "oaDMFileSys");

            //oaScalarName libName;
            //_oaTech->getLibName(libName);
            //oaTech::attach(lib, libName);
            //assert(oaTech::find(lib));
            
            oaTech* oaTech = oaTech::create(lib);
            oaTech->setDefaultManufacturingGrid(10);
            oaTech->setDBUPerUU(oaViewType::get(oacMaskLayout), 1000);

            oaLayer* drawingLayer = oaPhysicalLayer::create(oaTech, "drawing", 0);

            _library2OALibMap[library] = lib;

            return lib;
        }

        void createOATechForTechnology(const Technology* technology) {
            string strPathLib = _path + "/techlib";
            oaNativeNS ns;
            oaScalarName scNameLib(ns, "techlib");
            oaLib* lib = oaLib::create(scNameLib, strPathLib.c_str(), oacSharedLibMode, "oaDMFileSys");
            _oaTech = oaTech::create(lib);

            oaLayer* drawingLayer = oaPhysicalLayer::create(_oaTech, "drawing", 0);

#if 0
            for_each_layer(hlayer, technology->getLayers()) {
                oaString strLayerName = getString(hlayer->getName()).c_str();
                oaPhysicalLayer* layer = oaPhysicalLayer::create(techno, strLayerName, layerNum++);
                end_for;
            }
#endif
            lib->close();
        }

        oaDesign* getOADesignForCell(const Cell* cell) {
            cerr << "getOADesignForCell " << cell << endl;
            Cell2OADesignMap::iterator it = _cell2OADesignMap.find(cell);
            if (it != _cell2OADesignMap.end()) {
                return it->second;
            }

            oaNativeNS ns;
            oaLib* lib = getOALibForLibrary(cell->getLibrary());
            oaScalarName scNameDesign(ns, getString(cell->getName()).c_str()); 
            oaScalarName scNameView(ns, "layout");
            oaScalarName scNameLib;
            lib->getName(scNameLib);

            oaDesign* design = oaDesign::open(scNameLib, scNameDesign, scNameView, oaViewType::get(oacSchematic), 'w');
            //oaModule* module = oaModule::create(design);
            oaBlock* block = oaBlock::create(design);
            oaTech* tech = oaTech::find(lib);
            assert(tech);
            oaBox box;
            getOABoxForBox(box, cell->getAbutmentBox());
            cerr << cell << " " << cell->getAbutmentBox() << endl;
            oaRect::create(block, 0, oaPurpose::get(tech, oacDrawingPurposeType)->getNumber(), box);
            
            for_each_instance(instance, cell->getInstances()) {
                Cell* masterCell = instance->getMasterCell();
                oaDesign* masterDesign = getOADesignForCell(masterCell); 
                oaScalarName scDesignName;
                masterDesign->getCellName(scDesignName);
                oaString strDesignName;
                scDesignName.get(strDesignName);
                //oaModule* masterModule = masterDesign->getTopModule();
                //assert(masterModule);
                oaScalarName scInstName(ns, getString(instance->getName()).c_str());
                //oaModScalarInst* modInst = oaModScalarInst::create(masterModule, masterDesign, scInstName);

                oaTransform transform;
                getOATransformFromTransformation(transform, instance->getTransformation());
                oaScalarInst* blockInst = oaScalarInst::create(block, masterDesign, scInstName, transform);
                _instance2OAInstMap[instance] = blockInst;
                end_for;
            }

            for_each_net(net, cell->getNets()) {
                oaScalarName scNetName(ns, getString(net->getName()).c_str());
                //oaModNet* modNet = oaModScalarNet::create(module, scNetName, oacAnalogSigType);
                oaScalarNet* blockNet = oaScalarNet::create(block, scNetName, oacAnalogSigType);
                if (net->isExternal()) {
                    oaTerm* term = oaTerm::create(blockNet, scNetName, getOATermType(net->getDirection()));
                    oaPin* pin = oaPin::create(term);
                    oaTech* tech = oaTech::find(lib);
                    assert(tech);
                    Components externalComponents = NetExternalComponents::get(net);
                    for_each_component(component, externalComponents) {
                        oaBox box;
                        getOABoxForBox(box, component->getBoundingBox());
                        oaRect* rect = oaRect::create(block, 0,
                            oaPurpose::get(tech, oacDrawingPurposeType)->getNumber(),
                            box);
                        rect->addToPin(pin);
                        end_for;
                    }
                }

                for_each_plug(plug, net->getPlugs()) {
                    Instance* instance = plug->getInstance();
                    Instance2OAInstsMap::iterator it = _instance2OAInstMap.find(instance);
                    assert(it != _instance2OAInstMap.end());
                    //oaModInst* modInst = it->second.first;
                    oaInst* blockInst = it->second;
                    //oaModInstTerm* modInstTerm = getModInstTerm(modInst, plug);
                    oaInstTerm* instTerm = getInstTerm(blockInst, plug);
                    //modInstTerm->addToNet(modNet);
                    instTerm->addToNet(blockNet);
                    end_for;
                }

                for_each_component(component, net->getComponents()) {
                    end_for;
                }
                end_for;
            }

            _cell2OADesignMap[cell] = design;
            return design;
        }
};
#endif

}

namespace CRL {

void OpenAccessWrapper::oaLibLoader(const string& libPath, Library *lib, Catalog &catalog) {
    cerr << "\nDummy OpenAccess lib loader call for " << libPath << endl;
}

void OpenAccessWrapper::oaDesignLoader(const string& path, Cell* cell) {
    cerr << "\nDummy OpenAccess design loader call for " << path << endl;
}

void OpenAccessWrapper::oaDriver(const string& path, Cell* cell, unsigned int& saveState) {
#ifdef HAVE_OPENACCESS
    //for the moment a driver for hurricaneAMS
    //save the Cell and all used Cells
    cerr << "Saving " << cell << " in " << path << endl;
    try {

        oaDesignInit(
                oacAPIMajorRevNumber,
                oacAPIMinorRevNumber,
                oacDataModelRevNumber);

        Library* library = cell->getLibrary();
        string pathLib = path + "/" + getString(library->getName()); 

        const char* strPathLib = pathLib.c_str();

        if (oaLib::exists(strPathLib)) {
            throw Error("impossible to drive Cell, there is an existing OpenAccess db");
        }

        pathLib = "/dsk/l1/oatest";
        OADriver oaDriver(pathLib);
        oaDesign* design = oaDriver.getOADesignForCell(cell);

    } catch (oaException  &excp) {
        cerr << "ERROR: " << excp.getMsg() << endl;
        exit(1);
    }
#else
    cerr << "\nDummy OpenAccess driver call for " << path << endl;
#endif
}

}
