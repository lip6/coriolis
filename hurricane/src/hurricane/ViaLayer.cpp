

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
// |  C++ Module  :       "./ViaLayer.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  "hurricane/BasicLayer.h"
# include  "hurricane/ViaLayer.h"
# include  "hurricane/Technology.h"
# include  "hurricane/Error.h"


namespace {


  const char* nullLayer =
    "ViaLayer::create() [%s]\n"
    "  %s layer argument mustn't be NULL.";

  const char* badCutLayer =
    "ViaLayer::create() [%s]\n"
    "  %s layer is not a cut layer.";


}


namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::ViaLayer".


  ViaLayer::ViaLayer ( Technology* technology
                     , const Name& name
                     , BasicLayer* bottomLayer
                     , BasicLayer* cutLayer
                     , BasicLayer* topLayer
                     ) : Layer(technology
                              ,name
                              ,0  // minimalSize.
                              ,0) // minmalSpacing.
                       ,_basicLayers()
                       ,_enclosures()
                       ,_maximalEnclosure(0)
  {
    if ( !bottomLayer ) throw Error ( nullLayer, getString(name).c_str(), "Bottom" );
    if ( !cutLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Cut" );
    if ( !topLayer    ) throw Error ( nullLayer, getString(name).c_str(), "Top" );

    if ( cutLayer->getMaterial() != BasicLayer::Material::cut )
      throw Error ( badCutLayer, getString(name).c_str()
                               , getString(cutLayer->getName()).c_str() );

    _basicLayers.reserve ( 3 );
    _enclosures .reserve ( 3 );

  // Have to check wich one is top layer & cutLayer of type cut.
    _basicLayers.push_back ( bottomLayer );
    _basicLayers.push_back ( cutLayer );
    _basicLayers.push_back ( topLayer );

    _setMask        (  bottomLayer->getMask()
                     | cutLayer   ->getMask()
                     | topLayer   ->getMask() );
    _setExtractMask (  bottomLayer->getExtractMask()
                     | cutLayer   ->getExtractMask()
                     | topLayer   ->getExtractMask() );

    _enclosures.push_back ( 0 );
    _enclosures.push_back ( 0 );
    _enclosures.push_back ( 0 );
  }


  ViaLayer* ViaLayer::create ( Technology* technology
                             , const Name& name
                             , BasicLayer* bottomLayer
                             , BasicLayer* cutLayer
                             , BasicLayer* topLayer
                             )
  {
    ViaLayer* viaLayer = new ViaLayer ( technology
                                      , name
                                      , bottomLayer
                                      , cutLayer
                                      , topLayer
                                      );

    viaLayer->_postCreate();

    return viaLayer;
  }
 

  BasicLayers ViaLayer::getBasicLayers () const
  { return getCollection(_basicLayers); }


  DbU::Unit  ViaLayer::getEnclosure () const
  {
    return _maximalEnclosure;
  }


  DbU::Unit  ViaLayer::getEnclosure ( const BasicLayer* layer ) const
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( layer == _basicLayers[i] ) return _enclosures[i];
    }

    return 0;
  }


  void  ViaLayer::setEnclosure ( const BasicLayer* layer, DbU::Unit enclosure )
  {
    for ( size_t i=0 ; i<_basicLayers.size() ; i++ ) {
      if ( layer == _basicLayers[i] ) {
        _enclosures[i]    = enclosure;
        _maximalEnclosure = max ( _maximalEnclosure, enclosure );
      }
    }
  }


  string  ViaLayer::_getTypeName () const
  { return _TName("ViaLayer"); }


  string ViaLayer::_getString () const
  {
    string s = Layer::_getString();
    return s;
  }


  Record* ViaLayer::_getRecord () const
  {
    Record* record = Layer::_getRecord();
    if (record) {
      record->add(getSlot("_basicLayers", &_basicLayers));
      record->add(getSlot("_enclosures" , &_enclosures ));
    }
    return record;
  }


} // End of Hurricane namespace.
