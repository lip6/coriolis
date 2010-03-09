
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s p d   G l o b a l   r o u t i n g  -  M a i n   G U I    |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./IspdGui.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __ISPD_ISPD__
#define  __ISPD_ISPD__

#include  <set>
#include  <iostream>
using namespace std;

#include  "hurricane/viewer/CellViewer.h"

#include  "crlcore/Banner.h"
namespace CRL {
  class GraphicTool;
}


namespace Ispd {


  using std::set;
  using Hurricane::CellViewer;
  using Hurricane::Cell;
  using CRL::Banner;
  using CRL::GraphicTool;


  class IspdGui : public CellViewer {
      Q_OBJECT;

    public:
      static  IspdGui*           create        ( QWidget* parent=NULL);
              void               destroy       ();
      inline  const Banner&      getBanner     () const;
              void               registerTool  ( GraphicTool* );
    protected:
                                 IspdGui       ( QWidget* parent );
      virtual                    ~IspdGui      ();
      virtual void               _postCreate   ();
      virtual void               _preDestroy   ();
    protected:
              Banner             _banner;
              set<GraphicTool*>  _tools;
  };


  inline const Banner& IspdGui::getBanner () const { return _banner; }


} // End of Ispd namespace.


#endif  // __ISPD_ISPD__

