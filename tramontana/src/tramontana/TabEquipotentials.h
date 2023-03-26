// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
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
// |  C++ Header   :  "./tramontana/TabEquipotentials.h"             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/viewer/ControllerWidget.h"
#include "tramontana/EquipotentialsWidget.h"


namespace Tramontana {

  using Hurricane::ControllerTab;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TabEquipotentials".


  class TabEquipotentials : public ControllerTab {
      Q_OBJECT;

    public:
                                    TabEquipotentials     ( QWidget* parent=NULL );
      inline  QCheckBox*            getSyncEquipotentials ();
      inline  QCheckBox*            getSyncSelection      ();
      inline  EquipotentialsWidget* getBrowser            ();
      virtual void                  cellPreModificate     ();
      virtual void                  cellPostModificate    ();
    public slots:                                         
      virtual void                  setCell               ( Cell* );
      virtual void                  setCellWidget         ( CellWidget* );
      virtual void                  setSyncEquipotentials ( bool );
      virtual void                  setSyncSelection      ( bool );

    protected:
      EquipotentialsWidget* _browser;
      QCheckBox*            _syncEquipotentials;
      QCheckBox*            _syncSelection;
      bool                  _cwCumulativeSelection;
  };


  inline EquipotentialsWidget* TabEquipotentials::getBrowser            () { return _browser; }
  inline QCheckBox*            TabEquipotentials::getSyncEquipotentials () { return _syncEquipotentials; }
  inline QCheckBox*            TabEquipotentials::getSyncSelection      () { return _syncSelection; }


}
