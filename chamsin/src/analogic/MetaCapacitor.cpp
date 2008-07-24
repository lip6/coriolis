#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Capacitor.h"
#include "MetaCapacitor.h"

const Name MetaCapacitor::BottomPlateName("BOTTOMPLATE");
const Name MetaCapacitor::TopPlateName("TOPPLATE");

MetaCapacitor::MetaCapacitor(Library* library, const Name& name):
    Device(library, name),
    _capacitorMatrix(),
    _rows(0), _columns(0)
{}

MetaCapacitor* MetaCapacitor::create(Library* library, const Name& name) {
    MetaCapacitor* mCapacitor = new MetaCapacitor(library, name);

    mCapacitor->_postCreate();

    return mCapacitor;
}

//création des signaux et des noeuds:
//***********************************
void MetaCapacitor::_postCreate() {
    Inherit::_postCreate();

    _bottomPlate = Net::create(this, BottomPlateName);
    _bottomPlate->setExternal(true);
    _topPlate = Net::create(this, TopPlateName);
    _topPlate->setExternal(true);

    setTerminal(false);
}

//La matrice des capas:
void MetaCapacitor::setMatrixSize(unsigned rows, unsigned columns) {
    assert(_capacitorMatrix.size() == _rows);

    if (_rows != rows && _columns != columns) {
        UpdateSession::open();
    }
}
