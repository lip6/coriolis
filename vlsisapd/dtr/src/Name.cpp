/*
 *  Name.cpp
 *  DTR
 *
 *  Created by damien dupuis on 01/04/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "io/dtr/Name.h"

namespace DTR {
unsigned long Name::_globalId = 0;
map<string, unsigned long> Name::_dict;

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

bool Name::operator!=(const Name& n) {
    return (_id != n._id);
}

bool Name::operator!=(const string& str) {
    Name n(str);
    return (_id != n._id);
}

bool Name::operator<(const Name n) const {
    return (_id < n._id);
}
} // namespace

