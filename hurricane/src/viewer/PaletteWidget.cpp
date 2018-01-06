// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PaletteWidget.cpp"                      |
// +-----------------------------------------------------------------+


#include <limits>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/BasicLayers.h"
#include "hurricane/ExtensionSlice.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/PaletteItem.h"
#include "hurricane/viewer/PaletteNamedItem.h"
#include "hurricane/viewer/PaletteLayerItem.h"
#include "hurricane/viewer/PaletteExtensionGoItem.h"
#include "hurricane/viewer/PaletteWidget.h"


namespace {

  using namespace std;
  using namespace Hurricane;


  class GridBuffer {
    public:
                     GridBuffer     ( QGridLayout*, size_t rowMax, size_t startRow=0, size_t startColumn=0 );
                    ~GridBuffer     ();
      inline int     getRow         () const;
      inline int     getColumn      () const;
      inline int     getCurrentRow  () const;
             void    addSection     ( QWidget*, Qt::Alignment align=0 );
             void    addWidget      ( QWidget*, Qt::Alignment align=0 );
             void    newColumn      ();
             void    flushWidgets   ();
      inline bool    columnOverload () const;
    protected:
      QGridLayout*           _grid;
      size_t                 _rowMax;
      size_t                 _row;
      size_t                 _column;
      vector<QWidget*>       _widgets;
      vector<Qt::Alignment>  _aligns;
  };


  GridBuffer::GridBuffer ( QGridLayout* grid
                         , size_t maxRow
                         , size_t startRow
                         , size_t startColumn )
    : _grid   (grid)
    , _rowMax (maxRow)
    , _row    (startRow)
    , _column (startColumn)
    , _widgets()
    , _aligns ()
  {
    cdebug.log(19) << "GridBuffer::GridBuffer() column:" << _column << " row:" << _row << endl;
  }


  GridBuffer::~GridBuffer ()
  {
    cdebug.log(19) << "GridBuffer::~GridBuffer() column:" << _column << " row:" << _row << endl;
  }


  inline int   GridBuffer::getRow          () const { return _row; }
  inline int   GridBuffer::getColumn       () const { return _column; }
  inline int   GridBuffer::getCurrentRow   () const { return _widgets.size() + _row; }
  inline bool  GridBuffer::columnOverload  () const { return getCurrentRow() >= (int)_rowMax; }


  void  GridBuffer::flushWidgets ()
  {
    cdebug.log(19,1) << "GridBuffer::flushWidgets()" << endl;

    if ( columnOverload() ) {
      _column++;
      _row = 0;
    }

    for ( size_t i=0 ; i < _widgets.size() ; ++i ) {
      cdebug.log(19) << "add Widget to grid @(" << _column << "," << _row << ")" << endl;

      _grid->addWidget ( _widgets[i], _row++, _column, _aligns[i] );
    }
    _widgets.clear ();
    _aligns.clear ();

    cdebug.tabw(19,-1);
  }


  void  GridBuffer::newColumn ()
  {
    cdebug.log(19) << "GridBuffer::newColumn()" << endl;

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
  

} // Anonymous namespace.


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "PaletteWidget".

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


  PaletteWidget::PaletteWidget ( QWidget* parent )
    : QWidget          (parent)
    , _scrollArea      (new QScrollArea(this))
    , _layerItems      ()
    , _extensionGoItems()
    , _showAll         (new QPushButton(_scrollArea))
    , _hideAll         (new QPushButton(_scrollArea))
    , _grid            (new QGridLayout())
    , _columnHeight    (22)
    , _extensionRow    (0)
    , _extensionColumn (0)
    , _extensionGroup  (NULL)
  {
    setContentsMargins ( 0, 0, 0, 0 );
    _scrollArea->setWidgetResizable ( true );
    QVBoxLayout* vLayout = new QVBoxLayout ();
    QHBoxLayout* hLayout = new QHBoxLayout ();

    _showAll->setIcon ( QIcon(":/images/palette_show_all.png") );
    _showAll->setFlat ( true );

    _hideAll->setIcon ( QIcon(":/images/palette_hide_all.png") );
    _hideAll->setFlat ( true );

    connect ( _showAll, SIGNAL(clicked()), this, SLOT(showAll()) );
    connect ( _hideAll, SIGNAL(clicked()), this, SLOT(hideAll()) );

    hLayout->addStretch ();
    hLayout->addWidget  ( _showAll );
    hLayout->addStretch ();
    hLayout->addWidget  ( _hideAll );
    hLayout->addStretch ();

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    vLayout->setSpacing ( 0 );
    vLayout->addLayout  ( hLayout );
    vLayout->addWidget  ( separator );
    vLayout->addSpacing ( 5 );
    vLayout->addLayout  ( _grid );

    _grid->setHorizontalSpacing ( 10 );
    _grid->setVerticalSpacing   ( 0 );
  //_grid->setSizeConstraint    ( QLayout::SetFixedSize );

    vLayout->addStretch ();

    QWidget* scrollAdaptator = new QWidget ();
    scrollAdaptator->setLayout ( vLayout );
    _scrollArea->setContentsMargins           ( 0, 0, 0, 0 );
    _scrollArea->setWidget                    ( scrollAdaptator );
    _scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
    _scrollArea->setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
    _scrollArea->setFrameStyle                ( QFrame::Plain );

    vLayout = new QVBoxLayout ();
    vLayout->setSpacing ( 0 );
    vLayout->addWidget  ( _scrollArea );

    setLayout ( vLayout );

    readGraphics ();
  }


