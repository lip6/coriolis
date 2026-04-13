
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/NetsetWidget.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/viewer/NetlistWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "NetsetWidget".


  class NetsetWidget : public QWidget {
      Q_OBJECT;
    public:
                                     NetsetWidget           ( QWidget* parent=nullptr );
              void                   setCellWidget          ( CellWidget* );
      template<typename InformationType>                    
              void                   add                    ( const Net* );
              void                   clear                  ();
              void                   updateSelecteds        ();
              void                   resizeColumnToContents ( int );
    signals:
              void                   netSelected            ( Occurrence );
              void                   netUnselected          ( Occurrence );
              void                   netFitted              ( const Net* );
    private slots:                                          
              void                   updateSelecteds        ( const QItemSelection& , const QItemSelection& );
              void                   fitToNet               ();

    private:
              CellWidget*            _cellWidget;
              NetlistModel*          _baseModel;
              QTableView*            _view;
              int                    _rowHeight;
              SelectedNetSet         _selecteds;
              bool                   _forceReselect;
  };


  template<typename InformationType>
  void  NetsetWidget::add ( const Net* net )
  {
    _baseModel->add<InformationType>( net );
  }


}  // Hurricane namespace.
