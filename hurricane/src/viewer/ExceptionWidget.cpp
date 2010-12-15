
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ExceptionWidget.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QCloseEvent>
#include  <QLabel>
#include  <QPushButton>
#include  <QCheckBox>
#include  <QScrollArea>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QFrame>
#include  <QFont>
#include  <QFontMetrics>

#include  "hurricane/Exception.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/ExceptionWidget.h"


namespace Hurricane {


  ExceptionWidget::ExceptionWidget ( QWidget* parent )
    : QDialog   (parent)
    , _header   (new QLabel())
    , _message  (new QLabel())
    , _trace    (new QLabel())
    , _traceArea(new QScrollArea())
  {
    setAttribute  ( Qt::WA_DeleteOnClose );
    setModal      ( true );
    setWindowTitle( tr("<An Exception was Caught>") );
    setToolTip    ( tr("Ben mon cochon, t'es dans le bouillon") );

    _header->setMinimumWidth ( 200 );
    _header->setTextFormat   ( Qt::RichText );
    _header->setText         ( "<b>[ERROR]</b>" );

    _message->setTextFormat ( Qt::RichText );
    _message->setText       ( "<b>Oups! I did it again!</b>" );

    _trace->setTextFormat  ( Qt::RichText );
    _trace->setText        ( "<b>No program trace sets yet.</b>" );
    _trace->setSizePolicy  ( QSizePolicy::Ignored, QSizePolicy::Ignored );


    _traceArea->setWidget      ( _trace );
    _traceArea->hide           ();
  //_traceArea->setSizePolicy  ( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    _traceArea->setMinimumSize ( QSize(700,500) );

    QCheckBox* showTrace = new QCheckBox ();
    showTrace->setText    ( "Show Program Trace" );
    showTrace->setChecked ( false );

    QLabel* leftMargin = new QLabel ();
    leftMargin->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    leftMargin->setPixmap     ( QPixmap(":/images/gnome-core.png") );
    leftMargin->setStyleSheet ( "QLabel { background-color: #FF9999;"
                        "         padding:          5px }" );

    QPushButton* abortButton = new QPushButton ();
    abortButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    abortButton->setText       ( tr("Abort") );

    QPushButton* contButton = new QPushButton ();
    contButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    contButton->setText       ( tr("Try to Continue") );

    QFrame* separator = new QFrame ();
    separator->setFrameShape  ( QFrame::HLine );
    separator->setFrameShadow ( QFrame::Sunken );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( contButton, Qt::AlignCenter );
    hLayout2->addStretch ( 4 );
    hLayout2->addWidget  ( abortButton, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );

    QVBoxLayout* vLayout1 = new QVBoxLayout ();
    vLayout1->setContentsMargins ( 10, 10, 10, 10 );
    vLayout1->setSpacing ( 0 );
    vLayout1->addWidget  ( _header   , 0, Qt::AlignLeft );
    vLayout1->addWidget  ( _message  , 0, Qt::AlignLeft );
    vLayout1->addWidget  ( separator );
    vLayout1->addWidget  ( showTrace , 0, Qt::AlignLeft );
    vLayout1->addWidget  ( _traceArea, 0, Qt::AlignLeft );
    vLayout1->addSpacing ( 10 );
    vLayout1->addLayout  ( hLayout2  , Qt::AlignCenter );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->setSizeConstraint  ( QLayout::SetFixedSize );
    hLayout1->setContentsMargins ( 0, 0, 0, 0 );
    hLayout1->addWidget          ( leftMargin );
    hLayout1->addLayout          ( vLayout1 );

    setLayout      ( hLayout1 );
  //setMinimumSize ( QSize(400,150) );
    setSizePolicy  ( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );

    connect ( contButton , SIGNAL(clicked())        , this, SLOT(accept()) );
    connect ( abortButton, SIGNAL(clicked())        , this, SLOT(reject()) );
    connect ( showTrace  , SIGNAL(stateChanged(int)), this, SLOT(_showTrace(int)) );
  }


  void  ExceptionWidget::closeEvent ( QCloseEvent* event )
  {
    event->ignore ();
  }


  void  ExceptionWidget::setMessage ( const QString& message )
  {
    QString contents = message;

    if ( contents.startsWith("[ERROR]")) {
      contents.remove ( 0, 8 );
      _header->setText ("<b>[ERROR]</b>");
    } else if ( contents.startsWith("[WARNING]")) {
      contents.remove ( 0, 10 );
      _header->setText ("<b>[WARNING]</b>");
    } else
      _header->setText ("<b>[UNKNOW]</b>");

    _message->setText ( contents );
  }


  void  ExceptionWidget::setTrace ( const QString& where )
  {
    QFont        font     = Graphics::getFixedFont(QFont::Bold);
    QFontMetrics metrics  = QFontMetrics ( font );
    QSize        textSize = metrics.size ( 0, where );

  //textSize.setWidth  ( textSize.width () / 2  );
    textSize.setHeight ( textSize.height() + 70 );

    _trace->setText ( where );
    _trace->resize  ( textSize );
  }


  void  ExceptionWidget::_showTrace ( int state )
  {
    if ( state == Qt::Checked ) _traceArea->show ();
    else                        _traceArea->hide ();
  }


}  // End of Hurricane Namespace.
