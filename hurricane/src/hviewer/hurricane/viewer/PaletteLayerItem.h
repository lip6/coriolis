
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
// |  C++ Header  :       "./PaletteLayerItem.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_PALETTE_LAYER_ITEM__
#define  __HURRICANE_PALETTE_LAYER_ITEM__


class QCheckBox;

#include  "hurricane/viewer/PaletteItem.h"


namespace Hurricane {


  class BasicLayer;


  class PaletteLayerItem : public PaletteItem {
      Q_OBJECT;

    public:
      static  PaletteLayerItem* create           ( BasicLayer*, bool checked=true );
      virtual const Name&       getName          () const;
      virtual bool              isChecked        () const;
      virtual void              setChecked       ( bool state );

    protected:
              BasicLayer*       _basicLayer;
              QCheckBox*        _checkBox;

    protected:
                                PaletteLayerItem ( BasicLayer*, bool checked );
                                PaletteLayerItem ( const PaletteLayerItem& );
              PaletteLayerItem& operator=        ( const PaletteLayerItem& );
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_LAYER_ITEM__
