
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
// |  C++ Header  :       "./HExtensionPaletteEntry.h"               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_EXTENSION_PALETTE_ENTRY__
#define  __HURRICANE_EXTENSION_PALETTE_ENTRY__


class QCheckBox;

#include  "hurricane/Name.h"
#include  "hurricane/viewer/HPaletteEntry.h"


namespace Hurricane {


  class HExtensionPaletteEntry : public HPaletteEntry {
      Q_OBJECT;

    // Constructor.
    public:
      static  HExtensionPaletteEntry* create                ( HPalette* , const Name& );

    // Methods.
    public:
      virtual bool                    isGroup               () const;
      virtual bool                    isBasicLayer          () const;
      virtual bool                    isExtension           () const;
      virtual const Name&             getName               () const;
      virtual BasicLayer*             getBasicLayer         ();
      virtual bool                    isChecked             () const;
      virtual void                    setChecked            ( bool );

    // Slots.
    public slots:
      virtual void                    toggle                ();

    // Internal - Attributes.
    protected:
              QCheckBox*              _checkBox;
              Name                    _name;

    // Internal - Constructor.
                                      HExtensionPaletteEntry ( HPalette* , const Name& );
                                      HExtensionPaletteEntry ( const HExtensionPaletteEntry& );
              HExtensionPaletteEntry& operator=              ( const HExtensionPaletteEntry& );
      virtual void                    _postCreate            ();

  };


} // End of Hurricane namespace.


#endif  // __LAYER_PALETTE_ENTRY__
