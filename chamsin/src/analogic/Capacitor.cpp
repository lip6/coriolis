#include "hurricane/Technology.h"
#include "hurricane/Pad.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DataBase.h"
using namespace Hurricane;

#include "AEnv.h"
#include "ATechnology.h"
#include "Capacitor.h"

namespace {
Layer* getLayer(Technology* technology, const string& layerStr) {
   Layer* layer = technology->getLayer(layerStr);
   if (!layer) {
       throw Error("Unknown Layer : " + layerStr);
   }
   return layer; 
}

Pad* createPad(Technology* technology, Net* net, const string& layerName) {
    static Box emptyBox(0, 0, 0, 0);
    Layer* layer = getLayer(technology, layerName);
    Pad* pad = Pad::create(net, layer, emptyBox);
    return pad;
}
}


//Déclaration des deux armatures:
//*******************************
const Name Capacitor::BottomPlateName("BOTTOMPLATE");
const Name Capacitor::TopPlateName("TOPPLATE");


//la mise des valeurs initials:
//c'est le Constructeur:
//*********************
Capacitor::Capacitor(Library* library, const Name& name):
    AnalogComponent(library, name),
    _bottomPlate(NULL), _topPlate(NULL),
    _l(0), _w(0),
    _topPlate10(NULL),
    _topPlate20(NULL),_topPlate30(NULL),
    _bottomPlate00(NULL)
{}


//Déclaration de la library: 
//*************************
Capacitor* Capacitor::create(Library* library, const Name& name) {
    Capacitor* capacitor = new Capacitor(library, name);
    capacitor->_postCreate();
    return capacitor;
}


//création des signaux et des noeuds:
//***********************************
void Capacitor::_postCreate() {
    Inherit::_postCreate();

    DataBase* db = DataBase::getDB();
    Technology* technology = db->getTechnology();

    _bottomPlate = Net::create(this, BottomPlateName);
    _bottomPlate->setExternal(true);
    _topPlate = Net::create(this, TopPlateName);
    _topPlate->setExternal(true);

    _topPlate10 = createPad(technology, _topPlate, "topmim6");
    _topPlate20 = createPad(technology, _topPlate, "padopen");
    _topPlate30 = createPad(technology, _topPlate, "alucap");
    _bottomPlate00 = createPad(technology, _bottomPlate, "botmim6");
}


