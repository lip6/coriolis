// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DeviceDescriptor.h"                |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_DEVICE_DESCRIPTOR_H
#define HURRICANE_DEVICE_DESCRIPTOR_H

#include <vector>
#include "hurricane/Name.h"


namespace Hurricane {


  class DeviceDescriptor {
    public:
      struct DeviceDescriptorComp:
        public std::binary_function<const DeviceDescriptor*, const DeviceDescriptor*, bool> {
          bool operator () ( const DeviceDescriptor* ldd, const DeviceDescriptor* rdd ) const
          { return ldd->getName() < rdd->getName(); }
      };

      typedef std::vector<std::string>     Connectors;
      typedef std::pair<Name, std::string> Layout;
      typedef std::map<Name, std::string>  Layouts;
    public:
                               DeviceDescriptor ( const Name& );
      inline Name              getName          () const;
      inline void              setSpiceFilePath ( const std::string& spiceFilePath );
      inline std::string       getSpiceFilePath () const;
             void              addConnector     ( const std::string& connector );
      inline const Connectors& getConnectors    () const;
      inline const Layouts     getLayouts       () const;
             void              addLayout        ( const Name& layoutName, const std::string& moduleName );
    private:
      Name         _name;
      std::string  _spiceFilePath;
      Connectors   _connectors;
      Layouts      _layouts;
  };

  
  inline Name                                DeviceDescriptor::getName          () const { return _name; }
  inline void                                DeviceDescriptor::setSpiceFilePath ( const std::string& spiceFilePath ) { _spiceFilePath = spiceFilePath; }
  inline std::string                         DeviceDescriptor::getSpiceFilePath () const { return _spiceFilePath; }
  inline const DeviceDescriptor::Connectors& DeviceDescriptor::getConnectors    () const { return _connectors; } 
  inline const DeviceDescriptor::Layouts     DeviceDescriptor::getLayouts       () const { return _layouts; }


}  // Hurricane namespace.

#endif // HURRICANE_DEVICE_DESCRIPTOR_H
