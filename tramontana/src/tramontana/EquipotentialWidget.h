// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/EquipotentialWidget.h"           |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include "hurricane/Commons.h"
#include "hurricane/Bug.h"

namespace Hurricane {
  class CellWidget;
  class NetsetWidget;
}

#include "tramontana/Equipotential.h"
#include "tramontana/ShortCircuitWidget.h"


class QModelIndex;
class QLineEdit;
class QLabel;


namespace Tramontana {

  using std::string;
  using std::set;
  using Hurricane::Cell;
  using Hurricane::OccurrenceSet;
  using Hurricane::NetsetWidget;
  using Hurricane::CellWidget;


// -------------------------------------------------------------------
// Class  :  "EquipotentialWidget".


  class EquipotentialWidget : public QWidget {
      Q_OBJECT;
    public:
                                   EquipotentialWidget ( QWidget* parent=nullptr );
                    void           setCellWidget       ( CellWidget* );
      inline  const Equipotential* getEquipotential    () const;
    public slots:                        
                    void           setEquipotential    ( const Equipotential* );
                    void           updateSelecteds     ( const QItemSelection&, const QItemSelection& );
    signals:                        
                    void           reframe             ( const Box& );
                    void           select              ( Occurrence );
                    void           selectSet           ( const OccurrenceSet& );
    private:
            CellWidget*       _cellWidget;
      const Equipotential*    _equipotential;
            QLabel*           _equiName;
            NetsetWidget*     _netset;
            QTreeView*        _shortWidget;
            ShortCircuitModel _shortModel;
  };


  inline  const Equipotential* EquipotentialWidget::getEquipotential () const { return _equipotential; }


} // Tramontana namespace.
