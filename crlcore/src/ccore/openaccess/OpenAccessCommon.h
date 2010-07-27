// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-26 16:09:36" - OpenAccessCommon.h
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
#ifndef __OPENACCESSCOMMON_H__
#define __OPENACCESSCOMMON_H__

#include <iostream>
using namespace std;

#ifdef HAVE_OPENACCESS
#include "hurricane/Cell.h"
#include "hurricane/BasicLayer.h"
using namespace Hurricane;

#include "oa/oaDesignDB.h"
using namespace oa;

#undef assert
#define assert(cond) if (! (cond) ) throw Error("assertion failed : " + string( #cond ) )
//#define assert(cond)

namespace {    

    /**
       giving a oaDesign pointer that should be not NULL
       return the oaString representing its name
    */
    inline oaString getDesignName(oaDesign* design) {
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
    inline void printBlockTerms(oaBlock* block) {
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
    inline oaMaterial getOAMaterial(const BasicLayer::Material&  material) {
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
    inline oaTermType getOATermType(const Net::Direction& direction) {
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
    inline oaSigType getOASigType(const Net::Type& type) {
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
    inline oaOrient getOAOrientFromOrientation(const Transformation::Orientation& orientation) {
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
    inline void getOATransformFromTransformation(oaTransform& transform, const Transformation& transformation) {
        cerr << "getOATransformFromTransformation" << endl;
        transform.set(transformation.getTx(),
                      transformation.getTy(),
                      getOAOrientFromOrientation(transformation.getOrientation()));
    }

    /**
       Convertion helper ...
     */
    inline void getOABoxForBox(oaBox& box, const Box& hbox) {
        cerr << "getOABoxForBox" << endl;
        box.set(hbox.getXMin(), hbox.getYMin(), hbox.getXMax(), hbox.getYMax());
    }

    /**
       Create InstTerm representing connection of nets ...
       always return a non NULL value
    */
    inline oaInstTerm* getInstTerm(oaInst* inst, Plug* plug,oaNet* net) {
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
    inline void saveDesignsInMap(map<const Cell*, oaDesign*> cell2OAdesign){
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
    inline void printOALayers(oaTech* theOATech){
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
    inline BasicLayer::Material::Code oaMaterialToBasicLayerType(const oaMaterial& material) {
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

    /**
       generate info from library name
     */
    inline pair<oaScalarName,string> libInfos(const string& path,
                                              const string& libName){
        oaNativeNS ns;
        const char* strNameLib = libName.c_str();
        oaScalarName scNameLib(ns, strNameLib);
        string strPathLib = path + '/' + strNameLib;
        return make_pair<oaScalarName,string>(scNameLib,strPathLib);
    }

    /**
       open oaLib with the info gathered by libPath function
     */
    inline oaLib* openOALib(const pair<oaScalarName,string>& infos){
        oaLib *lib = oaLib::find(infos.first);
        const char* pathLib = infos.second.c_str();
        if (!lib) {
            if (oaLib::exists(pathLib)){
                lib = oaLib::open(infos.first, pathLib);
            }else{
                string cmd = "mkdir -p "+ infos.second;
                system(cmd.c_str());
            }
            if(!lib){
                lib = oaLib::create(infos.first, pathLib);
            }
        }
        return lib;
    }

    inline void createCDS(const pair<oaScalarName,string>& infos){
        try{
            cerr << "Overwriting cds.lib file begin" << endl;
            string cdsPath  = infos.second + "/cds.lib";
            oaLibDefList* ldl = oaLibDefList::get( cdsPath.c_str(), 'a');
            assert(ldl);
                assert(ldl->isValid());
                if(!oaLibDef::find(ldl, infos.first))
                    oaLibDef::create(ldl, infos.first, infos.second.c_str());
                ldl->save();
                ldl->destroy();//claim memory
                ldl = NULL;
                cerr << "Overwrited cds.lib file end" << endl;
        }catch(oaException& e){
            cerr << "ERROR cds: " << e.getMsg() << endl;
            exit(-2);
        }
    }

    inline oaCell* OADesignToOACell(oaDesign* design){
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

    //multiplicator of BBOX coordinates
    static int convertFactor = 1;
    //for namespaces in Hurricane
    static const Name OACellLibrariesName("OACellLibraries");
    static const Name OADesignLibrariesName("OADesignLibraries");

    inline Library* findLibraryByNameInLibrary(const Library* rootLibrary, const Name& libraryName) {
        for_each_library(library, rootLibrary->getLibraries()) {
            if (library->getName() == libraryName) {
                return library;
            }
            Library* foundLibrary = findLibraryByNameInLibrary(library, libraryName);
            if (foundLibrary) {
                return foundLibrary;
            }
            end_for;
        }
        return NULL;
    }

    inline Library* findLibraryByNameInDB(const DataBase* db, const Name& libraryName) {
        Library* rootLibrary = db->getRootLibrary();
        if (rootLibrary->getName() == libraryName) {
            return rootLibrary;
        }
        return findLibraryByNameInLibrary(rootLibrary, libraryName);
    }
    
    inline void getAllCells(Library* rootLibrary, set<Cell*>& cellSet) {
        for_each_cell(cell, rootLibrary->getCells()) {
            cellSet.insert(cell);
            end_for;
        }
        for_each_library(library, rootLibrary->getLibraries()) {
            getAllCells(library, cellSet);
            end_for;
        }
    }

    inline Cell* findCellInLibraries(const Library* rootLibrary, const Name& cellName) {
        for_each_cell(cell, rootLibrary->getCells()) {
            if (cell->getName() == cellName) {
                return cell;
            }
            end_for;
        }
        for_each_library(library, rootLibrary->getLibraries()) {
            Cell* cell = findCellInLibraries(library, cellName);
            if (cell) {
                return cell;
            }
            end_for;
        }
        return NULL;
    }

    inline Library* getRootLibrary() {
        DataBase* db = DataBase::getDB();
        if (!db) {
            return NULL;
        }
        return db->getRootLibrary();
    }

    /**
       function helper
     */
    inline Library* getOACellLibraries() {
        Library* rootLibrary = getRootLibrary();
        if (!rootLibrary) {
            return NULL;
        }
        for_each_library(library, rootLibrary->getLibraries()) {
            if (library->getName() == OACellLibrariesName) {
                return library;
            }
            end_for;
        }
        return NULL;
    }

    /**
       helper function
     */
    inline Library* getOADesignLibraries() {
        Library* rootLibrary = getRootLibrary();
        if (!rootLibrary) {
            return NULL;
        }
        for_each_library(library, rootLibrary->getLibraries()) {
            if (library->getName() == OADesignLibrariesName) {
                return library;
            }
            end_for;
        }
        return NULL;
    }

    /**
       utility to open a design by name
     */
    inline oaView* pickView(oaCell* oa_Cell) {
        //oacMaskLayout Type is first
        oaView* toReturnView = NULL;

        oaCollection<oaCellView, oaCell> cellViews = oa_Cell->getCellViews();
        oaIter<oaCellView> cellViewIter(cellViews);

        while (oaCellView* cellView = cellViewIter.getNext()) {
            oaView* view = cellView->getView();
            oaViewType* viewType = view->getViewType();
            oaScalarName viewName;
            view->getName(viewName);
            if (viewType == oaViewType::get(oacMaskLayout)) {
                return view;
            }
            if (viewType == oaViewType::get(oacSchematic) ||
                viewType == oaViewType::get(oacNetlist) ||
                viewType == oaViewType::get(oacSchematicSymbol)) {
                toReturnView = view;
            }
        }
        return toReturnView;
    }

    /**
       utility to open a design by name
     */
    inline oaDesign* openDesign(const oaNameSpace& oaNS, oaCell* oa_Cell) {
        oaView* view = pickView(oa_Cell);
        if (view != NULL) {
            oaScalarName libName;
            oaLib* cellLib = oa_Cell->getLib();
            cellLib->getName(libName);
            oaScalarName cellName;
            oa_Cell->getName(cellName);
            oaViewType* viewType = view->getViewType();
            oaScalarName viewName;
            view->getName(viewName);
            return oaDesign::open(libName, cellName, viewName, viewType, 'r');
        }
        return NULL;
    }

    void loadOACellInCell(oaCell* oa_Cell, Cell* cell) {
        oaNativeNS oaNS;
        oaDesign* cellDesign = openDesign(oaNS, oa_Cell);

        if (cellDesign != NULL) {
            oaModule* module = cellDesign->getTopModule();
            oaCollection<oaModInst, oaModule> oaModInsts = module->getInsts();
            oaIter<oaModInst> modInstIter(oaModInsts);
            while (oaModInst* modInst = modInstIter.getNext()) {
                oaString oaModInstStr;
                modInst->getName(oaNS, oaModInstStr);
                //cerr << "inst : " << oaModInstStr << endl;
                oaModule* masterModule = modInst->getMasterModule();
                if (masterModule) {
                    oaString oaModuleStr;
                    masterModule->getName(oaNS, oaModuleStr);
                    //cerr << "master : " << oaModuleStr << endl;
                    //find hurricane Cell
                    Cell* masterCell = findCellInLibraries(getRootLibrary(), Name(oaModuleStr));
                    if (!masterCell) {
                        cout << "\n***Quitting. Cannot get MasterCell :" ;
                        cout << oaModuleStr << endl;
                        exit(8);
                    }
                    Instance* instance = Instance::create(cell, Name(oaModInstStr) ,masterCell);
                    //cerr << instance << endl;
                } else {
                    cerr << "master : NULL" << endl;
                }
            }//end while

            //now treat nets
            oaCollection<oaModNet, oaModule> oaModNets = module->getNets();
            oaIter<oaModNet> oaModNetIter(oaModNets);
            while (oaModNet* oa_ModNet = oaModNetIter.getNext()) {
                oaString oaModNetStr;
                oa_ModNet->getName(oaNS, oaModNetStr);
                //cerr << oaModNetStr << endl;
                Net* net = cell->getNet(Name(oaModNetStr));
                if (!net) {
                    net = Net::create(cell, Name(oaModNetStr));
                }

                oaCollection<oaModInstTerm, oaModNet> oaModInstTerms = oa_ModNet->getInstTerms();
                oaIter<oaModInstTerm> oaModInstTermIter(oaModInstTerms);
                while (oaModInstTerm* oa_ModInstTerm = oaModInstTermIter.getNext()) {
                    oaModInst* modInst = oa_ModInstTerm->getInst();
                    oaString oaModInstStr;
                    modInst->getName(oaNS, oaModInstStr);
                    //find hurricane instance
                    Instance* instance = cell->getInstance(Name(oaModInstStr));
                    if (!instance) {
                        cout << "\n***Quitting. Cannot get Instance :" ;
                        cout << oaModInstStr << endl;
                        exit(8);
                    }
                    oaModTerm* oa_ModTerm = oa_ModInstTerm->getTerm();
                    oaString oaModTermStr;
                    oa_ModTerm->getName(oaNS, oaModTermStr);
                    Net* masterNet = instance->getMasterCell()->getNet(Name(oaModTermStr));
                    if (!masterNet) {
                        cout << "\n***Quitting. Cannot get Master Net :" ;
                        cout << oaModTermStr << endl;
                        exit(8);
                    }
                    Plug* plug = instance->getPlug(masterNet);
                    plug->setNet(net);
                    //cerr << plug << endl;
                }
                //cerr << net << endl;
            }


            oaScalarName cellName;
            oa_Cell->getName(cellName);
            oaString cellNameString;
            cellName.get(cellNameString);
            cell->setName(Name(cellNameString));
            cell->setTerminal(false);

            //physical part
            oaBlock* block = cellDesign->getTopBlock();
            if (block) {
                oaBox oa_box;
                block->getBBox(oa_box);
                Point lowerLeft(DbU::db(oa_box.lowerLeft().x()), DbU::db(oa_box.lowerLeft().y()));
                Point upperRight(DbU::db(oa_box.upperRight().x()), DbU::db(oa_box.upperRight().y()));
                cell->setAbutmentBox(Box(lowerLeft, upperRight));

                oaCollection<oaInst, oaBlock> oaInsts = block->getInsts();
                oaIter<oaInst> oaInstIter(oaInsts);
                while (oaInst* oa_Inst = oaInstIter.getNext()) {
                    oaString oaInstStr;
                    oa_Inst->getName(oaNS, oaInstStr);
                    Instance* instance = cell->getInstance(Name(oaInstStr));
                    if (instance) {
                        //cerr << "found " << instance << endl;
                        oaPlacementStatus placementStatus= oa_Inst->getPlacementStatus();
                        //switch (placementStatus) {
                        //    case oacNonePlacementStatus :
                        //        cerr << " none" << endl;
                        //        break;
                        //    case oacUnplacedPlacementStatus :
                        //        cerr << " unplaced" << endl;
                        //        break;
                        //    case oacPlacedPlacementStatus :
                        //        cerr << " placed" << endl;
                        //        break;
                        //    case oacFixedPlacementStatus :
                        //        cerr << " fixed" << endl;
                        //        break;
                        //    default :
                        //        cerr << "other" << endl;
                        //}
                        oaPoint instOrigin;
                        oa_Inst->getOrigin(instOrigin);
                        //cerr << instOrigin.x() << " " << instOrigin.y() << endl;
                    } else {
                        cerr << "cannot find " << oaInstStr << endl;
                    }
                }
                oaSitePattern sitePattern;
                block->getSitePattern(sitePattern);

                for (int i = 0; i < sitePattern.getNumElements(); i++) {
                    const oaSiteRef& siteRef = sitePattern.get(i);
                    cerr << "site : " << siteRef.siteName() << endl;
                }

                oaCollection<oaRow, oaBlock> oaRows = block->getRows();
                oaIter<oaRow> oaRowIter(oaRows);
                while (oaRow* oa_Row = oaRowIter.getNext()) {
                    cerr << "row" << endl;
                    oaString siteName;
                    oa_Row->getSiteDefName(siteName);
                    cerr << siteName << endl;
                }

                oaCollection<oaAreaBoundary, oaBlock> oaAreaBoundaries = block->getBoundaries();
                oaIter<oaBoundary> oaBoundaryIter(oaAreaBoundaries);
                while (oaBoundary* oa_Boundary = oaBoundaryIter.getNext()) {
                    cerr << "boundary" << endl;
                }
            } else {
                cerr << "no block view " << endl;
            }
        }//end if (cellDesign != NULL)
    }
}//end anonymous namespace

#endif//HAVE_OPENACCESS

#endif//__OPENACCESSCOMMON_H__
