// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Signature.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/Path.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Contact.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Pad.h"
#include "hurricane/Signature.h"


namespace Hurricane {

  using  std::ostringstream;


// -------------------------------------------------------------------
// Class  :  "Signature".

  void  Signature::setLayer ( const std::string& layer )
  {
    _layer = DataBase::getDB()->getTechnology()->getLayer( layer );
  }


// -------------------------------------------------------------------
// Class  :  "JsonSignature".

  JsonSignature::JsonSignature ( unsigned long flags )
    : JsonObject  (flags)
    , _subTypeName("unset")
  { }

  string  JsonSignature::getTypeName () const
  { return "Signature"; }


  void  JsonSignature::setSubType ( const std::string& subTypeName )
  {
    clear();
    _subTypeName = subTypeName;

    if        (_subTypeName == "Instance") {
      add( "_instance" , typeid(string) );
    } else if (_subTypeName == "Net") {
      add( "_name", typeid(string) );
    } else if (_subTypeName == "Plug") {
      add( "_masterNet", typeid(string) );
    } else if (_subTypeName == "Contact") {
      add( "_net", typeid(string)  );
      JsonContact jobject (0);
      copyAttrs( &jobject );
    } else if (_subTypeName == "Vertical") {
      add( "_net", typeid(string)  );
      JsonVertical jobject (0);
      copyAttrs( &jobject );
    } else if (_subTypeName == "Horizontal") {
      add( "_net", typeid(string)  );
      JsonHorizontal jobject (0);
      copyAttrs( &jobject );
    }
  }

  JsonSignature* JsonSignature::clone ( unsigned long flags ) const
  { return new JsonSignature ( flags ); }


  void JsonSignature::toData ( JsonStack& stack )
  {
    ltrace(51) << (void*)this << " _subTypeName:" << _subTypeName << endl;

    check( stack, "JsonSignature::toData" );

    Cell*   ownerCell  = get<Cell*> ( stack, ".Cell" );
    string  pathName   = get<string>( stack, "_path" );
    Path    path       ( ownerCell, pathName );
    Cell*   masterCell = path.getMasterCell();
    Entity* entity     = NULL;
    
    if (_subTypeName == "Instance") {
      entity = masterCell->getInstance( get<string>(stack,"_name") );
    } else if (_subTypeName == "Net") {
      entity = masterCell->getNet( get<string>(stack,"_name") );
    } else if (_subTypeName == "Plug") {
      Net* masterNet = masterCell->getNet( get<string>(stack,"_masterNet") );
      entity = path.getTailInstance()->getPlug( masterNet );
    } else if (_subTypeName == "Contact") {
      Signature signature;
      signature.setType ( Signature::TypeContact );
      signature.setName ( get<string>(stack,"_net"   ) );
      signature.setLayer( get<string>(stack,"_layer" ) );
      signature.setDim( Signature::ContactDx    , get<int64_t>(stack,"_dx"    ) );
      signature.setDim( Signature::ContactDy    , get<int64_t>(stack,"_dy"    ) );
      signature.setDim( Signature::ContactWidth , get<int64_t>(stack,"_width" ) );
      signature.setDim( Signature::ContactHeight, get<int64_t>(stack,"_height") );
      entity = masterCell->getEntity( signature );
    } else if (_subTypeName == "Vertical") {
      Signature signature;
      signature.setType ( Signature::TypeVertical );
      signature.setName ( get<string>(stack,"_net"  ) );
      signature.setLayer( get<string>(stack,"_layer") );
      signature.setDim( Signature::VerticalWidth   , get<int64_t>(stack,"_width"   ) );
      signature.setDim( Signature::VerticalX       , get<int64_t>(stack,"_x"       ) );
      signature.setDim( Signature::VerticalDySource, get<int64_t>(stack,"_dySource") );
      signature.setDim( Signature::VerticalDyTarget, get<int64_t>(stack,"_dyTarget") );
      entity = masterCell->getEntity( signature );
    } else if (_subTypeName == "Horizontal") {
      Signature signature;
      signature.setType ( Signature::TypeHorizontal );
      signature.setName ( get<string>(stack,"_net"  ) );
      signature.setLayer( get<string>(stack,"_layer") );
      signature.setDim( Signature::HorizontalWidth   , get<int64_t>(stack,"_width"   ) );
      signature.setDim( Signature::HorizontalY       , get<int64_t>(stack,"_y"       ) );
      signature.setDim( Signature::HorizontalDxSource, get<int64_t>(stack,"_dxSource") );
      signature.setDim( Signature::HorizontalDxTarget, get<int64_t>(stack,"_dxTarget") );
      entity = masterCell->getEntity( signature );
    } else if (_subTypeName == "Pad") {
      Signature signature;
      signature.setType ( Signature::TypePad );
      signature.setName ( get<string>(stack,"_net"  ) );
      signature.setLayer( get<string>(stack,"_layer") );
      Box bb = get<Box>(stack, "_boundingBox" );
      signature.setDim( Signature::PadXMin, bb.getXMin() );
      signature.setDim( Signature::PadYMin, bb.getYMin() );
      signature.setDim( Signature::PadXMax, bb.getXMax() );
      signature.setDim( Signature::PadYMax, bb.getYMax() );
      entity = masterCell->getEntity( signature );
    }
    
    update( stack, entity );
  }


}   // End of Hurricane namespace.
