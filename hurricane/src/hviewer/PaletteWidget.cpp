
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
// |  C++ Module  :       "./PaletteWidget.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QCheckBox>
#include  <QPushButton>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QGridLayout>

#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/BasicLayers.h"
#include  "hurricane/ExtensionSlice.h"
#include  "hurricane/Cell.h"

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/PaletteItem.h"
#include  "hurricane/viewer/PaletteNamedItem.h"
#include  "hurricane/viewer/PaletteLayerItem.h"
#include  "hurricane/viewer/PaletteExtensionGoItem.h"
#include  "hurricane/viewer/PaletteWidget.h"


namespace {


  using namespace std;
  using namespace Hurricane;


  class GridBuffer {
    public:
                     GridBuffer      ( QGridLayout*, int rowMax, int startRow=0, int startColumn=0 );
      inline int     getRow          () const;
      inline int     getColumn       () const;
      inline int     getCurrentRow   () const;
             void    addSection      ( QWidget*, Qt::Alignment align=0 );
             void    addWidget       ( QWidget*, Qt::Alignment align=0 );
             void    newColumn       ();
             void    flushWidgets    ();
      inline bool    columnOverload  () const;
    protected:
      QGridLayout*           _grid;
      int                    _rowMax;
      int                    _row;
      int                    _column;
      vector<QWidget*>       _widgets;
      vector<Qt::Alignment>  _aligns;
  };


  GridBuffer::GridBuffer ( QGridLayout* grid, int maxRow, int startRow, int startColumn )
    : _grid(grid)
    , _rowMax(maxRow)
    , _row(startRow)
    , _column(startColumn)
    , _widgets()
    , _aligns()
  { }


  inline int   GridBuffer::getRow          () const { return _row; }
  inline int   GridBuffer::getColumn       () const { return _column; }
  inline int   GridBuffer::getCurrentRow   () const { return _widgets.size() + _row; }
  inline bool  GridBuffer::columnOverload  () const { return getCurrentRow() > _rowMax; }


  void  GridBuffer::flushWidgets ()
  {
    if ( columnOverload() ) {
      _column++;
      _row = 0;
    }

    for ( size_t i=0 ; i < _widgets.size() ; ++i )
      _grid->addWidget ( _widgets[i], _row++, _column, _aligns[i] );
    _widgets.clear ();
    _aligns.clear ();
  }


  void  GridBuffer::newColumn ()
  {
    if ( columnOverload() ) return;

    flushWidgets ();
    _column++;
    _row = 0;
  }


  void  GridBuffer::addWidget ( QWidget* widget, Qt::Alignment align )
  {
    _widgets.push_back ( widget );
    _aligns .push_back ( align );
  }


  void  GridBuffer::addSection ( QWidget* widget, Qt::Alignment align )
  {
    flushWidgets ();
    _widgets.push_back ( widget );
    _aligns .push_back ( align );
  }
  

} // End of anonymous namespace.


namespace Hurricane {


  QWidget* PaletteWidget::_createGroupItem ( const Name& name )
  {
    QLabel* groupLabel = new QLabel ();
    groupLabel->setText ( getString(name).c_str() );
    groupLabel->setFont ( Graphics::getFixedFont(QFont::Bold,false,false) );
    return groupLabel;
  }


  PaletteNamedItem* PaletteWidget::_createNamedItem ( const Name& name, bool checked )
  {
    PaletteNamedItem* item = PaletteNamedItem::create ( name, checked );
    connect ( item, SIGNAL(visibleToggled()), this, SIGNAL(paletteChanged()) );
    return item;
  }


  PaletteLayerItem* PaletteWidget::_createLayerItem ( BasicLayer* layer, bool checked )
  {
    PaletteLayerItem* item = PaletteLayerItem::create ( layer, checked );
    connect ( item, SIGNAL(visibleToggled()), this, SIGNAL(paletteChanged()) );
    return item;
  }


