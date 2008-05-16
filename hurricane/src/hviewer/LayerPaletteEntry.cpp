
// -*- C++ -*-


# include  <QCheckBox>
# include  <QHBoxLayout>

# include  "BasicLayer.h"

# include  "Graphics.h"
# include  "LayerPaletteEntry.h"
# include  "Palette.h"


namespace Hurricane {


  LayerPaletteEntry::LayerPaletteEntry ( Palette* entry, BasicLayer* basicLayer )
    : PaletteEntry(entry)
    , _basicLayer(basicLayer)
  {
  }


  LayerPaletteEntry* LayerPaletteEntry::create ( Palette* palette, BasicLayer* basicLayer )
  {
    LayerPaletteEntry* entry = new LayerPaletteEntry ( palette, basicLayer );

    entry->_postCreate ();

    return entry;
  }


  void  LayerPaletteEntry::_postCreate ()
  {
    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    layout->addWidget ( new PaletteSample(this) );

    _checkBox = new QCheckBox ( this );
    _checkBox->setChecked ( true );
    _checkBox->setText    ( getString(getName()).c_str() );
    _checkBox->setFont    ( Graphics::getFixedFont() );
    layout->addWidget ( _checkBox );

    setLayout ( layout );

    connect ( _checkBox, SIGNAL(clicked()), this, SLOT(toggle()) );
  }


  bool LayerPaletteEntry::isGroup () const
  {
    return false;
  }


  bool LayerPaletteEntry::isBasicLayer () const
  {
    return true;
  }


  const Name& LayerPaletteEntry::getName () const
  {
    return _basicLayer->getName();
  }


  BasicLayer* LayerPaletteEntry::getBasicLayer ()
  {
    return _basicLayer;
  }


  bool  LayerPaletteEntry::isChecked () const
  {
    return _checkBox->isChecked ();
  }


  void  LayerPaletteEntry::setChecked ( bool state )
  {
    _checkBox->setChecked ( state );
  }


  void  LayerPaletteEntry::toggle ()
  {
    _palette->redrawCellWidget();
  }


} // End of Hurricane namespace.
