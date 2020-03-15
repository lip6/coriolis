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
// |  C++ Module  :  "./SlicingPlotWidget.cpp"                       |
// +-----------------------------------------------------------------+


#include <QColor>
#include <QPen>
#include <QPalette>
#include <QPoint>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_legend.h>
#include <math.h>      
#include "hurricane/Error.h"
#include "hurricane/DbU.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/viewer/DynamicLabel.h"
#include "hurricane/analog/AnalogCellExtension.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/TransistorFamily.h"
#include "crlcore/Utilities.h"
#include "bora/SlicingPlotWidget.h"


namespace {


  class SlicingScaleDraw : public QwtScaleDraw {
    protected:
      virtual QwtText  label ( double value ) const;
  };


  QwtText  SlicingScaleDraw::label ( double value ) const
  { return QString("%1").arg( value, 0, 'f', 1 ); }


} // Anonymous namespace.


namespace Bora {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::DbU;
  using Hurricane::DynamicLabel;
  using Hurricane::Graphics;
  using Analog::AnalogCellExtension;
  using Analog::Device;
  using Analog::TransistorFamily;


  SlicingPlotWidget::SlicingPlotWidget ( QWidget* parent )
    : QWidget        (parent)
    , _viewer        (NULL)
    , _plot          (new QwtPlot      ())
    , _picker        (new QwtPlotPicker( QwtPlot::xBottom           // X Axis id.
                                       , QwtPlot::yLeft             // Y Axis id. 
                                       , QwtPicker::CrossRubberBand
                                       , QwtPicker::ActiveOnly
                                       , _plot->canvas()) )
    , _gridDisplay   (new QGridLayout()) 
    , _gridLabel     ()                                             // Label line 2: text
    , _widths        (NULL)                                         // Coordinates X for STreeCurve
    , _heights       (NULL)                                         // Coordinates Y for STreeCurve
    , _widthSelected (NULL)                                         // Coordinate  X for selectedPoint
    , _heightSelected(NULL)                                         // Coordinate  Y for selectedPoint
    , _pareto        ()
    , _STreeCurve    (new QwtPlotCurve("SlicingTree's dimensions")) // Blue dots  : possible dimensions
    , _paretoCurve   (new QwtPlotCurve("Pareto"))                   // Black curve: pareto curve
    , _selectedPoint (new QwtPlotCurve("Selected"))                 // Red dot    : selected placement
  {
    _picker->setStateMachine(new QwtPickerClickPointMachine());
    setStyleSheet ( "border: 0px" );

    int ptSize = Graphics::isHighDpi() ? 5 : 3;

    QwtText xTitle ( QString::fromUtf8("Width (µm)") );
    QwtText yTitle ( QString::fromUtf8("Height (µm)") );
    xTitle.setFont( Graphics::getFixedFont(QFont::Bold,false,false,-1) );
    yTitle.setFont( Graphics::getFixedFont(QFont::Bold,false,false,-1) );

    _plot->setAxisTitle       ( QwtPlot::xBottom, xTitle );
    _plot->setAxisTitle       ( QwtPlot::yLeft  , yTitle );
    _plot->insertLegend       ( new QwtLegend(), QwtPlot::BottomLegend );
    _plot->setAxisFont        ( QwtPlot::xBottom, Graphics::getFixedFont(QFont::Normal,false,false,-2) );
    _plot->setAxisFont        ( QwtPlot::yLeft  , Graphics::getFixedFont(QFont::Normal,false,false,-2) );

    QString styleSheet("color: black; background:rgb(255,255,221); padding: 5px");
    _plot->setStyleSheet(styleSheet);
    _plot->canvas()->setStyleSheet(styleSheet);
    _plot->legend()->setStyleSheet( QString("padding: 0px") );

    QPen paretoPen ( Qt::green );
    paretoPen.setWidth( Graphics::isHighDpi() ? 3 : 1 );
    _paretoCurve->setStyle( QwtPlotCurve::Lines );
    _paretoCurve->setPen  ( paretoPen );
    _paretoCurve->attach  ( _plot );

    QPen dotPen ( Qt::blue );
    dotPen.setWidth( ptSize );
    _STreeCurve->setStyle( QwtPlotCurve::Dots );
    _STreeCurve->setPen  ( dotPen );
    _STreeCurve->attach  ( _plot );

    QwtSymbol* symbol = new QwtSymbol();
    symbol->setStyle( QwtSymbol::Cross );
    symbol->setSize ( 20 );
    symbol->setPen  ( dotPen );
    _STreeCurve->setSymbol( symbol );

    QPen selectPen ( Qt::red );
    selectPen.setWidth( ptSize+2 );
    _selectedPoint->setStyle( QwtPlotCurve::Dots );
    _selectedPoint->setPen  ( selectPen );
    _selectedPoint->attach  ( _plot );

    connect( _picker, SIGNAL(selected(const QPointF&)), this, SLOT(onPointSelect(const QPointF&)) );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget ( _plot );

    QHBoxLayout* line = new QHBoxLayout ();
    line->addStretch();

    DynamicLabel* dlabel = new DynamicLabel();
    dlabel->setStaticText ( QString::fromUtf8("Width (µm):") );
    dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
    dlabel->setDynamicText( QString("%1").arg( "0" ) );
    _gridLabel.push_back( dlabel );
    line->addWidget( dlabel );
    line->addStretch();

    dlabel = new DynamicLabel();
    dlabel->setStaticText ( QString::fromUtf8("Height (µm):") );
    dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
    dlabel->setDynamicText( QString("%1").arg( "0" ) );
    _gridLabel.push_back( dlabel );
    line->addWidget( dlabel );
    line->addStretch();
    vLayout->addLayout( line );

    line = new QHBoxLayout ();
    line->addStretch();

    dlabel = new DynamicLabel();
    dlabel->setStaticText ( "Occupation Area (%):" );
    dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
    dlabel->setDynamicText( QString("%1").arg( "100" ) );
    _gridLabel.push_back( dlabel );
    line->addWidget( dlabel );
    line->addStretch();

    dlabel = new DynamicLabel();
    dlabel->setStaticText ( "Ratio(W/H):" );
    dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
    dlabel->setDynamicText( QString("%1").arg( "0" ) );
    _gridLabel.push_back( dlabel );
    line->addWidget( dlabel );
    line->addStretch();

    dlabel = new DynamicLabel();
    dlabel->setStaticText ( QString::fromUtf8("Area (µm²):") );
    dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
    dlabel->setDynamicText( QString("%1").arg( "0" ) );
    _gridLabel.push_back( dlabel );
    line->addWidget( dlabel );
    line->addStretch();
    vLayout->addLayout( line );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );
    vLayout->addWidget ( separator );

