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


#ifndef HURRICANE_PHYSICAL_RULE_H
#define HURRICANE_PHYSICAL_RULE_H

#include "hurricane/DbU.h"
#include "hurricane/Rule.h"


namespace Hurricane {


  class PhysicalRule : public Rule {
    public:
      typedef Rule Super;
    public:
      inline               PhysicalRule ( const Name& name, DbU::Unit value, const std::string& reference );
      inline               PhysicalRule ( const PhysicalRule& rule );
      virtual             ~PhysicalRule ();
      inline  DbU::Unit    getValue     () const;
    public:
    // Inspector support.
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
    private:
      const Hurricane::DbU::Unit  _value;
  };
  
  
  inline PhysicalRule::PhysicalRule ( const Name&        name
                                    ,       DbU::Unit    value
                                    , const std::string& reference )
    : Rule(name,reference)
    , _value(value)
  { }
  
  inline PhysicalRule::PhysicalRule ( const PhysicalRule& rule )
    : Rule(rule.getName(),rule.getReference())
    , _value(rule._value)
  { }
  

  Hurricane::DbU::Unit PhysicalRule::getValue () const { return _value; }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::PhysicalRule);

#endif  // HURRICANE_PHYSICAL_RULE_H
