// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CapacitiesParameter.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CAPACITIES_PARAMETER_H
#define ANALOG_CAPACITIES_PARAMETER_H

#include "hurricane/analog/Parameter.h"

namespace Analog {
  

  class CapacitiesParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               CapacitiesParameter ( std::string id, size_t count );
      inline               CapacitiesParameter ( std::string id, const std::vector<double>& );
      inline  size_t       getCount            () const;
      inline  double       getValue            ( size_t index ) const;
      inline  void         setValue            ( size_t index, double value );
      virtual std::string  _getTypeName        () const;
      virtual std::string  _getString          () const;
      virtual Record*      _getRecord          () const;
    private:
      std::vector<double>  _values;
  };

  
  inline CapacitiesParameter::CapacitiesParameter ( std::string id, size_t count )
    : Parameter(id), _values(count,0.0)
  { } 

  
  inline CapacitiesParameter::CapacitiesParameter ( std::string id, const std::vector<double>& capacities )
    : Parameter(id), _values(capacities)
  { } 

  
  inline size_t  CapacitiesParameter::getCount   () const { return _values.size(); }
  inline double  CapacitiesParameter::getValue   ( size_t index ) const { return _values.at(index); }
  inline void    CapacitiesParameter::setValue   ( size_t index, double value  ) { _values.at(index) = value; }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::CapacitiesParameter);

#endif // ANALOG_CAPACITIES_PARAMETER_H
