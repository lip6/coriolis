// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-09-18 13:28:46" - OpenAccessParser.cpp
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
#include <set>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Library.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/NetExternalComponents.h"
using namespace Hurricane;

#include "OpenAccess.h"
#include "OpenAccessCommon.h"

namespace {
#ifdef HAVE_OPENACCESS
     using namespace CRL_OA;
    class OAParser{
    private:
        typedef map<oaLib*, Library*> OALib2LibMap;
        typedef map<Name, Library*> Name2LibMap;
        typedef map<oaLayerNum, Layer*> OALayerNum2LayerMap;
        typedef map<Cell*, oaCell*> Cell2OACellMap;

        oaTech* _oaTechnology; // only one technology;
        OALayerNum2LayerMap _oaLayerNum2LayerMap;
        OALib2LibMap _oaLib2LibMap;
        Name2LibMap _name2LibMap;
        Cell2OACellMap _cell2OACellMap;
        set<Cell*> _loadedCells;
    public:
        OAParser():
            _oaTechnology(NULL),
            _oaLayerNum2LayerMap(),
            _oaLib2LibMap(),
            _name2LibMap(),
            _cell2OACellMap(),
            _loadedCells(){
            DataBase* db = DataBase::getDB();
            if (!db) {
                db = DataBase::create();
            }
            Library* rootLibrary = db->getRootLibrary();
            if (!rootLibrary) {
                rootLibrary = Library::create ( db, "RootLibrary" );
            }
            Library::create(rootLibrary, OACellLibrariesName);
            Library::create(rootLibrary, OADesignLibrariesName);
        }

        ~OAParser() {
            for (OALib2LibMap::iterator lit = _oaLib2LibMap.begin();
                 lit != _oaLib2LibMap.end();
                 lit++) {
                lit->first->close();
            }
        }

