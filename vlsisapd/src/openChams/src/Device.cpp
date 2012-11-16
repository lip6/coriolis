
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Device.cpp"                                  |
// +-----------------------------------------------------------------+


#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {


  Device::Device ( const string& name
                 , const string& model
                 , unsigned      order
                 , const string& mosType
                 , bool          sourceBulkConnected
                 , Netlist*      netlist )
    : Instance            (name,model,order,netlist)
    , _mosType            (mosType)
    , _sourceBulkConnected(sourceBulkConnected)
    , _trans              ()
  { }


  Device::~Device ()
  { }


  Transistor* Device::addTransistor ( const string& name )
  {
    Transistor* tr = new Transistor(name, this);
    if (!tr)
      throw OpenChamsException("[ERROR] Cannot create transistor.");

    _trans.push_back(tr); 
    return tr;
  }


}  // OpenChams namespace

