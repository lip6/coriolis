using namespace std;

#include <boost/python.hpp>
using namespace boost::python;

#include "vlsisapd/cif/Polygon.h"
#include "vlsisapd/cif/Circuit.h"

namespace CIF {
BOOST_PYTHON_MODULE(CIF) {
    // class CIF::Polygon
    class_<Polygon>("Polygon", init<long>())
        .def("addPoint", &Polygon::addPoint)
    ;

    // class CIF::Circuit
    class_<Circuit>("Circuit", init<std::string, std::string, double>())
        .def("addPolygon" , &Circuit::addPolygon )
        .def("writeToFile", &Circuit::writeToFile)
    ;
}
} // namespace