  void  PaletteWidget::readGraphics ()
  {
    GridBuffer gridBuffer ( _grid, _columnHeight );

    size_t                       gi     = 0;
    const vector<DrawingGroup*>& groups = Graphics::getStyle()->getDrawingGroups();
    const DrawingStyles&         styles = groups[gi]->getDrawingStyles();

    gridBuffer.addSection ( _createGroupItem(groups[gi]->getName()), Qt::AlignHCenter );
    for ( size_t si=0 ; si<styles.size() ; si++ ) {
      if ( not styles[si]->isGoMatched() ) continue;
      if ( styles[si]->getName() == DisplayStyle::Foreground    ) continue;
      if ( styles[si]->getName() == DisplayStyle::Background    ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionDraw ) continue;
      if ( styles[si]->getName() == DisplayStyle::SelectionFill ) continue;

      PaletteNamedItem* item = _createNamedItem ( styles[si]->getName() );
      gridBuffer.addWidget ( item );
      _layerItems [ item->getName() ] = item;
    }
    if ( _columnHeight < numeric_limits<size_t>::max() )
      gridBuffer.newColumn ();
    else
      gridBuffer.flushWidgets ();

    DataBase* database = DataBase::getDB();
    if ( database ) {
      Technology* technology = database->getTechnology();
      if ( technology ) {
        for ( gi=1 ; gi<groups.size() ; gi++ ) {
          gridBuffer.addSection ( _createGroupItem(groups[gi]->getName()), Qt::AlignHCenter ); 

          const DrawingStyles& styles = groups[gi]->getDrawingStyles();
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

    gridBuffer.flushWidgets ();

    _extensionRow    = gridBuffer.getRow();
    _extensionColumn = gridBuffer.getColumn();
    _extensionGroup  = _createGroupItem ( "Extensions" );
    gridBuffer.addSection ( _extensionGroup, Qt::AlignHCenter );
    gridBuffer.flushWidgets ();

    cdebug.log(19) << " column:" << _extensionColumn << " row:" << _extensionRow << endl;
  }


  void  PaletteWidget::updateExtensions ( Cell* cell )
  {
    cdebug.log(19,1) << "PaletteWidget::updateExtensions()" << endl;

    if (_extensionGroup) {
      _grid->removeWidget ( _extensionGroup );
    //_extensionGroup->deleteLater ();
      delete _extensionGroup;
      _extensionGroup = NULL;

      PaletteItems::iterator  iextension = _extensionGoItems.begin();
      for ( ; iextension != _extensionGoItems.end() ; ++iextension ) {
        _grid->removeWidget ( iextension->second );
      //iextension->second->deleteLater ();
        delete iextension->second;
      }
      _extensionGoItems.clear ();
    }

    cdebug.log(19) << " column:" << _extensionColumn << " row:" << _extensionRow << endl;

    GridBuffer gridBuffer ( _grid, _columnHeight, _extensionRow, _extensionColumn );
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

    cdebug.tabw(19,-1);
  }


  void  PaletteWidget::resetGraphics ()
  {
    QLayoutItem* child = NULL;
    while ( (child = _grid->takeAt(0)) != 0 ) {
      _grid->removeWidget( child->widget() );
      delete child->widget();
    }

    _layerItems.clear();
    _extensionGroup = NULL;
    _extensionGoItems.clear();

  }


  void  PaletteWidget::rereadGraphics ()
  {
    resetGraphics();
    readGraphics();
  }


  void  PaletteWidget::_getSection ( const QString section, QLabel*& label, vector<PaletteItem*>& items ) const
  {
    label = NULL;
    items.clear ();

    bool found = false;
    for ( int index=0 ; index < _grid->count() ; index++ ) {
      QLayoutItem* item = _grid->itemAt ( index );
      if ( !item ) continue;

      QLabel* ilabel = dynamic_cast<QLabel*>(item->widget());
      if ( ilabel ) {
        if ( found ) break;
        if ( ilabel->text() != section ) continue;
        found = true;

        label = ilabel;
      } else if ( found ) {
        PaletteItem* paletteItem = dynamic_cast<PaletteItem*>(item->widget());
        if ( paletteItem ) items.push_back ( paletteItem );
      }
    }
  }


  void  PaletteWidget::setSectionVisible ( const QString section, bool visible )
  {
    QLabel* label;
    vector<PaletteItem*> items;

    _getSection ( section, label, items );
    if ( !label ) return;

    if ( visible ) label->show ();
    else           label->hide ();

    for ( size_t i=0 ; i<items.size() ; ++i ) {
      if ( visible ) items[i]->show ();
      else           items[i]->hide ();
    }
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


  void  PaletteWidget::showSection ( const QString section )
  {
    QLabel* label;
    vector<PaletteItem*> items;

    _getSection ( section, label, items );
    if ( !label ) return;

    for ( size_t i=0 ; i<items.size() ; ++i )
      items[i]->setItemVisible ( true );
  }


  void  PaletteWidget::hideSection ( const QString section )
  {
    QLabel* label;
    vector<PaletteItem*> items;

    _getSection ( section, label, items );
    if ( !label ) return;

    for ( size_t i=0 ; i<items.size() ; ++i )
      items[i]->setItemVisible ( false );
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


  void  PaletteWidget::setItemVisible ( const Name& name, bool visible )
  {
    PaletteItem* item = find ( name );
    if ( item ) {
      item->setItemVisible ( visible );
      emit paletteChanged();
    }
  }


  void  PaletteWidget::changeStyle ()
  {
    PaletteItems::iterator iitem = _layerItems.begin();
    for ( ; iitem != _layerItems.end() ; iitem++ )
      iitem->second->changeStyle ();

    iitem = _extensionGoItems.begin();
    for ( ; iitem != _extensionGoItems.end() ; iitem++ )
      iitem->second->changeStyle ();
  }


} // Hurricane namespace.
