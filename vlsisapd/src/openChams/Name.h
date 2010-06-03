/*
 *  Name.h
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_NAME_H__
#define __OPENCHAMS_NAME_H__

#include <string>
#include <map>

#include "OpenChamsException.h"

namespace OpenChams {
class Name {
    public:
    Name(std::string);
	Name(const char*);
    
	bool operator==(const Name&);
	bool operator==(const std::string&);
	bool operator<(const Name&) const;
    
	inline const std::string& getString() const;
        
    private:
    unsigned long _id;
	const std::string *_str;
    
	static std::map<std::string, unsigned long> _dict;
	static unsigned long _globalId;
};
    
inline const std::string& Name::getString() const{
    if (!_str) {
        throw OpenChamsException("[ERROR] Name object has no string");
    }
    return *_str;
}

} // namespace
#endif

