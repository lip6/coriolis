// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2018-2023, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/Text.h"                            |
// +-----------------------------------------------------------------+

#pragma  once
#include "hurricane/Component.h"


namespace Hurricane {

  class Layer;


// -------------------------------------------------------------------
// Class  :  "Text".

  class Text : public Go {
    public:
      typedef Go Super;
  
    public:
      static        Text*        create         ( Cell*, const Layer*, const Box&, std::string );
    // Accessors.                
      virtual       Cell*        getCell        () const;
      virtual       DbU::Unit    getX           () const;
      virtual       DbU::Unit    getY           () const;
      virtual       Box          getBoundingBox () const;
      virtual       Box          getBoundingBox ( const BasicLayer* ) const;
      inline        std::string  getText        () const;
    // Mutators.               
                    void         setLayer       ( const Layer* );
              const Layer*       getLayer       () const;
      virtual       void         translate      ( const DbU::Unit& dx, const DbU::Unit& dy );
      virtual       void         materialize    ();
      virtual       void         unmaterialize  ();
    // Hurricane management.     
      virtual       string       _getTypeName   () const;
      virtual       string       _getString     () const;
      virtual       Record*      _getRecord     () const;
    protected:                             
                                 Text           ( Cell*, const Layer*, const Box&, std::string );
    private:
            Cell*         _cell;
      const Layer*        _layer;
            Box           _boundingBox;
            std::string   _text;
      
  };


  inline std::string  Text::getText () const { return _text; }


} // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Text);
