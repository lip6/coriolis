#ifndef __SPICE_EXCEPTION_H
#define __SPICE_EXCEPTION_H

#include <exception>
#include <string>

namespace SPICE {
class SpiceException {
	public:
        SpiceException(const std::string& what) : _what(what) {}
        virtual const char* what() const { return _what.c_str(); }
        virtual ~SpiceException() {}
        
    private:
        std::string _what;
};
} // namespace
#endif


