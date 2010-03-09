
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
// |  C++ Header  :       "./solstice/ShortCircuitError.h"           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __SOLSTICE_SHORT_CIRCUIT_ERROR__
#define  __SOLSTICE_SHORT_CIRCUIT_ERROR__

#include <hurricane/Box.h>
#include <hurricane/Occurrence.h>


#include <solstice/RoutingError.h>

namespace Hurricane {

  class Point;


}


namespace Equinox {
  class Equi;
}



namespace Solstice {


  using Hurricane::Occurrence; 
  using Hurricane::Point;
  using Hurricane::Box;

  using namespace std;
  
  class ShortCircuitError : public RoutingError {
    
  public :
    inline static ShortCircuitError*    create                   (Cell*, 
								  const Occurrence&, 
								  const Occurrence&);
    // Accessors
  public:
    /**/   virtual const Name&          getName                  () const ;
    inline virtual string               getErrorName             () const ;
    /**/           string               getErrorDescription      () const ;
    inline virtual int                  getCount                 () const ;
    inline virtual set<Occurrence>*     getErrorOccurrences      () const ;
    inline         const Occurrence&    getOccurrence1           () const;
    inline         const Occurrence&    getOccurrence2           () const;
    inline         const Box&           getShortCircuitArea      () const;
    inline         const Occurrence&    getHyperNet1             () const;
    inline         const Occurrence&    getHyperNet2             () const;	   		        			         
    inline         void                 setOccurrence1           (const Occurrence occurrence);	   
    inline         void                 setOccurrence2           (const Occurrence occurrence);   
    /**/           string              _getString                () const;
    /**/           const Point          getShortCircuitAreaCenter() const; 
  private :
    /**/                                ShortCircuitError         (Cell*, 
								   const Occurrence&,
								   const Occurrence&);	
    inline                             ~ShortCircuitError         (); 
    
    // Attributs
  private :
    /**/           Occurrence          _occurrence1;
    /**/           Occurrence          _occurrence2;
    /**/           Occurrence          _hypernet1;
    /**/           Occurrence          _hypernet2;
    /**/           Box                 _shortcircuitarea;
    /**/           set<Occurrence>*    _errorOccurrences;
    

	   
  }; // End of class ShortCircuitError
  
  
  //inline functions
  inline string            ShortCircuitError::getErrorName             () const                      {return "Court-circuit";};
  inline const Occurrence& ShortCircuitError::getOccurrence1           () const                      { return _occurrence1; }; 
  inline const Occurrence& ShortCircuitError::getOccurrence2           () const                      { return _occurrence2; };	   
  inline const Box&        ShortCircuitError::getShortCircuitArea      () const                      { return _shortcircuitarea; };
  inline const Occurrence& ShortCircuitError::getHyperNet1             () const                      { return _hypernet1; }; 
  inline const Occurrence& ShortCircuitError::getHyperNet2             () const                      { return _hypernet2; };	   
  inline void              ShortCircuitError::setOccurrence1           (const Occurrence occurrence) { _occurrence1 = occurrence; };	   
  inline void              ShortCircuitError::setOccurrence2           (const Occurrence occurrence) { _occurrence2 = occurrence; };
  inline                   ShortCircuitError::~ShortCircuitError       ()                            {};
  inline ShortCircuitError*ShortCircuitError::create                   (Cell* cell, const Occurrence&  o1,const Occurrence&  o2) { ShortCircuitError* de = new ShortCircuitError (cell,o1,o2); de->_postCreate(); return de;};
 
} // End of Solstice namespace.

#endif  // __SOLSTICE_SHORT_CIRCUIT_ERROR__
