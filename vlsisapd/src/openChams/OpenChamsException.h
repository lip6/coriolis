/*
 *  OpenChamsException.h
 *  openChams
 *
 *  Created by damien dupuis on 20/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_EXCEPTION_NET_H__
#define __OPENCHAMS_EXCEPTION_NET_H__

#include <exception>
#include <string>

namespace OpenChams {
class OpenChamsException {
	public:
        OpenChamsException(const std::string& what) throw() : _what(what) {}
        virtual const char* what() const throw() { return _what.c_str(); }
        virtual ~OpenChamsException() throw() {}
        
    private:
        std::string _what;
};
} // namespace
#endif

