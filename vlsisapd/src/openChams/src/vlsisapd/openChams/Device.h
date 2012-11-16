
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
// |  C++ Header  :  "./vlsisapd/openChams/Instance.h"               |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_DEVICE_H__
#define __OPENCHAMS_DEVICE_H__

#include <map>
#include <vector>

#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Parameters.h"


namespace OpenChams {

  class Netlist;
  class Transistor;


  class Device : public Instance {
	public:
                                             Device                ( const std::string& name
                                                                   , const std::string& model
                                                                   , unsigned           order
                                                                   , const std::string& mosType
                                                                   , bool               sourceBulkConnected
                                                                   , Netlist* );
      virtual                               ~Device                ();
      inline const std::string&              getMosType            ();
      inline bool                            isSourceBulkConnected ();
    // Pour parcourir les transistors
      inline bool                            hasNoTransistors      ();
      inline const std::vector<Transistor*>& getTransistors        ();
             Transistor*                     addTransistor         (const std::string&);

	private:
      const std::string  	   	_mosType;
      bool                      _sourceBulkConnected;
      std::vector<Transistor*>  _trans;
  };


  inline const std::string&              Device::getMosType           () { return _mosType; };
  inline bool                            Device::isSourceBulkConnected() { return _sourceBulkConnected; };
  inline bool                            Device::hasNoTransistors     () { return (_trans.size() == 0)? true : false; };
  inline const std::vector<Transistor*>& Device::getTransistors       () { return _trans; };
    
} // namespace
#endif


