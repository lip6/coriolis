using namespace std;

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/WireLoad.h"
#include "vlsisapd/liberty/WireLoadArea.h"
#include "vlsisapd/liberty/WireLoadSelection.h"
#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/Timing.h"
#include "vlsisapd/liberty/FlipFlop.h"
#include "vlsisapd/liberty/Library.h"

#include "vlsisapd/liberty/PySTLMapWrapper.h"

namespace LIB {
//void translator(LibertyException const& e) {
//    PyErr_SetString(PyExc_UserWarning, e.what());
//}

// specify that Techno::addRule method has optional arguments
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addAttribute_overloads, addAttribute, 3, 4);  

BOOST_PYTHON_MODULE(LIBERTY) {
    // class Liberty::Name
    //////////////////////
    class_<Name>("Name", init<std::string>())
        // accessors
        .def("getString", &Name::getString, return_value_policy<copy_const_reference>()) // return_value_policy because this method return a reference on string
        .def("__repr__" , &Name::getString, return_value_policy<copy_const_reference>()) // automatic print for Name object
        // operators
        .def(self == self)
        .def(self == std::string())
        .def(self < self)
    ;

    implicitly_convertible<std::string, Name>();

    // class Liberty::Attribute
    ///////////////////////////
    {
    scope attrib = class_<Attribute, Attribute*>("Attribute", init<Name, Attribute::Type, const std::string&, const std::string&, const std::string&>())
        // properties
        .add_property("name",       &Attribute::getName) // no setter => readonly
        .add_property("type",       &Attribute::getType) // no setter => readonly
        .add_property("unit", make_function(&Attribute::getUnit, return_value_policy<copy_const_reference>())) // no setter => readonly // return reference
        // accessors
        .def("valueAsString",       &Attribute::valueAsString, return_value_policy<copy_const_reference>()) // return reference
        .def("secondValueAsString", &Attribute::secondValueAsString, return_value_policy<copy_const_reference>()) // return reference
        .def("valueAsBool",         &Attribute::valueAsBool)
        .def("valueAsInt",          &Attribute::valueAsInt)
        .def("valueAsDouble",       &Attribute::valueAsDouble)
        .def("secondValueAsDouble", &Attribute::secondValueAsDouble)
        .def("typeToString",        &Attribute::typeToString)
        // miscellaneous
        .def("write",               &Attribute::write)
    ;

    enum_<Attribute::Type>("Type")
        .value("Unknown", Attribute::Unknown)
        .value("String" , Attribute::String )
        .value("Bool"   , Attribute::Bool   )
        .value("Int"    , Attribute::Int    )
        .value("Double" , Attribute::Double )
        .value("Unit"   , Attribute::Unit   )
        .value("Pair"   , Attribute::Pair   )
//        .export_values()
    ;
    }

    // map wrapping
    STL_MAP_WRAPPING_PTR(Name, Attribute*, "AttributesMap")
    STL_MAP_WRAPPING_PTR(Name, WireLoad*,  "WiresLoadMap")
    STL_MAP_WRAPPING_PTR(Name, Cell*,      "CellsMap")
    STL_MAP_WRAPPING_PTR(Name, Pin*,       "PinsMap")

    class_<std::vector<WireLoadArea*> >("WiresLoadAreaVector")
        .def(vector_indexing_suite<std::vector<WireLoadArea*>, true>())
    ;

    class_<std::vector<Timing*> >("TimingsVector")
        .def(vector_indexing_suite<std::vector<Timing*>, true>())
    ;

    // class Liberty::WireLoad
    //////////////////////////
    class_<WireLoad, WireLoad*>("WireLoad", init<Name>())
        // properties
        .add_property("name", &WireLoad::getName) // no setter => readonly
        // accessors
        .def("getAttributes", &WireLoad::getAttributes, return_internal_reference<>())
        .def("getAttribute",  &WireLoad::getAttribute,  return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",  &WireLoad::addAttribute, addAttribute_overloads())
        // miscellaneous
        .def("__repr__",      &WireLoad::getString)
        .def("write",         &WireLoad::write)
    ;

    // class Liberty::WireLoadArea
    //////////////////////////////
    class_<WireLoadArea, WireLoadArea*>("WireLoadArea", init<double, double, Name>())
        // properties
        .add_property("min",  &WireLoadArea::getMin) // no setter => readonly
        .add_property("max",  &WireLoadArea::getMax) // no setter => readonly
        .add_property("name", &WireLoadArea::getName) // no setter => readonly
        // miscellaneous
        .def("write",         &WireLoad::write)
    ;

    // class Liberty::WireLoadSelection
    ///////////////////////////////////
    class_<WireLoadSelection, WireLoadSelection*>("WireLoadSelection", init<Name>())
        // properties
        .add_property("name",    &WireLoadSelection::getName) // no setter => readonly
        // accessors
        .def("getWiresLoadArea", &WireLoadSelection::getWiresLoadArea, return_internal_reference<>())
        // modifiers
        .def("addWireLoadArea",  &WireLoadSelection::addWireLoadArea)
        // miscellaneous
        .def("__repr__",         &WireLoadSelection::getString)
        .def("write",            &WireLoadSelection::write)
    ;

    // class Liberty::Cell
    //////////////////////
    class_<Cell, Cell*>("Cell", init<Name>())
        // properties
        .add_property("name", &Cell::getName) // no setter => readonly
        // accessors
        .def("getAttributes", &Cell::getAttributes, return_internal_reference<>())
        .def("getPins",       &Cell::getPins,       return_internal_reference<>())
        .def("getAttribute",  &Cell::getAttribute,  return_value_policy<reference_existing_object>())
        .def("getPin",        &Cell::getPin,        return_value_policy<reference_existing_object>())
        .def("getFF",         &Cell::getFF,         return_value_policy<reference_existing_object>())
        .def("getTestCell",   &Cell::getTestCell,   return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",  &Cell::addAttribute)
        .def("addPin",        &Cell::addPin)
        .def("addFF",         &Cell::addFF)
        .def("setTestCell",   &Cell::setTestCell)
        // miscellaneous
        .def("__repr__",      &Cell::getString)
        .def("write",         &Cell::write)
    ;

    // class Liberty::Pin
    /////////////////////
    class_<Pin, Pin*>("Pin", init<Name>())
        // properties
        .add_property("name", &Pin::getName) // no setter => readonly
        // accessors
        .def("getAttributes", &Pin::getAttributes, return_internal_reference<>())
        .def("getTimings",    &Pin::getTimings,    return_internal_reference<>())
        .def("getAttribute",  &Pin::getAttribute,  return_value_policy<reference_existing_object>())
        .def("getTiming",     &Pin::getTiming,     return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",  &Pin::addAttribute)
        .def("addTiming",     &Pin::addTiming)
        // miscellaneous
        .def("__repr__",      &Pin::getString)
        .def("write",         &Pin::write)
    ;

    // class Liberty::Timing
    ////////////////////////
    class_<Timing, Timing*>("Timing", init<>())
        // accessors
        .def("getAttributes", &Timing::getAttributes, return_internal_reference<>())
        .def("getAttribute",  &Timing::getAttribute,  return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",  &Timing::addAttribute)
        // miscellaneous
        .def("__repr__",      &Timing::getString)
        .def("write",         &Timing::write)
    ;

    // class Liberty::FlipFlop
    //////////////////////////
    class_<FlipFlop, FlipFlop*>("FlipFlop", init<Name, Name>())
        // properties
        .add_property("noninverting", &FlipFlop::getNonInverting) // no setter => readonly
        .add_property("inverting",    &FlipFlop::getInverting) // no setter => readonly
        // accessors
        .def("getAttributes",         &FlipFlop::getAttributes, return_internal_reference<>())
        .def("getAttribute",          &FlipFlop::getAttribute,  return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",          &FlipFlop::addAttribute)
        // miscellaneous
        .def("__repr__",              &FlipFlop::getString)
        .def("write",                 &FlipFlop::write)
    ;

    // class Liberty::Library
    /////////////////////////
    class_<Library, Library*>("Library", init<Name>())
        // properties
        .add_property("name",        &Library::getName) // no setter => readonly
        // accessors                 
        .def("getAttributes",        &Library::getAttributes,         return_internal_reference<>())
        .def("getWiresLoad",         &Library::getWiresLoad,          return_internal_reference<>())
        .def("getCells",             &Library::getCells,              return_internal_reference<>())
        .def("getAttribute",         &Library::getAttribute,          return_value_policy<reference_existing_object>())
        .def("getWireLoad",          &Library::getWireLoad,           return_value_policy<reference_existing_object>())
        .def("getCell",              &Library::getCell,               return_value_policy<reference_existing_object>())
        .def("getWireLoadSelection", &Library::getWireLoadSelection,  return_value_policy<reference_existing_object>())
        // modifiers
        .def("addAttribute",         &Library::addAttribute, addAttribute_overloads())
        .def("addWireLoad",          &Library::addWireLoad)
        .def("addWireLoadSelection", &Library::addWireLoadSelection)
        .def("addCell",              &Library::addCell)
        // miscellaneous
        .def("__repr__",             &Library::getString)
        .def("readFromFile",         &Library::readFromFile, return_value_policy<reference_existing_object>())
        .staticmethod("readFromFile")
        .def("writeToFile",          &Library::writeToFile)
    ;


    // LibertyException translator
//    register_exception_translator<LibertyException>(translator)
//    ;
}
}
