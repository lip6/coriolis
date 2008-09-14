
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
// |  C++ Header  :       "./HInspectorWidget.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HINSPECTOR_WIDGET_H__
#define  __HINSPECTOR_WIDGET_H__


#include  <QWidget>

#include  "hurricane/Commons.h"


class QSortFilterProxyModel;
class QModelIndex;
class QTableView;
class QLineEdit;
class QComboBox;
class QHeaderView;


namespace Hurricane {


  class RecordModel;


  class HInspectorWidget : public QWidget {

    public:
      typedef vector<Slot*>             SlotsVector;

    private:
      class History {
        public:
                       History       ();
                      ~History       ();
          void         push          ( Slot* slot );
          void         pop           ();
          void         back          ();
          void         goTo          ( int depth );
          void         clear         ( bool inDelete=false );
          void         setComboBox   ( QComboBox* comboBox );
          void         setRootRecord ( Record* rootRecord );
          size_t       getDepth      () const;
          Slot*        getSlot       () const;
        private:
          size_t       _depth;
          SlotsVector  _slots;
          QComboBox*   _comboBox;
        private:
                       History       ( const History& );
          History&     operator=     ( const History& );
      };

      Q_OBJECT;

    public:
                                        HInspectorWidget  ( QWidget* parent=NULL );
                                       ~HInspectorWidget  ();
              void                      setRootRecord     ( Record*  record );
    private slots:
              void                      forceRowHeight    ();
              void                      textFilterChanged ();
              void                      historyChanged    ( int depth );
              void                      forkInspector     ( const QModelIndex& index );
    protected:
              void                      keyPressEvent     ( QKeyEvent * event );
    private:
              void                      pushSlot          ( Slot* slot );
              void                      popSlot           ();
              void                      back              ();
              bool                      setSlot           ();

    private:
              RecordModel*              _baseModel;
              QSortFilterProxyModel*    _sortModel;
              QComboBox*                _historyComboBox;
              QTableView*               _view;
              QLineEdit*                _filterPatternLineEdit;
              int                       _rowHeight;
              History                   _history;
  };


} // End of Hurricane namespace.


#endif // __HINSPECTOR_WIDGET_H__
