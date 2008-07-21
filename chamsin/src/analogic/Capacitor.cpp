#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Pad.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

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

const Name Capacitor::BottomPlateName("BOTTOMPLATE");
const Name Capacitor::TopPlateName("TOPPLATE");

Capacitor::Capacitor(Library* library, const Name& name):
    AnalogComponent(library, name),
    _bottomPlate(NULL), _topPlate(NULL),
    _l(0), _w(0)
{}

Capacitor* Capacitor::create(Library* library, const Name& name) {
    Capacitor* capacitor = new Capacitor(library, name);

    capacitor->_postCreate();

    return capacitor;
}

void Capacitor::_postCreate() {
    Inherit::_postCreate();

    DataBase* db = DataBase::getDB();
    Technology* technology = db->getTechnology();

    _bottomPlate = Net::create(this, BottomPlateName);
    _bottomPlate->setExternal(true);
    _topPlate = Net::create(this, TopPlateName);
    _topPlate->setExternal(true);

    _topPlate20 = createPad(technology, _topPlate, "cut6");
}
