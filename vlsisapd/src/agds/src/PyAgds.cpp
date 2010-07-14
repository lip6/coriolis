using namespace std;

#include <boost/python.hpp>
using namespace boost::python;

#include "vlsisapd/agds/Library.h"
#include "vlsisapd/agds/Structure.h"
#include "vlsisapd/agds/Element.h"
#include "vlsisapd/agds/Rectangle.h"

namespace AGDS {
BOOST_PYTHON_MODULE(AGDS) {
    // class AGDS::Element
    class_<Element, boost::noncopyable>("Element", no_init) // abstract class : noncopyable + no_init
    ;

    // class AGDS::Rectangle
    class_<Rectangle, bases<Element> >("Rectangle", init<int, double, double, double, double>())
    ;

    // class AGDS::Structure
    class_<Structure>("Structure", init<std::string>())
        .def("addElement", &Structure::addElement )
        .def("getName"   , &Structure::getName    )
    ;

    // class AGDS::Library
    class_<Library>("Library", init<std::string>())
        .def("setUserUnits", &Library::setUserUnits)
        .def("setPhysUnits", &Library::setPhysUnits)
        .def("addStructure", &Library::addStructure)
        .def("writeToFile" , &Library::writeToFile )
    ;
}
} // namespace
