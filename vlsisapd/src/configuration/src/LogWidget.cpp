
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
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LogWidget.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <set>
#include  <string>
#include  <QCloseEvent>
#include  <QLabel>
#include  <QPushButton>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QFrame>

#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/LogWidget.h"


namespace Cfg {

  using std::set;
  using std::string;
  using std::cerr;
  using std::endl;


  LogWidget::LogWidget ( QWidget* parent )
    : QDialog      (parent)
    , _message     (new QLabel())
    , _contButton  (new QPushButton())
    , _cancelButton(new QPushButton())
  {
    setModal      ( true );
    setWindowTitle( tr("<Configuration Message>") );
    setToolTip    ( tr("You should better follow these instructions...") );

    _message->setTextFormat ( Qt::RichText );
    _message->setText       ( "<b>Oups! I did it again!</b>" );

    QLabel* ok = new QLabel ();
    ok->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::MinimumExpanding );
    ok->setPixmap     ( QPixmap(":/images/warning.png") );
    ok->setStyleSheet ( "QLabel { background-color: #FF9999;"
                        "         padding:          5px }" );

    _contButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    _contButton->setText       ( tr("Continue") );

    _cancelButton->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
    _cancelButton->setText       ( tr("Cancel") );

    QHBoxLayout* hLayout2 = new QHBoxLayout ();
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( _contButton, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );
    hLayout2->addWidget  ( _cancelButton, Qt::AlignCenter );
    hLayout2->addStretch ( 1 );

    QVBoxLayout* vLayout1 = new QVBoxLayout ();
    vLayout1->setContentsMargins ( 10, 10, 10, 10 );
    vLayout1->setSpacing ( 0 );
    vLayout1->addWidget  ( _message, Qt::AlignCenter );
    vLayout1->addSpacing ( 10 );
    vLayout1->addLayout  ( hLayout2, Qt::AlignCenter );

    QHBoxLayout* hLayout1 = new QHBoxLayout ();
    hLayout1->setSizeConstraint  ( QLayout::SetFixedSize );
    hLayout1->setContentsMargins ( 0, 0, 0, 0 );
    hLayout1->addWidget          ( ok );
    hLayout1->addLayout          ( vLayout1 );

    setLayout ( hLayout1 );

    connect ( _contButton  , SIGNAL(clicked()), this, SLOT(accept()) );
    connect ( _cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
  }


  void  LogWidget::closeEvent ( QCloseEvent* event )
  {
    event->ignore ();
  }


  void  LogWidget::updateLogs ( unsigned int mask )
  {
    Configuration* configuration = Configuration::get();
    QString        contents;
    bool           enableCont = true;

    for ( size_t ibit=0 ; ibit < 32 ; ++ibit ) {
      unsigned int bit    = 1 << ibit;
      const char*  header = NULL;

      switch ( bit ) {
        case Configuration::LogRestart:
          header = "<b>Program needs restart for these parameters to take effect:</b><br>";
          break;
        case Configuration::LogNeedExist:
          header = "<b>Those parameters needs file/path to exists:</b><br>";
          break;
      }

      if ( (header == NULL) or not (mask & bit) ) continue;
      mask &= ~bit;

      const set<Configuration::LogEntry>& logs = configuration->getLogs(bit);

      if ( not logs.empty() ) {
        switch ( bit ) {
          case Configuration::LogRestart: break;
          case Configuration::LogNeedExist: enableCont = false; break;
        }

        contents += header;

        set<Configuration::LogEntry>::const_iterator ientry = logs.begin();
        for ( ; ientry != logs.end() ; ++ientry ) {
          Parameter* p = configuration->getParameter((*ientry).getId());
          if ( p != NULL ) {
            WidgetDescription* desc = configuration->getLayout().getWidget ( p->getId() );

            contents += "&nbsp;&nbsp;&nbsp;";
            if ( desc != NULL ) {
              contents += desc->getLabel().c_str();
              contents += "&nbsp;&nbsp;";
            }
            contents += "<tt>&lt;";
            contents += p->getId().c_str();
            contents += "&gt;</tt>: ";
            if ( p->asString().size() > 30 ) contents += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
            contents += p->asString().c_str();
            contents += "<br>";
          }
        }
      }
    }
    _message->setText ( contents );
    _contButton->setEnabled ( enableCont );
  }


}  // End of Hurricane Namespace.
