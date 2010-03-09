
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
// |  C++ Header  :       "./AllianceLibrary.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "hurricane/Library.h"

# include  "crlcore/Utilities.h"
# include  "crlcore/AllianceLibrary.h"




namespace CRL {


// -------------------------------------------------------------------
// Class  :  "AllianceDirectory".

  AllianceLibrary::AllianceLibrary ()
    : _path("")
    , _library(NULL)
  {
  }


  AllianceLibrary::AllianceLibrary ( const Name& path, Library* library )
    : _path(path)
    , _library(library)
  {
    if ( _path.isEmpty() )
      throw ( Error("Can't create "+_TName("AllianceLibrary")+" : empty path") );
  }


  AllianceLibrary& AllianceLibrary::operator= ( const AllianceLibrary& d )
  {
    _path    = d._path;
    _library = d._library;
    return *this;
  }


  AllianceLibrary& AllianceLibrary::operator= ( const string& path )
  {
    _path    = path;
    _library = NULL;
    return *this;
  }


  string  AllianceLibrary::_getString () const
  {
    return ( "<" + _TName("AllianceLibrary") + " " + getString(_library->getName()) + ">" );
  }


  Record* AllianceLibrary::_getRecord() const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot("Path"   ,&_path   ) );
    record->add ( getSlot("Library", _library) );
    return ( record );
  }




} // End of CRL namespace.
