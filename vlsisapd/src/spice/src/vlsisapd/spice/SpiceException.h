#ifndef __SPICE_EXCEPTION_H
#define __SPICE_EXCEPTION_H

#include <exception>
#include <string>

namespace SPICE {
class SpiceException {
	public:
        SpiceException(const std::string& what) throw() : _what(what) {}
        virtual const char* what() const throw() { return _what.c_str(); }
        virtual ~SpiceException() throw() {}
        
    private:
        std::string _what;
};
} // namespace
#endif


