// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-08-06 01:21:19" - OpenAccessCommon.h
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
#include  <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

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
    //multiplicator of BoundingBOX coordinates
    const int convertFactor = 1;
    const Name OACellLibrariesName("OACellLibraries");
    const Name OADesignLibrariesName("OADesignLibraries");
}

namespace CRL_OA {
    struct oaFuncs {
        //for namespaces in Hurricane
        /**
           get a Hurricane::Library by Name in a Hurricane DB
        */
        static Library* findLibraryByNameInLibrary(const Library* rootLibrary, const Name& libraryName) {
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

        /**
           get a Hurricane::Library by Name in a Hurricane DB
        */
        static Library* findLibraryByNameInDB(const DataBase* db, const Name& libraryName) {
            Library* rootLibrary = db->getRootLibrary();
            if (rootLibrary->getName() == libraryName) {
                return rootLibrary;
            }
            return findLibraryByNameInLibrary(rootLibrary, libraryName);
        }

        /**
           populate the set passed as parameter with all cells of a Hurricane::Library
        */
        static void getAllCells(Library* rootLibrary, set<Cell*>& cellSet) {
            for_each_cell(cell, rootLibrary->getCells()) {
                cellSet.insert(cell);
                end_for;
            }
            for_each_library(library, rootLibrary->getLibraries()) {
                getAllCells(library, cellSet);
                end_for;
            }
        }

        /**
           find a cell by name in a Hurricane::Library
        */
        static Cell* findCellInLibraries(const Library* rootLibrary, const Name& cellName) {
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

        /**
           giving a oaDesign pointer that should be not NULL
           return the oaString representing its name
        */
        static oaString getOADesignName(oaDesign* design) {
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
        static void printOABlockTerms(oaBlock* block) {
            assert(block);
            oaNativeNS ns;
            oaDesign* design = block->getDesign();
            cerr << " o Printing " << getOADesignName(design) << " terms" << endl;
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
        static oaMaterial getOAMaterialFromMaterial(const BasicLayer::Material&  material) {
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
           @todo complete,verify ...
        */
        static BasicLayer::Material::Code getBasicLayerTypeFromOAMaterial(const oaMaterial& material) {
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
           Convertion helper for Net convertion ...
           @todo verify
        */
        static oaTermType getOATermTypeFromNetDirection(const Net::Direction& direction) {
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
        static oaSigType getOASigTypeFromNetType(const Net::Type& type) {
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
        static oaOrient getOAOrientFromOrientation(const Transformation::Orientation& orientation) {
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
        static oaTransform getOATransformFromTransformation(const Transformation& transformation) {
            oaTransform transform;
            transform.set(transformation.getTx(),
                          transformation.getTy(),
                          getOAOrientFromOrientation(transformation.getOrientation()));
            return transform;
        }

        /**
           Convertion helper ...
        */
        static oaBox getOABoxFromBox(const Box& b) {
            oaBox box;
            box.set(b.getXMin(), b.getYMin(), b.getXMax(), b.getYMax());
            return box;
        }

        /**
           Create InstTerm representing connection of nets between instance
           always return a non NULL value
        */
        static oaInstTerm* getOAInstTermFromOAInst(oaInst* inst, Plug* plug,oaNet* net) {
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
            printOABlockTerms(masterBlock);
            cerr << "oaInstTerm::create" << endl;
            instTerm = oaInstTerm::create(net, inst, term);
            assert(instTerm);
            return instTerm;
        }

        /**
           save and close design(s) stored in a map
        */
        static void saveOADesignsInMap(map<const Cell*, oaDesign*> cell2OAdesign){
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
        static void printOALayers(oaTech* theOATech){
            assert(theOATech);
            oaIter<oaLayer> lIter(theOATech->getLayers());
            while(oaLayer* l = lIter.getNext()){
                oaString layerName;
                l->getName(layerName);
                cerr << " o created oaLayer " << layerName << endl;
            }
        }

        /**
           handling realpath
        */
        static void realPath(string& pathToChange){
            if(bfs::path::default_name_check_writable())
                bfs::path::default_name_check(bfs::portable_posix_name);
            bfs::path resolvedPath = pathToChange;
            pathToChange = system_complete(resolvedPath).string();
        }

        /**
           generate info from library name
        */
        static pair<oaScalarName,string> libInfos(const string& path,
                                                  const string& libName){
            oaNativeNS ns;
            const char* strNameLib = libName.c_str();
            oaScalarName scNameLib(ns, strNameLib);
            string strPathLib = path + '/' + strNameLib;
            return make_pair<oaScalarName,string>(scNameLib,strPathLib);
        }

        /**
           suppose the path has been resolved with system_complete
           before calling this function and path are posix_name
           then split the path in boos::filesystem::path corresponding of each dir
           from most root parent to leaf dir
           @see create_all_dirs
        */
        static std::vector<bfs::path> split_in_dirs(const bfs::path& p){
            string pstr(p.string());
            register size_t len(pstr.length());
            register char delim('/');
            register size_t firstDelim=0;
            register size_t secondDelim=1;
            vector<bfs::path> dirs;
            while(firstDelim < len){
                while(secondDelim < len && pstr[secondDelim]!=delim)
                    secondDelim++;
                string dir = pstr.substr(0,secondDelim);
                if(dir.empty())
                    break;
                dirs.push_back(bfs::path(dir));
                firstDelim = secondDelim;
                secondDelim++;
            }
            return dirs;
        }

        /**
           work around for boost::filesystem::create_directories
           missing in old boost versions like 1.33.1
           and equivalent to recursivly creating directories
           instead this is done iteratively.
        */
        static void create_all_dirs(const bfs::path& p){
            if(p.empty() || bfs::exists(p))
                return;
            std::vector<bfs::path> test;
            test = split_in_dirs(p);
            std::vector<bfs::path>::iterator it = test.begin();
            for(;it != test.end();it++){
                if(it->empty() || bfs::exists(*it))
                    continue;
                bfs::create_directory(*it);
            }
        }

        /**
           open oaLib with the info gathered by libPath function
        */
        static oaLib* openOALib(const pair<oaScalarName,string>& infos){
            oaLib *lib = NULL;
            try{
                lib = oaLib::find(infos.first);
                const char* pathLib = infos.second.c_str();
                if (!lib) {
                    if (oaLib::exists(pathLib)){
                        lib = oaLib::open(infos.first, pathLib);
                    }
                    if(!lib){
                        if(bfs::path::default_name_check_writable())
                            bfs::path::default_name_check(bfs::portable_posix_name);
                        bfs::path bfs_pathLib = pathLib;
                        create_all_dirs(bfs_pathLib);
                        cerr << "creating lib : " << pathLib << endl;
                        lib = oaLib::create(infos.first, pathLib);
                    }
                }
                assert(lib);
            }catch(std::exception& e){
                cerr << e.what() << endl;
                exit(-1);
            }
            return lib;
        }

        /**
           create cds.lib file in the path containong OA libs so Cadence (c) software could
           open them
        */
        static void createCDS(const pair<oaScalarName,string>& infos,const string& path){
            try{
                cerr << "Overwriting cds.lib file begin " << endl;
                string cdsPath  = path + "/cds.lib";
                oaString oaCDSPath = oaString(cdsPath.c_str());
                oaLibDefList* ldl = oaLibDefList::find(oaCDSPath);
                if(!ldl)
                    ldl = oaLibDefList::get(oaCDSPath, 'a');
                assert(ldl);
                assert(ldl->isValid());
                oaLibDef* toAdd = oaLibDef::find(ldl, infos.first);
                if(!toAdd)
                    toAdd = oaLibDef::create(ldl, infos.first, infos.second.c_str());
                ldl->save();
                ldl->destroy();//claim memory
                ldl = NULL;
                cdsPath.clear();
                cerr << "Overwrited cds.lib file end" << endl;
            }catch(oaException& e){
                cerr << "ERROR cds: " << e.getMsg() << endl;
                exit(-2);
            }
        }

        /**
           given a oaDesign get the oaCell corresponding
        */
        static oaCell* getOACellFromOADesign(oaDesign* design){
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

        /**
           get the rootLibrary of a Hurricane DB
           if any
           @return the rootLibrary or NULL
        */
        static Library* getRootLibrary() {
            DataBase* db = DataBase::getDB();
            if (!db) {
                return NULL;
            }
            return db->getRootLibrary();
        }

        /**
           get in Hurricane the libraries with OA Design "namespace" : libs from given dev kits
        */
        static Library* getOACellLibraries() {
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
           get in Hurricane the libraries with OA Design "namespace" : analog designer libs
        */
        static Library* getOADesignLibraries() {
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
           just pick a view to open :
           first try layout then schematic netlist and symbolic
        */
        static oaView* pickView(oaCell* oa_Cell) {
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
           utility to open a design from a oaCell (i.e from a kit)
        */
        static oaDesign* openOADesign(oaCell* oa_Cell) {
            oaNativeNS oaNS;
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
    };//struct oaCommon
}//end CRL_OA namespace

#endif//HAVE_OPENACCESS

#endif//__OPENACCESSCOMMON_H__
