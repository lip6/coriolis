// author : Damien Dupuis
// date   : 24.02.2010
// -*- C++ -*-

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Go.h"
#include "hurricane/Cell.h"
#include "hurricane/Box.h"
#include "hurricane/Transformation.h"
#include "hurricane/Pad.h"
#include "hurricane/Segment.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Query.h"
using namespace Hurricane;

#include "io/cif/CifCircuit.h"
#include "io/cif/CifPolygon.h"
using namespace IO;

#include "Cif.h"

namespace {
class CifQuery : public Query {
    public:
        CifQuery ( Cell* );

        inline void setCircuit(CifCircuit*);

        virtual bool hasGoCallback() const;
        virtual void goCallback ( Go* );
        // not used but needed for compilation :
        virtual void extensionGoCallback(Go*) {};
        virtual void masterCellCallback() {};

  private:
        Cell*       _cell;
        CifCircuit* _circuit;
};

CifQuery::CifQuery(Cell* cell) : Query(), _cell(cell), _circuit(NULL) {
    Query::setQuery(_cell, _cell->getBoundingBox(), Transformation(), NULL, 0, Query::DoComponents);
}

inline void CifQuery::setCircuit(CifCircuit* circuit) { _circuit = circuit; }

bool CifQuery::hasGoCallback() const { return true; }

void CifQuery::goCallback(Go* go) {
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
    CifPolygon* poly = new CifPolygon ( layer->getExtractNumber() );
    long xMin = (long)round(DbU::getPhysical(b.getXMin(), DbU::Nano));
    long yMin = (long)round(DbU::getPhysical(b.getYMin(), DbU::Nano));
    long xMax = (long)round(DbU::getPhysical(b.getXMax(), DbU::Nano));
    long yMax = (long)round(DbU::getPhysical(b.getYMax(), DbU::Nano));
    poly->addPoint(xMin, yMin);
    poly->addPoint(xMax, yMin);
    poly->addPoint(xMax, yMax);
    poly->addPoint(xMin, yMax);

    _circuit->addPolygon(poly);
}
} // namespace

namespace CRL {
void cifDriver(const string& filePath, Cell* cell, string& name, string& units, double& scale) {
    name = getString(cell->getName());
    replace(name.begin(), name.end(), ' ', '_');
    units = "micro";
    scale = 0.001;
    CifCircuit* circuit = new CifCircuit(name, units, scale);
    CifQuery cifQuery (cell);

    cifQuery.setCircuit(circuit);

    forEach ( BasicLayer*, basicLayer, DataBase::getDB()->getTechnology()->getBasicLayers() ) {
        cifQuery.setBasicLayer(*basicLayer);
        cifQuery.doQuery();
    }

    circuit->write(filePath);
}
} // namespace CRL
