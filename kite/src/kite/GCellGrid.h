
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
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GCellGrid.h"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_GCELL_GRID__
#define  __KITE_GCELL_GRID__

namespace Hurricane {
  class Cell;
}

#include  "katabatic/GCellGrid.h"
#include  "kite/GCell.h"
#include  "kite/KiteEngine.h"


namespace Kite {


  using Hurricane::Cell;
  class KiteEngine;


// -------------------------------------------------------------------
// Class  :  "GCellGrid".
 

  class GCellGrid : public Katabatic::Grid<GCell> {

    public:
      static  GCellGrid*            create              ( KiteEngine* );
    public:                                            
      inline  Katabatic::GCellGrid* base                ();
      inline  KiteEngine*           getKite             ();
              Cell*                 getCell             () const;
      inline  bool                  checkEdgeSaturation ( float threshold ) const;
              void                  updateContacts      ( bool openSession=true );
              void                  updateDensity       ();
              double                getTotalWireLength  () const;
              void                  _check              () const;
      virtual Record*               _getRecord          () const;
      virtual string                _getString          () const;
      virtual string                _getTypeName        () const;

    protected:
      KiteEngine* _kite;

    protected:
    // Constructors & Destructors.
                         GCellGrid   ( KiteEngine* );
      virtual           ~GCellGrid   ();
      virtual void       _postCreate ();
      virtual void       _preDestroy ();
    private:
                         GCellGrid   ( const GCellGrid& );
              GCellGrid& operator=   ( const GCellGrid& );
  };


// Inline Functions.
  inline  Katabatic::GCellGrid* GCellGrid::base                () { return _kite->base()->getGCellGrid(); }
  inline  KiteEngine*           GCellGrid::getKite             () { return _kite; };

  inline  bool  GCellGrid::checkEdgeSaturation ( float threshold ) const
  { return const_cast<GCellGrid*>(this)->base()->checkEdgeSaturation(threshold); }


} // End of Kite namespace.


#endif  // __KITE_GCELL_GRID__
