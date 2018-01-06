// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ViewsWidget.cpp"                             |
// +-----------------------------------------------------------------+


#include <sstream>
#include <QFontMetrics>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QAction>
#include <QModelIndex>
#include "hurricane/viewer/Graphics.h"
#include "crlcore/ViewsModel.h"
#include "crlcore/ViewsWidget.h"
#include "crlcore/CellDatas.h"
#include "crlcore/LibraryManager.h"


namespace CRL {

  using namespace std;
  using Hurricane::Graphics;


  ViewsWidget::ViewsWidget ( QWidget* parent )
    : QWidget        (parent)
    , _baseModel     (new ViewsModel(this))
    , _view          (new QTableView(this))
    , _rowHeight     (20)
    , _note          (new QLabel())
  {
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_QuitOnClose, false );
    setContextMenuPolicy( Qt::ActionsContextMenu );

    _note->setTextFormat( Qt::RichText );

    _rowHeight = QFontMetrics( Graphics::getFixedFont() ).height() + 4;

    _view->setShowGrid            ( true );
    _view->setAlternatingRowColors( true );
    _view->setSelectionMode       ( QAbstractItemView::NoSelection );
    _view->setSelectionBehavior   ( QAbstractItemView::SelectRows );
    _view->setSortingEnabled      ( true );
    _view->setModel               ( _baseModel );
    _view->setMinimumSize         ( 150, 50 );
    _view->resizeColumnToContents ( 0 );

    QHeaderView* horizontalHeader = _view->horizontalHeader();
    horizontalHeader->setStretchLastSection( true );

    QHeaderView* verticalHeader = _view->verticalHeader();
    verticalHeader->setVisible( false );
    verticalHeader->setDefaultSectionSize( _rowHeight );

    QLabel* title = new QLabel( "Views" );
    title->setFont( Graphics::getFixedFont(QFont::Bold,false,false,+1) );

    QFrame* separator = new QFrame ();
    separator->setFrameShape ( QFrame::HLine );
    separator->setFrameShadow( QFrame::Sunken );

    QCheckBox* viewerCheckBox = new QCheckBox ();
    viewerCheckBox->setText ( tr("Open in new Viewer") );

    QPushButton* loadButton = new QPushButton ();
    loadButton->setText    ( "Load" );
    loadButton->setDefault ( true );

    ostringstream os;
    for ( auto iloader : CellLoaders::get()->getLoaders() ) {
      string comment = iloader->getComment();
      string format  = iloader->getFormat ();
      os << "<b>." << format << "</b> ";
      for ( size_t i=comment.size()+format.size() ; i<27 ; ++i ) os << ".";
      os << " " << comment << "<br>";

    }

    QLabel* formatsComments = new QLabel ();
    formatsComments->setFont      ( Graphics::getFixedFont() );
    formatsComments->setTextFormat( Qt::RichText );
    formatsComments->setText      ( os.str().c_str() );

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget ( title, 0, Qt::AlignHCenter );
    vLayout->addWidget ( _view );
    vLayout->addWidget ( formatsComments );
    vLayout->addStretch();
    vLayout->addWidget ( _note );
    vLayout->addWidget ( separator );
    vLayout->addWidget ( viewerCheckBox );
    vLayout->addWidget ( loadButton, 0, Qt::AlignHCenter );
    setLayout( vLayout );

    connect( loadButton    , SIGNAL(clicked     ()   ), _baseModel, SLOT(loadCell    ()   ) );
    connect( viewerCheckBox, SIGNAL(stateChanged(int)), _baseModel, SLOT(setNewViewer(int)) );
  }


  void  ViewsWidget::setCellDatas ( CellDatas* datas )
  {
    _baseModel->setCellDatas( datas );
    _view->resizeColumnToContents( 0 );

    if ( datas and datas->getState() and datas->getState()->isInMemory() ) {
      string  message 
        = "The Cell<br>"
          "<b>&lt;" + datas->getName() + "&gt;</b><br>"
          " is already loaded in memory.<br>"
          " You can only open it in a<br>"
          "(new) viewer";
      _note->setText( message.c_str() );
    } else {
      _note->setText( "" );
    }
  }


}  // CRL namespace.
