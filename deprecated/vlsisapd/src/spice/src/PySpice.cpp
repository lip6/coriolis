
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

#include "vlsisapd/spice/Instances.h"
#include "vlsisapd/spice/Sources.h"
#include "vlsisapd/spice/Subckt.h"
#include "vlsisapd/spice/Circuit.h"
#include "vlsisapd/spice/SpiceException.h"
#include "vlsisapd/spice/PySTLMapWrapper.h"
using namespace std;

namespace SPICE {
void translator(SpiceException const& e) {
    PyErr_SetString(PyExc_UserWarning, e.what());
}

// for optional arguments
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addLibrary_overloads, addLibrary, 1, 2);

BOOST_PYTHON_MODULE(SPICE) {

    // vector and map wrapping for Spice::Instance
    class_<std::vector<std::string> >("ConnectorsVector")
        .def(vector_indexing_suite<std::vector<std::string> >())
    ;
    STL_MAP_WRAPPING(std::string, std::string, "ParametersMap")
    // class SPICE::Instance
    class_<Instance, Instance*>("Instance", init<std::string, std::string>())
        // accessors
        .def("getName"          , &Instance::getName           )
        .def("getModel"         , &Instance::getModel          )
        .def("getParameterValue", &Instance::getParameterValue )
        // modifiers
        .def("addConnector", &Instance::addConnector )
        .def("addParameter", &Instance::addParameter )
        // stl containers
        .def("getConnectors", &Instance::getConnectors, return_value_policy<copy_const_reference>())
        .def("getParameters", &Instance::getParameters, return_value_policy<copy_const_reference>()) 
    ;

    // class SPICE::Mosfet
    class_<Mosfet, bases<Instance> >("Mosfet", init<std::string, std::string, std::string, std::string, std::string, std::string>())
        // accessors
        .def("getDrain" , &Mosfet::getDrain  )
        .def("getGrid"  , &Mosfet::getGrid   )
        .def("getSource", &Mosfet::getSource )
        .def("getBulk"  , &Mosfet::getBulk   )
    ;

    // class SPICE::Capacitor
    class_<Capacitor, bases<Instance> >("Capacitor", init<std::string, std::string, std::string, std::string>())
        // accessors
        .def("getPositive", &Capacitor::getPositive )
        .def("getNegative", &Capacitor::getNegative )
        .def("getValue"   , &Capacitor::getValue    )
    ;

    // class SPICE::Resistor
    class_<Resistor, bases<Instance> >("Resistor", init<std::string, std::string, std::string, std::string>())
        // accessors
        .def("getFirst" , &Resistor::getFirst  )
        .def("getSecond", &Resistor::getSecond )
        .def("getValue" , &Resistor::getValue  )
    ;

    // class SPICE::Source   !! abstract class !!
    class_<Source, Source*, boost::noncopyable>("Source", no_init)
        // accessors
        .def("getName"    , &Source::getName     )
        .def("getPositive", &Source::getPositive )
        .def("getNegative", &Source::getNegative )
        .def("getValue"   , &Source::getValue    )
    ;

    // class SPICE::Voltage
    class_<Voltage, bases<Source> >("Voltage", init<std::string, std::string, std::string, std::string>())
    ;

    // class SPICE::Current
    class_<Current, bases<Source> >("Current", init<std::string, std::string, std::string, std::string>())
    ;


    // vector indexing for Spice::Subckt
    // InterfacesVector = ConnectorsVector already exists
    class_<std::vector<Instance*> >("InstancesVector")
        .def(vector_indexing_suite<std::vector<Instance*>, true>())
    ;
    // ParametersMap is already defined
    // class SPICE::Subckt
    class_<Subckt, Subckt*>("Subckt", init<std::string>())
        // accessors
        .def("getName", &Subckt::getName )
        // modifiers
        .def("addInterface", &Subckt::addInterface )
        .def("addInstance" , &Subckt::addInstance  )
        .def("addParameter", &Subckt::addParameter )
        // stl containers
        .def("getInterfaces", &Subckt::getInterfaces, return_value_policy<copy_const_reference>())
        .def("getInstances" , &Subckt::getInstances , return_value_policy<copy_const_reference>())
        .def("getParameters", &Subckt::getParameters, return_value_policy<copy_const_reference>()) 
    ;

    // vector indexing for Spice::Circuit
    class_<std::vector<std::pair<std::string, std::string> > > ("LibrariesVector")
        .def(vector_indexing_suite<std::vector<std::pair<std::string, std::string> > >())
    ;
    // InstancesVector already exists
    // ParametersMap = OptionsMap already exists
    class_<std::vector<Source*> >("SourcesVector")
        .def(vector_indexing_suite<std::vector<Source*> >())
    ;
    class_<std::vector<Subckt*> >("SubcktsVector")
        .def(vector_indexing_suite<std::vector<Subckt*> >())
    ;
    // class SPICE::Circuit
    class_<Circuit, Circuit*>("Circuit", init<>())
        // properties
        .add_property("title", &Circuit::getTitle, &Circuit::setTitle )
        // modifiers
        .def("addInclude"   , &Circuit::addInclude   )
        .def("addLibrary"   , &Circuit::addLibrary   , addLibrary_overloads())
        .def("addInstance"  , &Circuit::addInstance  )
        .def("addSource"    , &Circuit::addSource    )
        .def("addOption"    , &Circuit::addOption    )
        .def("addParameter" , &Circuit::addParameter )
        .def("addSubckt"    , &Circuit::addSubckt    , return_value_policy<reference_existing_object>())
        // stl containers
        .def("getIncludes"  , &Circuit::getIncludes  , return_value_policy<copy_const_reference>())
        .def("getLibraries" , &Circuit::getLibraries , return_value_policy<copy_const_reference>())
        .def("getOptions"   , &Circuit::getOptions   , return_value_policy<copy_const_reference>())
        .def("getParameters", &Circuit::getParameters, return_value_policy<copy_const_reference>())
        .def("getSubckts"   , &Circuit::getSubckts   , return_value_policy<copy_const_reference>())
        .def("getInstances" , &Circuit::getInstances , return_value_policy<copy_const_reference>())
        .def("getSources"   , &Circuit::getSources   , return_value_policy<copy_const_reference>())
        // others
        .def("readFromFile" , &Circuit::readFromFile , return_value_policy<reference_existing_object>())
        .staticmethod("readFromFile")
        .def("writeToFile"  , &Circuit::writeToFile  )
        
    ;

    // SpiceException translator
    register_exception_translator<SpiceException>(translator)
    ;
}
}
