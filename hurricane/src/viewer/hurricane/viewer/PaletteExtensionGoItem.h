// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PaletteExtensionGoItem.h"               |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_PALETTE_EXTENSION_GO_ITEM_H
#define  HURRICANE_PALETTE_EXTENSION_GO_ITEM_H


class QCheckBox;

#include  "hurricane/Name.h"
#include  "hurricane/viewer/PaletteItem.h"


namespace Hurricane {


  class PaletteExtensionGoItem : public PaletteItem {
      Q_OBJECT;

    // Constructor.
    public:
      static  PaletteExtensionGoItem* create                ( const Name& );
      virtual const Name&             getName               () const;
      virtual bool                    isItemVisible         () const;
      virtual bool                    isItemSelectable      () const;
      virtual void                    setItemVisible        ( bool state );
      virtual void                    setItemSelectable     ( bool state );

    protected:
              QCheckBox*              _visible;
              QCheckBox*              _selectable;
              Name                    _name;

    protected:
                                      PaletteExtensionGoItem ( const Name& );
                                      PaletteExtensionGoItem ( const PaletteExtensionGoItem& );
      virtual                        ~PaletteExtensionGoItem ();
              PaletteExtensionGoItem& operator=              ( const PaletteExtensionGoItem& );
      virtual void                    _postCreate            ();
  };


} // Hurricane namespace.

#endif  // HURRICANE_PALETTE_EXTENSION_GO_ITEM_H
