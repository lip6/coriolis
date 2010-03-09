
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
// $Id: CLayerConnexity.h,v 1.7 2007/07/29 15:27:22 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |               C o r e   L i b r a r y                           |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./CLayerConnexity.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# ifndef  __CRL_LAYER_CONNEXITY_H__
#   define  __CRL_LAYER_CONNEXITY_H__


# include  <map>

# include  "hurricane/Layer.h"
# include  "hurricane/BasicLayer.h"
# include  "hurricane/Contact.h"

# include  "hurricane/Utilities.h"
# include  "hurricane/CXML.h"
# include  "hurricane/Slot.h"



namespace CRL {


// -------------------------------------------------------------------
// Forward Declarations.

  class CDataBase;



// -------------------------------------------------------------------
// Class  :  "Eolienne::CLayerConnexity".
 
  class CLayerConnexity {

    // Types.
    protected:
      typedef  vector<Layer*>           LayerTable;
      typedef  map<Layer::Mask,Layer*>  MaskLayerMap;
      typedef  map<Layer*,Layer*>       LayerMap;

    // Attributes.
    protected:
      Layer::Mask   _cutMask;
      LayerTable    _layerTable;
      MaskLayerMap  _contactMap;
      MaskLayerMap  _oppositeMap;
      MaskLayerMap  _contactTopMap;
      MaskLayerMap  _contactBottomMap;
      LayerMap      _topMap;
      LayerMap      _bottomMap;
      LayerMap      _connectorMap;

    // Constructors.
    protected:
      CLayerConnexity   ();
      void  _postCreate ();
    private:
      CLayerConnexity            ( const CLayerConnexity& );
      CLayerConnexity& operator= ( const CLayerConnexity& );

    // Destructors.
    protected:
      virtual ~CLayerConnexity ();
      virtual void  _preDestroy ();
    public:
      virtual void destroy();

    // Accessors.
    public:
      Layer*    getContactLayer       ( Layer* layer1, Layer* layer2 ) const;
      Layer*    getContactTopLayer    ( Layer* layer ) const;
      Layer*    getContactBottomLayer ( Layer* layer ) const;
      Layer*    getOppositeLayer      ( Contact* contact, Layer* layer ) const;
      Layer*    getTopLayer           ( Layer* layer ) const;
      Layer*    getBottomLayer        ( Layer* layer ) const;
      bool      IsOnTop               ( Layer* layer1, Layer* layer2 ) const;
      bool      IsConnectorLayer      ( Layer* layer ) const;
      Layer*    getNonConnectorLayer  ( Layer* layer ) const;
      Layer*    getLayer              ( unsigned index ) const;
      unsigned  getIndex              ( Layer* layer ) const;
      unsigned  getLayerTableSize     () const { return _layerTable.size(); };

    // Modifiers.
    public:
      void  AddContact ( Layer* bottomLayer, Layer* topLayer, Layer* contact );
      void  SortLayers ();

    // XML related constructors.
    public:
      static void  createFromXml ( CLayerConnexity* lc, CXmlNode& tree );

    // Hurricane Managment.
    public:
      Record* _getRecord  () const;
      string  _getString  () const;
      string  _getTypeName() const { return ( "CRL::CLayerConnexity" ); };

    // Friends.
    friend class CDataBase;
  };




// x-----------------------------------------------------------------x
// |                    Functions Declarations                       |
// x-----------------------------------------------------------------x


  // Global CLayerConnexity access.
  CLayerConnexity* getCLayerConnexity ();

         unsigned  getLayerTableSize     ();
         unsigned  getIndex              ( Layer* layer );
         Layer*    getLayer              ( unsigned index );
         Layer*    getContactLayer       ( Layer* layer1, Layer* layer2 );
  inline Layer*    getContactLayer       ( unsigned index1, unsigned index2 )
                                         { return getContactLayer(getLayer(index1),getLayer(index2)); };
         Layer*    getContactTopLayer    ( Layer* layer );
         Layer*    getContactBottomLayer ( Layer* layer );
         Layer*    getOppositeLayer      ( Contact* contact, Layer* layer );
         Layer*    getTopLayer           ( Layer* layer );
         Layer*    getBottomLayer        ( Layer* layer );
         bool      IsOnTop               ( Layer* layer1, Layer* layer2 );
         bool      IsConnectorLayer      ( Layer* layer );
         Layer*    getNonConnectorLayer  ( Layer* layer );




} // End of namespace CRL.




// x-----------------------------------------------------------------x
// |           Functions Overload for Hurricane Managment            |
// x-----------------------------------------------------------------x


# endif
