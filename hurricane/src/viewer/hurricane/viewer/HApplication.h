// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./HApplication.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include <QApplication>


namespace Hurricane {


  class HApplication : public QApplication {
      Q_OBJECT;
    public:
                    HApplication ( int& argc, char** argv );
                    HApplication ( int& argc, char** argv, bool GUIenabled );
//                  HApplication ( int& argc, char** argv, Type type );
#if defined(Q_WS_X11)
                    HApplication ( Display*, Qt::HANDLE visual=0, Qt::HANDLE colormap=0 );
                    HApplication ( Display*, int& argc, char** argv, Qt::HANDLE visual=0, Qt::HANDLE colormap=0 );
#endif
      virtual bool  notify       ( QObject*, QEvent* );
              void  sharedCtor   ();
  };


} // End of Hurricane namespace.

