// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/LibraryManager.h"                    |
// +-----------------------------------------------------------------+


#ifndef  CRL_LIBRARY_MANAGER_H
#define  CRL_LIBRARY_MANAGER_H

#include <QWidget>

class QLabel;
class QModelIndex;
class QTableView;
class QHeaderView;

namespace Hurricane {
  class Cell;
  class CellViewer;
}


namespace CRL {

  using Hurricane::Cell;
  using Hurricane::CellViewer;
  class AllianceLibrary;
  class ViewsWidget;
  class CellsWidget;
  class LibrariesWidget;


// -------------------------------------------------------------------
// Class  :  "LibraryManager".


  class LibraryManager : public QWidget {
      Q_OBJECT;
    public:
                         LibraryManager ( QWidget* parent=NULL );
      inline void        setCellViewer  ( CellViewer* );
      inline CellViewer* getCellViewer  () const;
    public slots:                 
      void               toggleShow     ();
      void               setLibrary     ( const AllianceLibrary* library );
      CellViewer*        openCell       ( Cell*, unsigned int flags );
    private:
      LibrariesWidget* _librariesWidget;
      CellsWidget*     _cellsWidget;
      ViewsWidget*     _viewsWidget;
      CellViewer*      _cellViewer;
      QLabel*          _libPath;
  };


  inline void        LibraryManager::setCellViewer ( CellViewer* cw ) { _cellViewer=cw; }
  inline CellViewer* LibraryManager::getCellViewer () const { return _cellViewer; }


}  // CRL namespace.

#endif  // CRL_LIBRARY_WIDGET_H