    vLayout->addLayout ( _gridDisplay  );

    setLayout( vLayout );
  }


  SlicingPlotWidget::~SlicingPlotWidget ()
  {
    delete _STreeCurve;
    delete _paretoCurve;
    delete _selectedPoint;
    delete _plot;

    if ( _widths  != NULL ) delete [] _widths;
    if ( _heights != NULL ) delete [] _heights;
  }


  void  SlicingPlotWidget::setViewer ( CellViewer* viewer )
  {
    if (_viewer) {
      disconnect( _viewer, 0, this   , 0 );
      disconnect( this   , 0, _viewer, 0 );
    }

    _viewer = viewer;
    setDatas();
  }


  void  SlicingPlotWidget::setDatas ()
  {
  // Clear previous datas 
    if (_widths  != NULL) delete [] _widths;
    if (_heights != NULL) delete [] _heights;

    _pareto = Pareto();

    for ( size_t igrid = 5 ; igrid < _gridLabel.size() ; ++igrid )
      delete _gridLabel[igrid];
    _gridLabel.resize( 5 );

    while ( _gridDisplay->count() ) {
      _gridDisplay->removeItem( _gridDisplay->itemAt(0) );
    }
    
    if (_widthSelected  != NULL) delete [] _widthSelected;
    if (_heightSelected != NULL) delete [] _heightSelected;
    _selectedPoint->hide();   

    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
    if (not slicingtree) {
      _STreeCurve->hide();
      _paretoCurve->hide();
      _selectedPoint->hide();      
      return;
    } 

  // Set new datas
    _widths  = new double [ slicingtree->getNodeSets()->size() ];
    _heights = new double [ slicingtree->getNodeSets()->size() ];
    size_t i = 0;
    NodeSets* nodeSets = slicingtree->getNodeSets();
    for (vector<BoxSet*>::iterator it = nodeSets->begin(); it != nodeSets->end(); it++){
      _widths [i] = DbU::toPhysical ( (*it)->getWidth (), DbU::Micro );
      _heights[i] = DbU::toPhysical ( (*it)->getHeight(), DbU::Micro );
      _pareto.mergePoint( _widths[i], _heights[i] );
      i++;
    }

    _STreeCurve->setSamples ( _widths     , _heights    , nodeSets->size() );
    _paretoCurve->setSamples( _pareto.xs(), _pareto.ys(), _pareto.size()  );
    _STreeCurve->show();
    _paretoCurve->show();
    _plot->replot();

    int ii = 0;
    int jj = 0;

    Cell* analogCell = _viewer->getCellWidget()->getCell();

    vector<Instance*> instances = vector<Instance*>();
    for( Instance* instance : analogCell->getInstances() ) {
      Cell*   model  = instance->getMasterCell();   
      Device* device = dynamic_cast<Device*>( model );
      if (device) instances.push_back( instance );
    }

    for( Instance* instance : instances ) {
      Cell*   model  = instance->getMasterCell();   
      Device* device = dynamic_cast<Device*>( model );

      if (device) {
        TransistorFamily* tf = dynamic_cast<TransistorFamily*>( device );
        if (tf) {
          DynamicLabel* dlabel = new DynamicLabel();
          dlabel->setDynamicWidth( 4, DynamicLabel::NoIndent );
          dlabel->setStaticText  ( QString("%1:").arg( instance->getName()._getString().c_str() ) );
          dlabel->setDynamicText ( QString("%1" ).arg( 0 ) );
          _gridDisplay->addWidget( dlabel, ii, jj );
          _gridLabel.push_back( dlabel );
          jj++;
          if (jj == 5){
            jj = 0;
            ii++;
          }
        }
      }
    }
  }


  void  SlicingPlotWidget::onPointSelect ( const QPointF& point )
  {
  // Clicking on SlicingTree's Pareto Graph:
  // 1) Update slicing tree
  // 2) Update selected point and labels
  // 3) Update selected row

    double       distance = 0.0;
    QwtScaleMap  xMap     = _plot->canvasMap( QwtPlot::xBottom ); 
    QwtScaleMap  yMap     = _plot->canvasMap( QwtPlot::yLeft ); 
    QPoint       qpoint   ( xMap.transform(point.x()), yMap.transform(point.y()) );
    int          iclosest = _STreeCurve->closestPoint( qpoint, &distance );
    int          dataSize = _STreeCurve->dataSize();

    cdebug.log(539) << "  Selection: [" << point.x() << " " << point.y() << "]" << endl;

    if ( (iclosest >= 0) and (iclosest < dataSize) ) {
      double x = _STreeCurve->sample( iclosest ).x();
      double y = _STreeCurve->sample( iclosest ).y();

      ostringstream message;
      message << "(" << DbU::getValueString(x) << "," << DbU::getValueString(y) << ")";
      cmess2 << "  o  Selecting analog placement" << endl;
      cmess2 << Dots::asInt   ( "     - Index"     , iclosest      ) << endl;
      cmess2 << Dots::asString( "     - Size (w,h)", message.str() ) << endl;

      SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );
      BoxSet*      boxSet      = slicingtree->getPairHW( DbU::fromPhysical(y, DbU::Micro)
                                                       , DbU::fromPhysical(x, DbU::Micro)
                                                       );

      cdebug.log(539) << "  iclosest: " << iclosest
                      << " (" << DbU::getValueString(DbU::fromPhysical(x, DbU::Micro))
                      <<  "," << DbU::getValueString(DbU::fromPhysical(y, DbU::Micro))
                      << ")" << endl;

      emit updatePlacement( boxSet );
      updateSelectedPoint   ( x, y );
      emit updateSelectedRow( x, y );

    } else {
      cdebug.log(539) << "  No closest point " << iclosest << endl;
    }
  }


  void  SlicingPlotWidget::updateSelectedPoint ( double x, double y )
  {
  // Clicking on SlicingTree's possible dimensions table:
  // 1) Update selected point
  // 2) Update labels

    if (_widthSelected  != NULL) delete [] _widthSelected;
    if (_heightSelected != NULL) delete [] _heightSelected;

    double* _widthSelected  = new double [ 1 ];
    double* _heightSelected = new double [ 1 ];

    _widthSelected [0] = x;
    _heightSelected[0] = y;

    _selectedPoint->setSamples ( _widthSelected, _heightSelected, 1 );
    _selectedPoint->show();
    _plot->replot();

    SlicingNode* slicingtree = AnalogCellExtension::get<SlicingNode>( _viewer->getCellWidget()->getCell() );

    _gridLabel[0]->setDynamicText( QString("%1").arg( DbU::getValueString(slicingtree->getWidth ()).c_str() ) );
    _gridLabel[1]->setDynamicText( QString("%1").arg( DbU::getValueString(slicingtree->getHeight()).c_str() ) );
    _gridLabel[2]->setDynamicText( QString("%1").arg( slicingtree->getOccupationArea() ));
    if ( slicingtree->getHeight() != 0 ){
      _gridLabel[3]->setDynamicText( QString("%1").arg(  DbU::getPhysical( slicingtree->getWidth (), DbU::Micro )
                                                      / DbU::getPhysical( slicingtree->getHeight(), DbU::Micro ) 
                                                      ));
    } else _gridLabel[3]->setDynamicText( QString("%1").arg(0));
    
    _gridLabel[4]->setDynamicText( QString("%1").arg( DbU::getPhysical( slicingtree->getWidth (), DbU::Micro )
                                                    * DbU::getPhysical( slicingtree->getHeight(), DbU::Micro ) 
                                                    ));

    Cell* analogCell = _viewer->getCellWidget()->getCell();
    size_t i = 0;

    vector<Instance*> instances;
    for( Instance* iInstance : analogCell->getInstances() ) {
      Cell*   model  = iInstance->getMasterCell();   
      Device* device = dynamic_cast<Device*>(model);
      if (device) instances.push_back(iInstance);
    }
    for( Instance* iInstance : instances ) {
      Cell*   model  = iInstance->getMasterCell();   
      Device* device = dynamic_cast<Device*>(model);

      if (device) {
        TransistorFamily* tf = dynamic_cast<TransistorFamily*>( device );
        if (tf) {
          _gridLabel[i+5]->setDynamicText ( QString("%1" ).arg( tf->getNfing() ) );
          i++;
        }
      }
    }
  }


}  // Bora namespace.
