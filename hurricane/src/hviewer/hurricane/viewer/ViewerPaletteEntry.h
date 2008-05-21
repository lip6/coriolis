
// -*- C++ -*-


# ifndef  __VIEWER_PALETTE_ENTRY_H__
#   define  __VIEWER_PALETTE_ENTRY_H__


class QCheckBox;

# include  "hurricane/Name.h"
# include  "hurricane/viewer/PaletteEntry.h"


namespace Hurricane {


  class ViewerPaletteEntry : public PaletteEntry {
      Q_OBJECT;

    // Constructor.
    public:
      static  ViewerPaletteEntry* create             ( Palette* palette, const Name& name );

    // Methods.
    public:
      virtual bool                isGroup            () const;
      virtual bool                isBasicLayer       () const;
      virtual const Name&         getName            () const;
      virtual BasicLayer*         getBasicLayer      ();
      virtual bool                isChecked          () const;
      virtual void                setChecked         ( bool state );

    // Slots.
    public slots:
      virtual void                toggle             ();

    // Internal - Attributes.
    protected:
              const Name          _name;
              QCheckBox*          _checkBox;

    // Internal - Constructor.
                                  ViewerPaletteEntry ( Palette* palette, const Name& name );
                                  ViewerPaletteEntry ( const ViewerPaletteEntry& );
              ViewerPaletteEntry& operator=          ( const ViewerPaletteEntry& );
      virtual void                _postCreate        ();

  };


} // End of Hurricane namespace.


# endif
