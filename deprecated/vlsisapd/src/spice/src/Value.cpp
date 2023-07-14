#include <iostream>
#include <sstream>
using namespace std;

#include "vlsisapd/spice/Value.h"
#include "vlsisapd/spice/SpiceException.h"

namespace SPICE {
Value::Value(double value, Value::Unit unit): _value(value), _unit(unit)
{}

double Value::getUnitAsDouble(Value::Unit unit) {
    switch(unit) {
        case Value::T:     return 1e12;  // T
        case Value::G:     return 1e9;   // G
        case Value::Meg:   return 1e6;   // Meg
        case Value::K:     return 1e3;   // K
        case Value::Unity: return 1;     // Unity
        case Value::m:     return 1e-3;  // m
        case Value::u:     return 1e-6;  // u
        case Value::n:     return 1e-9;  // n
        case Value::p:     return 1e-12; // p
        case Value::f:     return 1e-15; // f
        default: throw SpiceException("SPICE:: Unknown unit for Value !");
    }
}

double Value::getConvertedDouble(Value::Unit unit) {
    return _value * (getUnitAsDouble(_unit)/getUnitAsDouble(unit));
}

void Value::print() {
    switch(_unit) {
    case Value::T:     cerr << _value << " T"   << endl; break;
    case Value::G:     cerr << _value << " G"   << endl; break;
    case Value::Meg:   cerr << _value << " Meg" << endl; break;
    case Value::K:     cerr << _value << " K"   << endl; break;
    case Value::m:     cerr << _value << " m"   << endl; break;
    case Value::u:     cerr << _value << " u"   << endl; break;
    case Value::n:     cerr << _value << " n"   << endl; break;
    case Value::p:     cerr << _value << " p"   << endl; break;
    case Value::f:     cerr << _value << " f"   << endl; break;
    default:           cerr << _value           << endl; break;
    }
}

Value* Value::string2Value(string str) {
    size_t it = str.find_last_of("0123456789");
    if (it == string::npos)
        throw SpiceException("SPICE::Value::string2Value: Could not correctly identify value.");
    istringstream iss;
    iss.str(str.substr(0, it+1));
    double val;
    iss >> val;
    string unit = str.substr(it+1);
    Value* value= NULL;
    if      (!unit.compare(0, 1, "T")  )  value = new Value(val, Value::T);
    else if (!unit.compare(0, 1, "G")  )  value = new Value(val, Value::G);
    else if (!unit.compare(0, 3, "Meg"))  value = new Value(val, Value::Meg);
    else if (!unit.compare(0, 1, "K")  )  value = new Value(val, Value::K);
    else if (!unit.compare(0, 1, "m")  )  value = new Value(val, Value::m);
    else if (!unit.compare(0, 1, "u")  )  value = new Value(val, Value::u);
    else if (!unit.compare(0, 1, "n")  )  value = new Value(val, Value::n);
    else if (!unit.compare(0, 1, "p")  )  value = new Value(val, Value::p);
    else if (!unit.compare(0, 1, "f")  )  value = new Value(val, Value::f);
    else                                  value = new Value(val, Value::Unity);
    return value;
}
}
