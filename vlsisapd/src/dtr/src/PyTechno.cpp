using namespace std;

#include <boost/python.hpp>
using namespace boost::python;

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"
#include "vlsisapd/dtr/DTRException.h"

namespace DTR {
void translator(DTRException const& e) {
    PyErr_SetString(PyExc_UserWarning, e.what());
}

BOOST_PYTHON_MODULE(pyDTR) {
    // class DTR::Name
    class_<Name>("Name", init<std::string>())
        .def("getString", &Name::getString, return_value_policy<copy_const_reference>()) // return_value_policy because this method return a refenrce on string
    ;

    // class DTR::Rule
    class_<Rule>("Rule", init<Name, double, Name, Name, Name, Name>())
        // accessors
        .def("getName"         , &Rule::getName         )
        .def("getType"         , &Rule::getType         )
        .def("getValue"        , &Rule::getValue        )
        .def("getValueAsString", &Rule::getValueAsString)
        .def("getRef"          , &Rule::getRef          )
        .def("getLayer1"       , &Rule::getLayer1       )
        .def("getLayer2"       , &Rule::getLayer2       )
        // modifiers
        .def("setType"         , &Rule::setType         )
    ; 

    // class DTR::ARule derived from DTR::Rule
    class_<ARule, bases<Rule> >("ARule", init<Name, double, Name, Name, Name, Name>())
    ;

    // class DTR::Techno
    class_<Techno>("Techno", init<Name, Name>())
        .def("readFromFile", &Techno::readFromFile, return_value_policy<manage_new_object>())
        .staticmethod("readFromFile")

        .def("getName"         , &Techno::getName )
        .def("getUnit"         , &Techno::getUnit )
        .def("getValue"        , static_cast<double(Techno::*)(Name            )>(&Techno::getValue))
        .def("getValue"        , static_cast<double(Techno::*)(Name, Name      )>(&Techno::getValue))
        .def("getValue"        , static_cast<double(Techno::*)(Name, Name, Name)>(&Techno::getValue))
        .def("getValueAsString", static_cast<string(Techno::*)(Name            )>(&Techno::getValueAsString))
        .def("getValueAsString", static_cast<string(Techno::*)(Name, Name      )>(&Techno::getValueAsString))
        .def("getValueAsString", static_cast<string(Techno::*)(Name, Name, Name)>(&Techno::getValueAsString))
    ;

    // DTRException translator
    register_exception_translator<DTRException>(translator)
    ;
}
}
