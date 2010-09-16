
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
// |  C++ Header  :  "./vlsisapd/configuration/FilePathEdit.h"       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __CFG_FILE_LINE_EDIT__
#define __CFG_FILE_LINE_EDIT__

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


}  // End of Cfg namespace.

#endif // __CFG_FILE_LINE_EDIT__
