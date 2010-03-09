
// -*- C++ -*-


#ifndef  __CRL_ENVIRONMENT__
#define  __CRL_ENVIRONMENT__

#include  <regex.h>

#include  <string>

#include  <crlcore/SearchPath.h>


namespace CRL {


  class Environment {

    public:
    // Internal: Static Methods.
      static const char*   getEnv                 ( const char* variable, const char* defaultValue );
    // Constructors & destructors.
                           Environment            ();
                          ~Environment            ();

    // Accessors.
      inline const string& getCORIOLIS_TOP        () const;
      inline const string& getDisplayStyle        () const;
      inline       long    getSCALE_X             () const;
      inline const string& getSYMBOLIC_TECHNOLOGY () const;
      inline const string& getREAL_TECHNOLOGY     () const;
      inline const string& getLEF_TECHNOLOGY      () const;
      inline const string& getDISPLAY             () const;
      inline const string& getIN_LO               () const;
      inline const string& getIN_PH               () const;
      inline const string& getOUT_LO              () const;
      inline const string& getOUT_PH              () const;
      inline const string& getPOWER               () const;
      inline const string& getGROUND              () const;
      inline const string& getCLOCK               () const;
      inline const string& getOBSTACLE            () const;
      inline const string& getCATALOG             () const;
      inline SearchPath&   getLIBRARIES           ();

    // Predicates.
             bool          isPOWER                ( const char* name ) const;
             bool          isGROUND               ( const char* name ) const;
             bool          isCLOCK                ( const char* name ) const;
             bool          isOBSTACLE             ( const char* name ) const;

    // Modifiers.
             void          loadFromXml            ( const string& path="", bool warnNotFound=true );
             void          loadFromShell          ();
      inline void          setDisplayStyle        ( const char* );
      inline void          setSCALE_X             ( long value );
      inline void          setSYMBOLIC_TECHNOLOGY ( const char* value );
      inline void          setREAL_TECHNOLOGY     ( const char* value );
      inline void          setLEF_TECHNOLOGY      ( const char* value );
      inline void          setDISPLAY             ( const char* value );
      inline void          setIN_LO               ( const char* value );
      inline void          setIN_PH               ( const char* value );
      inline void          setOUT_LO              ( const char* value );
      inline void          setOUT_PH              ( const char* value );
             void          setPOWER               ( const char* value );
             void          setGROUND              ( const char* value );
             void          setCLOCK               ( const char* value );
             void          setOBSTACLE            ( const char* value );
      inline void          setCATALOG             ( const char* value );
      inline void          setWORKING_LIBRARY     ( const char* value );
      inline void          addSYSTEM_LIBRARY      ( const char* value );

    // Methods.
             string        getPrint               () const;

    protected:
    // Internal: Attributes.
             string        _CORIOLIS_TOP;
             string        _displayStyle;
             long          _SCALE_X;
             string        _SYMBOLIC_TECHNOLOGY;
             string        _LEF_TECHNOLOGY;
             string        _REAL_TECHNOLOGY;
             string        _DISPLAY;
             string        _IN_LO;
             string        _IN_PH;
             string        _OUT_LO;
             string        _OUT_PH;
             string        _POWER;
             string        _GROUND;
             string        _CLOCK;
             string        _OBSTACLE;
             string        _CATALOG;
             SearchPath    _LIBRARIES;
             regex_t       _PowerRegex;
             regex_t       _GroundRegex;
             regex_t       _ClockRegex;
             regex_t       _ObstacleRegex;
             bool          _inConstructor;

    // Internal: Modifiers.
             void          setRegex               ( regex_t* regex, const string& pattern, const char* name );
             void          check                  () const;
  };


  // Inline Member Functions.
  inline const string&     Environment::getCORIOLIS_TOP        () const { return _CORIOLIS_TOP; }
  inline const string&     Environment::getDisplayStyle        () const { return _displayStyle; }
  inline       long        Environment::getSCALE_X             () const { return _SCALE_X; }
  inline const string&     Environment::getSYMBOLIC_TECHNOLOGY () const { return _SYMBOLIC_TECHNOLOGY; }
  inline const string&     Environment::getREAL_TECHNOLOGY     () const { return _REAL_TECHNOLOGY; }
  inline const string&     Environment::getLEF_TECHNOLOGY      () const { return _LEF_TECHNOLOGY; }
  inline const string&     Environment::getDISPLAY             () const { return _DISPLAY; }
  inline const string&     Environment::getIN_LO               () const { return _IN_LO; }
  inline const string&     Environment::getIN_PH               () const { return _IN_PH; }
  inline const string&     Environment::getOUT_LO              () const { return _OUT_LO; }
  inline const string&     Environment::getOUT_PH              () const { return _OUT_PH; }
  inline const string&     Environment::getPOWER               () const { return _POWER; }
  inline const string&     Environment::getGROUND              () const { return _GROUND; }
  inline const string&     Environment::getCLOCK               () const { return _CLOCK; }
  inline const string&     Environment::getOBSTACLE            () const { return _OBSTACLE; }
  inline const string&     Environment::getCATALOG             () const { return _CATALOG; }
  inline       SearchPath& Environment::getLIBRARIES           () { return _LIBRARIES; }

  inline void              Environment::setDisplayStyle        ( const char* value ) { _displayStyle = value; }
  inline void              Environment::setSCALE_X             ( long        value ) { _SCALE_X = value; }
  inline void              Environment::setSYMBOLIC_TECHNOLOGY ( const char* value ) { _SYMBOLIC_TECHNOLOGY = value; }
  inline void              Environment::setREAL_TECHNOLOGY     ( const char* value ) { _REAL_TECHNOLOGY = value; }
  inline void              Environment::setLEF_TECHNOLOGY      ( const char* value ) { _LEF_TECHNOLOGY = value; }
  inline void              Environment::setDISPLAY             ( const char* value ) { _DISPLAY = value; }
  inline void              Environment::setIN_LO               ( const char* value ) { _IN_LO = value; }
  inline void              Environment::setIN_PH               ( const char* value ) { _IN_PH = value; }
  inline void              Environment::setOUT_LO              ( const char* value ) { _OUT_LO = value; }
  inline void              Environment::setOUT_PH              ( const char* value ) { _OUT_PH = value; }
  inline void              Environment::setCATALOG             ( const char* value ) { _CATALOG = value; }
  inline void              Environment::setWORKING_LIBRARY     ( const char* value ) { _LIBRARIES.replace(value,0); }
  inline void              Environment::addSYSTEM_LIBRARY      ( const char* value ) { _LIBRARIES.append(value); }


} // End of CRL namespace.


# endif
