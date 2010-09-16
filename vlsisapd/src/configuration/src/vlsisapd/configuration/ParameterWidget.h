
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ParameterWidget.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __CFG_PARAMETER_WIDGET__
#define  __CFG_PARAMETER_WIDGET__

#include  <string>
#include  <QObject>
class QLabel;


namespace Cfg {

  class Parameter;


// -------------------------------------------------------------------
// Class  :  "Cfg::ParameterWidget".

  
  class ParameterWidget : public QObject {
      Q_OBJECT;
    public:
      enum Flags { UseSpinBox=0x1, IsFileName=0x2, IsPathName=0x4 };
    public:
                        ParameterWidget ( QObject* parent, Parameter*, const std::string& label, int flags );
      inline Parameter* getParameter    ();
      inline QLabel*    getLabelWidget  ();
      inline QWidget*   getValueWidget  ();
      inline int        getFlags        () const;
      inline bool       hasFlags        ( int mask ) const;
      inline void       setFlags        ( int mask );
      inline void       unsetFlags      ( int mask );
             void       updateValueCb   ( Parameter* );
    public slots:
             void       updateValue     ();
             void       enableSlaves    ( int );
    public:
      Parameter* _parameter;
      QLabel*    _labelWidget;
      QWidget*   _valueWidget;
      int        _flags;
  };


// Inline Methods.
  inline Parameter* ParameterWidget::getParameter   () { return _parameter; }
  inline QLabel*    ParameterWidget::getLabelWidget () { return _labelWidget; }
  inline QWidget*   ParameterWidget::getValueWidget () { return _valueWidget; }
  inline int        ParameterWidget::getFlags       () const { return _flags; }
  inline bool       ParameterWidget::hasFlags       ( int mask ) const { return (_flags & mask); }
  inline void       ParameterWidget::setFlags       ( int mask )   { _flags |= mask; }
  inline void       ParameterWidget::unsetFlags     ( int mask )   { _flags &= ~mask; }


}   // End of Cfg namespace.

#endif  // __CFG_PARAMETER_WIDGET__
