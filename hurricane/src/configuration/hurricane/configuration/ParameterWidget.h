// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ParameterWidget.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <QObject>
class QLabel;


namespace Cfg {

  class Parameter;
  class ConfTabWidget;
  class ConfigurationWidget;


// -------------------------------------------------------------------
// Class  :  "Cfg::ParameterWidget".

  class ParameterWidget : public QObject {
      Q_OBJECT;
    public:
      enum Flags { UseSpinBox=0x1000, IsFileName=0x2000, IsPathName=0x4000 };
    public:
                                   ParameterWidget        ( ConfTabWidget* parent, Parameter*, const std::string& label, int flags );
      virtual                     ~ParameterWidget        ();
      inline  Parameter*           getParameter           ();
      inline  QLabel*              getLabelWidget         ();
      inline  QWidget*             getValueWidget         ();
      inline  ConfTabWidget*       getConfTabWidget       ();
              ConfigurationWidget* getConfigurationWidget ();
      inline  int                  getFlags               () const;
      inline  bool                 hasFlags               ( int mask ) const;
      inline  void                 setFlags               ( int mask );
      inline  void                 unsetFlags             ( int mask );
              void                 onUpdateValueCb        ( Parameter* );
    public slots:                                        
              void                 updateValue            ();
              void                 enableSlaves           ( int );
    public:
      ConfTabWidget* _confTabWidget;
      Parameter*     _parameter;
      QLabel*        _labelWidget;
      QWidget*       _valueWidget;
      int            _flags;
  };


// Inline Methods.
  inline Parameter*     ParameterWidget::getParameter     () { return _parameter; }
  inline QLabel*        ParameterWidget::getLabelWidget   () { return _labelWidget; }
  inline QWidget*       ParameterWidget::getValueWidget   () { return _valueWidget; }
  inline ConfTabWidget* ParameterWidget::getConfTabWidget () { return _confTabWidget; }
  inline int            ParameterWidget::getFlags         () const { return _flags; }
  inline bool           ParameterWidget::hasFlags         ( int mask ) const { return (_flags & mask); }
  inline void           ParameterWidget::setFlags         ( int mask )   { _flags |= mask; }
  inline void           ParameterWidget::unsetFlags       ( int mask )   { _flags &= ~mask; }


}  // Cfg namespace.
