
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
  class HPalette;
  class HPaletteEntry;


  class HPaletteSample : public QWidget {
      Q_OBJECT;

    // Constructor.
    public:
                     HPaletteSample ( HPaletteEntry* entry );

    // Internals - Attributes.
    protected:
      QPixmap        _sample;
      HPaletteEntry*  _entry;

    // Internals - Methods.
      void           redraw        ();
      void           paintEvent    ( QPaintEvent* );

  };


  class HPaletteEntry : public QWidget {
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
              HPalette*      _palette;

    // Internal - Constructor.
                             HPaletteEntry ( HPalette* palette );
                             HPaletteEntry ( const HPaletteEntry& );
              HPaletteEntry& operator=     ( const HPaletteEntry& );
      virtual void           _postCreate   () = 0;

  };


} // End of Hurricane namespace.


# endif
