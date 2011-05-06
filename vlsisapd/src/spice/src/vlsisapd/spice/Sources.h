#ifndef __SPICE_SOURCES_H
#define __SPICE_SOURCES_H

namespace SPICE {
class Source {
    private:
    std::string _name;
    std::string _pos;
    std::string _neg;
    std::string _value;

    protected:
    Source(std::string name, std::string pos, std::string neg, std::string value): _name(name), _pos(pos), _neg(neg), _value(value) {}
    virtual ~Source() {}

    public:
    inline std::string getName();
    inline std::string getPositive();
    inline std::string getNegative();
    inline std::string getValue();
};
inline std::string Source::getName()     { return _name; }
inline std::string Source::getPositive() { return _pos; }
inline std::string Source::getNegative() { return _neg; }
inline std::string Source::getValue()    { return _value; }

class Voltage: public Source {
    public:
    Voltage(std::string name, std::string pos, std::string neg, std::string value): Source(name, pos, neg, value) {}
    ~Voltage() {}
};

class Current: public Source {
    public:
    Current(std::string name, std::string pos, std::string neg, std::string value): Source(name, pos, neg, value) {}
    ~Current() {}
};
}
#endif
