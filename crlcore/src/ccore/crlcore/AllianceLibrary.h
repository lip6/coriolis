

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
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                       Rémy Escassut              |
// |  E-mail      :           Remy.Escassut@silvaco.com              |
// | =============================================================== |
// |  C++ Header  :       "./AllianceLibrary.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# ifndef  __CRL_ALLIANCE_LIBRARY_H__
#   define  __CRL_ALLIANCE_LIBRARY_H__

# include  <string>
# include  <vector>

# include  "hurricane/Name.h"
# include  "hurricane/Slot.h"

namespace Hurricane {
  class Library;
}


namespace CRL {


  using std::vector;

  using Hurricane::Name;
  using Hurricane::Library;
  using Hurricane::Record;
  using Hurricane::_TName;


// -------------------------------------------------------------------
// Class  :  "CRL::AllianceLibrary".

  class AllianceLibrary {

    public:
    // Constructors.
                              AllianceLibrary ();
                              AllianceLibrary ( const Name& path, Library* library=NULL );
    // Operators
             AllianceLibrary& operator=       ( const AllianceLibrary& directory );
             AllianceLibrary& operator=       ( const string& path );
    // Accessors
      inline const Name&      getPath         () const;
      inline Library*         getLibrary      () const;
    // Hurricane management.
      inline string           _getTypeName    () const;
             string           _getString      () const;
             Record*          _getRecord      () const;

    protected:
    // Internal - Attributes.
             Name             _path;
             Library*         _library;

  };


  typedef  vector<AllianceLibrary*>  AllianceLibraries;


// Inline Functions.
  inline const Name& AllianceLibrary::getPath      () const { return _path; }
  inline Library*    AllianceLibrary::getLibrary   () const { return _library; }
  inline string      AllianceLibrary::_getTypeName () const { return _TName("AllianceLibrary"); }


} // End of CRL namespace.


#endif