  PaletteExtensionGoItem* PaletteWidget::_createExtensionGoItem ( const Name& name, bool checked )
  {
    PaletteExtensionGoItem* item = PaletteExtensionGoItem::create ( name );
    connect ( item, SIGNAL(visibleToggled()), this, SIGNAL(paletteChanged()) );
    return item;
  }


  PaletteWidget::PaletteWidget ( QWidget* parent ) : QScrollArea(parent)
                                                   , _layerItems()
                                                   , _extensionGoItems()
                                                   , _showAll(new QPushButton(this))
                                                   , _hideAll(new QPushButton(this))
                                                   , _grid(new QGridLayout())
                                                   , _extensionRow(0)
                                                   , _extensionColumn(0)
                                                   , _extensionGroup(NULL)
  {
    setWidgetResizable ( true );
    QVBoxLayout* vLayout = new QVBoxLayout ();
    QHBoxLayout* hLayout = new QHBoxLayout ();

    _showAll->setIcon ( QIcon(":/images/palette_show_all.png") );
    _showAll->setFlat ( true );

    _hideAll->setIcon ( QIcon(":/images/palette_hide_all.png") );
    _hideAll->setFlat ( true );

    connect ( _showAll, SIGNAL(clicked()), this, SLOT(showAll()) );
    connect ( _hideAll, SIGNAL(clicked()), this, SLOT(hideAll()) );

    hLayout->addStretch ();
    hLayout->addWidget ( _showAll );
    hLayout->addStretch ();
    hLayout->addWidget ( _hideAll );
    hLayout->addStretch ();

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    vLayout->setSpacing ( 0 );
    vLayout->addLayout ( hLayout );
    vLayout->addWidget ( separator );
    vLayout->addSpacing ( 5 );
    vLayout->addLayout ( _grid );

    GridBuffer gridBuffer ( _grid, 22 );
    _grid->setHorizontalSpacing ( 10 );
    _grid->setVerticalSpacing   ( 0 );
  //_grid->setSizeConstraint    ( QLayout::SetFixedSize );

    size_t                       gi     = 0;
    const vector<DrawingGroup*>& groups = Graphics::getStyle()->getDrawingGroups();
    const vector<DrawingStyle*>& styles = groups[gi]->getDrawingStyles();

    gridBuffer.addSection ( _createGroupItem(groups[gi]->getName()), Qt::AlignHCenter );
    for ( size_t si=0 ; si<styles.size() ; si++ ) {
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;
      if ( styles[si]->getName() == DisplayStyle::Background    ) continue;
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionDraw ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionFill ) continue;
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;

      PaletteNamedItem* item = _createNamedItem ( styles[si]->getName() );
      gridBuffer.addWidget ( item );
      _layerItems [ item->getName() ] = item;
    }
    gridBuffer.newColumn ();

    DataBase* database = DataBase::getDB();
    if ( database ) {
      Technology* technology = database->getTechnology();
      if ( technology ) {
        for ( gi=1 ; gi<groups.size() ; gi++ ) {
          gridBuffer.addSection ( _createGroupItem(groups[gi]->getName()), Qt::AlignHCenter ); 

          const vector<DrawingStyle*>& styles = groups[gi]->getDrawingStyles();
          for ( size_t si=0 ; si<styles.size() ; si++ ) {
            BasicLayer* basicLayer = technology->getBasicLayer ( styles[si]->getName() );

            if ( basicLayer ) {
              PaletteLayerItem* item = _createLayerItem ( basicLayer );
              gridBuffer.addWidget ( item );
              _layerItems [ item->getName() ] = item;
            }
          }
        }
        size_t si = 0;
        forEach ( BasicLayer*, basicLayer, technology->getBasicLayers() ) {
          if ( !find((*basicLayer)->getName()) ) {
            if ( !si )
              gridBuffer.addSection ( _createGroupItem("Unmatcheds"), Qt::AlignHCenter );

            PaletteLayerItem* item = _createLayerItem ( *basicLayer );
            gridBuffer.addWidget ( item );
            _layerItems [ item->getName() ] = item;

            cerr << "[WARNING] BasicLayer \"" << (*basicLayer)->getName()
                 << "\" has no associated DisplayStyle." << endl;

            si++;
          }
        }
        if ( si ) gi++;
      }
    }

