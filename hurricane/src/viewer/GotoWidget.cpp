
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GotoWidget.cpp"                         |
// +-----------------------------------------------------------------+


#include  <iostream>
#include  <sstream>
using namespace std;

#include  <QApplication>
#include  <QLabel>
#include  <QFrame>
#include  <QLineEdit>
#include  <QRegExpValidator>
#include  <QComboBox>
#include  <QPushButton>
#include  <QHBoxLayout>
#include  <QVBoxLayout>

#include  "hurricane/viewer/GotoWidget.h"


namespace Hurricane {


  GotoWidget::GotoWidget ( QWidget* parent )
    : QDialog      (parent)
    , _xyRegexp    ("(\\d+),(\\d+)")
    , _xyEdit      (new QLineEdit())
    , _apertureEdit(new QLineEdit())
    , _dbuMode     (new QComboBox())
    , _x           (0)
    , _y           (0)
    , _aperture    (0)
    , _hasXy       (false)
    , _hasAperture (false)
  {
    setModal       ( true );
    setWindowTitle ( "<Goto>" );
    setToolTip     ( "Center the view on that coordinates" );

    QRegExpValidator* xyValidator = new QRegExpValidator ( _xyRegexp, this );
    _xyEdit->setValidator ( xyValidator );

    QRegExp           apertureRegexp    ( "\\d+" );
    QRegExpValidator* apertureValidator = new QRegExpValidator ( apertureRegexp, this );
    _apertureEdit->setValidator ( apertureValidator );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    QLabel*      label    = new QLabel ();
    label->setTextFormat ( Qt::RichText );
    label->setText       ( "<b>X,Y:</b>" );

    hLayout1->addWidget ( label );
    hLayout1->addWidget ( _xyEdit );

    label = new QLabel ();
    label->setTextFormat ( Qt::RichText );
    label->setText       ( "<b>Aperture:</b>(Zoom)" );

    hLayout1->addWidget ( label );
    hLayout1->addWidget ( _apertureEdit );

    _dbuMode->addItem ( "DataBase"  , Db         );
    _dbuMode->addItem ( "Grid"      , Grid       );
    _dbuMode->addItem ( "Symbolic"  , Symbolic   );
    _dbuMode->addItem ( "Micrometer", Micrometer );
    _dbuMode->addItem ( "Nanometer" , Nanometer  );
    unsigned int    dbuStringMode = DbU::Symbolic;
    DbU::UnitPower  unitPower     = DbU::Nano;
    DbU::getStringMode( dbuStringMode, unitPower );
    switch ( dbuStringMode ) {
      default:
      case DbU::Db:       _dbuMode->setCurrentIndex( 0 ); break;
      case DbU::Grid:     _dbuMode->setCurrentIndex( 1 ); break;
      case DbU::Symbolic: _dbuMode->setCurrentIndex( 2 ); break;
      case DbU::Physical: 
        switch ( unitPower ) {
          default:
          case DbU::Micro: _dbuMode->setCurrentIndex( 3 ); break;
          case DbU::Nano:  _dbuMode->setCurrentIndex( 4 ); break;
        }
        break;
    }
    hLayout1->addWidget ( _dbuMode );
    _dbuMode->setCurrentIndex ( 0 );

    QPushButton* cancelButton = new QPushButton ();
    cancelButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    cancelButton->setText       ( tr("Cancel") );

    QPushButton* okButton = new QPushButton ();
    okButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    okButton->setText       ( tr("Goto") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( okButton, 0, Qt::AlignCenter );
    hLayout2->addStretch ( 4 );
    hLayout2->addWidget  ( cancelButton, 0, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );

    QFrame* hLine = new QFrame ();
    hLine->setFrameShape  ( QFrame::HLine );
    hLine->setFrameShadow ( QFrame::Sunken );

    QVBoxLayout* vLayout = new QVBoxLayout ();
  //vLayout->setSizeConstraint  ( QLayout::SetFixedSize );
    vLayout->addLayout ( hLayout1 );
    vLayout->addWidget ( hLine );
    vLayout->addLayout ( hLayout2 );
    setLayout ( vLayout );

    connect ( okButton    , SIGNAL(clicked()), this, SLOT(acceptAndUpdate()) );
    connect ( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  void  GotoWidget::acceptAndUpdate ()
  {
    if ( (_hasXy = _xyEdit->hasAcceptableInput()) ) {
      _xyRegexp.indexIn ( _xyEdit->displayText() );
      _x = toDbu ( _xyRegexp.cap(1).toDouble() );
      _y = toDbu ( _xyRegexp.cap(2).toDouble() );
    }
    if ( (_hasAperture = _apertureEdit->hasAcceptableInput()) ) {
      _aperture    = toDbu ( _apertureEdit->displayText().toDouble() );
      _hasAperture = (_aperture != 0 );
    }

    emit accept();
  }


  void  GotoWidget::changeDbuMode ( unsigned int mode, DbU::UnitPower p )
  {
    if ( mode == DbU::Physical ) {
      switch ( p ) {
        case DbU::Nano:  mode = Nanometer; break;
        default:
        case DbU::Micro: mode = Micrometer; break;
      }
    }

    int index = _dbuMode->findData ( mode );
    switch ( index ) {
      case Nanometer: 
      case Micrometer: 
      case Grid: 
      case Db:       _dbuMode->setCurrentIndex(index); break;
      default:
      case Symbolic: _dbuMode->setCurrentIndex(2); break;
    }

    ostringstream s;
    s << (long)fromDbu(_x) << "," << (long)fromDbu(_y);
    _xyEdit->setText ( s.str().c_str() );

    s.str("");
    s << (long)fromDbu(_aperture);
    _apertureEdit->setText ( s.str().c_str() );
  }


  DbU::Unit  GotoWidget::toDbu ( double d ) const
  {
    DbU::Unit unit;
    switch ( _dbuMode->currentIndex() ) {
      case Nanometer:  unit = DbU::grid(DbU::physicalToGrid(d,DbU::Nano )); break;
      case Micrometer: unit = DbU::grid(DbU::physicalToGrid(d,DbU::Micro)); break;
      default:
      case Symbolic:   unit = DbU::lambda(d);       break;
      case Grid:       unit = DbU::grid  (d);       break;
      case Db:         unit = DbU::db    ((long)d); break;
    }
    return unit;
  }


  double  GotoWidget::fromDbu ( DbU::Unit u ) const
  {
    double d;
    switch ( _dbuMode->currentIndex() ) {
      case Nanometer:  d = DbU::getPhysical(u,DbU::Nano); break;
      case Micrometer: d = DbU::getPhysical(u,DbU::Micro); break;
      default:
      case Symbolic:   d = DbU::getLambda(u); break;
      case Grid:       d = DbU::getGrid  (u); break;
      case Db:         d = DbU::getDb    (u); break;
    }
    return d;
  }


}  // End of Hurricane Namespace.
