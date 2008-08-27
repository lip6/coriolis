
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
// |  C++ Module  :       "./GroupPaletteEntry.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <QPushButton>
# include  <QHBoxLayout>

# include  "hurricane/viewer/Graphics.h"
# include  "hurricane/viewer/GroupPaletteEntry.h"
# include  "hurricane/viewer/HPalette.h"


namespace Hurricane {


  GroupPaletteEntry::GroupPaletteEntry ( HPalette* palette, const Name& name )
    : HPaletteEntry(palette)
    , _name(name)
    , _button(NULL)
    , _index(0)
    , _expanded(true)
  {
  }


  GroupPaletteEntry* GroupPaletteEntry::create ( HPalette* palette, const Name& name )
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
    vector<HPaletteEntry*>  entries = _palette->getEntries ();

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
