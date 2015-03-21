// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ImportCell.h"                           |
// +-----------------------------------------------------------------+


#ifndef  UNICORN_IMPORT_CELL_H
#define  UNICORN_IMPORT_CELL_H

#include <string>
#include <map>
#include <functional>

namespace Hurricane {
  class Cell;
}

namespace Unicorn {

  class ImportCellDialog;


  class ImportCell {
    public:
      typedef  std::map< int, std::pair< std::string, std::function<Hurricane::Cell*(std::string)> > >  ImportLut;
    public:
                              ImportCell  ();
      inline void             setDialog   ( ImportCellDialog* );
             Hurricane::Cell* load        ( const std::string&, int format );
      void                    addImporter ( std::string, std::function<Hurricane::Cell*(std::string)> );
    private:
      int               _count;
      ImportLut         _lut;
      ImportCellDialog* _dialog;
  };


  inline void  ImportCell::setDialog ( ImportCellDialog* dialog ) { _dialog = dialog; }


}  // Unicorn namespace.

#endif
