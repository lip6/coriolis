
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |           Design Technological Rules (DTR)                      |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/dtr/Techno.h"                       |
// +-----------------------------------------------------------------+


#ifndef __DTR_TECHNO_H__
#define __DTR_TECHNO_H__

#include <vector>


namespace DTR {

  class Rule;
  class ARule;


  class Techno {
    public:
                                 Techno           ( const std::string& name
                                                  , const std::string& unit
                                                  , const std::string& version );
                                 Techno           ( const char* name
                                                  , const char* unit
                                                  , const char* version );
      inline const std::string&  getName          () const;
      inline const std::string&  getUnit          () const;
      inline const std::string&  getVersion       () const;
      inline std::vector<Rule*>& getRules         ();
                                                  
      inline void                setName          ( const std::string&);
      inline void                setUnit          ( const std::string&);
      inline void                setVersion       ( const std::string&);
             Rule*               addRule          ( const char* name
                                                  , double value
                                                  , const char* ref
                                                  , const char* layer1=""
                                                  , const char* layer2="" );
             ARule*              addARule         ( const char* name
                                                  , double value
                                                  , const char* ref
                                                  , const char* layer1
                                                  , const char* layer2 );
             Rule*               getRule          ( const char* name
                                                  , const char* layer1=""
                                                  , const char* layer2="" );
             double              getValue         ( const char* name
                                                  , const char* layer1=""
                                                  , const char* layer2="" );
             const std::string&  getValueAsString ( const char* name
                                                  , const char* layer1=""
                                                  , const char* layer2="" );
             bool                writeToFile      ( const char* filePath );
      static Techno*             readFromFile     ( const char* filePath );
    private:
      std::string  	      _name;
      std::string  	      _unit;
      std::string         _version;
      std::vector<Rule*>  _rules;
  };

    
  inline const std::string&  Techno::getName   () const { return _name; };
  inline const std::string&  Techno::getUnit   () const { return _unit; };
  inline const std::string&  Techno::getVersion() const { return _version; };
  inline std::vector<Rule*>& Techno::getRules  () { return _rules; };
  inline void                Techno::setName   (const std::string& name)    { _name = name; };
  inline void                Techno::setUnit   (const std::string& unit)    { _unit = unit; };
  inline void                Techno::setVersion(const std::string& version) { _version = version; };

  
}  // namespace DTR.

#endif

