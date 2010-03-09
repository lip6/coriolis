
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
// |  C++ Header  :       "./equinox/Strategy.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef _EQUINOX_STRATEGY_H
#define _EQUINOX_STRATEGY_H

namespace Hurricane {
  class Net;
  class Occurrence;
  class Component;


}

namespace Equinox {
  
  
  using std::vector;
  using std::map;
  using std::string;
  using Hurricane::Cell;
  using Hurricane::Occurrence;
  using Hurricane::Box;
  using Hurricane::Filter;
  using Hurricane::BasicLayer;
  using Hurricane::Component;
  using Hurricane::_TName;
  using Hurricane::Net;

  class EquinoxEngine;
  class Tile;  
  
  
  class Strategy {
    
  public:
   //implemented
    static  bool const isExtractableLayer(BasicLayer *);

  public :
    /**/              Strategy                 ();
    virtual           ~Strategy                (){};


    virtual void      run                      (EquinoxEngine* , unsigned int){};
    virtual void      getTilesFor              (EquinoxEngine*){};
    virtual void      operationAfterScanLine   (EquinoxEngine*){};

    //not implemented
    //virtual void      createIntervalSets       (TileSweepLine::IntervalSets*);
  private :
    /**/              Strategy                 (const Strategy&);
    /**/    Strategy& operator=                (const Strategy&);	   
    
  }; // End of Strategy
  
 

  class WithAlimStrategy : public Strategy {
    
  public : 
    /**/         WithAlimStrategy();
    virtual      ~WithAlimStrategy();


    virtual void run(EquinoxEngine* , unsigned int); 
    virtual void getTilesFor(EquinoxEngine*) ;
    virtual void operationAfterScanLine(EquinoxEngine*);
    
};// End of WithAlimStrategy


  class WithoutAlimStrategy : public Strategy {
    
  public : 
    /**/         WithoutAlimStrategy();
    virtual      ~WithoutAlimStrategy();
    virtual void run(EquinoxEngine* , unsigned int); 
    virtual void getTilesFor(EquinoxEngine*) ;
    virtual void operationAfterScanLine(EquinoxEngine*);
    
};// End of WithAlimStrategy




  class DefaultStrategy : public  WithAlimStrategy {
  public : 
    /**/         DefaultStrategy() {};    
  };// End of DefaultStrategy





  // -------------------------------------------------------------------
  // Class  :  "Equinox::WithAlimStrategyFilter".
  
  class WithAlimStrategyFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/    WithAlimStrategyFilter                             ();
    virtual Filter<Occurrence>*       getClone                () const;	   
    virtual string                    _getString              () const;
    virtual bool                      accept                  (Occurrence item) const;

    /**/          WithAlimStrategyFilter                       (const WithAlimStrategyFilter&);
    /**/    WithAlimStrategyFilter&    operator=               (const WithAlimStrategyFilter&);
  }; // End of class WithAlimStrategyFilter



  
    // -------------------------------------------------------------------
  // Class  :  "Equinox::WithoutAlimStrategyFilter".
  
  class WithoutAlimStrategyFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/    WithoutAlimStrategyFilter                             ();
    virtual Filter<Occurrence>*       getClone                () const;	   
    virtual string                    _getString              () const;
    virtual bool                      accept                  (Occurrence item) const;

    /**/          WithoutAlimStrategyFilter                       (const WithoutAlimStrategyFilter&);
    /**/    WithoutAlimStrategyFilter&    operator=               (const WithoutAlimStrategyFilter&);
  }; // End of class WithoutAlimStrategyFilter
  
  





}// End of namespace Equinox



#endif  // _EQUINOX_STRATEGY_H
