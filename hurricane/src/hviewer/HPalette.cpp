
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
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
// |  C++ Module  :       "./HPalette.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <vector>

#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QCheckBox>
#include  <QPushButton>

#include  "hurricane/Name.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/BasicLayers.h"
#include  "hurricane/ExtensionSlice.h"
#include  "hurricane/Cell.h"

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/HPaletteEntry.h"
#include  "hurricane/viewer/LayerPaletteEntry.h"
#include  "hurricane/viewer/HExtensionPaletteEntry.h"
#include  "hurricane/viewer/GroupPaletteEntry.h"
#include  "hurricane/viewer/ViewerPaletteEntry.h"
#include  "hurricane/viewer/HPalette.h"


namespace Hurricane {



  HPalette::HPalette ( QWidget* parent ) : QScrollArea(parent)
                                         , _entries()
                                         , _showAll(NULL)
                                         , _hideAll(NULL)
  {
    setWidgetResizable ( true );

    QWidget*     adaptator = new QWidget ();
    QVBoxLayout* layout    = new QVBoxLayout ();

    _showAll = new QPushButton ( this );
    _showAll->setIcon ( QIcon(":/images/palette_show_all.png") );
    _showAll->setFlat ( true );

    _hideAll = new QPushButton ( this );
    _hideAll->setIcon ( QIcon(":/images/palette_hide_all.png") );
    _hideAll->setFlat ( true );

    connect ( _showAll, SIGNAL(clicked()), this, SLOT(showAll()) );
    connect ( _hideAll, SIGNAL(clicked()), this, SLOT(hideAll()) );

    QHBoxLayout* topEntry = new QHBoxLayout ();
    topEntry->setContentsMargins ( 0, 0, 0, 0 );
    topEntry->addWidget ( _showAll );
    topEntry->addWidget ( _hideAll );
    layout->addLayout ( topEntry );

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
        forEach ( BasicLayer*, basicLayer, technology->getBasicLayers() ) {
          if ( !find((*basicLayer)->getName()) ) {
            if ( !unmatched ) {
              unmatched = true;
              gentry    = GroupPaletteEntry::create ( this, "Unmatcheds" );
              layout->addWidget  ( gentry, 0, Qt::AlignLeft );
              layout->addSpacing ( -5 );
              _entries.push_back ( gentry );
            }
            LayerPaletteEntry* entry = LayerPaletteEntry::create ( this, *basicLayer );
            layout->addWidget  ( entry, 0, Qt::AlignLeft );
            _entries.push_back ( entry );

            cerr << "[WARNING] BasicLayer \"" << (*basicLayer)->getName()
                 << "\" has no associated DisplayStyle." << endl;
          }
        }
        if ( unmatched )
          gentry->setChecked ( false );

      }
    }
    layout->addStretch ();

    adaptator->setLayout ( layout );
    setWidget ( adaptator );
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
    setFrameStyle                ( QFrame::Plain );
  }


  void  HPalette::updateExtensions ( Cell* cell )
  {
    QVBoxLayout* layout        = qobject_cast<QVBoxLayout*> ( widget()->layout() );
    const Name   extensionName = "Extensions";

    for ( size_t ientry=0 ; ientry<_entries.size() ; ) {
      if (  ( _entries[ientry]->getName() == extensionName )
           || _entries[ientry]->isExtension() ) {
        layout->removeWidget ( _entries[ientry] );
        _entries[ientry]->deleteLater ();

        for ( size_t j=ientry+1 ; j<_entries.size() ; j++ )
          _entries[j-1] = _entries[j];
        _entries.pop_back ();
      } else
        ientry++;
    }

    if ( !cell ) return;

    GroupPaletteEntry* gentry = GroupPaletteEntry::create ( this, extensionName );
    layout->insertWidget  ( -1, gentry, 0, Qt::AlignLeft );
    layout->addSpacing ( -5 );
    _entries.push_back ( gentry );

    forEach ( ExtensionSlice*, extension, cell->getExtensionSlices() ) {
      HExtensionPaletteEntry* entry = HExtensionPaletteEntry::create ( this, (*extension)->getName() );
      layout->insertWidget  ( -1, entry, 0, Qt::AlignLeft );
      _entries.push_back ( entry );
    }
    gentry->setChecked ( false );
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
