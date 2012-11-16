
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2009-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Circuit.h"                |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_CIRCUIT_H__
#define __OPENCHAMS_CIRCUIT_H__

#include <vector>
#include <map>
#include <sstream>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/SimulModel.h"


namespace OpenChams {

  class Netlist;
  class Instance;
  class Device;
  class Net;
  class Schematic;
  class Sizing;
  class Transistor;
  class Operator;
  class Layout;
  class Node;

  class Circuit {
    public:
                                       Circuit                   ( const std::string&, const std::string& techno );
    // Accessors.                                                
      inline const std::string&        getName                   ();
      inline const std::string&        getTechno                 ();
      inline const std::string&        getValue                  ( const std::string& );
      inline Netlist*                  getNetlist                ();
      inline Schematic*                getSchematic              ();
      inline Sizing*                   getSizing                 ();
      inline Layout*                   getLayout                 ();
      inline void                      addParameter              ( const std::string&, const char* );
      inline void                      addParameter              ( const std::string&, const std::string& );
      inline Parameters                getParameters             ();
      inline void                      addSubCircuitPath         ( std::string );
      inline std::vector<std::string>& getSubCircuitPaths        ();
    // Mutators.                                                 
             void                      addSimulModel             ( unsigned
                                                                 , SimulModel::Base
                                                                 , SimulModel::Version
                                                                 , std::string );
      inline void                      setSizing                 ( Sizing* );
      inline void                      setLayout                 ( Layout* );
             Netlist*                  createNetlist             ();
             Schematic*                createSchematic           ();
             Sizing*                   createSizing              ();
             Layout*                   createLayout              ();
             void                      driveHBTree               ( std::ofstream&, Node*, unsigned );
             bool                      writeToFile               ( std::string filePath );
      static Circuit*                  readFromFile              ( const std::string filePath );
    
	private:
    // Internal methods (XML parser).
             std::string               readParameter             ( xmlNode*, const xmlChar*& );
             std::string               readParameterEq           ( xmlNode*, std::string& );
             std::string               readConnector             ( xmlNode* );
             void                      readSubCircuitsPaths      ( xmlNode* );
             void                      readCircuitParameters     ( xmlNode* );
             void                      readSimulModels           ( xmlNode* );
             void                      readNetList               ( xmlNode* );
             void                      readInstances             ( xmlNode*, Netlist* );
             Instance*                 readInstance              ( xmlNode*, Netlist* );
             void                      readInstanceParameters    ( xmlNode*, Instance* );
             void                      readInstanceConnectors    ( xmlNode*, Instance* );
             void                      readInstanceTransistors   ( xmlNode*, Device* );
             void                      readTransistor            ( xmlNode*, Device* );
             void                      readTransistorConnection  ( xmlNode*, Transistor* );
             void                      readNets                  ( xmlNode*, Netlist* );
             Net*                      readNet                   ( xmlNode*, Netlist* );
             void                      readNetConnector          ( xmlNode*, Net* );
             void                      readSchematic             ( xmlNode* );
             void                      readInstanceSchematic     ( xmlNode*, Schematic* );
             void                      readNetSchematic          ( xmlNode*, Circuit* );
             void                      readPortSchematic         ( xmlNode*, Net* );
             void                      readWireSchematic         ( xmlNode*, Net* );
             void                      readSizing                ( xmlNode* );
             void                      readInstanceSizing        ( xmlNode*, Sizing* );
             void                      readConstraint            ( xmlNode*, Operator* );
    // Equation related XML methods.
             void                      readEquations             ( xmlNode*, Sizing* );
             void                      readEquation_CircuitLevel ( xmlNode*, Sizing* );
             void                      readEquation_NRC          ( xmlNode*, Sizing* );
             void                      readEquation_DDPs         ( xmlNode*, Sizing* );
             void                      readEquation_DesignerCstr ( xmlNode*, Sizing* );
    // Layout related XML methods.
             void                      readLayout                ( xmlNode* );
             void                      readInstanceLayout        ( xmlNode*, Layout* );
             void                      readHBTree                ( xmlNode*, Layout* );
             Node*                     readNodeOrBloc            ( xmlNode*, Node* parent = NULL );
             void                      setAbsolutePath           ( const std::string filePath );
    // Utilities methods.
             void                      check_uppercase           ( std::string& str, std::vector<std::string>& compares, std::string message );
             void                      check_lowercase           ( std::string& str, std::vector<std::string>& compares, std::string message );
    
	private:
      const std::string& 	           _name;
      std::string                      _absolutePath;
      const std::string& 	           _techno;
      Parameters                       _params;
      Netlist*                         _netlist;
      Schematic*                       _schematic;
      Sizing*                          _sizing;
      Layout*                          _layout;
      std::vector<std::string>         _subCircuitsPaths;
      std::map<unsigned, SimulModel*>  _simulModels;
  };
    

  inline const std::string&        Circuit::getName            () { return _name; }
  inline const std::string&        Circuit::getTechno          () { return _techno; }
  inline const std::string&        Circuit::getValue           (const std::string& name) { return _params.getValue(name); }
  inline Netlist*                  Circuit::getNetlist         () { return _netlist; }
  inline Schematic*                Circuit::getSchematic       () { return _schematic; }
  inline Sizing*                   Circuit::getSizing          () { return _sizing; }
  inline Layout*                   Circuit::getLayout          () { return _layout; }
  inline void                      Circuit::addParameter       (const std::string& name, const char* value)        { _params.addParameter(name, value); }
  inline void                      Circuit::addParameter       (const std::string& name, const std::string& value) { _params.addParameter(name, value); }
  inline Parameters                Circuit::getParameters      () { return _params; }
  inline void                      Circuit::addSubCircuitPath  (std::string path) { _subCircuitsPaths.push_back(path); }
  inline std::vector<std::string>& Circuit::getSubCircuitPaths () { return _subCircuitsPaths; }


  template<typename T>
  inline std::string  asString ( T value )
  { std::ostringstream output; output << value; return output.str(); }


  std::string  asStringBool      ( bool );
  std::string  asStringDirection ( long );


  template<typename T>
  inline T  stringAs ( const char* str )
  { T value; std::istringstream input(str); input >> value; return value; }


  template<typename T>
  inline T  stringAs ( const xmlChar* str )
  { T value; std::istringstream input((const char*)str); input >> value; return value; }


  template<typename T>
  inline T  stringAs ( const std::string& str )
  { return stringAs<T>(str.c_str()); }


  bool  stringAsBool      ( const std::string& );
  long  stringAsDirection ( const std::string& );
    
    
}  // namespace OpenChams.

#endif

