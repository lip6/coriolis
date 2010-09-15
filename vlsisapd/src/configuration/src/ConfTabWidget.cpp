

// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfTabWidget.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QApplication>

#include "vlsisapd/configuration/Configuration.h"
#include "vlsisapd/configuration/ParameterWidget.h"
#include "vlsisapd/configuration/ConfTabWidget.h"


namespace Cfg {

  using std::max;
  using std::cerr;
  using std::endl;
  using std::string;
  using std::vector;


// -------------------------------------------------------------------
// Class  :  "Cfg::ConfTabWidget".


  ConfTabWidget::ConfTabWidget ( const string& name, QWidget* parent )
    : QWidget    (parent)
    , _gridLayout(new QGridLayout())
    , _columns   (2)
    , _rowsCount (new int[_columns])
    , _parameters()
  {
    for ( int i=0 ; i<_columns ; ++i ) _rowsCount[i] = 0;

    setObjectName ( name.c_str() );

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addLayout  ( _gridLayout );
    vLayout->addStretch ();

    setLayout ( vLayout );
  }


  ConfTabWidget::~ConfTabWidget ()
  {
    delete [] _rowsCount;

    for ( size_t i=0 ; i<_parameters.size() ; ++i )
      _parameters[i]->deleteLater();
  }


  int  ConfTabWidget::_getMaxRowCount ()
  {
    int maxrow = _rowsCount[0];
    for ( int column=1 ; column<_columns ; ++column )
      maxrow = max ( maxrow, _rowsCount[column] );

    return maxrow;
  }


  int  ConfTabWidget::_alignMaxRowCount ()
  {
    int maxrow = _gridLayout->rowCount() + 1;
    for ( int column=0 ; column<_columns ; ++column )
      _rowsCount[column] = maxrow;

    return maxrow;
  }


  void  ConfTabWidget::addRuler ()
  {
    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    _gridLayout->addWidget ( separator, _getMaxRowCount(), 0, 1, 4 );
    _alignMaxRowCount ();
  }


  QFont& ConfTabWidget::getParentBoldFont ()
  {
    ConfigurationWidget* cw = rparent<ConfigurationWidget*> ( this );
    return cw->getBoldFont();
  }


  void  ConfTabWidget::addTitle ( const string& title )
  {
    QLabel* label = new QLabel();
    label->setText ( title.c_str() );
    label->setFont ( getParentBoldFont() );

    _gridLayout->addWidget ( label, _getMaxRowCount(), 0, 1, 4, Qt::AlignLeft );
    _alignMaxRowCount ();
  }


  void  ConfTabWidget::addSection ( const string& section, int column )
  {
    QLabel* label = new QLabel();
    label->setText ( section.c_str() );
    label->setFont ( getParentBoldFont() );

    int row = _rowsCount[column];
    if ( (column >= _columns) or ( column < 0) ) {
      cerr << "[WARNING] Column " << column << " is out of bound (" << _columns << ")." << endl;
      if ( column < 0 ) column = 0;
      else              column = _columns-1;
    }

    _gridLayout->addWidget ( label, row, column*2, 1, 2, Qt::AlignLeft );
    _rowsCount[column]++;
  }


  ParameterWidget* ConfTabWidget::addParameter ( Parameter*         parameter
                                               , const std::string& label
                                               , int                column
                                               , int                span
                                               , int                flags )
  {
    ConfigurationWidget* cw = rparent<ConfigurationWidget*> ( this );
    ParameterWidget*     pw = cw->find(parameter);

    if ( pw != NULL ) {
      cerr << "[ERROR] Parameter <" << parameter->getId() << "> already added." << endl;
      return pw;
    }

    int row = _rowsCount[column];
    if ( (column >= _columns) or ( column < 0) ) {
      cerr << "[WARNING] Column " << column << " is out of bound (" << _columns << ")." << endl;
      if ( column < 0 ) column = 0;
      else              column = _columns-1;
    }
    if ( span < 0 ) span = 1;
    if ( (column+span > _columns) ) span = _columns - column;
    if ( span > 1 ) _alignMaxRowCount ();

    int qspan = 2*span-1;
  //cerr << parameter->getId() << " span:" << span << " " << qspan << endl;

    pw = new ParameterWidget ( this, parameter, label, flags );
    _gridLayout->addWidget ( pw->getLabelWidget(), row, column*2  , Qt::AlignRight );
    _gridLayout->addWidget ( pw->getValueWidget(), row, column*2+1, 1, qspan, Qt::AlignLeft  );

    connect ( this, SIGNAL(updateParameters()), pw, SLOT(updateValue()) );

    _rowsCount[column]++;

    if ( span > 1 ) _alignMaxRowCount ();

    return pw;
  }


}  // End of Cfg namespace.
