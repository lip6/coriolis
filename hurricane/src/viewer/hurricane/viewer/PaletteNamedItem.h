
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PaletteNamedItem.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_PALETTE_NAMED_ITEM__
#define  __HURRICANE_PALETTE_NAMED_ITEM__


class QCheckBox;

#include  "hurricane/Name.h"
#include  "hurricane/viewer/PaletteItem.h"


namespace Hurricane {


  class PaletteNamedItem : public PaletteItem {
      Q_OBJECT;

    // Constructor.
    public:
      static  PaletteNamedItem* create           ( const Name&, bool visible=true );
      virtual const Name&       getName          () const;
      virtual bool              isItemVisible    () const;
      virtual void              setItemVisible   ( bool );

    protected:
              QCheckBox*        _visible;
              Name              _name;

    protected:
                                PaletteNamedItem ( const Name&, bool visible );
                                PaletteNamedItem ( const PaletteNamedItem& );
              PaletteNamedItem& operator=        ( const PaletteNamedItem& );
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_NAMED_ITEM__
