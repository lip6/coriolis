// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :      "./hurricane/analog/LayoutGenerator.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_LAYOUT_GENERATOR_H
#define ANALOG_LAYOUT_GENERATOR_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "hurricane/Box.h"
#include "hurricane/DbU.h"

namespace Hurricane {
  class Box;
}
namespace Isobar {
  class Script;
}

// To use PyObject without including Python.h which is included in .cpp file
extern "C" {
  struct  _object;
  typedef _object PyObject;
}


namespace Analog {

  class Device;


  class LayoutGenerator  {
    public:
      typedef std::map<std::string, std::map<std::string, double> > ParamMap;
      enum Verbose { Quiet         = 0
                   , Verbose       = 1
                   , VeryVerbose   = 2
                   , Debug         = 3
                   };
      enum Flags   { NoFlags       = 0x0000
                   , ShowTimeTag   = 0x0001
                   , ShowError     = 0x0002
                   , StatusOk      = 0x0004
                   , ComputeBbOnly = 0x0008
                   };
  
    public:               
      static inline void     setVerboseLevel            ( int );
      static inline int      getVerboseLevel            ();
    public:                                      
                             LayoutGenerator            ();
                            ~LayoutGenerator            ();
      inline Device*         getDevice                  ();
             unsigned        getNumberTransistor        ();
             unsigned        getNumberStack             ();
             Hurricane::Box  getDeviceBox               ();
             Hurricane::Box  getActiveBox               ();
      inline PyObject*       getMatrix                  ();
             PyObject*       getRow                     ( unsigned i );
             PyObject*       getDic                     ( PyObject* row, unsigned j );
             PyObject*       getParamValue              ( PyObject* dic, std::string paramName ); 
             double          getParameterValue          ( unsigned i, unsigned j, std::string paramName, bool& ok );
      inline double          unitToMicro                ( int );
      inline double          unitToMetter               ( int );
    public:                                             
      inline void            setDevice                  ( Device* );
             bool            initialize                 ();
             void            finalize                   ( unsigned int flags );
             bool            checkScript                ();
             bool            checkFunctions             ();
             bool            callCheckCoherency         ( PyObject* pArgs,  unsigned int flags );
             bool            callLayout                 ( PyObject* pArgs );
             bool            drawLayout                 ();
             bool            toPyArguments              ( PyObject*& pArgsLayout, unsigned int flags );
    private:
      static int      _verboseLevel;
    private:          
      Device*         _device;
      Hurricane::Box* _box;
      Hurricane::Box* _activeBox;
      PyObject*       _matrix;
      Isobar::Script* _script;
  };
  
  
  inline int                      LayoutGenerator::getVerboseLevel ()                          { return _verboseLevel; }
  inline Device*                  LayoutGenerator::getDevice       ()                          { return _device; }
  inline void                     LayoutGenerator::setDevice       ( Device* device )          { _device = device; }
  inline void                     LayoutGenerator::setVerboseLevel (int lvl )                  { _verboseLevel = lvl; }
  inline PyObject*                LayoutGenerator::getMatrix       ()                          { return _matrix; }
  
  inline double LayoutGenerator::unitToMicro ( int unit ) {
    return Hurricane::DbU::getPhysical(Hurricane::DbU::getOnPhysicalGrid(unit), Hurricane::DbU::Micro);
  }
  
  inline double LayoutGenerator::unitToMetter ( int unit ) {
    return Hurricane::DbU::getPhysical(Hurricane::DbU::getOnPhysicalGrid(unit), Hurricane::DbU::Unity);
  }
  

}  // Analog namespace.

#endif  // ANALOG_LAYOUT_GENERATOR_H
