
// -*- C++ -*-


# include  <QPushButton>
# include  <QHBoxLayout>

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/GroupPaletteEntry.h"
# include  "hurricane/viewer/Palette.h"


namespace Hurricane {


  GroupPaletteEntry::GroupPaletteEntry ( Palette* palette, const Name& name )
    : PaletteEntry(palette)
    , _name(name)
    , _button(NULL)
    , _index(0)
    , _expanded(true)
  {
  }


  GroupPaletteEntry* GroupPaletteEntry::create ( Palette* palette, const Name& name )
  {
    GroupPaletteEntry* entry = new GroupPaletteEntry ( palette, name );

    entry->_postCreate ();

    return entry;
  }


  void  GroupPaletteEntry::_postCreate ()
  {

    _index = _palette->getEntries().size();

    QHBoxLayout* layout = new QHBoxLayout ();
    layout->setContentsMargins ( 0, 0, 0, 0 );

    _button = new QPushButton ( this );
    _button->setFlat  ( true );
    _button->setText  ( getString(getName()).c_str() );
    _button->setFont  ( Graphics::getFixedFont(QFont::Bold,false,true) );

    layout->addWidget  ( _button );
    layout->addStretch ();

    setLayout ( layout );

    connect ( _button, SIGNAL(clicked()), this, SLOT(toggle()) );
  }


  bool GroupPaletteEntry::isGroup () const
  {
    return true;
  }


  bool GroupPaletteEntry::isBasicLayer () const
  {
    return false;
  }


  const Name& GroupPaletteEntry::getName () const
  {
    return _name;
  }


  BasicLayer* GroupPaletteEntry::getBasicLayer ()
  {
    return NULL;
  }


  bool  GroupPaletteEntry::isChecked () const
  {
    return _expanded;
  }


  void  GroupPaletteEntry::setChecked ( bool state )
  {
    _expanded = state;
    
    hideShow ();
  }


  void  GroupPaletteEntry::toggle ()
  {
    _expanded = !_expanded;

    hideShow ();
  }


  void  GroupPaletteEntry::hideShow ()
  {
    vector<PaletteEntry*>  entries = _palette->getEntries ();

    if ( entries[_index] != this )
      cerr << "[ERROR] Incoherent index for group \"" << getString(getName()) << "\"." << endl;

    _button->setText ( getLabel().c_str() );
                                                                  
    for ( size_t i=_index+1 ; i<entries.size() ; i++ ) {
      if ( entries[i]->isGroup() ) break;

      if ( _expanded ) entries[i]->show ();
      else             entries[i]->hide ();
    }
  }


  string  GroupPaletteEntry::getLabel ()
  {
    string  label = getString(getName());

    for ( size_t i=0 ; i<label.size() ; i++ ) {
      if ( label[i] == '&' ) {
        label.insert ( i, 1, '&' );
        i += 2;
      }
    }

    size_t  spacingLeft  = 0;
    size_t  spacingRight = 0;

    if ( label.size() < 15 ) {
      spacingLeft  = (15 - label.size()) / 2;
      spacingRight = spacingLeft + ( ((15 - label.size()) % 2) ? 1 : 0 );
    }

    if ( !_expanded )
      _button->setFont  ( Graphics::getFixedFont(QFont::Bold,false,true) );
    else
      _button->setFont  ( Graphics::getFixedFont(QFont::Bold,false,false) );

    label.insert ( 0, spacingLeft, ' ' );
    label.append ( spacingRight, ' ' );

    return label;
  }


} // End of Hurricane namespace.