        /**
           @todo complete,verify ...
        */
        static BasicLayer::Material::Code fromOAMaterial(const oaMaterial& material) {
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

        Cell* getCell(const string& cellNameStr) {
            DataBase* db = DataBase::getDB();
            if (!db || !db->getRootLibrary() ) {
                return NULL;
            }
            Name cellName(cellNameStr);
            Cell* cell = oaFuncs::findCellInLibraries(oaFuncs::getOADesignLibraries(), cellName);
            if (!cell) {
                return NULL;
            }

            set<Cell*>::const_iterator csit = _loadedCells.find(cell);
            if (csit == _loadedCells.end()) {
                Cell2OACellMap::const_iterator cit = _cell2OACellMap.find(cell);
                if (cit == _cell2OACellMap.end()) {
                    cerr << "error : cannot find cell" << endl;
                    exit(8);
                }
                oaCell* oa_Cell = cit->second;
                loadOACellInCell(oa_Cell, cell);
                _loadedCells.insert(cell);
            }
            return cell;
        }

        /**
           heart of the parser algorithm
        */
        oaLib* loadOALib(const string& libNameStr, const string& libPathStr, bool asDesignLibrary) {
            oaNativeNS oaNS;
            Name libName(libNameStr);
            Name2LibMap::const_iterator nit = _name2LibMap.find(libName);
            if (nit != _name2LibMap.end())
                Library* library = nit->second;

            string libPath = libPathStr;
            realPath(libPath);

            oaLib* oaLibrary = NULL;
            try {
                pair<oaScalarName,string> libInfos = oaFuncs::libInfos(libPath, libNameStr);
                oaLibrary = oaFuncs::openOALib(libInfos);
                if (oaLibrary->isReadable()) {
                    if (!oaLibrary->getAccess(oacReadLibAccess)) {
                        cout << "\n***Quitting. Cannot get LibAccess\n" ;
                        exit(8);
                    }

                    cerr << "TITI" << endl;
                    fromOATech(oaLibrary);

                    //create Hurricane library
                    DataBase* db = DataBase::getDB();
                    if (!db) {
                        cerr << "No DataBase" << endl;
                        exit(8);
                    }
                    if (oaFuncs::findLibraryByNameInDB(db, libName)) {
                        cerr << "ERROR_PARSER(1)" << endl;
                        exit(8);
                    }

                    Library* library;
                    if (asDesignLibrary) {
                        library = Library::create(oaFuncs::getOADesignLibraries(), libName);
                    } else {
                        library = Library::create(oaFuncs::getOACellLibraries(), libName);
                    }
                    cerr << library << endl;

                    oaCollection<oaCell, oaLib> cells = oaLibrary->getCells();
                    oaIter<oaCell> cellsIter(cells);

                    while (oaCell* cell = cellsIter.getNext()) {
                        oaScalarName cellOAName;
                        oaString cellNameString;
                        cell->getName(cellOAName);
                        cellOAName.get(cellNameString);

                        cerr << cellNameString << endl;

                        oaDesign* cellDesign = oaFuncs::openOADesign(cell);
                        if (cellDesign != NULL) {
                            Cell* hCell = NULL;
                            //logic part
                            oaModule* module = cellDesign->getTopModule();
                            if (module) {
                                hCell = Cell::create(library, Name(cellNameString));
                                hCell->setTerminal(false);
                                if (!asDesignLibrary) 
                                    hCell->setTerminalNetlist(true);
                                cerr << hCell << endl;
                                if (asDesignLibrary) 
                                    _cell2OACellMap[hCell] = cell;
                                
                                oaCollection<oaModTerm, oaModule> oaModTerms = module->getTerms();
                                oaIter<oaModTerm> oaModTermIter(oaModTerms);
                                while (oaModTerm* modTerm = oaModTermIter.getNext()) {
                                    oaString oaModTermStr;
                                    modTerm->getName(oaNS, oaModTermStr);
                                    Net* net = Net::create(hCell, Name(oaModTermStr));
                                    net->setExternal(true);
                                    switch (modTerm->getTermType()) {
                                    case oacInputTermType:
                                        net->setDirection(Net::Direction::IN);
                                        break;
                                    case oacOutputTermType:
                                        net->setDirection(Net::Direction::OUT);
                                        break;
                                    case oacInputOutputTermType:
                                        net->setDirection(Net::Direction::INOUT);
                                        break;
                                    case oacTristateTermType:
                                        net->setDirection(Net::Direction::TRISTATE);
                                        break;
                                    default:
                                        net->setDirection(Net::Direction::UNDEFINED);
                                        break;
                                    }
                                    cerr << net << endl;
                                    oaModNet* oaNet = modTerm->getNet();
                                    if (oaNet->isGlobal()) {
                                        net->setGlobal(true);
                                        cerr << "global" << endl;
                                    }
                                    if (oaNet->getSigType() == oacClockSigType) {
                                        net->setType(Net::Type::CLOCK);
                                    } else if (oaNet->getSigType() == oacPowerSigType) {
                                        net->setGlobal(true);
                                        net->setType(Net::Type::POWER);
                                    } else if (oaNet->getSigType() == oacGroundSigType) {
                                        net->setGlobal(true);
                                        net->setType(Net::Type::GROUND);
                                    } else {
                                        net->setType(Net::Type::LOGICAL);
                                    }


                                }
                            }

                            //physical part
                            oaBlock* block = cellDesign->getTopBlock();
                            if (block && hCell) {
                                oaBox oa_box;
                                block->getBBox(oa_box);
                                cerr << "box values" << endl;
                                cerr << oa_box.lowerLeft().x() << " , " << oa_box.lowerLeft().y() << endl;
                                cerr << oa_box.upperRight().x() << " , " << oa_box.upperRight().y() << endl;
                                cerr << endl;
                                Point lowerLeft(DbU::db(oa_box.lowerLeft().x()), DbU::db(oa_box.lowerLeft().y()));
                                Point upperRight(DbU::db(oa_box.upperRight().x()), DbU::db(oa_box.upperRight().y()));
                                hCell->setAbutmentBox(Box(lowerLeft, upperRight));
                                oaCollection<oaTerm, oaBlock> oaTerms = block->getTerms();
                                oaIter<oaTerm> oaTermIter(oaTerms);
                                while (oaTerm* term = oaTermIter.getNext()) {
                                    oaString oaTermStr;
                                    term->getName(oaNS, oaTermStr);
                                    Net* net = hCell->getNet(Name(oaTermStr));
                                    if (net) {
                                        cerr << "found net : " << net << endl;
                                        oaCollection<oaPin, oaTerm> oaPins(term->getPins());
                                        oaIter<oaPin> oaPinIter(oaPins);
                                        while (oaPin* pin = oaPinIter.getNext()) {
                                            oaString oaPinStr;
                                            pin->getName(oaPinStr);
                                            cerr << "pin : " << oaPinStr << endl;

                                            oaCollection<oaPinFig, oaPin> oaPinFigs(pin->getFigs());
                                            oaIter<oaPinFig> oaPinFigIter(oaPinFigs);
                                            while (oaPinFig* pinFig = oaPinFigIter.getNext()) {
                                                cerr << "pinFig" << endl;
                                                if (pinFig->getType() == oacRectType) {
                                                    oaRect* rect = static_cast<oaRect*>(pinFig);
                                                    OALayerNum2LayerMap::iterator layerMapIt =  _oaLayerNum2LayerMap.find(rect->getLayerNum());
                                                    if (layerMapIt != _oaLayerNum2LayerMap.end()) {
                                                        Layer* layer = layerMapIt->second;
                                                        oaBox pinBBox;
                                                        rect->getBBox(pinBBox);
                                                        DbU::Unit width  = DbU::db( (pinBBox.right() - pinBBox.left()) * (double)convertFactor);
                                                        DbU::Unit height = DbU::db( (pinBBox.top() - pinBBox.bottom()) * (double)convertFactor);
                                                        DbU::Unit cx = DbU::db(pinBBox.right() * (double)convertFactor) + width/2;
                                                        DbU::Unit cy = DbU::db(pinBBox.top() * (double)convertFactor) + height/2;
                                                        if (width > height) {
                                                            Horizontal* horizontal=Horizontal::create(net, layer, cy, height, DbU::db(pinBBox.right() * (double)convertFactor), DbU::db(pinBBox.left() * (double)convertFactor));
                                                            NetExternalComponents::setExternal(horizontal);
                                                        } else {
                                                            Vertical* vertical= Vertical::create(net, layer, cx, width, DbU::db(pinBBox.bottom() * (double)convertFactor), DbU::db(pinBBox.top() * (double)convertFactor));
                                                            NetExternalComponents::setExternal(vertical);
                                                        }
                                                    } else {
                                                        cerr << "cannot find layer..." << endl;
                                                    }

                                                    cerr << "rect " << rect->getLayerNum() << endl;
                                                } else {
                                                    cerr << "no rect" << endl;
                                                }
                                            }
                                        }
                                    } else {//if not a net
                                        //TODO
                                    }
                                }
                            } else {
                                cerr << "no block" << endl;
                            }
                        }
                    }
                    oaLibrary->releaseAccess();
                    //oaLibrary->close(); do not close !!
                    //FIXME save opened libraries ??
                }
            } catch (oaException  &excp) {
                cout << "ERROR_PARSER(2): " << excp.getMsg() << endl;
                exit(1);
            }
            return oaLibrary;
        }

        /**
           heart of the parser algorithm
        */
        void fromOATech(oaLib* oaLibrary) {
            assert(oaLibrary);
            try {
                oaTech* tech = oaTech::open(oaLibrary);
                    cout << "TOTO:" << endl;

                if (!tech) {
                    cout << "ERROR_PARSER(3):" << endl;
                    exit(1);
                }
                if (_oaTechnology && tech != _oaTechnology) {
                    cout << "ERROR_PARSER(4):" << endl;
                    exit(1);
                }
                if (_oaTechnology) {
                    return;
                }
                _oaTechnology = tech;

                DataBase* db = DataBase::getDB();
                if (!db) {
                    cerr << "No DataBase" << endl;
                    exit(8);
                }
                Technology* technology = db->getTechnology();
                if (!technology) {
                    technology = Technology::create(db, "OpenAccess");
                }

                oaCollection<oaLayer, oaTech> oaLayers(_oaTechnology->getLayers());
                oaIter<oaLayer> oaLayerIt(oaLayers);
                while (oaLayer* oaLayer = oaLayerIt.getNext()) {
                    oaString oaLayerStr;
                    oaLayer->getName(oaLayerStr);
                    Name layerName(oaLayerStr);
                    Layer* layer = technology->getLayer(layerName);
                    if (!layer) {
                        layer = BasicLayer::create(technology, layerName, BasicLayer::Material::other, oaLayer->getNumber());
                    }
                    cerr << layer << endl;
                    _oaLayerNum2LayerMap[oaLayer->getNumber()] = layer;
                }
            } catch (oaException  &excp) {
                cout << "ERROR_PARSER(5): " << excp.getMsg() << endl;
                exit(1);
            }
        }

        void getDesigns(set<Cell*>& designCellSet) {
            oaFuncs::getAllCells(oaFuncs::getOADesignLibraries(), designCellSet);
        }

        /**
           heart of the parser algorithm
        */
        static void loadOACellInCell(oaCell* oa_Cell, Cell* cell) {
            oaNativeNS oaNS;
            oaDesign* cellDesign = oaFuncs::openOADesign(oa_Cell);

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
                        Cell* masterCell = oaFuncs::findCellInLibraries(oaFuncs::getRootLibrary(), Name(oaModuleStr));
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
                    //ici on approxime l'abutment box avec la boundingBox
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
                            switch (placementStatus) {
                            case oacNonePlacementStatus :
                                cerr << " none" << endl;
                                break;
                            case oacUnplacedPlacementStatus :
                                cerr << " unplaced" << endl;
                                break;
                            case oacPlacedPlacementStatus :
                                cerr << " placed" << endl;
                                break;
                            case oacFixedPlacementStatus :
                                cerr << " fixed" << endl;
                                break;
                            default :
                                cerr << "other" << endl;
                            }
                            oaPoint instOrigin;
                            oa_Inst->getOrigin(instOrigin);
                            cerr << instOrigin.x() << " " << instOrigin.y() << endl;
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
        }//end loadOACellInCell
    };//OAParser class
#endif
}//namespace

namespace CRL {
    Cell* OpenAccess::oaCellParser(const std::string& cellLibPath, const std::string& cellLibName, const std::string& cellName,
                                   const std::string& techLibPath, const std::string& techLibName) {
        Cell* convertedCell = NULL;
#ifdef HAVE_OPENACCESS
        try {
            oaDesignInit(oacAPIMajorRevNumber,
                         oacAPIMinorRevNumber,
                         oacDataModelRevNumber);

            OAParser oaParser;
            oaScalarName scalarCellName(oaNativeNS(),cellName.c_str());

            oaLib* oaLibraryTech = oaParser.loadOALib(techLibName, techLibPath, true);
            oaLib* oaLibrary = oaParser.loadOALib(cellLibName, cellLibPath, true);
            Cell* hcell = oaParser.getCell(cellName);
            oaCell* cell = oaCell::find(oaLibrary,scalarCellName);
            oaParser.loadOACellInCell(cell,hcell);

        }catch (oaException  &e) {
            cerr << "OA::ERROR => " << e.getMsg() << endl;
            exit(1);
        }catch(std::exception& e){
            cerr << "STD::ERROR => " << e.what() << endl;
            exit(2);
        }
#else
        cerr << "\nDummy OpenAccess driver call for " << endl;
#endif
        return convertedCell;
    }
}
