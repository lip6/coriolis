
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
#include  <QScrollArea>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"


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
                               PaletteWidget    ( QWidget* parent=NULL );
             PaletteItem*      find             ( const Name& name ) const;
             bool              isDrawable       ( const Name& name ) const;
             bool              isSelectable     ( const Name& name ) const;
    signals:                   
             void              paletteChanged   ();
    public slots:              
             void              updateExtensions ( Cell* cell );
             void              showAll          ();
             void              hideAll          ();
             void              changeStyle      ();
                             
    protected:               
             PaletteItems     _layerItems;
             PaletteItems     _extensionGoItems;
             QPushButton*     _showAll;
             QPushButton*     _hideAll;
             QGridLayout*     _grid;
             int              _extensionRow;
             int              _extensionColumn;
             QWidget*         _extensionGroup;
                             
    protected:               
                                     PaletteWidget          ( const PaletteWidget& );
             PaletteWidget&          operator=              ( const PaletteWidget& );
    protected:                                              
             QWidget*                _createGroupItem       ( const Name& );
             PaletteNamedItem*       _createNamedItem       ( const Name&, bool checked=true );
             PaletteLayerItem*       _createLayerItem       ( BasicLayer*, bool checked=true );
             PaletteExtensionGoItem* _createExtensionGoItem ( const Name&, bool checked=true );
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_PALETTE_WIDGET__
