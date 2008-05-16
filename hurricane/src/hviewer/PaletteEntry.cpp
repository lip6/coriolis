
// -*- C++ -*-


# include  <QPainter>

# include  "Graphics.h"
# include  "PaletteEntry.h"


namespace Hurricane {


  PaletteSample::PaletteSample ( PaletteEntry* entry )
    : QWidget()
    , _sample(QSize(20,20))
    , _entry(entry)
  {
    setAttribute  ( Qt::WA_StaticContents );
    setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFixedSize  ( 20, 20 );

    redraw ();
  }


  void  PaletteSample::redraw ()
  {
    QPainter  painter ( &_sample );

    painter.setPen        ( Qt::NoPen );
    painter.setBackground ( Graphics::getBrush("background") );
    painter.eraseRect     ( 0, 0, 20, 20 );
    painter.setPen        ( Graphics::getPen  (_entry->getName()) );
    painter.setBrush      ( Graphics::getBrush(_entry->getName()) );
    painter.drawRect      ( 2, 2, 16, 16 );
  }


  void  PaletteSample::paintEvent ( QPaintEvent* )
  {
    QPainter painter ( this );
    painter.drawPixmap ( 0, 0, _sample );
  }


  PaletteEntry::PaletteEntry ( Palette* palette )
    : QWidget()
    , _palette(palette)
  {
  }


} // End of Hurricane namespace.
