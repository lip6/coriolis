
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
// |  C++ Header  :       "./HSelection.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTION_WIDGET_H__
#define  __HURRICANE_SELECTION_WIDGET_H__


#include  <QWidget>
#include  <QTableView>
#include  <QSortFilterProxyModel>

#include  "hurricane/Commons.h"
#include  "hurricane/Occurrence.h"
#include  "hurricane/viewer/HSelectionModel.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;
class QCheckBox;


namespace Hurricane {


  class Selector;
  class QCloseEvent;


  class HSelection : public QWidget {
      Q_OBJECT;

    public:
                                     HSelection        ( QWidget* parent=NULL );
              void                   runInspector      ( const QModelIndex& index  );
              bool                   isCumulative      () const;
    signals:
              void                   showSelected      ( bool );
              void                   occurrenceToggled ( Occurrence, bool );
              void                   cumulativeToggled ( bool );
              void                   selectionCleared  ();
    public slots:
              void                   setSelection      ( const set<Selector*>& selection, Cell* cell=NULL );
              void                   toggleSelection   ( Occurrence occurrence );
              void                   toggleSelection   ( const QModelIndex& index );
              void                   forceRowHeight    ();
    private slots:
              void                   textFilterChanged ();
    protected:
      virtual void                   keyPressEvent     ( QKeyEvent * event );
      virtual void                   hideEvent         ( QHideEvent* event );

    private:
              HSelectionModel*       _baseModel;
              QSortFilterProxyModel* _sortModel;
              QTableView*            _view;
              QLineEdit*             _filterPatternLineEdit;
              QCheckBox*             _cumulative;
              int                    _rowHeight;
  };


} // End of Hurricane namespace.


#endif // __HURRICANE_SELECTION_WIDGET_H__
