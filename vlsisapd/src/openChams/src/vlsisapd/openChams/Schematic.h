
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Schematic.h"              |
// +-----------------------------------------------------------------+


#ifndef VLSISAPD_OPENCHAMS_SCHEMATIC_H
#define VLSISAPD_OPENCHAMS_SCHEMATIC_H

#include <map>
#include <string>

namespace OpenChams {

  class Circuit;


  class Schematic {

    public:
      class Infos {
        public:
                                    Infos         (double x, double y, const std::string& orient);
          inline double             getX          ();
          inline double             getY          ();
          inline const std::string& getOrientation();
        private:
          double       _x;
          double       _y;
          std::string  _orient;
      };

    public:
                                                  Schematic      ( Circuit* );
             void                                 addInstance    ( const std::string& instanceName
                                                                 , double x
                                                                 , double y
                                                                 , const std::string& orient );
      inline bool                                 hasNoInstances ();
      inline const std::map<std::string, Infos*>& getInstances();
    private:
      Circuit*                       _circuit;
      std::map<std::string, Infos*>  _instances;
};

inline bool                                            Schematic::hasNoInstances() { return _instances.empty(); };
inline const std::map<std::string, Schematic::Infos*>& Schematic::getInstances  () { return _instances; };    
    
inline double             Schematic::Infos::getX          () { return _x; };
inline double             Schematic::Infos::getY          () { return _y; };
inline const std::string& Schematic::Infos::getOrientation() { return _orient; };
    

} // OpenChams namespace.

#endif
