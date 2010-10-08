using namespace std;

#include "vlsisapd/liberty/Name.h"

namespace LIB {
unsigned long Name::_globalId = 0;
map<string, unsigned long> Name::_dict;

Name::Name() : _str(NULL) {
    map<string, unsigned long>::iterator it = _dict.find("");
    if (it != _dict.end()) {
        _id  = (*it).second;
        _str = &((*it).first);
    } else {
        _id  = _globalId++;
        it   = _dict.insert(_dict.begin(), make_pair("", _id));
        _str = &((*it).first);
    }
}

Name::Name(string str) : _str(NULL) {
    map<string, unsigned long>::iterator it = _dict.find(str);
    if (it != _dict.end()) {
        _id  = (*it).second;
        _str = &((*it).first);
    } else {
        _id  = _globalId++;
        it = _dict.insert(_dict.begin(), make_pair(str, _id));
        _str = &((*it).first);
    }
}
    
Name::Name(const char* c) : _str(NULL) {
    string str(c);
    map<string, unsigned long>::iterator it = _dict.find(str);
    if (it != _dict.end()) {
        _id  = (*it).second;
        _str = &((*it).first);
    } else {
        _id  = _globalId++;
        it = _dict.insert(_dict.begin(), make_pair(str, _id));
        _str = &((*it).first);
    }
}
    
bool Name::operator==(const Name& n) {
    return (_id == n._id);
}
    
bool Name::operator==(const string& str) {
    Name n(str);
    return (_id == n._id);
}
bool Name::operator<(const Name& n) const {
    //return (_id < n._id); // does not assert determinism in driving since hurricane do not care about name's ID --> need an alphabetical check on string
    return (*_str < n.getString());
}
} // namespace

