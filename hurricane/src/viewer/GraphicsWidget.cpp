// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GraphicsWidget.cpp"                     |
// +-----------------------------------------------------------------+


#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/DisplayStyle.h"
#include "hurricane/viewer/GraphicsWidget.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "GraphicsWidget".


  GraphicsWidget::GraphicsWidget ( QWidget* parent )
    : QWidget     (parent)
    , _cellWidget (NULL)
    , _stylesGrid (new QGridLayout())
    , _stylesGroup(new QButtonGroup())
    , _updateState(ExternalEmit)
  {
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Display Styles") );
    setFont        ( Graphics::getNormalFont(true) );

    QGroupBox*    groupBox = new QGroupBox    ( tr("Display Styles") );
    QVBoxLayout*  wLayout  = new QVBoxLayout  ();

    if (Graphics::isHighDpi()) _stylesGrid->setContentsMargins( 30, 30, 30, 30 );

    groupBox->setLayout ( _stylesGrid );
    wLayout->addWidget  ( groupBox );
    wLayout->addStretch ();
    setLayout ( wLayout );

    connect ( _stylesGroup, SIGNAL(buttonClicked(int)), this, SLOT(setStyle(int)) );

    readGraphics ();
  }


  void  GraphicsWidget::readGraphics ()
  {
    const vector<DisplayStyle*>& styles      = Graphics::getStyles ();
    DisplayStyle*                activeStyle = Graphics::getStyle ();

    size_t hideFallback = (styles.size() > 1) ? 1 : 0;
    for ( size_t istyle=hideFallback ; istyle < styles.size() ; istyle++ ) {
      QRadioButton* button  = new QRadioButton ();
      button->setText ( getString(styles[istyle]->getName()).c_str() );
      if ( activeStyle == styles[istyle] )
        button->setChecked ( true );
      QLabel* label = new QLabel ();
      label->setText ( styles[istyle]->getDescription().c_str() );
      label->setFont ( Graphics::getNormalFont() );

      _stylesGrid ->addWidget ( button, istyle-hideFallback, 0 );
      _stylesGrid ->addWidget ( label , istyle-hideFallback, 1 );
      _stylesGroup->addButton ( button, istyle );
    }
  }


  void  GraphicsWidget::setCellWidget ( CellWidget* cw )
  {
    if ( _cellWidget ) {
      disconnect ( _cellWidget, 0, this       , 0 );
      disconnect ( this       , 0, _cellWidget, 0 );
    }

    _cellWidget = cw;
    if ( !_cellWidget ) return;

    connect ( _cellWidget, SIGNAL(styleChanged()), this, SLOT(changeStyle()) );

    _updateState = ExternalEmit;
    changeStyle ();
  }


  void  GraphicsWidget::changeStyle ()
  {
    if ( _updateState != InternalEmit ) {
    // Should read style here and sets the widget accordingly.
    }
    _updateState = ExternalEmit;
  }


  void  GraphicsWidget::setStyle ( int id )
  {
    if ( _cellWidget ) {
      _updateState = InternalEmit;
      _cellWidget->setStyle ( (size_t)id );
    }
  }


  void  GraphicsWidget::resetGraphics ()
  {
    QLayoutItem* child = NULL;
    while ( (child = _stylesGrid->takeAt(0)) != 0 ) {
      _stylesGrid->removeWidget( child->widget() );
      delete child->widget();
    }
  }


  void  GraphicsWidget::rereadGraphics ()
  {
    resetGraphics();
    readGraphics();
  //setStyle( 0 );
  }


}  // Hurricane namespace.
