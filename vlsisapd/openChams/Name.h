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
using namespace std;

namespace OpenChams {
class Name {
    public:
        Name(string);
	    Name(const char*);
    
	    bool operator==(const Name&);
	    bool operator==(const string&);
	    bool operator<(const Name) const;
    
	    inline const string& getString() const;
        
    private:
        unsigned long _id;
        const string *_str;
    
    	static map<string, unsigned long> _dict;
	    static unsigned long              _globalId;
};
    
const string& Name::getString() const{
    return *_str;
}

} // namespace

#endif

