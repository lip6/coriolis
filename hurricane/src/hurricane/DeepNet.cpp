

// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
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
// $Id: DeepNet.cpp,v 1.4 2008/02/15 12:26:38 dosfin Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./DeepNet.cpp"                            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include "DeepNet.h"
# include "Cell.h"
# include "Instance.h"
# include "Plug.h"
# include "RoutingPad.h"
# include "Pin.h"
# include "Contact.h"
# include "Vertical.h"
# include "Horizontal.h"
# include "Pad.h"
# include "UpdateSession.h"
# include "Error.h"


namespace Hurricane {

namespace {


  using namespace std;
  using namespace Hurricane;




# if !defined(__DOXYGEN_PROCESSOR__)


# endif


} // End of local namespace.


// x-----------------------------------------------------------------x
// |                 "::DeepNet" Class Definitions                   |
// x-----------------------------------------------------------------x


#   if !defined(__DOXYGEN_PROCESSOR__)


// -------------------------------------------------------------------
// Constructor  :  "DeepNet::DeepNet ()".

DeepNet::DeepNet ( Occurrence& netOccurrence )
  : Net(netOccurrence.GetOwnerCell()
       ,netOccurrence.GetName()
       )
  , _netOccurrence(netOccurrence)
{
}




// -------------------------------------------------------------------
// Inspector Management  :  "DeepNet::_GetRecord ()".

Record* DeepNet::_GetRecord () const
{
  Record* record = Net::_GetRecord();
  if (record) {
		record->Add(GetSlot("_netOccurrence", &_netOccurrence));
  }
  return record;
}


# endif


// -------------------------------------------------------------------
// Constructor  :  "DeepNet::Create ()".

DeepNet* DeepNet::Create ( HyperNet& hyperNet )
{
  if ( !hyperNet.IsValid() )
    throw Error ( "Can't create " + _TName("DeepNet") + ": occurence is invalid." );

  Occurrence  rootNetOccurrence = GetHyperNetRootNetOccurrence ( hyperNet.GetNetOccurrence() );

  if (  rootNetOccurrence.GetMasterCell()->IsFlattenLeaf() ) return NULL;
  if (  rootNetOccurrence.GetPath().IsEmpty() )              return NULL;

  DeepNet* deepNet = new DeepNet ( rootNetOccurrence );
  deepNet->_PostCreate ();

  return deepNet;
}




// -------------------------------------------------------------------
// Internal Modifier  :  "DeepNet::_CreateRoutingPads ()".

size_t  DeepNet::_CreateRoutingPads ( bool buildRings )
{
  size_t  nbRoutingPads = 0;
  HyperNet  hyperNet ( _netOccurrence );

  RoutingPad* previousRP = NULL;
  RoutingPad* currentRP  = NULL;
  for_each_occurrence ( plugOccurrence, hyperNet.GetLeafPlugOccurrences() ) {
    nbRoutingPads++;

    currentRP = CreateRoutingPad ( this, plugOccurrence );
    if ( buildRings ) {
      if ( previousRP ) {
        currentRP->GetBodyHook()->Attach ( previousRP->GetBodyHook() );
      }
      previousRP = currentRP;
    }

    end_for
  }

  return nbRoutingPads;
}


// -------------------------------------------------------------------
// 

Net* GetDeepNet(HyperNet& hypernet)
// ********************************
{
    Occurrence  rootNetOccurrence = GetHyperNetRootNetOccurrence ( hypernet.GetNetOccurrence() );

  //if (  rootNetOccurrence.GetMasterCell()->IsFlattenLeaf() ) return NULL;
  //if (  rootNetOccurrence.GetPath().IsEmpty() )              return NULL;

    return  rootNetOccurrence.GetOwnerCell()->GetNet(rootNetOccurrence.GetName());

}


} // End of Hurricane namespace.
