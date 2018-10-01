// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/Choices.h"                  |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CHOICES_H
#define ANALOG_CHOICES_H

#include <string>
#include <vector>

namespace Analog {
  

  class Choices {
    public:
      friend class ChoiceParameter;
      friend class MCheckBoxParameter;
      typedef std::vector<std::string> Values;
    public:
                           Choices      ();
                           Choices      ( const Choices& );
             Choices&      operator<<   ( const std::string& choice ); 
      inline Values        getValues    () const;
             unsigned int  findIndexFor ( const std::string& choice ) const;
    private:
      Values _values;
  };


  inline Choices::Values  Choices::getValues () const { return _values; }


}  // Analog namespace.

#endif
