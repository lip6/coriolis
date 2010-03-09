
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
// | =============================================================== |
// |  C++ Header  :       "./Brick.h"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __SOLSTICE_BRICK__
#define  __SOLSTICE_BRICK__

namespace Hurricane {

  class Occurrence;
  class Box;
  class BasicLayer;
  class Unit;
}

namespace Equinox {
  class Interval;
}
namespace Solstice {

  using namespace std;
  using Equinox::Interval;
  using Hurricane::Occurrence;
  using Hurricane::Box;
  using Hurricane::BasicLayer;
  using Hurricane::DbU;
  class Brick : public Interval
  {

  public : 
    static Brick* create(const Occurrence&, const Occurrence&, const Box&, BasicLayer*);
    void destroy();


  public :

    /**/      bool              isEmpty () const;
    /**/      string            _getString() const;

    /**/      const  DbU::Unit&       getXmin() const;  	   
    /**/      const  DbU::Unit&       getYmin() const;  	   
    /**/      const  DbU::Unit&       getXmax() const;  	   
    /**/      const  DbU::Unit&       getYmax() const;  
    inline    const Box&        getBoundingBox();
    inline    const Occurrence& getHyperNet(); 
    inline    const Occurrence& getComponentOccurrence() const;
    inline    BasicLayer*       getBasicLayer();
    virtual long  GetLowPoint() const;
    virtual long GetHighPoint() const;
 

  private :
    Occurrence  _hypernet;
    Occurrence  _componentoccurrence;
    Box         _box;
    BasicLayer* _basicLayer;	    
    
  protected : 
    Brick(const Occurrence& ,const Occurrence&, const Box&, BasicLayer*);
    virtual ~Brick();

  private :
    Brick(const Brick&);
    Brick& operator=(const Brick&);	   

}; // End of class Brick 
  
  
  // Inline Functions.
  inline    const Box&        Brick::getBoundingBox() { return _box; };
  inline    const Occurrence& Brick::getHyperNet() { return _hypernet; };
  inline    const Occurrence& Brick::getComponentOccurrence() const { return _componentoccurrence; };
  inline    BasicLayer*       Brick::getBasicLayer(){ return _basicLayer; };
    






} // End of Equinox namespace.







#endif  // __SOLSTICE_BRICK__
