// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/GCellGrid.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_GCELL_GRID_H
#define  KATABATIC_GCELL_GRID_H

namespace Hurricane {
  class Cell;
}

#include  "katabatic/Constants.h"
#include  "katabatic/Grid.h"
#include  "katabatic/GCell.h"


namespace Katabatic {


  using Hurricane::Cell;
  using Hurricane::_TName;

  class KatabaticEngine;


// -------------------------------------------------------------------
// Class  :  "Katabatic::GCellGrid".

 
  class GCellGrid : public Grid<GCell> {
    public:
      enum DensityMode { AverageHVDensity=1  // Average between all densities.
                       , AverageHDensity =2  // Average between all H densities.
                       , AverageVDensity =3  // Average between all V densities.
                       , MaxHVDensity    =4  // Maximum between average H and average V.
                       , MaxVDensity     =5  // Maximum of V densities.
                       , MaxHDensity     =6  // Maximum of H densities.
                       , MaxDensity      =7  // Maximum of H & V densities.
                       };

    public:
              Cell*            getCell             () const;
      inline  KatabaticEngine* getKatabatic        () const;
      inline  unsigned int     getDensityMode      () const;
      inline  size_t           getHEdgeCapacity    () const;
      inline  size_t           getVEdgeCapacity    () const;
              Interval         getUSide            ( unsigned int ) const;
              size_t           checkDensity        () const;
              bool             checkEdgeSaturation ( float threshold ) const;
              size_t           updateDensity       ();
              void             updateContacts      ( unsigned int flags=KbOpenSession );
      inline  void             setDensityMode      ( unsigned int );
              void             _xmlWrite           ( ostream& );
      virtual Record*          _getRecord          () const;
      virtual string           _getString          () const;
      virtual string           _getTypeName        () const;

    // Attributes.
    protected:
      KatabaticEngine* _katabatic;
      int              _densityMode;
      size_t           _hEdgeCapacity;
      size_t           _vEdgeCapacity;

    // Constructors & Destructors.
    protected:
                         GCellGrid   ( KatabaticEngine* );
      virtual           ~GCellGrid   ();
              void       _postCreate ();
              void       _preDestroy ();
      static  GCellGrid* create      ( KatabaticEngine* );
    private:                          
                         GCellGrid   ( const GCellGrid& );
              GCellGrid& operator=   ( const GCellGrid& );

    // Friends.
    friend class KatabaticEngine;
  };


  inline  KatabaticEngine* GCellGrid::getKatabatic     () const { return _katabatic; }
  inline  unsigned int     GCellGrid::getDensityMode   () const { return _densityMode; }
  inline  size_t           GCellGrid::getHEdgeCapacity () const { return _hEdgeCapacity; }
  inline  size_t           GCellGrid::getVEdgeCapacity () const { return _vEdgeCapacity; }
  inline  void             GCellGrid::setDensityMode   ( unsigned int mode ) { _densityMode=mode; }


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::GCellGrid);



#endif  // __KATABATIC_GCELL_GRID__
