
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
// |  C++ Header  :       "./equinox/Tile.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __EQUINOX_TILE__
#define  __EQUINOX_TILE__

#include <hurricane/Occurrence.h>
#include <hurricane/Box.h>
#include <equinox/Interval.h>

namespace Hurricane {

  class Box;
  class Cell;
  class Entity;
  class BasicLayer;
  class Record;
  class Occurrence;

} // End of Hurricane namespace.


namespace Equinox {

  using std::string;

  using Hurricane::BasicLayer;
  using Hurricane::Occurrence;
  using Hurricane::Box;
  using Hurricane::DbU;
  using Hurricane::Record;

  class Equi;


  // -------------------------------------------------------------------
  // Class  :  "EQUINOX::Tile".
  
  class Tile : public Interval
  {     

    

    //Statics
  public: 
    static  Tile*             create             (const Occurrence,
						  const Box,
						  BasicLayer*,
						  Equi* equi = NULL); 

	    	            
    //Functions		            
  public:   	            
    inline     void           destroy            ();
    
  public:
    inline          const Box&        getBoundingBox     ();
    inline          const Occurrence& getOccurrence      () const;
    inline          BasicLayer*       getBasicLayer      () const;
    inline          Equi*             getEqui            () const;
    inline          void              setEqui            (Equi* equi);
    	            
    inline          const DbU::Unit&  getXmin            () const;
    inline          const DbU::Unit&  getYmin            () const;
    inline          const DbU::Unit&  getXmax            () const;
    inline          const DbU::Unit&  getYmax            () const;
    inline          bool              isEmpty            () const;
    inline virtual  long              GetLowPoint        () const;
    inline virtual  long              GetHighPoint       () const;
    /**/            bool              intersect          (const Tile* tile) const;
    
    inline virtual  string           _getTypeName        () const;
    /**/   virtual  string           _getString          () const;
    inline virtual  Record*          _getRecord          () const;
    
  private:
    /**/            const Occurrence _occurrence;
    /**/            const Box        _box;
    /**/            BasicLayer*      _basicLayer;
    /**/            Equi*            _equi; 
    
    // Constructors & Destructors.
  protected:
    /**/                              Tile               (const Occurrence, 
							  const Box,
							  BasicLayer*, 
							  Equi* equi = NULL); 
    inline virtual                   ~Tile              ();
    inline virtual  void             _postCreate        ();
    inline virtual  void             _preDelete         ();
  private:       	       
    /**/                              Tile              (const Tile&);
    /**/            Tile&             operator=         (const Tile&);

  }; // End of class Equi 
  
  
  // Inline Functions.
  inline Equi*               Tile::getEqui         () const                  { return _equi; };
  inline const  Box&         Tile::getBoundingBox  ()                        { return _box; };
  inline const Occurrence&   Tile::getOccurrence   () const                  { return _occurrence; };
  inline BasicLayer*         Tile::getBasicLayer   () const                  { return _basicLayer; };
  inline void                Tile::setEqui         (Equi* equi)              {  _equi = equi; };  
  
  inline long                Tile::GetLowPoint     () const                  { return _box.getYMin(); }  
  inline long                Tile::GetHighPoint    () const                  { return _box.getYMax(); }  
  inline string              Tile::_getTypeName    () const                  { return "Equinox::Tile"; }
  inline const DbU::Unit&    Tile::getXmin         () const                  { return _box.getXMin(); };	   
  inline const DbU::Unit&    Tile::getYmin         () const                  { return _box.getYMin(); };	   
  inline const DbU::Unit&    Tile::getXmax         () const                  { return _box.getXMax(); };	   
  inline const DbU::Unit&    Tile::getYmax         () const                  { return _box.getYMax(); };
  inline bool                Tile::isEmpty         () const                  { return _box.isEmpty(); };
  inline Record*             Tile::_getRecord      () const                  { return NULL; };
  inline void                Tile::destroy         ()                        { _preDelete();  delete this; }; 
  inline void                Tile::_postCreate     ()                        { };
  inline void                Tile::_preDelete      ()                        { };
  inline /**/                Tile::~Tile           ()                        { };








} // End of Equinox namespace.




#endif  // __EQUINOX_TILE__
