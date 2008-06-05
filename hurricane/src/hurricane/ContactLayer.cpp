

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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ContactLayer.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "hurricane/BasicLayer.h"
# include  "hurricane/ContactLayer.h"
# include  "hurricane/Technology.h"
# include  "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "ContactLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::ContactLayer".


  ContactLayer::ContactLayer ( Technology* technology
                             , const Name& name
                             , BasicLayer* metalLayer
                             , BasicLayer* cutLayer
                             , BasicLayer* activeLayer
                             , BasicLayer* diffusionLayer
                             , BasicLayer* wellLayer
                             ) : Layer(technology
                                      ,name
                                      ,0  // minimalSize.
                                      ,0) // minmalSpacing.
                               ,_basicLayers()
                               ,_enclosures()
                               ,_maximalEnclosure(0)
  {
    if ( !metalLayer     ) throw Error ( nullLayer, getString(name).c_str(), "Metal" );
    if ( !cutLayer       ) throw Error ( nullLayer, getString(name).c_str(), "Cut" );
    if ( !activeLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Active" );
    if ( !diffusionLayer ) throw Error ( nullLayer, getString(name).c_str(), "Diffusion" );

    _basicLayers.reserve ( 5 );
    _enclosures .reserve ( 5 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back ( metalLayer );
    _basicLayers.push_back ( cutLayer );
    _basicLayers.push_back ( activeLayer );
    _basicLayers.push_back ( diffusionLayer );

    for ( size_t i=0 ; i<4 ; i++ ) {
      _enclosures.push_back ( 0 );
    }

    _setMask        (  metalLayer    ->getMask()
                     | cutLayer      ->getMask()
                     | activeLayer   ->getMask()
                     | diffusionLayer->getMask() );
    _setExtractMask (  metalLayer    ->getExtractMask()
                     | cutLayer      ->getExtractMask()
                     | activeLayer   ->getExtractMask()
                     | diffusionLayer->getExtractMask() );

    if ( wellLayer ) {
      _basicLayers.push_back ( wellLayer );
      _enclosures .push_back ( 0 );

      _setMask        ( getMask() | wellLayer->getMask()        );
      _setExtractMask ( getMask() | wellLayer->getExtractMask() );
    }
  }


  ContactLayer* ContactLayer::create ( Technology* technology
                                     , const Name& name
                                     , BasicLayer* metalLayer
                                     , BasicLayer* cutLayer
                                     , BasicLayer* activeLayer
                                     , BasicLayer* diffusionLayer
                                     , BasicLayer* wellLayer
                                     )
  {
    ContactLayer* transistorLayer = new ContactLayer ( technology
                                                     , name
                                                     , metalLayer
                                                     , cutLayer
                                                     , activeLayer
                                                     , diffusionLayer
                                                     , wellLayer
                                                     );

    transistorLayer->_postCreate();

    return transistorLayer;
  }
 

  BasicLayers ContactLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  Unit  ContactLayer::getEnclosure () const
  { return _maximalEnclosure; }


  Unit  ContactLayer::getEnclosure ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer )
        return _enclosures[i];
    }

    return 0;
  }


  void  ContactLayer::setEnclosure ( const BasicLayer* layer, Unit enclosure )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( _basicLayers[i] == layer ) {
        _enclosures[i] = enclosure;
        _maximalEnclosure = max ( _maximalEnclosure, enclosure );
      }
    }
  }


  string  ContactLayer::_getTypeName () const
  { return _TName("ContactLayer"); }


  string ContactLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* ContactLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers", &_basicLayers));
      record->add(getSlot("_enclosures" , &_enclosures ));
    }
    return record;
  }


} // End of Hurricane namespace.
