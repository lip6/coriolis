
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
// |  C++ Header  :       "./HPaletteEntry.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


# ifndef  __PALETTE_ENTRY_H__
#   define  __PALETTE_ENTRY_H__

# include  <QPixmap>
# include  <QWidget>
# include  <QFont>

class QPaintEvent;
class QCheckBox;

# include  "hurricane/Commons.h"


namespace Hurricane {


  class Name;
  class BasicLayer;
  class HPalette;
  class HPaletteEntry;


  class HPaletteSample : public QWidget {
      Q_OBJECT;

    // Constructor.
    public:
                     HPaletteSample ( HPaletteEntry* entry );

    // Internals - Attributes.
    protected:
      QPixmap        _sample;
      HPaletteEntry*  _entry;

    // Internals - Methods.
      void           redraw        ();
      void           paintEvent    ( QPaintEvent* );

  };


  class HPaletteEntry : public QWidget {
      Q_OBJECT;

    // Methods.
    public:
      virtual const Name&   getName        () const = 0;
      virtual bool          isGroup        () const = 0;
      virtual bool          isBasicLayer   () const = 0;
      virtual BasicLayer*   getBasicLayer  () = 0;
      virtual bool          isChecked      () const = 0;
      virtual void          setChecked     ( bool state ) = 0;

    // Slots.
    public slots:
      virtual void          toggle         () = 0;

    // Internal - Attributes.
    protected:
              HPalette*      _palette;

    // Internal - Constructor.
                             HPaletteEntry ( HPalette* palette );
                             HPaletteEntry ( const HPaletteEntry& );
              HPaletteEntry& operator=     ( const HPaletteEntry& );
      virtual void           _postCreate   () = 0;

  };


} // End of Hurricane namespace.


# endif
