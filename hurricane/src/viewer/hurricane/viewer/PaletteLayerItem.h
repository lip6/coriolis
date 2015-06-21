
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
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
      static  PaletteLayerItem*   create            ( BasicLayer*, bool visible=true );
      virtual const Name&         getName           () const;
      virtual bool                isItemVisible     () const;
      virtual bool                isItemSelectable  () const;
      virtual void                setItemVisible    ( bool state );
      virtual void                setItemSelectable ( bool state );
    public slots:                 
      virtual void                changeStyle       ();
                                                  
    protected:                                    
              BasicLayer*         _basicLayer;      
              QCheckBox*          _visible;         
              QCheckBox*          _selectable;      
              DrawingStyleSample* _sample;
                                                  
    protected:                                    
                                  PaletteLayerItem  ( BasicLayer*, bool visible );
                                  PaletteLayerItem  ( const PaletteLayerItem& );
              PaletteLayerItem&   operator=         ( const PaletteLayerItem& );
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_LAYER_ITEM__