//la fonction updateLayout 
//qui met en oeuvre de nouvau les parametres du capa 
//au cas ou il y a n' importe auel changement
//******************************************
void Capacitor::updateLayout() {
    DataBase* db = DataBase::getDB();

    if (!db) {
        throw Error("Error : no DataBase");
    }
    Technology* techno = db->getTechnology();
    if (!techno) {
        throw Error("Error : no Technology");
    }

    ATechnology* atechno = AEnv::getATechnology();
    
    //**Pour topPlate20 -> PADOPEN-MIM
    //********************************
    DbU::Unit widthCut6                 = atechno->getPhysicalRule("padOpenMIMWidth")->getValue();
    DbU::Unit spacingCut6               = atechno->getPhysicalRule("padOpenMIMSpace")->getValue();
    DbU::Unit enclosureCut6             = atechno->getPhysicalRule("padOpenMIMMETAL6Enclosure")->getValue();
  
    //**Pour topPlate30 -> ALUCAP-MIM
    //*******************************
    DbU::Unit widthAluCap               = atechno->getPhysicalRule("aluCapMIMWidth")->getValue();  
    DbU::Unit enclosureVia6             = atechno->getPhysicalRule("aluCapMIMVIA6Enclosure")->getValue();
    //DbU::Unit distanceAlucapTopmin      = atechno->getPhysicalRule("aluCapMIMDistance")->getValue();
    DbU::Unit enclosureTopMim           = atechno->getPhysicalRule("aluCapMIMTopMIM6Enclosure")->getValue();
    DbU::Unit enclosureBotMim           = atechno->getPhysicalRule("aluCapMIMBotMIM6Enclosure")->getValue();
    //DbU::Unit distanceAluc            = atechno->getPhysicalRule("minAlucapDistance")->getValue();
   
    //**Pour topPlate10 -> TOPMIM6
    //****************************
    DbU::Unit widthTopmim               = atechno->getPhysicalRule("topPlateMIMminWidth")->getValue();
    DbU::Unit spacingTopmim             = atechno->getPhysicalRule("topPlateMIMSpacing")->getValue();
    DbU::Unit enclosureByBotmimTopmim   = atechno->getPhysicalRule("topPlateMIMBotEnclosure")->getValue();
    //DbU::Unit enclosureVia6Topmim     = atechno->getPhysicalRule("minViaEnclosure")->getValue();
    DbU::Unit enclosureBotmimContact    = atechno->getPhysicalRule("topPlateMIMBotmimContact")->getValue();
  
    //**Pour BottomPlate -> BOTMIM6
    //*****************************
    DbU::Unit widthBotmim                = atechno->getPhysicalRule("botPlateMIMWidth")->getValue();
    DbU::Unit spacingBotmim              = atechno->getPhysicalRule("botPlateMIMSpacing")->getValue();
    DbU::Unit enclosureVia6Botmim        = atechno->getPhysicalRule("botPlateMIMViaEnclosure")->getValue();

    UpdateSession::open();
    DbU::setStringMode(1);


 //*les Box*//
 //*********//

 //topPlate 10:
 //l'armature haute est le reference 
 //car la valeur du capa corespond au W et L de l'armature haute
 //*************************************************************
    DbU::Unit x10 = 0;
    DbU::Unit y10 = 0;
    DbU::Unit dx10 = _l;
    DbU::Unit dy10 = _w;
    Box box10(x10, y10, x10 + dx10, y10 + dy10);
    _topPlate10->setBoundingBox(box10);

 //topPlate 30:
 //***********
    DbU::Unit x30 = enclosureTopMim;
    DbU::Unit y30 = enclosureTopMim;
    DbU::Unit dx30 = _l -(2 * enclosureTopMim);
    DbU::Unit dy30 = _w -(2 * enclosureTopMim) ;
    Box box30(x30, y30, x30 + dx30, y30 + dy30);
    _topPlate30->setBoundingBox(box30);

 //topPlate 20:
 //***********
    DbU::Unit x20 = enclosureTopMim +  enclosureVia6 ;
    DbU::Unit y20 = enclosureTopMim +  enclosureVia6 ;
    DbU::Unit dx20 = _l -(2 * enclosureTopMim)-(2 *  enclosureVia6 );
    DbU::Unit dy20 = _w -(2 * enclosureTopMim)-(2 *  enclosureVia6 );
    Box box20(x20, y20, x20 + dx20, y20 + dy20);
    _topPlate20->setBoundingBox(box20);

 //bottomPlate 00:
 //**************
    DbU::Unit x00 = 0 - enclosureByBotmimTopmim;
    DbU::Unit y00 = 0 - enclosureByBotmimTopmim;
    DbU::Unit dx00 = _l + (2 * enclosureByBotmimTopmim);
    DbU::Unit dy00 = _w + (2 * enclosureByBotmimTopmim);
    Box box00(x00, y00, x00 + dx00, y00 + dy00);
    _bottomPlate00->setBoundingBox(box00);

    UpdateSession::close();
}

 

Record* Capacitor::_getRecord() const {
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("BottomPlate", _bottomPlate));
        record->add(getSlot("TopPlate", _topPlate));
        
        record->add(getSlot("L", &_l));
        record->add(getSlot("W", &_w));
       
        record->add(getSlot("TopPlate20", _topPlate20));
        record->add(getSlot("TopPlate30", _topPlate30));
        record->add(getSlot("TopPlate10", _topPlate10));
        record->add(getSlot("BottomPlate00", _bottomPlate00));
       
      }
    return record;
}
