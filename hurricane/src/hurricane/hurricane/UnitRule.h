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
// |  C++ Header  :  "./hurricane/UnitRule.h"                        |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_UNIT_RULE_H
#define HURRICANE_UNIT_RULE_H

#include "hurricane/Rule.h"


namespace Hurricane {


  class UnitRule : public Rule {
    public:
      typedef Rule Super;
    public:
      inline               UnitRule     ( const Name& name, double value, const std::string& reference );
      inline               UnitRule     ( const UnitRule& rule );
      virtual             ~UnitRule     ();
      inline  double       getValue     () const;
    public:
    // Inspector support.
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
    private:
      const double  _value;
  };
  
  
  inline UnitRule::UnitRule ( const Name&        name
                            ,       double       value
                            , const std::string& reference )
    : Rule(name,reference)
    , _value(value)
  { }
  
  inline UnitRule::UnitRule ( const UnitRule& rule )
    : Rule(rule.getName(),rule.getReference())
    , _value(rule._value)
  { }
  

  double  UnitRule::getValue () const { return _value; }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::UnitRule);

#endif  // HURRICANE_UNIT_RULE_H
