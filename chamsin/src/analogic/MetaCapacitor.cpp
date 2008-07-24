#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Capacitor.h"
#include "MetaCapacitor.h"

const Name MetaCapacitor::BottomPlateName("BOTTOMPLATE");
const Name MetaCapacitor::TopPlateName("TOPPLATE");
#if 0
const Name Capacitor::AnonymousName("ANONYMOUS");
#endif 


MetaCapacitor::MetaCapacitor(Library* library, const Name& name):
    Device(library, name),
    Capacitor::Capacitor(Library* library, const Name& name):
    AnalogComponent(library, name),
    _bottomPlate(NULL), 
    _topPlate(NULL),
    _anonymous(NULL),
    _l(0), _w(0),
    _topPlate10(NULL),
    _topPlate20(NULL),_topPlate30(NULL),
    _bottomPlate00(NULL)
#if 0
    _anonymous01(NULL),
    _anonymous11(NULL), 
    _anonymous12(NULL),
    _anonymous13(NULL),
    _anonymous21(NULL),
    _anonymousT11(NULL),
    _anonymousT12(NULL),
    _anonymousT13(NULL),
    _anonymousT14(NULL),
    _anonymousT21(NULL)
#endif    
{}



//Déclaration de la library: 
//*************************
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
    //_anonymous = Net::create(this, AnonymousName);

    _topPlate10    = createPad(technology, _topPlate, "topmim6");
    _topPlate20    = createPad(technology, _topPlate, "padopen");
    _topPlate30    = createPad(technology, _topPlate, "alucap");
    _bottomPlate00 = createPad(technology, _bottomPlate, "botmim6");
 #if 0

    _anonymous01  = createPad(technology, _anonymous, "Implant");
    _anonymous11  = createPad(technology, _anonymous, "topmim6");
    _anonymous12  = createPad(technology, _anonymous, "topmim6");
    _anonymous13  = createPad(technology, _anonymous, "topmim6");
    _anonymous21  = createPad(technology, _anonymous, "padopen");
    _anonymous14 = createPad(technology, _anonymous, "topmim6");

#endif

}


//La matrice des capas:
  void MetaTransistor::setMatrix(unsigned m , unsigned n) {
    assert(_Capacitor.largeur() == _w);
    assert(getInstances().getlargeur() == _w);
    assert(_Capacitor.longeur() == _l);
    assert(getInstances().getlongeur() == _l);

    if (_w != m && _l != n) {
        UpdateSession::open();
        if ( (m > _w ) || (n> _l ) ) {
            Library* library = getLibrary();
            Transformation transformation;
            for (unsigned i=_w; i<m; i++)
                for (unsigned j=_l; j<n; j++){
                }

         }
    }
  }










    setTerminal(false);
}
