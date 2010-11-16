
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Environment.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CRL_ENVIRONMENT__
#define  __CRL_ENVIRONMENT__

#include  <regex.h>

#include  <string>

#include  <crlcore/SearchPath.h>


namespace CRL {


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
      inline const std::string& getSYMBOLIC_TECHNOLOGY () const;
      inline const std::string& getREAL_TECHNOLOGY     () const;
      inline const std::string& getDISPLAY             () const;
      inline const std::string& getIN_LO               () const;
      inline const std::string& getIN_PH               () const;
      inline const std::string& getOUT_LO              () const;
      inline const std::string& getOUT_PH              () const;
      inline const std::string& getPOWER               () const;
      inline const std::string& getGROUND              () const;
      inline const std::string& getCLOCK               () const;
      inline const std::string& getOBSTACLE            () const;
      inline const std::string& getPad                 () const;
      inline const std::string& getCATALOG             () const;
      inline SearchPath&        getLIBRARIES           ();

    // Predicates.
             bool               isPOWER                ( const char* name ) const;
             bool               isGROUND               ( const char* name ) const;
             bool               isCLOCK                ( const char* name ) const;
             bool               isOBSTACLE             ( const char* name ) const;
             bool               isPad                  ( const char* name ) const;

    // Modifiers.
             void               loadFromXml            ( const std::string& path="", bool warnNotFound=true );
             void               loadFromShell          ();
      inline void               setDisplayStyle        ( const char* );
      inline void               setSCALE_X             ( long value );
      inline void               setSYMBOLIC_TECHNOLOGY ( const char* value );
      inline void               setREAL_TECHNOLOGY     ( const char* value );
      inline void               setDISPLAY             ( const char* value );
      inline void               setIN_LO               ( const char* value );
      inline void               setIN_PH               ( const char* value );
      inline void               setOUT_LO              ( const char* value );
      inline void               setOUT_PH              ( const char* value );
             void               setPOWER               ( const char* value );
             void               setGROUND              ( const char* value );
             void               setCLOCK               ( const char* value );
             void               setOBSTACLE            ( const char* value );
             void               setPad                 ( const char* value );
      inline void               setCATALOG             ( const char* value );
      inline void               setWORKING_LIBRARY     ( const char* value );
             void               addSYSTEM_LIBRARY      ( const char* value, unsigned int mode=Append );

    // Methods.
             std::string        getPrint               () const;

    protected:
    // Internal: Attributes.
             std::string        _CORIOLIS_TOP;
             std::string        _displayStyle;
             long               _SCALE_X;
             std::string        _SYMBOLIC_TECHNOLOGY;
             std::string        _REAL_TECHNOLOGY;
             std::string        _DISPLAY;
             std::string        _IN_LO;
             std::string        _IN_PH;
             std::string        _OUT_LO;
             std::string        _OUT_PH;
             std::string        _POWER;
             std::string        _GROUND;
             std::string        _CLOCK;
             std::string        _OBSTACLE;
             std::string        _pad;
             std::string        _CATALOG;
             SearchPath         _LIBRARIES;
             regex_t            _PowerRegex;
             regex_t            _GroundRegex;
             regex_t            _ClockRegex;
             regex_t            _ObstacleRegex;
             regex_t            _padRegex;
             bool               _inConstructor;

    private:
             void         _setRegex       ( regex_t* regex, const std::string& pattern, const char* name );
             void         _check          () const;
      static std::string  _getLibraryName ( const std::string& path );
  };


  // Inline Member Functions.
  inline const std::string&     Environment::getCORIOLIS_TOP        () const { return _CORIOLIS_TOP; }
  inline const std::string&     Environment::getDisplayStyle        () const { return _displayStyle; }
  inline       long             Environment::getSCALE_X             () const { return _SCALE_X; }
  inline const std::string&     Environment::getSYMBOLIC_TECHNOLOGY () const { return _SYMBOLIC_TECHNOLOGY; }
  inline const std::string&     Environment::getREAL_TECHNOLOGY     () const { return _REAL_TECHNOLOGY; }
  inline const std::string&     Environment::getDISPLAY             () const { return _DISPLAY; }
  inline const std::string&     Environment::getIN_LO               () const { return _IN_LO; }
  inline const std::string&     Environment::getIN_PH               () const { return _IN_PH; }
  inline const std::string&     Environment::getOUT_LO              () const { return _OUT_LO; }
  inline const std::string&     Environment::getOUT_PH              () const { return _OUT_PH; }
  inline const std::string&     Environment::getPOWER               () const { return _POWER; }
  inline const std::string&     Environment::getGROUND              () const { return _GROUND; }
  inline const std::string&     Environment::getCLOCK               () const { return _CLOCK; }
  inline const std::string&     Environment::getOBSTACLE            () const { return _OBSTACLE; }
  inline const std::string&     Environment::getPad                 () const { return _pad; }
  inline const std::string&     Environment::getCATALOG             () const { return _CATALOG; }
  inline       SearchPath&      Environment::getLIBRARIES           () { return _LIBRARIES; }
                               
  inline void                   Environment::setDisplayStyle        ( const char* value ) { _displayStyle = value; }
  inline void                   Environment::setSCALE_X             ( long        value ) { _SCALE_X = value; }
  inline void                   Environment::setSYMBOLIC_TECHNOLOGY ( const char* value ) { _SYMBOLIC_TECHNOLOGY = value; }
  inline void                   Environment::setREAL_TECHNOLOGY     ( const char* value ) { _REAL_TECHNOLOGY = value; }
  inline void                   Environment::setDISPLAY             ( const char* value ) { _DISPLAY = value; }
  inline void                   Environment::setIN_LO               ( const char* value ) { _IN_LO = value; }
  inline void                   Environment::setIN_PH               ( const char* value ) { _IN_PH = value; }
  inline void                   Environment::setOUT_LO              ( const char* value ) { _OUT_LO = value; }
  inline void                   Environment::setOUT_PH              ( const char* value ) { _OUT_PH = value; }
  inline void                   Environment::setCATALOG             ( const char* value ) { _CATALOG = value; }
  inline void                   Environment::setWORKING_LIBRARY     ( const char* value ) { _LIBRARIES.replace(value,0); }


} // End of CRL namespace.


# endif
