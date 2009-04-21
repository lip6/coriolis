
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Header  :       "./PaletteWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_PALETTE_WIDGET__
#define  __HURRICANE_PALETTE_WIDGET__

#include  <map>
#include  <limits>
#include  <QScrollArea>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"


class QLabel;
class QCheckBox;
class QPushButton;
class QGridLayout;


namespace Hurricane {


  using std::map;

  class BasicLayer;
  class Cell;
  class CellWidget;
  class PaletteItem;
  class PaletteNamedItem;
  class PaletteLayerItem;
  class PaletteExtensionGoItem;


  class PaletteWidget : public QScrollArea {
      Q_OBJECT;

    public:
      typedef  map<const Name,PaletteItem*>  PaletteItems;
    public:
                               PaletteWidget     ( QWidget* parent=NULL );
             PaletteItem*      find              ( const Name& name ) const;
             bool              isDrawable        ( const Name& name ) const;
             bool              isSelectable      ( const Name& name ) const;
      inline void              setOneColumn      ();
      inline void              setColumnHeight   ( size_t height=std::numeric_limits<size_t>::max() );
             void              build             ();
    signals:                                     
             void              paletteChanged    ();
    public slots:              
             void              updateExtensions  ( Cell* cell );
             void              showAll           ();
             void              hideAll           ();
             void              hideSection       ( const QString );
             void              showSection       ( const QString );
             void              setSectionVisible ( const QString, bool visible );
             void              changeStyle       ();
             void              setItemVisible    ( const Name& name, bool visible );
                             
    protected:               
             PaletteItems     _layerItems;
             PaletteItems     _extensionGoItems;
             QPushButton*     _showAll;
             QPushButton*     _hideAll;
             QGridLayout*     _grid;
             size_t           _columnHeight;
             int              _extensionRow;
             int              _extensionColumn;
             QWidget*         _extensionGroup;
                             
    private:               
                                     PaletteWidget          ( const PaletteWidget& );
             PaletteWidget&          operator=              ( const PaletteWidget& );
    private:                                              
             QWidget*                _createGroupItem       ( const Name& );
             PaletteNamedItem*       _createNamedItem       ( const Name&, bool checked=true );
             PaletteLayerItem*       _createLayerItem       ( BasicLayer*, bool checked=true );
             PaletteExtensionGoItem* _createExtensionGoItem ( const Name&, bool checked=true );
             void                    _getSection            ( const QString, QLabel*&, vector<PaletteItem*>& ) const;
  };


// Inline Functions.
  inline void  PaletteWidget::setOneColumn    () { setColumnHeight(std::numeric_limits<size_t>::max()); }
  inline void  PaletteWidget::setColumnHeight ( size_t height ) { _columnHeight = height; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_WIDGET__
