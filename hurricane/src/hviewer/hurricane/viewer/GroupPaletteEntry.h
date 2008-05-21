
// -*- C++ -*-


# ifndef  __GROUP_PALETTE_ENTRY_H__
#   define  __GROUP_PALETTE_ENTRY_H__


class QPushButton;

# include  "hurricane/Name.h"

# include  "hurricane/viewer/PaletteEntry.h"


namespace Hurricane {


  class GroupPaletteEntry : public PaletteEntry {
      Q_OBJECT;

    // Constructor.
    public:
      static  GroupPaletteEntry* create             ( Palette* palette, const Name& name );

    // Methods.
    public:
      virtual bool               isGroup            () const;
      virtual bool               isBasicLayer       () const;
      virtual const Name&        getName            () const;
      virtual BasicLayer*        getBasicLayer      ();
      virtual bool               isChecked          () const;
      virtual void               setChecked         ( bool state );

    // Slots.
    public slots:
      virtual void               toggle             ();

    // Internal - Attributes.
    protected:
              const Name         _name;
              QPushButton*       _button;
              size_t             _index;
              bool               _expanded;

    // Internal - Constructor.
                                 GroupPaletteEntry  ( Palette* palette, const Name& name );
                                 GroupPaletteEntry  ( const GroupPaletteEntry& );
              GroupPaletteEntry& operator=          ( const GroupPaletteEntry& );
      virtual void               _postCreate        ();

    // Internal - Methods.
              void               hideShow           ();
              string             getLabel           ();
  };


} // End of Hurricane namespace.


# endif
