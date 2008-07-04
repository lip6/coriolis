
// -*- C++ -*-


# include  <vector>

# include  <QHBoxLayout>
# include  <QVBoxLayout>
# include  <QCheckBox>
# include  <QPushButton>

# include  "hurricane/Name.h"
# include  "hurricane/DataBase.h"
# include  "hurricane/Technology.h"
# include  "hurricane/BasicLayer.h"
# include  "hurricane/BasicLayers.h"

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/HPaletteEntry.h"
# include  "hurricane/viewer/LayerPaletteEntry.h"
# include  "hurricane/viewer/GroupPaletteEntry.h"
# include  "hurricane/viewer/ViewerPaletteEntry.h"
# include  "hurricane/viewer/HPalette.h"
# include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {



  HPalette::HPalette ( QWidget* parent ) : QScrollArea(parent)
                                       , _entries()
                                       , _showAll(NULL)
                                       , _hideAll(NULL)
  {
    setWidgetResizable ( true );

    QWidget*     adaptator = new QWidget ();
    QVBoxLayout* layout    = new QVBoxLayout ();

  //layout->setContentsMargins ( 0, 0, 0, 0 );

    vector<DrawingGroup*>  groups = Graphics::getStyle()->getDrawingGroups();
    vector<DrawingStyle*>  styles = groups[0]->getDrawingStyles();

    GroupPaletteEntry*     gentry = GroupPaletteEntry::create ( this, groups[0]->getName() );
    layout->addWidget  ( gentry, 0, Qt::AlignLeft );
    layout->addSpacing ( -5 );
    _entries.push_back ( gentry );

    for ( size_t si=0 ; si<styles.size() ; si++ ) {
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;
      if ( styles[si]->getName() == DisplayStyle::Background    ) continue;
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionDraw ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionFill ) continue;
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;

      ViewerPaletteEntry* entry = ViewerPaletteEntry::create ( this, styles[si]->getName() );
      layout->addWidget  ( entry, 0, Qt::AlignLeft );
      _entries.push_back ( entry );
    }
    gentry->setChecked ( false );

    DataBase* database = DataBase::getDB();
    if ( database ) {
      Technology* technology = database->getTechnology();
      if ( technology ) {
        for ( size_t gi=1 ; gi<groups.size() ; gi++ ) {
          gentry = GroupPaletteEntry::create ( this, groups[gi]->getName() );
          layout->addWidget  ( gentry, 0, Qt::AlignLeft );
          layout->addSpacing ( -5 );
          _entries.push_back ( gentry );

          styles = groups[gi]->getDrawingStyles();
          for ( size_t si=0 ; si<styles.size() ; si++ ) {
            BasicLayer* basicLayer = technology->getBasicLayer ( styles[si]->getName() );

            if ( basicLayer ) {
              LayerPaletteEntry* entry = LayerPaletteEntry::create ( this, basicLayer );
              layout->addWidget  ( entry, 0, Qt::AlignLeft );
              _entries.push_back ( entry );
            }
          }
          gentry->setChecked ( false );
        }
        bool unmatched = false;
        for_each_basic_layer ( basicLayer, technology->getBasicLayers() ) {
          if ( !find(basicLayer->getName()) ) {
            if ( !unmatched ) {
              unmatched = true;
              gentry    = GroupPaletteEntry::create ( this, "Unmatcheds" );
              layout->addWidget  ( gentry, 0, Qt::AlignLeft );
              layout->addSpacing ( -5 );
              _entries.push_back ( gentry );
            }
            LayerPaletteEntry* entry = LayerPaletteEntry::create ( this, basicLayer );
            layout->addWidget  ( entry, 0, Qt::AlignLeft );
            _entries.push_back ( entry );

            cerr << "[WARNING] BasicLayer \"" << basicLayer->getName()
                 << "\" has no associated DisplayStyle." << endl;
          }
          end_for;
        }
        if ( unmatched )
          gentry->setChecked ( false );
      }
    }

    _showAll = new QPushButton ( this );
    _showAll->setIcon ( QIcon(":/images/palette_show_all.png") );
    _showAll->setFlat ( true );

    _hideAll = new QPushButton ( this );
    _hideAll->setIcon ( QIcon(":/images/palette_hide_all.png") );
    _hideAll->setFlat ( true );

    connect ( _showAll, SIGNAL(clicked()), this, SLOT(showAll()) );
    connect ( _hideAll, SIGNAL(clicked()), this, SLOT(hideAll()) );

    QHBoxLayout* bottomEntry = new QHBoxLayout ();
    bottomEntry->setContentsMargins ( 0, 0, 0, 0 );
    bottomEntry->addWidget ( _showAll );
    bottomEntry->addWidget ( _hideAll );
    layout->addLayout ( bottomEntry );
    layout->addStretch ();

    adaptator->setLayout ( layout );
    setWidget ( adaptator );
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
    setFrameStyle                ( QFrame::Plain );
  }


  bool  HPalette::isDrawable ( size_t index )
  {
    if ( index < _entries.size() )
      return _entries[index]->isChecked ();

    return false;
  }


  void  HPalette::showAll ()
  {
    for ( size_t i=0 ; i<_entries.size() ; i++ )
      if ( !_entries[i]->isGroup() )
        _entries[i]->setChecked ( true );

    emit paletteChanged();
  }


  void  HPalette::hideAll ()
  {
    for ( size_t i=0 ; i<_entries.size() ; i++ )
      if ( !_entries[i]->isGroup() )
        _entries[i]->setChecked ( false );

    emit paletteChanged();
  }


  void  HPalette::redrawCellWidget ()
  {
    emit paletteChanged();
  }


  HPaletteEntry* HPalette::find ( const Name& name )
  {
    for ( size_t i=0 ; i<_entries.size() ; i++ ) {
      if ( _entries[i]->getName() == name )
        return _entries[i];
    }
    return NULL;
  }


} // End of Hurricane namespace.
