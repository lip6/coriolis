
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
// |  C++ Module  :       "./HDisplayFilter.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QLabel>
#include  <QSpinBox>
#include  <QGroupBox>
#include  <QGridLayout>

#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/HDisplayFilter.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "HDisplayFilter".


  HDisplayFilter::HDisplayFilter ( QWidget* parent )
    : QWidget(parent)
    , _cellWidget(NULL)
    , _startSpinBox(NULL)
    , _stopSpinBox(NULL)
  {
    setAttribute   ( Qt::WA_QuitOnClose, false );
    setWindowTitle ( tr("Display Filter") );
    setFont        ( Graphics::getNormalFont(true) );

    QGroupBox*    groupBox = new QGroupBox    ( tr("Hierarchy Levels") );
    QGridLayout*  gLayout  = new QGridLayout  ();
    QGridLayout*  wLayout  = new QGridLayout  ();

    QLabel* label = new QLabel ();
    label->setText ( "Hierarchy Start Level" );
    label->setFont ( Graphics::getNormalFont() );

    _startSpinBox = new QSpinBox ();
    _startSpinBox->setFont ( Graphics::getNormalFont() );

    gLayout->addWidget ( label        , 0, 0 );
    gLayout->addWidget ( _startSpinBox, 0, 1 );

    label = new QLabel ();
    label->setText ( "Hierarchy Stop Level" );
    label->setFont ( Graphics::getNormalFont() );

    _stopSpinBox = new QSpinBox ();
    _stopSpinBox->setFont  ( Graphics::getNormalFont() );
    _stopSpinBox->setValue ( 100 );

    gLayout->addWidget ( label       , 1, 0 );
    gLayout->addWidget ( _stopSpinBox, 1, 1 );

    groupBox->setLayout ( gLayout );
    wLayout->addWidget ( groupBox, 0, 0 );
    setLayout ( wLayout );

    connect ( _startSpinBox, SIGNAL(valueChanged(int)), this, SLOT(startLevelChanged(int)) );
    connect ( _stopSpinBox , SIGNAL(valueChanged(int)), this, SLOT(stopLevelChanged (int)) );
  }


  void  HDisplayFilter::setCellWidget ( CellWidget* cw )
  {
    if ( !cw ) {
      if ( _cellWidget )
        disconnect ( _cellWidget, SLOT(redraw()) );
      _cellWidget = NULL;
      return;
    }
    _cellWidget = cw;
    connect ( this, SIGNAL(filterChanged()), _cellWidget, SLOT(redraw()) );
  }


  void  HDisplayFilter::startLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      _cellWidget->setStartLevel ( level );
      if ( _stopSpinBox->value() < level ) {
        _stopSpinBox->setValue ( level );
        return;
      }
      emit filterChanged();
    }
  }


  void  HDisplayFilter::stopLevelChanged ( int level )
  {
    if ( _cellWidget ) {
      _cellWidget->setStopLevel ( level );
      if ( _startSpinBox->value() > level ) {
        _startSpinBox->setValue ( level );
        return;
      }
      emit filterChanged();
    }
  }


}
