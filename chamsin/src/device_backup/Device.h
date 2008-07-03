
// ****************************************************************************************************
// File: Device.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef DEVICE_H
#define DEVICE_H


#include "Cell.h"
using namespace Hurricane;

namespace DEVICE {

class Device : public Cell {

#if !defined(__DOXYGEN_PROCESSOR__)
// Types
// *****
     public : typedef Cell Inherit; 

// Attributes
// **********


// Constructors
// ************
    protected : Device(Library* library, const Name& name);
    protected : virtual void _postCreate();


#endif

// Operations
// **********
//    public : virtual void Create(const char, const bool) = 0;
    public : virtual void dses()   = 0; 
    public : virtual void shape()  = 0;
//    public : virtual void Generate() = 0;


#   if !defined(__DOXYGEN_PROCESSOR__)
    public : virtual void SaveLogicalView();
    public : virtual void SavePhysicalView() {};

// Accessors
// *********	    

// Updators
// ********

// Others
// ******
    public : virtual void _Flush() = 0;

    // Description of Layout
    // **********************
    public: void _Place(Instance* ins, const Transformation::Orientation& orientation, const Point& point); 
    public: void _setRefIns(Instance*) const;
    public: void _PlaceRight(Instance* ins, const Transformation::Orientation& orientation, const Point& offset=Point());
   

#endif 

};

}

#endif // DEVICE_H
