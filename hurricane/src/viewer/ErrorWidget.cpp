// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ErrorWidget.cpp"                        |
// +-----------------------------------------------------------------+


#include <unistd.h>
#include <csignal>
#include <memory>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QFont>
#include <QFontMetrics>
#include "hurricane/Bug.h"
#include "hurricane/Error.h"
#include "hurricane/Error.h"
#include "hurricane/TextTranslator.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/ErrorWidget.h"


namespace Hurricane {


  bool  ErrorWidget::run ( const QString& message, const QString& trace )
  {
    ErrorWidget* ew = new ErrorWidget();

    ew->setMessage( message );
    if (not trace.isEmpty())
      ew->setTrace( trace );

    if (ew->exec() == QDialog::Rejected) {
      cerr << "\n[BUG] A Python exception was catched:\n" << endl;
      cerr << TextTranslator::toTextTranslator().translate(  message.toStdString() ) << endl;
      cerr << "\n[Backtrace]" << endl;
      cerr << TextTranslator::toTextTranslator().translate( trace.toStdString() ) << endl;
      return false;
    }
    return true;
  }


  ErrorWidget::ErrorWidget ( QWidget* parent )
    : QDialog   (parent)
    , _header   (new QLabel())
    , _message  (new QTextEdit())
    , _trace    (new QTextEdit())
  {
    setAttribute  ( Qt::WA_DeleteOnClose );
    setModal      ( true );
    setWindowTitle( tr("<A Python exception was raised>") );
    setToolTip    ( tr("Caramba! Encore rate!") );

  //_header->setMinimumWidth( Graphics::isHighDpi() ? 1500 : 400 );
    _header->setTextFormat  ( Qt::RichText );
    _header->setText        ( "<b>[ERROR]</b>" );

    QString labelBackground = _header->palette().color( _header->backgroundRole() ).name();
    _message->setFrameStyle          ( QFrame::NoFrame|QFrame::Plain );
    _message->setStyleSheet          ( QString("p, li { white-space: wrap; } * { background-color: %1 }").arg(labelBackground) );
    _message->setTextInteractionFlags( Qt::TextSelectableByMouse );
    _message->setAcceptRichText      ( false );
    _message->setLineWrapMode        ( QTextEdit::NoWrap );
    _message->setWordWrapMode        ( QTextOption::NoWrap );
    _message->setMinimumSize         ( Graphics::isHighDpi() ? QSize(2500,200) : QSize(1000,100) );
  //_message->setTextFormat          ( Qt::RichText );
    _message->setFont                ( Graphics::getFixedFont(QFont::Normal,false,false) );
    _message->setPlainText           ( "Oups! I did it again!" );

    _trace->setTextInteractionFlags ( Qt::TextBrowserInteraction );
    _trace->setAcceptRichText       ( true );
    _trace->setStyleSheet           ( "* { white-space: pre; }" );
    _trace->setPlainText            ( "<b>No program trace sets yet.</b>" );
    _trace->setMinimumSize          ( Graphics::isHighDpi() ? QSize(2500,1000) : QSize(1000,500) );
    _trace->setFont                 ( Graphics::getFixedFont(QFont::Normal,false,false,-2) );
    _trace->setLineWrapMode         ( QTextEdit::FixedColumnWidth );
    _trace->setLineWrapColumnOrWidth( 140 );
    _trace->setSizePolicy           ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    _trace->setSizeAdjustPolicy     ( QAbstractScrollArea::AdjustToContents );
    _trace->hide                    ();

    QCheckBox* showTrace = new QCheckBox ();
    showTrace->setText   ( "Show Program Trace" );
    showTrace->setChecked( false );

    QLabel* leftMargin = new QLabel ();
    leftMargin->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    leftMargin->setPixmap    ( QPixmap(":/images/angry-birds-red.png").scaledToWidth( Graphics::isHighDpi() ? 200 : 80 ) );
    leftMargin->setStyleSheet( "QLabel { background-color: #FF9999;"
                        "         padding:          5px }" );

    QPushButton* abortButton = new QPushButton ();
    abortButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    abortButton->setText      ( tr("Abort") );

    QPushButton* contButton = new QPushButton ();
    contButton->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    contButton->setText      ( tr("Try to Continue") );

    QFrame* separator = new QFrame ();
    separator->setFrameShape ( QFrame::HLine );
    separator->setFrameShadow( QFrame::Sunken );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch( 1 );
    hLayout2->addWidget ( contButton, Qt::AlignCenter );
    hLayout2->addStretch( 4 );
    hLayout2->addWidget ( abortButton, Qt::AlignCenter );
    hLayout2->addStretch( 1 );

    QVBoxLayout* vLayout1 = new QVBoxLayout ();
    vLayout1->setContentsMargins ( 10, 10, 10, 10 );
    vLayout1->setSpacing( 0 );
    vLayout1->addWidget ( _header   , 0, Qt::AlignLeft );
    vLayout1->addWidget ( _message  , 0, Qt::AlignLeft );
    vLayout1->addWidget ( separator );
    vLayout1->addWidget ( showTrace , 0, Qt::AlignLeft );
    vLayout1->addWidget ( _trace    , 0, Qt::AlignLeft );
    vLayout1->addSpacing( 10 );
    vLayout1->addLayout ( hLayout2  , Qt::AlignCenter );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
  //hLayout1->setSizeConstraint ( QLayout::SetFixedSize );
    hLayout1->setContentsMargins( 0, 0, 0, 0 );
    hLayout1->addWidget         ( leftMargin );
    hLayout1->addLayout         ( vLayout1 );

    setLayout         ( hLayout1 );
  //setMinimumSize    ( QSize(400,150) );
  //setSizePolicy     ( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding) );
    setSizeGripEnabled( true );

    connect( contButton , SIGNAL(clicked())        , this, SLOT(accept()) );
    connect( abortButton, SIGNAL(clicked())        , this, SLOT(reject()) );
    connect( showTrace  , SIGNAL(stateChanged(int)), this, SLOT(_showTrace(int)) );
  }


  void  ErrorWidget::closeEvent ( QCloseEvent* event )
  { event->ignore(); }


  void  ErrorWidget::setMessage ( const QString& message )
  {
    QString contents = message;
    _header->setText ("<b>[ERROR]</b>");
    _message->setPlainText( contents );
  }


  void  ErrorWidget::setTrace ( const QString& where )
  { 
    _trace->setPlainText( where );
    _trace->setStyleSheet( "* { white-space: pre; }" );
  //cerr << _trace->toHtml().toStdString() << endl;
  }


  void  ErrorWidget::_showTrace ( int state )
  {
    if (state == Qt::Checked) _trace->show();
    else                      _trace->hide();

    adjustPosition( NULL );
  }


}  // End of Hurricane Namespace.
