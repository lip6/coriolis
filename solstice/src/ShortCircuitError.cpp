
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ShortCircuitError.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x
#include <hurricane/Cell.h>
#include <solstice/SolsticeEngine.h>
#include <solstice/ShortCircuitError.h>
#include <string>

namespace Solstice {

  using namespace Hurricane;




  ShortCircuitError::ShortCircuitError(Cell*cell, const Occurrence&occurrence1, const Occurrence&occurrence2) 
    : RoutingError(cell,1)
  {
    _errorOccurrences = new set<Occurrence>;
    _errorOccurrences->insert(occurrence1);
    _errorOccurrences->insert(occurrence2);

    if(occurrence1 < occurrence2) {
      _occurrence1 = occurrence2; 
      _occurrence2 = occurrence1;
    }
    else { 
      _occurrence1 = occurrence1; 
      _occurrence2 = occurrence2;
    }
    
    _hypernet1 = SolsticeEngine::getTopNetOccurrence(_occurrence1);
    _hypernet2 = SolsticeEngine::getTopNetOccurrence(_occurrence2);
    _shortcircuitarea = _occurrence1.getBoundingBox().getIntersection(_occurrence2.getBoundingBox());


    //  forEach ( Component*, component, dynamic_cast<Net*>(_hypernet1.getEntity())->getComponents() ) {
    //	_occurrences->insert(Occurrence ( *component , _hypernet1.getPath() ) );
    //  }
    //
    //  forEach ( Component*, component, dynamic_cast<Net*>(_hypernet2.getEntity())->getComponents() ) {
    //	_occurrences->insert(Occurrence ( *component , _hypernet2.getPath()) );
    //  }
      //_occurrences->insert(Occurrence(this));

    _occurrences->insert(_hypernet1);
    _occurrences->insert(_hypernet2);

    _area = _occurrence1.getBoundingBox().getUnion(_occurrence2.getBoundingBox());
    _area = _shortcircuitarea;
  };
  
  




 string ShortCircuitError::getErrorDescription ()const 
 {
   return "contact invalide entre " 
     + (_hypernet1).getEntity()->_getString() + " et " 
     + (_hypernet2).getEntity()->_getString() ;
 };


  const Name&        ShortCircuitError::getName    () const {return _extensionName;} ;
  int                ShortCircuitError::getCount() const {return 1;};
  set<Occurrence>*   ShortCircuitError::getErrorOccurrences     () const {return _errorOccurrences;};


string ShortCircuitError::_getString() const
// *****************************************
{
   string s = "  " + getString(getCell()->getName()) + " : Short-Circuit happen at the position : ";
   s +=  getString(_shortcircuitarea) ;
   s += "\n  between " + getString(_occurrence1) + " of " + getString(_occurrence1.getBoundingBox()) + " of HyperNet " + getString(_hypernet1);
   s += "\n  and " + getString(_occurrence2) + " of " + getString(_occurrence2.getBoundingBox()) + " of HyperNet " + getString(_hypernet2);
   return s;
}  


} // End of Solstice namespace.
