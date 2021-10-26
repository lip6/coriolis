// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/PhysicalRule.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include <tuple>
#include "hurricane/DbU.h"
#include "hurricane/Rule.h"


namespace Hurricane {


  class RuleStep {
    public:
      inline                      RuleStep     ( Hurricane::DbU::Unit uValue );
      inline                      RuleStep     ( Hurricane::DbU::Unit hValue, Hurricane::DbU::Unit vValue );
      inline Hurricane::DbU::Unit getValue     () const;
      inline Hurricane::DbU::Unit getHValue    () const;
      inline Hurricane::DbU::Unit getVValue    () const;
      inline Hurricane::DbU::Unit getThreshold () const;
      inline void                 setThreshold (  Hurricane::DbU::Unit );
    public:
             std::string          _getTypeName () const;
             std::string          _getString   () const;
             Record*              _getRecord   () const;
    private:
      Hurricane::DbU::Unit _hValue;
      Hurricane::DbU::Unit _vValue;
      Hurricane::DbU::Unit _threshold;
  };


  inline RuleStep::RuleStep ( Hurricane::DbU::Unit uValue )
    : _hValue   (uValue)
    , _vValue   (uValue)
    , _threshold(0)
  { }

  inline RuleStep::RuleStep ( Hurricane::DbU::Unit hValue, Hurricane::DbU::Unit vValue )
    : _hValue   (hValue)
    , _vValue   (vValue)
    , _threshold(0)
  { }

  inline Hurricane::DbU::Unit RuleStep::getValue     () const { return _hValue; }
  inline Hurricane::DbU::Unit RuleStep::getHValue    () const { return _hValue; }
  inline Hurricane::DbU::Unit RuleStep::getVValue    () const { return _vValue; }
  inline Hurricane::DbU::Unit RuleStep::getThreshold () const { return _threshold; }
  inline void                 RuleStep::setThreshold (  Hurricane::DbU::Unit t ) { _threshold = t; }


  class PhysicalRule : public Rule {
    public:
      typedef Rule Super;
      typedef std::vector<RuleStep>  Steps;
    public:
      inline               PhysicalRule   ( const Name& name, const std::string& reference );
      inline               PhysicalRule   ( const PhysicalRule& rule );
      virtual             ~PhysicalRule   ();
      inline  bool         isDouble       () const;
      inline  bool         isDbU          () const;
      inline  bool         isSymmetric    () const;
      inline  bool         hasSteps       () const;
      inline  void         setSymmetric   ( bool );
      inline  double       getDoubleValue () const;
      inline  DbU::Unit    getValue       ( Hurricane::DbU::Unit length=0, bool hDir=true ) const;
      inline  void         addValue       ( double );
      inline  void         addValue       ( Hurricane::DbU::Unit value
                                          , Hurricane::DbU::Unit maxLength );
      inline  void         addValue       ( Hurricane::DbU::Unit hValue
                                          , Hurricane::DbU::Unit vValue
                                          , Hurricane::DbU::Unit maxLength );
    public:
      virtual std::string  _getTypeName   () const;
      virtual std::string  _getString     () const;
      virtual Record*      _getRecord     () const;
    public:
      inline  void         _addValue      ( const RuleStep& );
    private:
      Steps   _stepsValue;
      double  _doubleValue;
      bool    _symmetric;
  };
  
  
  inline PhysicalRule::PhysicalRule ( const Name&        name
                                    , const std::string& reference )
    : Rule(name,reference)
    , _stepsValue ()
    , _doubleValue(0.0)
    , _symmetric  (true)
  { }


  inline PhysicalRule::PhysicalRule ( const PhysicalRule& rule )
    : Rule(rule.getName(),rule.getReference())
    , _stepsValue (rule._stepsValue)
    , _doubleValue(rule._doubleValue)
  { }
  

  inline bool    PhysicalRule::isDouble       () const { return _doubleValue != 0; }
  inline bool    PhysicalRule::isDbU          () const { return not _stepsValue.empty(); }
  inline bool    PhysicalRule::isSymmetric    () const { return _symmetric; }
  inline bool    PhysicalRule::hasSteps       () const { return not (_stepsValue.size() > 1); }
  inline double  PhysicalRule::getDoubleValue () const { return _doubleValue; }
  inline void    PhysicalRule::setSymmetric   ( bool state ) { _symmetric = state; }
  inline void    PhysicalRule::addValue       ( double value ) { _doubleValue = value; }

    
  inline Hurricane::DbU::Unit PhysicalRule::getValue ( Hurricane::DbU::Unit length, bool hDir ) const
  {
    if (_stepsValue.empty()) return 0;
    for ( const RuleStep& step : _stepsValue ) {
      if (length < step.getThreshold()) {
        return (hDir) ? step.getHValue() : step.getVValue();
      }
    }
    return (hDir) ? _stepsValue.back().getHValue() : _stepsValue.back().getVValue();;
  }

  
  inline  void  PhysicalRule::addValue ( Hurricane::DbU::Unit value, Hurricane::DbU::Unit maxLength )
  {
    RuleStep step ( value );
    step.setThreshold( maxLength );
    _addValue( step );
  }

  
  inline  void  PhysicalRule::addValue ( Hurricane::DbU::Unit hValue, Hurricane::DbU::Unit vValue, Hurricane::DbU::Unit maxLength )
  {
    RuleStep step ( hValue, vValue );
    step.setThreshold( maxLength );
    _addValue( step );
  }

  
  inline  void  PhysicalRule::_addValue ( const RuleStep& step )
  {
    for ( auto istep = _stepsValue.begin() ; istep != _stepsValue.end() ; ++istep ) {
      if (step.getThreshold() < (*istep).getThreshold()) {
        _stepsValue.insert( istep, step );
        return;
      }
    }
    _stepsValue.push_back( step );
  }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::PhysicalRule);
INSPECTOR_PR_SUPPORT(Hurricane::RuleStep);
