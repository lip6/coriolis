
// -*- C++ -*-


# ifndef  __PALETTE_ENTRY_H__
#   define  __PALETTE_ENTRY_H__

# include  <QPixmap>
# include  <QWidget>
# include  <QFont>

class QPaintEvent;
class QCheckBox;

# include  "hurricane/Commons.h"


namespace Hurricane {


  class Name;
  class BasicLayer;
  class Palette;
  class PaletteEntry;


  class PaletteSample : public QWidget {
      Q_OBJECT;

    // Constructor.
    public:
                     PaletteSample ( PaletteEntry* entry );

    // Internals - Attributes.
    protected:
      QPixmap        _sample;
      PaletteEntry*  _entry;

    // Internals - Methods.
      void           redraw        ();
      void           paintEvent    ( QPaintEvent* );

  };


  class PaletteEntry : public QWidget {
      Q_OBJECT;

    // Methods.
    public:
      virtual const Name&   getName        () const = 0;
      virtual bool          isGroup        () const = 0;
      virtual bool          isBasicLayer   () const = 0;
      virtual BasicLayer*   getBasicLayer  () = 0;
      virtual bool          isChecked      () const = 0;
      virtual void          setChecked     ( bool state ) = 0;

    // Slots.
    public slots:
      virtual void          toggle         () = 0;

    // Internal - Attributes.
    protected:
              Palette*      _palette;

    // Internal - Constructor.
                            PaletteEntry  ( Palette* palette );
                            PaletteEntry  ( const PaletteEntry& );
              PaletteEntry& operator=     ( const PaletteEntry& );
      virtual void          _postCreate   () = 0;

  };


} // End of Hurricane namespace.


# endif
