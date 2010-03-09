
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
// |      C y c l o p  -  S i m p l e   V i e w e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Cyclop.h"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __CRL_CYCLOP_H__
#define  __CRL_CYCLOP_H__

#include  <iostream>
using namespace std;

#include  "hurricane/viewer/CellViewer.h"

#include  "crlcore/Banner.h"
#include  "crlcore/AllianceFramework.h"


namespace CRL {


  using Hurricane::CellViewer;
  using Hurricane::Cell;


  class Cyclop : public CellViewer {
      Q_OBJECT;

    public:
      static  Cyclop*       create        ( QWidget* parent=NULL );
              void          destroy       ();
      inline  const Banner& getBanner     () const;
      virtual Cell*         getCellFromDb ( const char* name );
    public slots:
              void          openCell      ();
              void          stressDisplay ();
    protected:
                            Cyclop        ( QWidget* parent );
      virtual              ~Cyclop        ();
      virtual void          _postCreate   ();
      virtual void          _preDestroy   ();
    protected:
              Banner        _banner;
              QAction*      _stressDisplayAction;
  };


  inline const Banner& Cyclop::getBanner () const { return _banner; }



};


#endif

