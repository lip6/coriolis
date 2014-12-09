

#include <boost/python.hpp>
using namespace boost::python;

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"
#include "vlsisapd/dtr/DTRException.h"
using namespace std;


namespace DTR {

  void translator(DTRException const& e) {
    PyErr_SetString(PyExc_UserWarning, e.what());
  }


// specify that Techno::getRule & Techno::getValue & Techno::getValueAsString methods have optional arguments
  BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getRule_overloads         , getRule         , 1, 3);
  BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getValue_overloads        , getValue        , 1, 3);
  BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getValueAsString_overloads, getValueAsString, 1, 3);
// specify that Techno::addRule method has optional arguments
  BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addRule_overloads, addRule, 3, 5);


  BOOST_PYTHON_MODULE(DTR) {

  // class DTR::Rule
    class_<Rule>("Rule", init<const char*, double, const char*, const char*, const char*>())
    // Accessors.
      .def("getName"         , &Rule::getName         , return_value_policy<copy_const_reference>())
      .def("getType"         , &Rule::getType         , return_value_policy<copy_const_reference>())
      .def("getValue"        , &Rule::getValue        )
      .def("getValueAsString", &Rule::getValueAsString, return_value_policy<copy_const_reference>())
      .def("getRef"          , &Rule::getRef          , return_value_policy<copy_const_reference>())
      .def("getLayer1"       , &Rule::getLayer1       , return_value_policy<copy_const_reference>())
      .def("getLayer2"       , &Rule::getLayer2       , return_value_policy<copy_const_reference>())
    // Mutators.
      .def("setType"         , &Rule::setType         )
      .def("setValue"        , &Rule::setValue        )
      .def("setRef"          , &Rule::setRef          )
      ; 

  // class DTR::ARule derived from DTR::Rule
    class_<ARule,bases<Rule> >("ARule", init<const char*, double, const char*, const char*, const char*>())
      ;

  // class DTR::Techno
    class_<Techno>("Techno", init<const char*, const char*, const char*>())
    // Accessors
      .def("getName"         , &Techno::getName         , return_value_policy<copy_const_reference>())
      .def("getUnit"         , &Techno::getUnit         , return_value_policy<copy_const_reference>())
      .def("getVersion"      , &Techno::getVersion      , return_value_policy<copy_const_reference>())
      .def("getRule"         , &Techno::getRule         , getRule_overloads()[return_value_policy<reference_existing_object>()])
      .def("getValue"        , &Techno::getValue        , getValue_overloads())
      .def("getValueAsString", &Techno::getValueAsString, getValueAsString_overloads()[return_value_policy<copy_const_reference>()])
        
    // Mutators.
      .def("setName"         , &Techno::setName)
      .def("setUnit"         , &Techno::setUnit)
      .def("setVersion"      , &Techno::setVersion)
      .def("addRule"         , &Techno::addRule , addRule_overloads()[return_value_policy<reference_existing_object>()])
      .def("addARule"        , &Techno::addARule, return_value_policy<reference_existing_object>())

    // Miscellaneous.
      .def("readFromFile", &Techno::readFromFile, return_value_policy<reference_existing_object>())
      .staticmethod("readFromFile")
      .def("writeToFile" , &Techno::writeToFile)
      ;

  // DTRException translator
    register_exception_translator<DTRException>(translator)
      ;

  } // DTR boost::python module.


}  // DTR namespace.
