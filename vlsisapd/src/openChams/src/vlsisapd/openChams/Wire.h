
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2011-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Wire.h"                   |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_WIRE_H__
#define __OPENCHAMS_WIRE_H__

#include <string>
#include <vector>


namespace OpenChams {


  class WirePoint {
    public:
               WirePoint() {}
      virtual ~WirePoint() {};
  };


  class InstancePoint: public WirePoint {
    public:
      inline                    InstancePoint (const std::string& name, const std::string& plug);
      inline virtual           ~InstancePoint ();
      inline const std::string& getName       ();
      inline const std::string& getPlug       ();
    private:
      std::string  _name;
      std::string  _plug;
  };


  class PortPoint: public WirePoint {
    public:
      inline          PortPoint (unsigned idx);
      inline virtual ~PortPoint ();
      inline unsigned getIndex  ();
    private:
      unsigned _idx;
  };


  class IntermediatePoint: public WirePoint {

    public:
      inline          IntermediatePoint (double x, double y);
      inline virtual ~IntermediatePoint ();
      inline double   getX              ();
      inline double   getY              ();
    private:
      double _x;
      double _y;
  };


  class Wire {
	public:
      inline            Wire                   ();
      inline           ~Wire                   ();
    // Accessors
      inline WirePoint* getStartPoint          ();
      inline WirePoint* getEndPoint            ();
      inline bool       hasNoIntermediatePoints();
      inline const std::vector<IntermediatePoint*>& getIntermediatePoints();
    // Modifiers
             void       setStartPoint          (const std::string& name, const std::string& plug);
             void       setStartPoint          (unsigned idx);
             void       setEndPoint            (const std::string& name, const std::string& plug);
             void       setEndPoint            (unsigned idx);
             void       addIntermediatePoint   (double x, double y);
	private:
      WirePoint*                       _start;
      WirePoint*                       _end;
      std::vector<IntermediatePoint*>  _inters;
  };


// Inlines of InstancePoint.
  inline                    InstancePoint::InstancePoint  (const std::string& name, const std::string& plug) : _name(name), _plug(plug) {}
  inline                    InstancePoint::~InstancePoint () {} 
  inline const std::string& InstancePoint::getName        () { return _name; }
  inline const std::string& InstancePoint::getPlug        () { return _plug; }


// Inlines of PortPoint.
  inline          PortPoint::PortPoint  (unsigned idx): _idx(idx) {}
  inline          PortPoint::~PortPoint () {}
  inline unsigned PortPoint::getIndex   () { return _idx; }


// Inlines of IntermediatePoint.
  inline          IntermediatePoint::IntermediatePoint (double x, double y): _x(x), _y(y) {}
  inline          IntermediatePoint::~IntermediatePoint() {}
  inline double   IntermediatePoint::getX              () { return _x; }
  inline double   IntermediatePoint::getY              () { return _y; }


// Inlines of Wire.
  inline            Wire::Wire                   () : _start(NULL), _end(NULL), _inters() {}
  inline            Wire::~Wire                  () {}
  inline WirePoint* Wire::getStartPoint          () { return _start; }
  inline WirePoint* Wire::getEndPoint            () { return _end; }
  inline bool       Wire::hasNoIntermediatePoints() { return _inters.empty(); }
  inline const std::vector<IntermediatePoint*>& Wire::getIntermediatePoints() { return _inters; }


} // namespace

#endif

