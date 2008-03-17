

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
// $Id: DeepNet.h,v 1.4 2008/02/15 12:26:38 dosfin Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./DeepNet.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# ifndef  __HURRICANE_DEEPNET__
#   define  __HURRICANE_DEEPNET__

# include  "Net.h"
# include  "HyperNet.h"
# include  "Occurrence.h"




namespace Hurricane {


  class DeepNet : public Net {

#   if !defined(__DOXYGEN_PROCESSOR__)

    // Attributes.
    protected:
      Occurrence  _netOccurrence;

    // Constructors.
    protected:
      DeepNet ( Occurrence& netOccurrence );

    // Inspector Management.
    public:
      virtual Record* _getRecord  () const;
      virtual string  _getTypeName() const { return "DeepNet"; };

# endif

    // Constructors.
    public:
      static  DeepNet* Create      ( HyperNet& hyperNet );

    // Accessors.
    public:

    // Predicates.
    public:
      virtual bool IsDeepNet () const { return true; };

    // Internal Modifiers.
    public:
      size_t  _CreateRoutingPads ( bool buildRings=false );

};

Net* getDeepNet(HyperNet& hyperNet);


} // End of Hurricane namespace.




# endif