    gridBuffer.newColumn ();
    _extensionRow    = gridBuffer.getRow();
    _extensionColumn = gridBuffer.getColumn();
    _extensionGroup  = _createGroupItem ( "Extensions" );
    gridBuffer.addSection ( _extensionGroup, Qt::AlignHCenter );
    gridBuffer.flushWidgets ();

    vLayout->addStretch ();

    QWidget* adaptator = new QWidget ();
    adaptator->setLayout ( vLayout );
    setWidget ( adaptator );
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
    setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
    setFrameStyle                ( QFrame::Plain );
  }


  void  PaletteWidget::updateExtensions ( Cell* cell )
  {
    _grid->removeWidget ( _extensionGroup );
    _extensionGroup->deleteLater ();
    _extensionGroup = NULL;

    PaletteItems::iterator  iextension = _extensionGoItems.begin();
    for ( ; iextension != _extensionGoItems.end() ; ++iextension ) {
      _grid->removeWidget ( iextension->second );
      iextension->second->deleteLater ();
    }
    _extensionGoItems.clear ();


    GridBuffer gridBuffer ( _grid, 22, _extensionRow, _extensionColumn );
    _extensionGroup  = _createGroupItem ( "Extensions" );
    gridBuffer.addSection ( _extensionGroup, Qt::AlignHCenter );

    if ( cell ) {
      forEach ( ExtensionSlice*, extension, cell->getExtensionSlices() ) {
        PaletteExtensionGoItem* item = _createExtensionGoItem ( (*extension)->getName(), false );
        gridBuffer.addWidget ( item );
        _extensionGoItems [ item->getName() ] = item;
      }
    }
    gridBuffer.flushWidgets ();
  }


  bool  PaletteWidget::isDrawable ( const Name& name ) const
  {
    PaletteItem* item = find ( name );
    if ( item )
      return item->isItemVisible ();

    return false;
  }


  bool  PaletteWidget::isSelectable ( const Name& name ) const
  {
    PaletteItem* item = find ( name );
    if ( item )
      return item->isItemSelectable ();

    return false;
  }


  void  PaletteWidget::showAll ()
  {
    PaletteItems::iterator iitem = _layerItems.begin();
    for ( ; iitem != _layerItems.end() ; iitem++ )
      iitem->second->setItemVisible ( true );

    iitem = _extensionGoItems.begin();
    for ( ; iitem != _extensionGoItems.end() ; iitem++ )
      iitem->second->setItemVisible ( true );

    emit paletteChanged();
  }


  void  PaletteWidget::hideAll ()
  {
    PaletteItems::iterator iitem = _layerItems.begin();
    for ( ; iitem != _layerItems.end() ; iitem++ )
      iitem->second->setItemVisible ( false );
       
    iitem = _extensionGoItems.begin();
    for ( ; iitem != _extensionGoItems.end() ; iitem++ )
      iitem->second->setItemVisible ( false );

    emit paletteChanged();
  }


  PaletteItem* PaletteWidget::find ( const Name& name ) const
  {
    PaletteItems::const_iterator iitem = _layerItems.find(name);
    if ( iitem != _layerItems.end() )
      return iitem->second;

    iitem = _extensionGoItems.find(name);
    if ( iitem != _extensionGoItems.end() )
      return iitem->second;

    return NULL;
  }


  void  PaletteWidget::styleChange ( void* emitter )
  {
    if ( emitter == this ) return;

    PaletteItems::iterator iitem = _layerItems.begin();
    for ( ; iitem != _layerItems.end() ; iitem++ )
      iitem->second->styleChange ();

    iitem = _extensionGoItems.begin();
    for ( ; iitem != _extensionGoItems.end() ; iitem++ )
      iitem->second->styleChange ();
  }


} // End of Hurricane namespace.
