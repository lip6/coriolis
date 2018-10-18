// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/SlicingWidget.h"                        |
// +-----------------------------------------------------------------+


#ifndef  BORA_SLICING_WIDGET_H
#define  BORA_SLICING_WIDGET_H

#include <QTabWidget>
namespace Hurricane {
  class Cell;
  class CellViewer;
}
#include "bora/SlicingNode.h"


namespace Bora {

  using Hurricane::Cell;
  using Hurricane::CellViewer;
  class BoraEngine;
  class SlicingPlotWidget;
  class SlicingDataWidget;
  

// -------------------------------------------------------------------
// Class  :  "Bora::SlicingTab".


  class SlicingTab : public QWidget {
      Q_OBJECT;
    public:
                         SlicingTab         ( QWidget* parent=NULL );
      inline CellViewer* getViewer          ();
    public slots:
      virtual void       setViewer          ( CellViewer* );
      virtual void       cellPreModificate  ();
      virtual void       cellPostModificate ();
    private:
      CellViewer* _viewer;
  };

  inline CellViewer* SlicingTab::getViewer () { return _viewer; }


// -------------------------------------------------------------------
// Class  :  "Bora::TabPlot".


  class TabPlot : public SlicingTab {
      Q_OBJECT;
    public:
                                TabPlot           ( QWidget* parent=NULL );
      inline SlicingPlotWidget* getPlot           ();
             void               setDatas          ();
    public slots:
             void               setViewer         ( CellViewer* );
    signals:      
             void               updateSelectedRow ( double x, double y );
    private:
      SlicingPlotWidget* _plot;
  };

  inline  SlicingPlotWidget* TabPlot::getPlot () { return _plot; }


// -------------------------------------------------------------------
// Class  :  "Bora::TabDatas".


  class TabDatas : public SlicingTab {
      Q_OBJECT;
    public:
                                 TabDatas            ( QWidget* parent=NULL );
      inline  SlicingDataWidget* getDatas            ();
              void               updateContents      ();
    public slots:
              void               setViewer           ( CellViewer* );
    signals:      
              void               updateSelectedPoint ( double x, double y );
    private:
      SlicingDataWidget* _datas;
  };

  inline  SlicingDataWidget* TabDatas::getDatas () { return _datas; }


// -------------------------------------------------------------------
// Class  :  "Bora::SlicingWidget".


  class SlicingWidget : public QTabWidget {
      Q_OBJECT;
    public:
                                SlicingWidget      ( QWidget* parent=NULL );
      inline CellViewer*        getViewer          ();
      inline SlicingPlotWidget* getPlotWidget      ();
      inline SlicingDataWidget* getDataWidget      ();
             void               setViewer          ( CellViewer* );
             void               setCell            ( Cell* );
             void               updateContents();
    signals:                  
             void               updatePlacement    ( BoxSet* );
    
    public slots:             
             void               cellPreModificate  ();
             void               cellPostModificate ();
             void               cellChanged        ( Cell* );
             void               toggleShow         ();
    private:
      Cell*       _cell;
      CellViewer* _viewer;
      TabPlot*    _tabPlot;
      TabDatas*   _tabDatas;
  };


  inline SlicingPlotWidget* SlicingWidget::getPlotWidget () { return _tabPlot->getPlot(); }
  inline SlicingDataWidget* SlicingWidget::getDataWidget () { return _tabDatas->getDatas(); }


}  // Bora namespace.

#endif  // BORA_SLICING_WIDGET_H
