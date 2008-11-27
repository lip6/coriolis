
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
      static  PaletteNamedItem* create           ( const Name&, bool checked=true );
      virtual const Name&       getName          () const;
      virtual bool              isChecked        () const;
      virtual void              setChecked       ( bool );

    protected:
              QCheckBox*        _checkBox;
              Name              _name;

    protected:
                                PaletteNamedItem ( const Name&, bool checked );
                                PaletteNamedItem ( const PaletteNamedItem& );
              PaletteNamedItem& operator=        ( const PaletteNamedItem& );
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_NAMED_ITEM__
