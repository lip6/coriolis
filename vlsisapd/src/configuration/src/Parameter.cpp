
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Parameter.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  "vlsisapd/configuration/Parameter.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;


  string  Parameter::typeToString  ( Parameter::Type type )
  {
    switch ( type ) {
      case String:     return "string";
      case Int:        return "int";
      case Double:     return "double";
      case Bool:       return "bool";
      case Percentage: return "percentage";
      case Enumerate:  return "enumerate";
    }
    return "unsupported";
  }


  Parameter::Parameter ( const std::string& id
                       , Type               type
                       , const std::string& value )
    : _id       (id)
    , _type     (type)
    , _value    (value)
    , _flags    (0)
    , _minInt   (0)
    , _maxInt   (0)
    , _minDouble(0.0)
    , _maxDouble(0.0)
  {
    if ( type == Percentage ) {
      setPercentage ( asDouble() );
    }
  }
  

}  // End of Cfg namespace.
