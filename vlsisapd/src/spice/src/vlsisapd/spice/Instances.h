#ifndef __SPICE_INSTANCES_H
#define __SPICE_INSTANCES_H

#include <string>

#include "vlsisapd/spice/Instance.h"

namespace SPICE {
class Mosfet : public Instance {
    public:
    Mosfet(std::string name, std::string nd, std::string ng, std::string ns, std::string nb, std::string model)
          : Instance(name, model), _nd(nd), _ng(ng), _ns(ns), _nb(nb) {}
    virtual ~Mosfet() {};

    inline std::string getDrain();
    inline std::string getGrid();
    inline std::string getSource();
    inline std::string getBulk();

    private:
    std::string _nd;
    std::string _ng;
    std::string _ns;
    std::string _nb;
};
inline std::string Mosfet::getDrain()  { return _nd; }
inline std::string Mosfet::getGrid()   { return _ng; }
inline std::string Mosfet::getSource() { return _ns; }
inline std::string Mosfet::getBulk()   { return _nb; }

class Resistor : public Instance {
    public:
    Resistor(std::string name, std::string first, std::string second, std::string value)
            : Instance(name, ""), _first(first), _second(second), _value(value) {}
    virtual ~Resistor() {};

    inline std::string getFirst();
    inline std::string getSecond();
    inline std::string getValue();

    private:
    std::string _first;
    std::string _second;
    std::string _value;
};
inline std::string Resistor::getFirst()  { return _first;  }
inline std::string Resistor::getSecond() { return _second; }
inline std::string Resistor::getValue()  { return _value;  }

class Capacitor : public Instance {
    public:
    Capacitor(std::string name, std::string pos, std::string neg, std::string value)
             : Instance(name, ""), _pos(pos), _neg(neg), _value(value) {}
    virtual ~Capacitor() {};

    inline std::string getPositive();
    inline std::string getNegative();
    inline std::string getValue();

    private:
    std::string _pos;
    std::string _neg;
    std::string _value;
};
inline std::string Capacitor::getPositive() { return _pos;   };
inline std::string Capacitor::getNegative() { return _neg;   };
inline std::string Capacitor::getValue()    { return _value; };
}
#endif
