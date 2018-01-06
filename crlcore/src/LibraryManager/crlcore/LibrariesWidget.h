// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/LibrariesWidget.h"                   |
// +-----------------------------------------------------------------+


#ifndef  CRL_LIBRARIES_WIDGET_H
#define  CRL_LIBRARIES_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "crlcore/AllianceLibrary.h"

class QModelIndex;
class QTableView;
class QHeaderView;


namespace CRL {

  class LibrariesModel;


// -------------------------------------------------------------------
// Class  :  "LibrariesWidget".

  class LibrariesWidget : public QWidget {
      Q_OBJECT;
    public:
            LibrariesWidget ( QWidget* parent=NULL );
      void  initSelection   ();
      void  update          ();
    signals:
      void  selectedLibrary ( const AllianceLibrary* );
    public slots:
      void  selectCurrent   ( const QModelIndex& index, const QModelIndex& );
    private:
              LibrariesModel*  _baseModel;
              QTableView*      _view;
              int              _rowHeight;
  };


}  // CRL namepsace.

#endif  // CRL_LIBRARIES_WIDGET_H
