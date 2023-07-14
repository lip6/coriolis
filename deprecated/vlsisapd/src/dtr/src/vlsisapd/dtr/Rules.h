
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
// |  C++ Header  :  "./vlsisapd/dtr/Rule.h"                         |
// +-----------------------------------------------------------------+


#ifndef __DTR_RULES_H__
#define __DTR_RULES_H__

#include <string>
#include <sstream>


namespace DTR {

  template<typename T>
  inline std::string  asString ( T value )
  { std::ostringstream output; output << value; return output.str(); }


  class Rule {
    public:
    // CTOR & DTOR.
      inline                            Rule             ( const char* name
                                                         , double      value
                                                         , const char* ref
                                                         , const char* layer1
                                                         , const char* layer2 );
      inline virtual                   ~Rule             ();
    // Accessors.
      inline         const std::string& getName          ();
      inline         const std::string& getType          ();
      inline         double             getValue         ();
      inline         const std::string& getValueAsString ();
      inline         const std::string& getRef           ();
      inline         const std::string& getLayer1        ();
      inline virtual const std::string& getLayer2        ();
    // Modifiers
      inline         void               setType          ( const char* );
      inline         void               setValue         ( double );
      inline         void               setRef           ( const char* );

    // Attributes.
    private:
      std::string   _name;
      std::string   _type;
      double        _value;
      std::string   _valueStr;
      std::string   _ref;
      std::string   _layer1;
      std::string   _layer2;
  };
  

  class ARule : public Rule {
    public:
      inline          ARule ( const char* name
                            , double      value
                            , const char* ref
                            , const char* layer1
                            , const char* layer2 );
      inline virtual ~ARule ();
  };


  inline Rule::Rule ( const char* name
                    , double      value
                    , const char* ref
                    , const char* layer1
                    , const char* layer2 )
    : _name    (name)
    , _type    ("")
    , _value   (value)
    , _valueStr(asString(value))
    , _ref     (ref)
    , _layer1  (layer1)
    , _layer2  (layer2)
  { }

  inline Rule::~Rule ()
  { }

  inline const std::string& Rule::getName          () { return _name;   };
  inline const std::string& Rule::getType          () { return _type;   };
  inline double             Rule::getValue         () { return _value;  };
  inline const std::string& Rule::getRef           () { return _ref;    };
  inline const std::string& Rule::getLayer1        () { return _layer1; };
  inline const std::string& Rule::getLayer2        () { return _layer2; };
  inline const std::string& Rule::getValueAsString () { return _valueStr; };
  inline void               Rule::setType          ( const char* type ) { _type=type; };
  inline void               Rule::setValue         ( double value ) { _value=value; _valueStr=asString(value); };
  inline void               Rule::setRef           ( const char* ref ) { _ref=ref; };

  inline ARule::ARule ( const char* name
                      , double      value
                      , const char* ref
                      , const char* layer1
                      , const char* layer2 )
    : Rule(name, value, ref, layer1, layer2)
  { }
      
  inline ARule::~ARule ()
  { }


} // namespace DTR
#endif

