
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/FilePathEdit.h"      |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <QLineEdit>
class QToolButton;
class QFileDialog;


namespace Cfg {


  class FilePathEdit : public QLineEdit
  {
      Q_OBJECT;
    public:
      enum Type { FileName=0x1, PathName=0x2 };
    public:
           FilePathEdit   ( Type type=FileName, std::string filter="", QWidget* parent=NULL );
    protected:
      void resizeEvent    ( QResizeEvent* );
    private slots:
      void updateLineEdit ();
    private:
      QToolButton* _chooseButton;
      QFileDialog* _filePathDialog;
      Type         _type;
      std::string  _filter;
  };


}  // Cfg namespace.
