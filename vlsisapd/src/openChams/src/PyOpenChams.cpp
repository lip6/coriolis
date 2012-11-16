using namespace std;

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

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
#include "vlsisapd/openChams/Node.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/Port.h"
#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

#include "vlsisapd/openChams/PySTLMapWrapper.h"

namespace OpenChams {

void translator(OpenChamsException const& e) {
    PyErr_SetString(PyExc_UserWarning, e.what());
}

BOOST_PYTHON_MODULE(OPENCHAMS) {
    // map wrapping for OpenChams::Parameters
    STL_MAP_WRAPPING(std::string, std::string, "ValuesMap")
    // class OpenChams::Parameters
    class_<Parameters>("Parameters", init<>())
        // accessors
        .def("getValue"    , &Parameters::getValue, return_value_policy<copy_const_reference>())
        .def("isEmpty"     , &Parameters::isEmpty   )
        // modifiers
        .def("addParameter", static_cast<void(Parameters::*)(const std::string&, const std::string&)>(&Parameters::addParameter))
        // stl containers
        .def("getValues"   , &Parameters::getValues  , return_value_policy<copy_const_reference>())
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
    class_<Transistor, Transistor*>("Transistor", init<const std::string&, Instance*>())
        // properties
        .add_property("name"      , make_function(&Transistor::getName  ,return_value_policy<copy_const_reference>()), &Transistor::setName  )
        .add_property("gate"      , make_function(&Transistor::getGate  ,return_value_policy<copy_const_reference>()), &Transistor::setGate  )
        .add_property("source"    , make_function(&Transistor::getSource,return_value_policy<copy_const_reference>()), &Transistor::setSource)
        .add_property("drain"     , make_function(&Transistor::getDrain ,return_value_policy<copy_const_reference>()), &Transistor::setDrain )
        .add_property("bulk"      , make_function(&Transistor::getBulk  ,return_value_policy<copy_const_reference>()), &Transistor::setBulk  )
        .add_property("parameters", &Transistor::getParameters                        ) // no setter => params will be readonly
        // modifiers
        .def("addParameter", static_cast<void(Transistor::*)(const std::string&, const char*)>(&Transistor::addParameter))
        .def("addParameter", static_cast<void(Transistor::*)(const std::string&, const std::string&)>(&Transistor::addParameter))
    ;

    // map wrapping and vector_indexing for OpenChams::Instance
    STL_MAP_WRAPPING_PTR(std::string, Net*, "ConnectorsMap")
    // class OpenChams::Instance
    class_<Instance, Instance*>("Instance", init<const std::string&, const std::string&, unsigned, Netlist*>())
        // properties
        .add_property("name"               , make_function(&Instance::getName ,return_value_policy<copy_const_reference>()))
        .add_property("model"              , make_function(&Instance::getModel,return_value_policy<copy_const_reference>()))
        .add_property("order"              , &Instance::getOrder             )
        .add_property("parameters"         , &Instance::getParameters        )
        .add_property("netlist"            , make_function(&Instance::getNetlist ,return_value_policy<reference_existing_object>())) //make_function since we need to specify a return value policy
        // accessors
        .def("hasNoConnectors" , &Instance::hasNoConnectors )
        // modifiers
        .def("addConnector" , &Instance::addConnector )
        .def("connect"      , &Instance::connect      )
        .def("addParameter" , static_cast<void(Transistor::*)(const std::string&, const char*)>(&Transistor::addParameter))
        .def("addParameter" , static_cast<void(Transistor::*)(const std::string&, const std::string&)>(&Transistor::addParameter))
        // stl containers
        .def("getConnectors" , &Instance::getConnectors , return_internal_reference<>())
    ;

    // vector_indexing for OpenChams::Device
    class_<std::vector<Transistor*> >("TransistorsVector")
        .def(vector_indexing_suite<std::vector<Transistor*>, true>())
    ;
    // class OpenChams::Device
    class_<Device, bases<Instance> >("Device", init<const std::string&, const std::string&, unsigned, const std::string&, bool, Netlist*>())
        // properties
        .add_property("mosType"            , make_function(&Device::getMosType,return_value_policy<copy_const_reference>()) )
        .add_property("sourceBulkConnected", &Device::isSourceBulkConnected)
        // accessors
        .def("hasNoTransistors", &Device::hasNoTransistors)
        // modifiers
        .def("addTransistor", &Device::addTransistor, return_value_policy<reference_existing_object>())
        // stl containers
        .def("getTransistors", &Device::getTransistors, return_internal_reference<>())
    ;

    // class OpenChams::Port
    class_<Port, Port*>("Port", init<const std::string&, unsigned, double, double, const std::string&>())
        // properties
        .add_property("type"       , make_function(&Port::getType,return_value_policy<copy_const_reference>()))
        .add_property("index"      , &Port::getIndex      )
        .add_property("x"          , &Port::getX          )
        .add_property("y"          , &Port::getY          )
        .add_property("orientation", make_function(&Port::getOrientation,return_value_policy<copy_const_reference>()))
    ;

    // class OpenChams::WirePoint
    class_<WirePoint, WirePoint*>("WirePoint", init<>())
    ;
    // class OpenChams::InstancePoint
    class_<InstancePoint, bases<WirePoint> >("InstancePoint", init<const std::string&, const std::string&>())
        // properties
        .add_property("name", make_function(&InstancePoint::getName, return_value_policy<copy_const_reference>()))
        .add_property("plug", make_function(&InstancePoint::getPlug, return_value_policy<copy_const_reference>()))
    ;
    // class OpenChams::PortPoint
    class_<PortPoint, bases<WirePoint> >("PortPoint", init<unsigned>())
        // properties
        .add_property("index", &PortPoint::getIndex)
    ;
    // class OpenChams::IntermediatePoint
    class_<IntermediatePoint, bases<WirePoint> >("IntermediatePoint", init<double, double>())
        // properties
        .add_property("x", &IntermediatePoint::getX)
        .add_property("y", &IntermediatePoint::getY)
    ;

    // vector_indexing for OpenChams::Wire
    class_<std::vector<IntermediatePoint*> >("IntermediatePointsVector")
        .def(vector_indexing_suite<std::vector<IntermediatePoint*>, true>())
    ;
    // class OpenChams::Wire
    class_<Wire, Wire*>("Wire", init<>())
        // properties
        .add_property("startPoint", make_function(&Wire::getStartPoint, return_value_policy<reference_existing_object>()))
        .add_property("endPoint"  , make_function(&Wire::getEndPoint  , return_value_policy<reference_existing_object>()))
        // accessors
        .def("hasNoIntermediatePoints", &Wire::hasNoIntermediatePoints)
        // modifiers
        .def("setStartPoint"       , static_cast<void(Wire::*)(const std::string&, const std::string&)>(&Wire::setStartPoint))
        .def("setStartPoint"       , static_cast<void(Wire::*)(unsigned  )>(&Wire::setStartPoint))
        .def("setEndPoint"         , static_cast<void(Wire::*)(const std::string&, const std::string&)>(&Wire::setEndPoint))
        .def("setEndPoint"         , static_cast<void(Wire::*)(unsigned  )>(&Wire::setEndPoint))
        .def("addIntermediatePoint", &Wire::addIntermediatePoint)
        // stl containers
        .def("getIntermediatePoints"  , &Wire::getIntermediatePoints, return_internal_reference<>())
    ;

    // vector_indexing for OpenChams::Net
    class_<std::vector<Net::Connection*> >("ConnectionsVector")
        .def(vector_indexing_suite<std::vector<Net::Connection*>, true>())
    ;
    class_<std::vector<Port*> >("PortsVector")
        .def(vector_indexing_suite<std::vector<Port*>, true>())
    ;
    class_<std::vector<Wire*> >("WiresVector")
        .def(vector_indexing_suite<std::vector<Wire*>, true>())
    ;
    { //this scope is used to define Connection as a subclass of Net
    // class OpenChams::Net
    scope netScope = class_<Net, Net*>("Net", init<const std::string&, const std::string&, bool, Netlist*>())
        // properties
        .add_property("name"    , make_function(&Net::getName   ,return_value_policy<copy_const_reference>()))
        .add_property("type"    , make_function(&Net::getType   ,return_value_policy<copy_const_reference>()))
        .add_property("external", &Net::isExternal)
        .add_property("netlist" , make_function(&Net::getNetlist, return_value_policy<reference_existing_object>())) 
        // accessors
        .def("hasNoConnections", &Net::hasNoConnections)
        .def("hasNoPorts"      , &Net::hasNoPorts      )
        .def("hasNoWires"      , &Net::hasNoWires      )
        // modifiers
        .def("connectTo"       , &Net::connectTo )
        .def("addPort"         , &Net::addPort, return_value_policy<reference_existing_object>())
        .def("addWire"         , &Net::addWire, return_value_policy<reference_existing_object>())
        // stl containers
        .def("getConnections", &Net::getConnections, return_internal_reference<>())
        .def("getPorts"      , &Net::getPorts      , return_internal_reference<>())
        .def("getWires"      , &Net::getWires      , return_internal_reference<>())
    ;

    // class OpenChams::Net::Connection
    class_<Net::Connection, Net::Connection*>("Connection", init<const std::string&, const std::string&>())
      .add_property("instanceName" , make_function(&Net::Connection::getInstanceName ,return_value_policy<copy_const_reference>()))
      .add_property("connectorName", make_function(&Net::Connection::getConnectorName,return_value_policy<copy_const_reference>())) 
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
    STL_MAP_WRAPPING_PTR(std::string, Schematic::Infos*, "SchematicInstancesMap")
    { // this scope is used to define Infos as a subclass of Schematic
    // class OpenChams::Schematic
    scope schematicScope = class_<Schematic, Schematic*>("Schematic", init<Circuit*>())
        // accessors
        .def("hasNoInstances", &Schematic::hasNoInstances)
        // modifiers
        .def("addInstance"   , &Schematic::addInstance)
        // stl containers
        .def("getInstances"  , &Schematic::getInstances, return_internal_reference<>())
    ;

    // class OpenChams::Schematic::Infos
    class_<Schematic::Infos, Schematic::Infos*>("Infos", init<double, double, const std::string&>())
        .add_property("x"          , &Schematic::Infos::getX          )
        .add_property("y"          , &Schematic::Infos::getY          ) 
        .add_property("orientation", make_function(&Schematic::Infos::getOrientation,return_value_policy<copy_const_reference>()))
    ;
    } // end schematicScope

    // map wrapping for OpenChams::Operator
    STL_MAP_WRAPPING_PTR(std::string, Operator::Constraint*, "ConstraintsMap")
    { // this scope is used to define Constraint as a subclass of Operator
    // class OpenChams::Operator
    scope operatorScope = class_<Operator, Operator*>("Operator", init<const std::string&, const std::string&>())
        // properties
        .add_property("name"      , make_function(&Operator::getName      , return_value_policy<copy_const_reference>()))
        .add_property("simulModel", make_function(&Operator::getSimulModel, return_value_policy<copy_const_reference>()))
        // accessors
        .def("hasNoConstraints", &Operator::hasNoConstraints)
        // modifiers
        .def("addConstraint", static_cast<void(Operator::*)(const std::string&, const std::string&              )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(const std::string&, const std::string&, double      )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(const std::string&, const std::string&, const std::string&        )>(&Operator::addConstraint))
        .def("addConstraint", static_cast<void(Operator::*)(const std::string&, const std::string&, const std::string&, double)>(&Operator::addConstraint))
        // stl containers
        .def("getConstraints", &Operator::getConstraints, return_internal_reference<>())
    ;

    // class OpenChams::Operator::Constraint
    class_<Operator::Constraint, Operator::Constraint*>("Constraint", init<const std::string&, const std::string&, double>())
        .add_property("ref"     , make_function(&Operator::Constraint::getRef     , return_value_policy<copy_const_reference>()))
        .add_property("refParam", make_function(&Operator::Constraint::getRefParam, return_value_policy<copy_const_reference>())) 
        .add_property("factor"  , &Operator::Constraint::getFactor  )
    ;
    } // end operatorScope

    /*
    // map wrapping for OpenChams::Sizing
    STL_MAP_WRAPPING_PTR(std::string, Operator*, "OperatorsMap")
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
    */

    // map wrapping for OpenChams::Layout
    STL_MAP_WRAPPING(std::string, std::string, "LayoutInstancesMap")
    // class OpenChams::Layout
    class_<Layout, Layout*>("Layout", init<Circuit*>())
        // properties
        .add_property("hbTreeRoot", make_function(&Layout::getHBTreeRoot, return_value_policy<reference_existing_object>()), &Layout::setHBTreeRoot)
        // accessors
        .def("hasNoInstance", &Layout::hasNoInstance)
        // modifiers
        .def("addInstance", &Layout::addInstance)
        // stl containers
        .def("getInstances", &Layout::getInstances, return_internal_reference<>())
    ;

    class_<Circuit, Circuit*>("Circuit", init<const std::string&, const std::string&>())
        // properties
        .add_property("name"        , make_function(&Circuit::getName        ,return_value_policy<copy_const_reference>()))
        .add_property("techno"      , make_function(&Circuit::getTechno      ,return_value_policy<copy_const_reference>()))
        .add_property("parameters"  , &Circuit::getParameters  )
        .add_property("netlist"     , make_function(&Circuit::getNetlist  , return_value_policy<reference_existing_object>()))
        .add_property("schematic"   , make_function(&Circuit::getSchematic, return_value_policy<reference_existing_object>()))
        .add_property("sizing"      , make_function(&Circuit::getSizing   , return_value_policy<reference_existing_object>()))
        .add_property("layout"      , make_function(&Circuit::getLayout   , return_value_policy<reference_existing_object>()))
        // accessors
        .def("getValue", &Circuit::getValue, return_value_policy<copy_const_reference>() )
        // modifiers
        .def("createNetlist"  , &Circuit::createNetlist  , return_value_policy<reference_existing_object>())
        .def("createSchematic", &Circuit::createSchematic, return_value_policy<reference_existing_object>())
        .def("createSizing"   , &Circuit::createSizing   , return_value_policy<reference_existing_object>())
        .def("createLayout"   , &Circuit::createLayout   , return_value_policy<reference_existing_object>())
        .def("addParameter", static_cast<void(Circuit::*)(const std::string&, const char*)>(&Circuit::addParameter))
        .def("addParameter", static_cast<void(Circuit::*)(const std::string&, const std::string&)>(&Circuit::addParameter))
        // others
        .def("readFromFile", &Circuit::readFromFile, return_value_policy<manage_new_object>())
        .staticmethod("readFromFile")
        .def("writeToFile" , &Circuit::writeToFile)
    ;

    { //this scope is used to define Position as a subenum of Node
    // class OpenChams::Node
    scope nod = class_<Node, Node*, boost::noncopyable>("Node", no_init)
        // properties
        .add_property("top"  , make_function(&Node::getTop  , return_value_policy<reference_existing_object>()), &Node::setTop  )
        .add_property("right", make_function(&Node::getRight, return_value_policy<reference_existing_object>()), &Node::setRight)
        // accessors
        .def("getName"    , &Node::getName    , return_value_policy<copy_const_reference>())
        .def("getPosition", &Node::getPosition)
        .def("getParent"  , &Node::getParent  , return_value_policy<reference_existing_object>())
        .def("isRoot"     , &Node::isRoot     )
    ;

    enum_<Node::Position>("Position")
        .value("NONE" , Node::NONE )
        .value("RIGHT", Node::RIGHT)
        .value("TOP"  , Node::TOP  )
        .export_values()
    ;
    } // end of node scope

    // class OpenChams::Bloc
    class_<Bloc, bases<Node> >("Bloc", init<const std::string&, optional<Node::Position, Node*> >())
    ;

    { // this scope is used to define Align as a subenum of Group
    // class OpenChams::Group
    scope grou = class_<Group, bases<Node> >("Group", init<const std::string&, optional<Node::Position, Node*> >())
        .add_property("rootNode", make_function(&Group::getRootNode, return_value_policy<reference_existing_object>()), &Group::setRootNode)
        .add_property("isolated", &Group::isIsolated, &Group::setIsolated)
        .add_property("paired"  , &Group::isPaired  , &Group::setPaired  )
        .add_property("align"   , &Group::getAlign  , &Group::setAlign   )
    ;

    enum_<Group::Align>("Align")
        .value("NONE"      , Group::NONE      )
        .value("VERTICAL"  , Group::VERTICAL  )
        .value("HORIZONTAL", Group::HORIZONTAL)
    ;
    } // end of group scope

    // OpenChamsException translator
    register_exception_translator<OpenChamsException>(translator)
    ;
}


}  // OpenChams namespace.
