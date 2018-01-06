// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/OpenBlobDialog.h"      |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_OPEN_BLOB_DIALOG_H
#define HURRICANE_OPEN_BLOB_DIALOG_H

#include <QDialog>

class QLineEdit;


namespace Hurricane {


  class OpenBlobDialog : public QDialog {
      Q_OBJECT;
    public:
      static  bool           runDialog        ( QWidget* parent, QString& name );
              const QString  getBlobName      () const;
    protected:
                             OpenBlobDialog   ( QWidget* parent=NULL );
    protected:
              QLineEdit*     _lineEdit;
  };


} // Hurricane namespace.

#endif  // HURRICANE_OPEN_BLOB_DIALOG_H
