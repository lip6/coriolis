using namespace std;

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/SimulModel.h"
#include "vlsisapd/openChams/Sizing.h"
#include "vlsisapd/openChams/Layout.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/OpenChamsException.h"

#include "vlsisapd/openChams/PySTLMapWrapper.h"

namespace OpenChams {
void translator(OpenChamsException const& e) {
    PyErr_SetString(PyExc_UserWarning, e.what());
}

BOOST_PYTHON_MODULE(OPENCHAMS) {
    // class OpenChams::Name
    class_<Name>("Name", init<std::string>())
        .def(init<>())
        // accessors
        .def("getString", &Name::getString, return_value_policy<copy_const_reference>()) // return_value_policy because this method return a refenrce on string
        .def("__repr__" , &Name::getString, return_value_policy<copy_const_reference>()) // automatic print for Name object
        // operators
        .def(self == self)
        .def(self == std::string())
        .def(self < self)
    ;

    implicitly_convertible<std::string, Name>();

    // map wrapping for OpenChams::Parameters
    STL_MAP_WRAPPING(Name, double     , "ValuesMap"   )
    STL_MAP_WRAPPING(Name, std::string, "EquationsMap")
    // class OpenChams::Parameters
    class_<Parameters>("Parameters", init<>())
        // accessors
        .def("getValue"    , &Parameters::getValue  )
        .def("getEqValue"  , &Parameters::getEqValue)
        .def("isEmpty"     , &Parameters::isEmpty   )
        // modifiers
        .def("addParameter", static_cast<void(Parameters::*)(Name, double     )>(&Parameters::addParameter))
        .def("addParameter", static_cast<void(Parameters::*)(Name, std::string)>(&Parameters::addParameter))
        // stl containers
        .def("getValues"   , &Parameters::getValues  , return_value_policy<copy_const_reference>())
        .def("getEqValues" , &Parameters::getEqValues, return_value_policy<copy_const_reference>()) 
    ;

    { //this scope is used to define Base as a subenum of SimulModel
    // class OpenChams::SimulModel
    scope simMod = class_<SimulModel>("SimulModel", init<unsigned, SimulModel::Base, SimulModel::Version, std::string>())
        // properties
        .add_property("id"      , &SimulModel::getId      )
        .add_property("base"    , &SimulModel::getBase    )
        .add_property("version" , &SimulModel::getVersion )
        .add_property("filePath", &SimulModel::getFilePath)
    ;

    enum_<SimulModel::Base>("Base")
        .value("BSIM3V3", SimulModel::BSIM3V3)
        .value("BSIM4"  , SimulModel::BSIM4  )
        .value("PSP"    , SimulModel::PSP    )
    ;

    enum_<SimulModel::Version>("Version")
        .value("UNDEFINED", SimulModel::UNDEFINED)
        .value("SVT"      , SimulModel::SVT      )
        .value("HVT"      , SimulModel::HVT      )
        .value("LVT"      , SimulModel::LVT      )
    ;
    }

    // class OpenChams::Transistor
    class_<Transistor, Transistor*>("Transistor", init<Name, Instance*>())
        // properties
        .add_property("name"      , &Transistor::getName      , &Transistor::setName  )
        .add_property("gate"      , &Transistor::getGate      , &Transistor::setGate  )
        .add_property("source"    , &Transistor::getSource    , &Transistor::setSource)
        .add_property("drain"     , &Transistor::getDrain     , &Transistor::setDrain )
        .add_property("bulk"      , &Transistor::getBulk      , &Transistor::setBulk  )
        .add_property("parameters", &Transistor::getParameters                        ) // no setter => params will be readonly
        // modifiers
        .def("addParameter", static_cast<void(Transistor::*)(Name, double     )>(&Transistor::addParameter))
        .def("addParameter", static_cast<void(Transistor::*)(Name, std::string)>(&Transistor::addParameter))
    ;

    // map wrapping and vector_indexing for OpenChams::Instance
    STL_MAP_WRAPPING_PTR(Name, Net*, "ConnectorsMap")
    // class OpenChams::Instance
    class_<Instance, Instance*>("Instance", init<Name, Name, Netlist*>())
        // properties
        .add_property("name"               , &Instance::getName              )
        .add_property("model"              , &Instance::getModel             )
        .add_property("parameters"         , &Instance::getParameters        )
        .add_property("netlist"            , make_function(&Instance::getNetlist ,return_value_policy<reference_existing_object>())) //make_function since we need to specify a return value policy
        // accessors
        .def("hasNoConnectors" , &Instance::hasNoConnectors )
        // modifiers
        .def("addConnector" , &Instance::addConnector )
        .def("connect"      , &Instance::connect      )
        .def("addParameter" , static_cast<void(Transistor::*)(Name, double     )>(&Transistor::addParameter))
        .def("addParameter" , static_cast<void(Transistor::*)(Name, std::string)>(&Transistor::addParameter))
        // stl containers
        .def("getConnectors" , &Instance::getConnectors , return_internal_reference<>())
    ;

    // vector_indexing for OpenChams::Device
    class_<std::vector<Transistor*> >("TransistorsVector")
        .def(vector_indexing_suite<std::vector<Transistor*>, true>())
    ;
    // class OpenChams::Device
    class_<Device, bases<Instance> >("Device", init<Name, Name, Name, bool, Netlist*>())
        // properties
        .add_property("mosType"            , &Device::getMosType           )
        .add_property("sourceBulkConnected", &Device::isSourceBulkConnected)
        // accessors
        .def("hasNoTransistors", &Device::hasNoTransistors)
        // modifiers
        .def("addTransistor", &Device::addTransistor, return_value_policy<reference_existing_object>())
        // stl containers
        .def("getTransistors", &Device::getTransistors, return_internal_reference<>())
    ;


    // vector_indexing for OpenChams::Net
    class_<std::vector<Net::Connection*> >("ConnectionsVector")
        .def(vector_indexing_suite<std::vector<Net::Connection*>, true>())
    ;
    { //this scope is used to define Connection as a subclass of Net
    // class OpenChams::Net
    scope netScope = class_<Net, Net*>("Net", init<Name, Name, bool, Netlist*>())
        // properties
        .add_property("name"    , &Net::getName   )
        .add_property("type"    , &Net::getType   )
        .add_property("external", &Net::isExternal)
        .add_property("netlist" , make_function(&Net::getNetlist, return_value_policy<reference_existing_object>())) 
        // accessors
        .def("hasNoConnections", &Net::hasNoConnections)
        // modifiers
        .def("connectTo"       , &Net::connectTo )
        // stl containers
        .def("getConnections", &Net::getConnections, return_internal_reference<>())
    ;

    // class OpenChams::Net::Connection
    class_<Net::Connection, Net::Connection*>("Connection", init<Name, Name>())
        .add_property("instanceName" , &Net::Connection::getInstanceName )
        .add_property("connectorName", &Net::Connection::getConnectorName) 
    ;
    } // end netScope

    // vector_indexing for OpenChams::Netlist
    class_<std::vector<Instance*> >("InstancesVector")
        .def(vector_indexing_suite<std::vector<Instance*>, true>())
    ;
    class_<std::vector<Net*> >("NetsVector")
        .def(vector_indexing_suite<std::vector<Net*>, true>())
    ;
    // class OpenChams::Netlist
    class_<Netlist, Netlist*>("Netlist", init<Circuit*>())
        // accessors
        .def("hasNoInstances", &Netlist::hasNoInstances)
        .def("hasNoNets"     , &Netlist::hasNoNets     )
        .def("getInstance"   , make_function(&Netlist::getInstance , return_value_policy<reference_existing_object>()))
        .def("getNet"        , make_function(&Netlist::getNet      , return_value_policy<reference_existing_object>()))
        // modifiers
        .def("addInstance"   , &Netlist::addInstance, return_value_policy<reference_existing_object>())
        .def("addDevice"     , &Netlist::addDevice  , return_value_policy<reference_existing_object>())
        .def("addNet"        , &Netlist::addNet     , return_value_policy<reference_existing_object>())
        // stl containers
        .def("getInstances"  , &Netlist::getInstances, return_internal_reference<>())
        .def("getNets"       , &Netlist::getNets     , return_internal_reference<>())
    ;

    // map wrapping for OpenChams::Schematic
    STL_MAP_WRAPPING_PTR(Name, Schematic::Infos*, "SchematicInstancesMap")
    { // this scope is used to define Infos as a subclass of Schematic
    // class OpenChams::Schematic
    scope schematicScope = class_<Schematic, Schematic*>("Schematic", init<Circuit*, double>())
        // properties
        .add_property("zoom" , &Schematic::getZoom)
        // accessors
        .def("hasNoInstances", &Schematic::hasNoInstances)
        // modifiers
        .def("addInstance"   , &Schematic::addInstance)
        // stl containers
        .def("getInstances"  , &Schematic::getInstances, return_internal_reference<>())
    ;

    // class OpenChams::Schematic::Infos
    class_<Schematic::Infos, Schematic::Infos*>("Infos", init<double, double, Name>())
        .add_property("x"       , &Schematic::Infos::getX       )
        .add_property("y"       , &Schematic::Infos::getY       ) 
        .add_property("symmetry", &Schematic::Infos::getSymmetry)
    ;
    } // end schematicScope

    // map wrapping for OpenChams::Operator
    STL_MAP_WRAPPING_PTR(Name, Operator::Constraint*, "ConstraintsMap")
    { // this scope is used to define Constraint as a subclass of Operator
    // class OpenChams::Operator
    scope operatorScope = class_<Operator, Operator*>("Operator", init<Name, Name, unsigned>())
        // properties
        .add_property("name"      , &Operator::getName      )
        .add_property("simulModel", &Operator::getSimulModel)
        .add_property("callOrder" , &Operator::getCallOrder )
        // accessors
        .def("hasNoConstraints", &Operator::hasNoConstraints)
        // modifiers
        .def("addConstraint", static_cast<void(Operator::*)(Name, Name              )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(Name, Name, double      )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(Name, Name, Name        )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(Name, Name, Name, double)>(&Operator::addConstraint))
        // stl containers
        .def("getConstraints", &Operator::getConstraints, return_internal_reference<>())
    ;

    // class OpenChams::Operator::Constraint
    class_<Operator::Constraint, Operator::Constraint*>("Constraint", init<Name, Name, double>())
        .add_property("ref"     , &Operator::Constraint::getRef     )
        .add_property("refParam", &Operator::Constraint::getRefParam) 
        .add_property("factor"  , &Operator::Constraint::getFactor  )
    ;
    } // end operatorScope

    // map wrapping for OpenChams::Sizing
    STL_MAP_WRAPPING_PTR(Name, Operator*, "OperatorsMap")
    // class OpenChams::Sizing
    class_<Sizing, Sizing*>("Sizing", init<Circuit*>())
        // accessors
        .def("hasNoEquations", &Sizing::hasNoEquations)
        .def("hasNoOperators", &Sizing::hasNoOperators)
        // modifiers
        .def("addEquation", &Sizing::addEquation)
        .def("addOperator", &Sizing::addOperator, return_value_policy<reference_existing_object>())
        // stl containers
        .def("getEquations", &Sizing::getEquations, return_internal_reference<>())
        .def("getOperators", &Sizing::getOperators, return_internal_reference<>())
    ;

    // map wrapping for OpenChams::Layout
    STL_MAP_WRAPPING(Name, Name, "LayoutInstancesMap")
    // class OpenChams::Layout
    class_<Layout, Layout*>("Layout", init<Circuit*>())
        // accessors
        .def("hasNoInstance", &Layout::hasNoInstance)
        // modifiers
        .def("addInstance", &Layout::addInstance)
        // stl containers
        .def("getInstances", &Layout::getInstances, return_internal_reference<>())
    ;

    class_<Circuit, Circuit*>("Circuit", init<Name, Name>())
        // properties
        .add_property("name"      , &Circuit::getName      )
        .add_property("techno"    , &Circuit::getTechno    )
        .add_property("parameters", &Circuit::getParameters)
        .add_property("netlist"   , make_function(&Circuit::getNetlist  , return_value_policy<reference_existing_object>()))
        .add_property("schematic" , make_function(&Circuit::getSchematic, return_value_policy<reference_existing_object>()))
        .add_property("sizing"    , make_function(&Circuit::getSizing   , return_value_policy<reference_existing_object>()))
        .add_property("layout"    , make_function(&Circuit::getLayout   , return_value_policy<reference_existing_object>()))
        // accessors
        .def("getValue", &Circuit::getValue)
        // modifiers
        .def("createNetlist"  , &Circuit::createNetlist  , return_value_policy<reference_existing_object>())
        .def("createSchematic", &Circuit::createSchematic, return_value_policy<reference_existing_object>())
        .def("createSizing"   , &Circuit::createSizing   , return_value_policy<reference_existing_object>())
        .def("createLayout"   , &Circuit::createLayout   , return_value_policy<reference_existing_object>())
        .def("addParameter", static_cast<void(Circuit::*)(Name, double     )>(&Circuit::addParameter))
        .def("addParameter", static_cast<void(Circuit::*)(Name, std::string)>(&Circuit::addParameter))
        // others
        .def("readFromFile", &Circuit::readFromFile, return_value_policy<manage_new_object>())
        .staticmethod("readFromFile")
        .def("writeToFile" , &Circuit::writeToFile)
    ;

    // OpenChamsException translator
    register_exception_translator<OpenChamsException>(translator)
    ;
}
}
