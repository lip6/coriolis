// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-09-18 13:38:28" - OpenAccessCommon.h
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

#include "PathsUtils.h"

#include "hurricane/Cell.h"
#include "hurricane/BasicLayer.h"
using namespace Hurricane;

#include "oa/oaDesignDB.h"
using namespace oa;

#include "MyAssert.h"

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
           utility function to open a technology in OA format
         */
        static pair<oaTech*,bool> openOATech(oaLib* techOAlib){
            assert(techOAlib);
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
            return make_pair<oaTech*,bool>(theOATech,created);
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
           open oaLib with the info gathered by libInfos function
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
           create cds.lib file in the path containing OA libs 
           so Cadence (c) software can open them
        */
        static void createCDS(const pair<oaScalarName,string>& infos,
                              const string& path){
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
    };//struct oaFuncs
}//end CRL_OA namespace

#endif//HAVE_OPENACCESS

#endif//__OPENACCESSCOMMON_H__
