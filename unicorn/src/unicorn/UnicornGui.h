// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./unicorn/UnicornGui.h"                        |
// +-----------------------------------------------------------------+




#ifndef  UNICORN_UNICORN_H
#define  UNICORN_UNICORN_H

#include <set>
#include <iostream>
#include <functional>
#include <boost/any.hpp>
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/Banner.h"
namespace CRL {
  class GraphicTool;
  class LibraryManager;
}

#include "unicorn/ImportCell.h"


namespace Unicorn {


  using std::set;
  using Hurricane::CellViewer;
  using Hurricane::Cell;
  using CRL::Banner;
  using CRL::GraphicTool;
  using CRL::LibraryManager;

  class ImportCellDialog;
  class ExportCellDialog;


  class UnicornGui : public CellViewer {
      Q_OBJECT;
    public:
      static        UnicornGui* create             ( QWidget* parent=NULL );
                    void        destroy            ();
      static inline Banner&     getBanner          ();
      virtual       Cell*       getCellFromDb      ( const char* name );
      inline        ImportCell* getImportCell      ();
                    void        registerTool       ( GraphicTool* );
      virtual       UnicornGui* vcreate            () const;
      virtual       std::string _getString         () const;
    public slots:                                  
                    void        openCell           ();
                    void        saveCell           ();
                    void        importCell         ();
                    void        exportCell         ();
    protected:                                     
                                UnicornGui         ( QWidget* parent );
      virtual                  ~UnicornGui         ();
      virtual       void        _postCreate        ();
      virtual       void        _preDestroy        ();
                    void        _runUnicornInit    ();
    protected:
      static  Banner             _banner;
              set<GraphicTool*>  _tools;
              ImportCell         _importCell;
              ImportCellDialog*  _importDialog;
              ExportCellDialog*  _exportDialog;
              LibraryManager*    _libraryManager;
  };


  inline Banner&     UnicornGui::getBanner     () { return  _banner; }
  inline ImportCell* UnicornGui::getImportCell () { return &_importCell; }


} // End of Unicorn namespace.


GETSTRING_POINTER_SUPPORT(Unicorn::UnicornGui)
IOSTREAM_POINTER_SUPPORT(Unicorn::UnicornGui)


#endif  // __UNICORN_UNICORN__

