
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
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./equinox/Equi.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __EQUINOX_EQUI__
#define  __EQUINOX_EQUI__

#include  "hurricane/Entity.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/Occurrences.h"


namespace Hurricane {
  class Box;
  class Cell;
  class Net;
  class Entity;
  class Occurence;
  class Component;
}

namespace Equinox {

  using std::set;
  using std::string;
  using std::map;

  using namespace Hurricane;
  
  class EquinoxEngine;


  class Equi : public Entity {

    //Statics
  public:
    static  Equi*                            create                                (EquinoxEngine*       equinox,
				             					    Equi*                nextequi = NULL,
				             					    const unsigned long& number   = 0
				             					    ); 
    //Some filters
    static  GenericFilter<Occurrence>        getIsEquiFilter                       ();
    static  GenericFilter<Occurrence>        getIsComponentFilter                  ();
    static  GenericFilter<Occurrence>        getIsNetFilter                        ();
    static  GenericFilter<Occurrence>        getIsLeafEquiFilter                   ();
    static  GenericFilter<Occurrence>        getIsNotLeafEquiFilter                ();
    //    static  GenericFilter<Component*>        getIsUsedByExtractFilter              ();
    static  GenericFilter<Occurrence>        getIsNetOrComponentFilter             ();
    	

    //Entity
  public :
    virtual Cell*                            getCell                               () const; 
    inline virtual Box                              getBoundingBox                        () const;
  
    //Functions	
  public:
    /**/    bool                             isLeafEqui                            () const;	
    /**/    bool                             hasComponentOccurrence                () const;
    /**/    bool                             hasNetOccurrence                      () const;
    /**/    Occurrences                      getEquiOccurrences                    () const; 
    /**/    Occurrences                      getNetOccurrences                     () const; 
    /**/    Occurrences                      getAllOccurrences                     () const;
    /**/    Occurrences                      getNetAndComponentOccurrences         () const;

    /**/    Occurrences                      getEquiComponentOccurrences           () const; 

    /**/    Occurrences                      getCurrentComponentOccurrences        () const; 

    /**/    Occurrences                      getComponentOccurrences               () const; 


    
    inline  void                             setNextEqui                           (Equi* nextequi); 
    inline  Equi *                           getNextEqui                           () const; 
    inline  void                             setNumber                             (const unsigned long& number); 
    inline  void                             incrementCount                        ();	
    inline  void                             decrementCount                        ();
    inline  const unsigned long &            getCount                              () const;
    inline  Occurrences                      getOccurrences                        () const;
    inline  void                             addOccurrence                         (const Occurrence& occurrence);
    inline  void                             erase                                 ();
    
    /**/    void                             factorizeComponents                   ();
    inline  void                             merge                                 (Equi* equi);

    
    virtual string                           _getString                            () const;
  protected:
    virtual void                             _postCreate                           ();
    virtual void                             _preDestroy                           ();

    //Attributes
  protected :
    static  map<Net*, unsigned long>         _map_net2nb_usefulcomponent; 
  private:
    /**/    EquinoxEngine *                  _equinox                                ;
    /**/    set<Occurrence>                  _occurrences                            ;
    /**/    Equi      *                      _nextequi                               ;
    /**/    unsigned long                    _count                                  ;
    /**/    unsigned long                    _number                                 ;

    // Constructors & Destructors.
  private:
    /**/                                     Equi                                   (EquinoxEngine *      equinox,
										     Equi*                nextequi = NULL,
										     const unsigned long& number   = 0
										     );
    inline virtual                          ~Equi                                   ();
    /**/                                     Equi                                   (const Equi&);
    /**/    Equi&                            operator=                              (const Equi&);
    
  }; // End of class Equi 
  
  
  // Inline Functions.
  inline  Occurrences            Equi::getOccurrences            () const                       { return getCollection(_occurrences);};
  inline  Equi *                 Equi::getNextEqui               () const                       { return _nextequi; };   
  inline  const unsigned long &  Equi::getCount                  () const                       { return _count; };
  inline  void                   Equi::setNextEqui               (Equi* nextequi)               { _nextequi = nextequi; }	 ;  
  inline  void                   Equi::setNumber                 (const unsigned long& number)  { _number = number; };   							        
  inline  void                   Equi::incrementCount            ()                             { _count++ ; }	;	
  inline  void                   Equi::decrementCount            ()                             { _count--;  if( ( _count == 0 ) && ( _occurrences.empty() ) ) this->destroy(); };  							        
  inline  void                   Equi::addOccurrence             (const Occurrence& occurrence) { _occurrences.insert(occurrence);};
  inline  void                   Equi::erase                     ()                             { _occurrences.clear();};
  inline  Box                    Equi::getBoundingBox            () const                       { return Box();}
  inline                         Equi::~Equi                     ()                             {}  
  inline  void                   Equi::merge                     (Equi* equi)                   
  {
    forEach(Occurrence,occurrence,equi->getOccurrences())
      _occurrences.insert((*occurrence)); 
    equi->erase();
  };
  
  
} // End of Equinox namespace.





#endif  // __EQUINOX_EQUI__
