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

class Device;
class Arguments;
class TransistorArguments;
class TransistorMultiArguments;
class CapacitorArguments;

// To use PyObject without including Python.h which is included in .cpp file
extern "C" {
  struct  _object;
  typedef _object PyObject;
}


namespace Analog {

  class LayoutGenerator  {
    public:
      class Logger {
        public:
                                   Logger         ( LayoutGenerator* );
          virtual                 ~Logger         ();
          inline  LayoutGenerator* getGenerator   ();
          virtual void             popStatus      ( const std::string& );
          virtual void             popError       ( const std::string& );
          virtual void             popScriptError ();
        private:
          LayoutGenerator* _generator;
      };
  
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
      inline Logger*         getLogger                  ();
      inline Device*         getDevice                  ();
             unsigned        getNumberTransistor        ();
             unsigned        getNumberStack             ();
             Hurricane::Box  getDeviceBox               ();
             Hurricane::Box  getActiveBox               ();
      inline PyObject*       getMatrix                  ();
             PyObject*       getRow                     ( unsigned i );
             PyObject*       getDic                     ( PyObject* row, unsigned j );
             PyObject*       getParamValue              ( PyObject* dic, std::string paramName ); 
  //         double          getParameterValue          ( std::string trName    , std::string paramName, bool& ok );
             double          getParameterValue          ( unsigned i, unsigned j, std::string paramName, bool& ok );
      inline double          unitToMicro                ( int );
      inline double          unitToMetter               ( int );
    public:                                             
      inline void            setLogger                  ( Logger* );
      inline void            setDevice                  ( Device* );
      inline void            popStatus                  ( const std::string& );
      inline void            popError                   ( const std::string& );
      inline void            popScriptError             ();
             bool            initialize                 ();
             void            finalize                   ( unsigned int flags );
             bool            checkScript                ();
             bool            checkFunctions             ();
             bool            callCheckCoherency         ( PyObject* pArgsCheck, unsigned int flags );
             bool            callLayout                 ( PyObject* pArgsLayout );
             bool            drawLayout                 ();
             bool            toPyArguments              ( Arguments*, PyObject*& pArgsCheck, PyObject*& pArgsLayout, unsigned int flags );
             void            pyTransistorArguments      ( TransistorArguments*     , unsigned m, PyObject*& pArgsCheck, PyObject*& pArgsLayout, unsigned int flags );
             void            pyTransistorMultiArguments ( TransistorMultiArguments*, unsigned m, PyObject*& pArgsCheck, PyObject*& pArgsLayout, unsigned int flags );
             void            pyCapacitorArguments       ( CapacitorArguments*                  , PyObject*& pArgsCheck, PyObject*& pArgsLayout, unsigned int flags );
    private:
      static int      _verboseLevel;
    private:          
      Logger*         _logger;
      Device*         _device;
      Hurricane::Box* _box;
      Hurricane::Box* _activeBox;
      PyObject*       _matrix;
      Isobar::Script* _script;
  };
  
  
  inline LayoutGenerator* LayoutGenerator::Logger::getGenerator () { return _generator; }
  
  
  inline int                      LayoutGenerator::getVerboseLevel ()                          { return _verboseLevel; }
  inline LayoutGenerator::Logger* LayoutGenerator::getLogger       ()                          { return _logger; }
  inline Device*                  LayoutGenerator::getDevice       ()                          { return _device; }
  inline void                     LayoutGenerator::setDevice       ( Device* device )          { _device = device; }
  inline void                     LayoutGenerator::setVerboseLevel (int lvl )                  { _verboseLevel = lvl; }
  inline void                     LayoutGenerator::popStatus       ( const std::string& text ) { if (_logger) _logger->popStatus(text); }
  inline void                     LayoutGenerator::popError        ( const std::string& text ) { if (_logger) _logger->popError(text); }
  inline void                     LayoutGenerator::popScriptError  ()                          { if (_logger) _logger->popScriptError(); }
  inline PyObject*                LayoutGenerator::getMatrix       ()                          { return _matrix; }
  
  inline double LayoutGenerator::unitToMicro ( int unit ) {
    return Hurricane::DbU::getPhysical(Hurricane::DbU::getOnPhysicalGrid(unit), Hurricane::DbU::Micro);
  }
  
  inline double LayoutGenerator::unitToMetter ( int unit ) {
    return Hurricane::DbU::getPhysical(Hurricane::DbU::getOnPhysicalGrid(unit), Hurricane::DbU::Unity);
  }
  
  inline void  LayoutGenerator::setLogger ( Logger* logger )
  {
    if ( _logger ) delete _logger;
    _logger = logger;
  }


}  // Analog namespace.

#endif  // ANALOG_LAYOUT_GENERATOR_H
