
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
// |  C++ Module  :  "./Wire.cpp"                                    |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"
using namespace std;

namespace OpenChams {


  void Wire::setStartPoint(const string& name, const string& plug)
  {
    if (_start)
      throw OpenChamsException("[ERROR] Wire::setStartPoint: cannot set start point since it has already been set.");
    
    _start = new InstancePoint(name, plug);
  }


  void Wire::setStartPoint(unsigned idx)
  {
    if (_start)
      throw OpenChamsException("[ERROR] Wire::setStartPoint: cannot set start point since it has already been set.");

    _start = new PortPoint(idx);
  }


  void Wire::setEndPoint(const string& name, const string& plug)
  {
    if (!_start)
      throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since start point has not been set.");
    if (_end)
      throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since it has already been set.");

    _end = new InstancePoint(name, plug);
  }


  void Wire::setEndPoint(unsigned idx)
  {
    if (!_start)
      throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since start point has not been set.");
    if (_end)
      throw OpenChamsException("[ERROR] Wire::setEndPoint: cannot set end point since it has already been set.");

    _end = new PortPoint(idx);
  }


  void Wire::addIntermediatePoint(double x, double y)
  {
    if (!_start)
      throw OpenChamsException("[ERROR] Wire::addIntermediatePoint: cannot add point since start point has not been set.");
    if (_end)
      throw OpenChamsException("[ERROR] Wire::addIntermediatePoint: cannot add point since end point has already been set.");
    
    _inters.push_back(new IntermediatePoint(x, y));
  }


} // OpenChams namespace.
