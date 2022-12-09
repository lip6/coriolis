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
// |  C++ Header  :  "./bora/DSlicingNode.h"                         |
// +-----------------------------------------------------------------+


#ifndef  BORA_SLICING_PLOT_WIDGET_H
#define  BORA_SLICING_PLOT_WIDGET_H

//Work sround for older qwt releases
#include <QtGlobal>
#if QT_VERSION >= 0x050400
 #define QT_STATIC_CONST static const
#endif

#include <vector>
#include <qwt_plot.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>
class QHBoxLayout;
class QGridLayout;
class QwtPlotCurve;
class QwtPlotPicker;
#include <hurricane/DbU.h>
namespace Hurricane {
  class DynamicLabel;
  class CellViewer;
}
#include "bora/SlicingNode.h"
#include "bora/Pareto.h"


namespace Bora {

  using Hurricane::CellViewer;
  class GraphicBoraEngine;


  class SlicingPlotWidget : public QWidget {
      Q_OBJECT;
    public:
                     SlicingPlotWidget   ( QWidget* parent=NULL );
      virtual       ~SlicingPlotWidget   ();
               Cell* getCell             ();
               void  setViewer           ( CellViewer* );
               void  setDatas            ();
    public slots:
               void  onPointSelect       ( const QPointF& );
               void  updateSelectedPoint ( double x, double y );
    signals:
               void  updatePlacement     ( BoxSet* );
               void  updateSelectedRow   ( double x, double y );
    private:
      CellViewer*                            _viewer;
      QwtPlot*                               _plot;
      QwtPlotPicker*                         _picker;
      QGridLayout*                           _gridDisplay;  
      std::vector<Hurricane::DynamicLabel*>  _gridLabel;   
      double*                                _widths;
      double*                                _heights;
      double*                                _widthSelected;
      double*                                _heightSelected;
      Pareto                                 _pareto;
      QwtPlotCurve*                          _STreeCurve;
      QwtPlotCurve*                          _paretoCurve;
      QwtPlotCurve*                          _selectedPoint;
  } ;


} // Bora namespace.

#endif  // BORA_SLICING_PLOT_WIDGET_H
