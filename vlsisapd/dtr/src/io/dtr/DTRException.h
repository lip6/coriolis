/*
 *  DTRException.h
 *  DTR
 *
 *  Created by damien dupuis on 01/04/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __DTR_EXCEPTION_H__
#define __DTR_EXCEPTION_H__

#include <exception>
#include <string>

namespace DTR {
class DTRException {
	public:
        DTRException(const std::string& what) throw() : _what(what) {}
        virtual const char* what() const throw() { return _what.c_str(); }
        virtual ~DTRException() throw() {}
        
    private:
        std::string _what;
};
} // namespace
#endif

