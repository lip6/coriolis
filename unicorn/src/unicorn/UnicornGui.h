
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./UnicornGui.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __UNICORN_UNICORN__
#define  __UNICORN_UNICORN__

#include  <set>
#include  <iostream>
using namespace std;

#include  "hurricane/viewer/CellViewer.h"

#include  "crlcore/Banner.h"
namespace CRL {
  class GraphicTool;
}


namespace Unicorn {


  using std::set;
  using Hurricane::CellViewer;
  using Hurricane::Cell;
  using CRL::Banner;
  using CRL::GraphicTool;


  class UnicornGui : public CellViewer {
      Q_OBJECT;
    public:
      static        UnicornGui* create        ( QWidget* parent=NULL );
                    void        destroy       ();
      static inline Banner&     getBanner     ();
      virtual       Cell*       getCellFromDb ( const char* name );
                    void        registerTool  ( GraphicTool* );
    public slots:
                    void        openCell      ();
                    void        saveCell      ();
    protected:
                                UnicornGui    ( QWidget* parent );
      virtual                  ~UnicornGui    ();
      virtual       void        _postCreate   ();
      virtual       void        _preDestroy   ();
    protected:
      static  Banner             _banner;
              set<GraphicTool*>  _tools;
  };


  inline Banner& UnicornGui::getBanner () { return _banner; }


} // End of Unicorn namespace.


#endif  // __UNICORN_UNICORN__

