
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
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :           Damien.Dupuis@soc.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./FilePathEdit.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <QToolButton>
#include  <QStyle>
#include  <QFileDialog>

#include  "vlsisapd/configuration/FilePathEdit.h"


namespace Cfg {


  FilePathEdit::FilePathEdit ( Type type, std::string filter, QWidget* parent )
	: QLineEdit      (parent)
    , _chooseButton  (new QToolButton(this))
    , _filePathDialog(NULL)
    , _type          (type)
    , _filter        (filter)
  {
	QPixmap pixmap(":/images/choose.png");

	_chooseButton->setIcon      (QIcon(pixmap));
	_chooseButton->setIconSize  (pixmap.size());
	_chooseButton->setCursor    (Qt::ArrowCursor);
	_chooseButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
	_chooseButton->setToolTip   (tr( (type&FileName)?"Choose file ...":"Choose directory ..." ));

	connect ( _chooseButton, SIGNAL(clicked()), this, SLOT(updateLineEdit()) );

	setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(_chooseButton->sizeHint().width() + 1));
  }


  void FilePathEdit::resizeEvent(QResizeEvent *)
  {
	QSize sz = _chooseButton->sizeHint ();
    int l, t, r, b;
    getContentsMargins (&l, &t, &r, &b );
	_chooseButton->move ( rect().right() - r - sz.width(), (rect().bottom() + 2 - sz.height())/2 );
  }


  void FilePathEdit::updateLineEdit()
  {
    QString result;

#if defined(__APPLE__)
    if (_type == FileName) {
      result = QFileDialog::getOpenFileName ( this, tr("Choose File"), "", tr(_filter.c_str()) );
    } else {
      result = QFileDialog::getExistingDirectory ( this
                                                 , tr("Choose Directory")
                                                 , ""
                                                 , QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                                                 );
    }
#else
    if ( _filePathDialog == NULL ) {
      _filePathDialog = new QFileDialog ( this, tr((_type&FileName)?"Choose File":"Choose Directory") );
      if (_type == FileName) {
        _filePathDialog->setFileMode ( QFileDialog::ExistingFile );
      } else {
        _filePathDialog->setOption ( QFileDialog::ShowDirsOnly );
        _filePathDialog->setOption ( QFileDialog::DontResolveSymlinks );
      }
      _filePathDialog->setViewMode      ( QFileDialog::Detail );
      _filePathDialog->selectNameFilter ( tr(_filter.c_str()) );
    }

    if ( _filePathDialog->exec() == QDialog::Accepted )
      result = _filePathDialog->selectedFiles().at(0);
#endif

    if ( not result.isEmpty() ) setText ( result );
  }


}  // End of Cfg namespace.
