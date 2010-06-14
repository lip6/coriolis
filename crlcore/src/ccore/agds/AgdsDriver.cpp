// author : Damien Dupuis
// date   : 08.12.2009
// -*- C++ -*-

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Library.h"
#include "hurricane/Go.h"
#include "hurricane/Cell.h"
#include "hurricane/Box.h"
#include "hurricane/Transformation.h"
#include "hurricane/Pad.h"
#include "hurricane/Segment.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Query.h"
using namespace Hurricane;

#include "vlsisapd/agds/Library.h"
#include "vlsisapd/agds/Structure.h"
#include "vlsisapd/agds/Rectangle.h"
// Cannot use AGDS namespace : conflicts with Hurricane::Library object

#include "Agds.h"

namespace {
class AgdsQuery : public Query {
    public:
        AgdsQuery ( Cell* );

        inline void setStructure(AGDS::Structure*);

        virtual bool hasGoCallback() const;
        virtual void goCallback ( Go* );
        // not used but needed for compilation :
        virtual void extensionGoCallback(Go*) {};
        virtual void masterCellCallback() {};

  private:
        Cell*            _cell;
        AGDS::Structure* _str;
};

AgdsQuery::AgdsQuery(Cell* cell) : Query(), _cell(cell), _str(NULL) {
    Query::setQuery(_cell, _cell->getBoundingBox(), Transformation(), NULL, 0, Query::DoComponents);
}

inline void AgdsQuery::setStructure(AGDS::Structure* str) { _str = str; }

bool AgdsQuery::hasGoCallback() const { return true; }

void AgdsQuery::goCallback(Go* go) {
    Box b;
    const BasicLayer* layer;
    if (dynamic_cast<const Pad*>(go)) {
        const Pad* pad = static_cast<const Pad*>(go);
        b = pad->getBoundingBox();
        layer = dynamic_cast<const BasicLayer*>(pad->getLayer());
    }
    else if (dynamic_cast<const Contact*>(go)) {
        const Contact* contact = static_cast<const Contact*>(go);
        b = contact->getBoundingBox();
        layer = dynamic_cast<const BasicLayer*>(contact->getLayer());
    }
    else if (dynamic_cast<const Segment*>(go)) {
        const Segment* segment = static_cast<const Segment*>(go);
        b = segment->getBoundingBox();
        layer = dynamic_cast<const BasicLayer*>(segment->getLayer());
    }
    else {
        return;
    }
    AGDS::Rectangle* rect = new AGDS::Rectangle ( layer->getExtractNumber()
                                          , DbU::getPhysical(b.getXMin(), DbU::Nano)
                                          , DbU::getPhysical(b.getYMin(), DbU::Nano)
                                          , DbU::getPhysical(b.getXMax(), DbU::Nano)
                                          , DbU::getPhysical(b.getYMax(), DbU::Nano));
    _str->addElement(rect);
}
} // namespace

namespace CRL {
void agdsDriver(const string& filePath, Cell* cell, string& name, string& lib, double& uUnits, double& pUnits) {
    name = getString(cell->getName());
    replace(name.begin(), name.end(), ' ', '_');
    lib = getString(cell->getLibrary()->getName());
    replace(lib.begin(), lib.end(), ' ', '_');
    uUnits = 0.001;
    pUnits = 1.0E-9;
    AGDS::Library* gdsLib = new AGDS::Library(lib);
    gdsLib->setUserUnits(uUnits);
    gdsLib->setPhysUnits(pUnits);
    AgdsQuery agdsQuery (cell);

    AGDS::Structure* str = new AGDS::Structure(getString(name));
    agdsQuery.setStructure(str);

    forEach ( BasicLayer*, basicLayer, DataBase::getDB()->getTechnology()->getBasicLayers() ) {
        agdsQuery.setBasicLayer(*basicLayer);
        agdsQuery.doQuery();
    }

    gdsLib->addStructure(str);
    gdsLib->writeToFile(filePath);
}
} // namespace CRL
