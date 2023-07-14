// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/Environment.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <regex.h>
#include <string>
#include "hurricane/Commons.h"
#include "crlcore/SearchPath.h"


namespace CRL {

  using Hurricane::JsonObject;
  using Hurricane::JsonStack;


// -------------------------------------------------------------------
// Class  :  "Environment".

  class Environment {
    public:
      enum AddMode { Append=1, Prepend=2, Replace=3 };
    public:
    // Internal: Static Methods.
      static const char*        getEnv                 ( const char* variable, const char* defaultValue );
    // Constructors & destructors.
                                Environment            ();
                               ~Environment            ();
    // Accessors.
      inline const std::string& getCORIOLIS_TOP        () const;
      inline const std::string& getDisplayStyle        () const;
      inline       long         getSCALE_X             () const;
      inline const std::string& getDISPLAY             () const;
      inline const std::string& getIN_LO               () const;
      inline const std::string& getIN_PH               () const;
      inline const std::string& getOUT_LO              () const;
      inline const std::string& getOUT_PH              () const;
      inline const std::string& getPOWER               () const;
      inline const std::string& getGROUND              () const;
      inline const std::string& getCLOCK               () const;
      inline const std::string& getBLOCKAGE            () const;
      inline const std::string& getPad                 () const;
      inline const std::string& getRegister            () const;
      inline const std::string& getCATALOG             () const;
      inline SearchPath&        getLIBRARIES           ();
             std::string        getLIBRARYPath         ( size_t i );
    // Predicates.
             bool               isPOWER                ( const char* name ) const;
             bool               isGROUND               ( const char* name ) const;
             bool               isCLOCK                ( const char* name ) const;
             bool               isBLOCKAGE             ( const char* name ) const;
             bool               isPad                  ( const char* name ) const;
             bool               isRegister             ( const char* name ) const;
    // Modifiers.
             void               validate               () const;
      inline void               setDisplayStyle        ( const char* );
      inline void               setSCALE_X             ( long value );
      inline void               setDISPLAY             ( const char* value );
      inline void               setIN_LO               ( const char* value );
      inline void               setIN_PH               ( const char* value );
      inline void               setOUT_LO              ( const char* value );
      inline void               setOUT_PH              ( const char* value );
             void               setPOWER               ( const char* value );
             void               setGROUND              ( const char* value );
             void               setCLOCK               ( const char* value );
             void               setBLOCKAGE            ( const char* value );
             void               setPad                 ( const char* value );
             void               setRegister            ( const char* value );
      inline void               setCATALOG             ( const char* value );
             void               setWORKING_LIBRARY     ( const char* value );
             void               addSYSTEM_LIBRARY      ( const char* value, const char* libName, unsigned int mode=Append );
    // Methods.
             std::string        getPrint               () const;
             void               toJson                 ( JsonWriter* ) const;
      inline std::string        _getTypeName           () const;
             std::string        _getString             () const;
             Record*            _getRecord             () const;
    protected:
    // Internal: Attributes.
             std::string        _CORIOLIS_TOP;
             std::string        _displayStyle;
             long               _SCALE_X;
             std::string        _IN_LO;
             std::string        _IN_PH;
             std::string        _OUT_LO;
             std::string        _OUT_PH;
             std::string        _CATALOG;
             std::string        _POWER;
             std::string        _GROUND;
             std::string        _CLOCK;
             std::string        _BLOCKAGE;
             std::string        _pad;
             std::string        _register;
             SearchPath         _LIBRARIES;
             regex_t*           _PowerRegex;
             regex_t*           _GroundRegex;
             regex_t*           _ClockRegex;
             regex_t*           _BlockageRegex;
             regex_t*           _padRegex;
             regex_t*           _registerRegex;
    private:
             void         _setRegex       ( regex_t*& regex, const std::string& pattern, const char* name );
  };


  // Inline Member Functions.
  inline const std::string&     Environment::getCORIOLIS_TOP        () const { return _CORIOLIS_TOP; }
  inline const std::string&     Environment::getDisplayStyle        () const { return _displayStyle; }
  inline       long             Environment::getSCALE_X             () const { return _SCALE_X; }
  inline const std::string&     Environment::getIN_LO               () const { return _IN_LO; }
  inline const std::string&     Environment::getIN_PH               () const { return _IN_PH; }
  inline const std::string&     Environment::getOUT_LO              () const { return _OUT_LO; }
  inline const std::string&     Environment::getOUT_PH              () const { return _OUT_PH; }
  inline const std::string&     Environment::getPOWER               () const { return _POWER; }
  inline const std::string&     Environment::getGROUND              () const { return _GROUND; }
  inline const std::string&     Environment::getCLOCK               () const { return _CLOCK; }
  inline const std::string&     Environment::getBLOCKAGE            () const { return _BLOCKAGE; }
  inline const std::string&     Environment::getPad                 () const { return _pad; }
  inline const std::string&     Environment::getRegister            () const { return _register; }
  inline const std::string&     Environment::getCATALOG             () const { return _CATALOG; }
  inline       SearchPath&      Environment::getLIBRARIES           () { return _LIBRARIES; }
                               
  inline void                   Environment::setDisplayStyle        ( const char* value ) { _displayStyle = value; }
  inline void                   Environment::setSCALE_X             ( long        value ) { _SCALE_X = value; }
  inline void                   Environment::setIN_LO               ( const char* value ) { _IN_LO = value; }
  inline void                   Environment::setIN_PH               ( const char* value ) { _IN_PH = value; }
  inline void                   Environment::setOUT_LO              ( const char* value ) { _OUT_LO = value; }
  inline void                   Environment::setOUT_PH              ( const char* value ) { _OUT_PH = value; }
  inline void                   Environment::setCATALOG             ( const char* value ) { _CATALOG = value; }
  inline std::string            Environment::_getTypeName           () const { return "Environment"; }


// -------------------------------------------------------------------
// Class  :  "JsonEnvironment".

  class JsonEnvironment : public JsonObject {
    public:
      static  void             initialize      ();
                               JsonEnvironment ( unsigned long flags );
      virtual std::string      getTypeName     () const;
      virtual JsonEnvironment* clone           ( unsigned long flags ) const;
      virtual void             toData          ( JsonStack& ); 
  };


} // CRL namespace.


INSPECTOR_P_SUPPORT(CRL::Environment);
