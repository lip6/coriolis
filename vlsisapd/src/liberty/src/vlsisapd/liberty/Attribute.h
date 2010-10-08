#ifndef  __LIB_ATTRIBUTE_H__
#define  __LIB_ATTRIBUTE_H__

#include<string>
#include<fstream>
#include "Name.h"

namespace LIB {
class Attribute {
    public:
    enum Type { Unknown = 0
              , String  = 1
              , Bool    = 2
              , Int     = 3
              , Double  = 4
              , Unit    = 5
              , Pair    = 6
              };

    public:
    Attribute(const Name name, Type type, const std::string& value, const std::string& unit="", const std::string& value2="");

    inline const Name getName() const;
    inline const Type getType() const;
    inline const std::string& getUnit() const;

    inline const std::string& valueAsString() const;
    inline const std::string& secondValueAsString() const;
    bool   valueAsBool() const;
    int    valueAsInt() const;
    double valueAsDouble() const;
    double secondValueAsDouble() const;
    std::string typeToString(Type type);
    bool   write(std::ofstream &file);

    private:
    Name _name;
    Type _type;
    std::string _value;
    std::string _unit;
    std::string _value2;
};

inline const std::string&    Attribute::getUnit() const  { return _unit; }
inline const Name            Attribute::getName() const  { return _name; }
inline const Attribute::Type Attribute::getType() const  { return _type; }
inline const std::string&    Attribute::valueAsString() const { return _value; }
inline const std::string&    Attribute::secondValueAsString() const { return _value2; }
}  // namespace
#endif
