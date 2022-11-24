#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

#include "vlsisapd/liberty/Attribute.h"

namespace LIB {
Attribute::Attribute(const Name name, Type type, const std::string& value, const std::string& unit, const std::string& value2) : _name (name), _type(type), _value(value), _unit(unit), _value2(value2) {};

bool Attribute::valueAsBool() const {
    if ( _type != Bool )
        cerr << "[ERROR] Accessing parameter " << _name.getString() << " (type mismatch)." << endl;

    istringstream s ( _value );
    bool b;
    s >> boolalpha >> b;
    return b;
}

int Attribute::valueAsInt() const {
    if ( _type != Int )
        cerr << "[ERROR] Accessing parameter " << _name.getString() << " (type mismatch)." << endl;

    istringstream s ( _value );
    int i;
    s >> i;
    return i;
}

double Attribute::valueAsDouble() const {
    if ( (_type != Double) & (_type != Pair) )
        cerr << "[ERROR] Accessing parameter " << _name.getString() << " (type mismatch)." << endl;

    std::istringstream s ( _value );
    double d;
    s >> d;
    return d;
}

double Attribute::secondValueAsDouble() const {
    if ( _type != Pair )
        cerr << "[ERROR] Accessing parameter " << _name.getString() << " (type mismatch)." << endl;

    double d;
    if(!_value2.empty())
    {
        std::istringstream s ( _value2 );
        s >> d;
    } else {
        d = 0;
    }
    return d;
}

string Attribute::typeToString() {
    switch(_type) {
      case Unknown: return "unknown";
      case String:  return "string";
      case Bool:    return "bool";
      case Int:     return "int";
      case Double:  return "double";
      case Unit:    return "unit";
      case Pair:    return "pair";
    }
    return "unsupported";
}

bool Attribute::write(ofstream &file) {
    if( (_type == Attribute::String) | (_type == Attribute::Double) ) {
        if( (_name.getString() == "cell_footprint") | (_name.getString() == "function") | (_name.getString() == "related_pin") | (_name.getString() == "clocked_on") | (_name.getString() == "next_state"))
            file << _name.getString() << " : \"" << _value << "\";" << endl;
        else
            file << _name.getString() << " : " << _value << ";" << endl;
    } else if(_type == Attribute::Unit) {
        if( (_name.getString() == "capacitive_load_unit"))
            file << _name.getString() << "(" << _value << "," << _unit << ");" << endl;
        else
            file << _name.getString() << " : \"" << _value << _unit << "\";" << endl;
    } else if(_type == Attribute::Pair) {
            file << _name.getString() << "(" << _value << "," << _value2 << ");" << endl;
    }

    return true;
}
} // namespace
