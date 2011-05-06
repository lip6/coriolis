#ifndef __SPICE_VALUE_H
#define __SPICE_VALUE_H

#include <string>

namespace SPICE {
class Value {
    public:
    enum Unit { T     = 0
              , G     = 1
              , Meg   = 2 
              , K     = 3
              , Unity = 4
              , m     = 5
              , u     = 6 
              , n     = 7
              , p     = 8
              , f     = 9
              };

    private:
    double _value;
    Unit   _unit;

    public:
    Value(double value, Unit unit);
    virtual ~Value() {};

    inline double getDouble();
    inline Unit getUnit();
    static double getUnitAsDouble(Unit unit);

    double getConvertedDouble(Unit unit);
    void print();

    static Value* string2Value(std::string);
};
inline double Value::getDouble() { return _value; }
inline Value::Unit Value::getUnit() { return _unit; }
}
#endif
