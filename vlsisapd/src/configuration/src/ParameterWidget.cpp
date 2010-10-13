
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
// |  C++ Module  :       "./ParameterWidget.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  "boost/bind.hpp"
#include  <QLabel>
#include  <QSpinBox>
#include  <QCheckBox>
#include  <QComboBox>
#include  <QLineEdit>
#include  <QIntValidator>
#include  <QDoubleValidator>
#include  "vlsisapd/configuration/Parameter.h"
#include  "vlsisapd/configuration/FilePathEdit.h"
#include  "vlsisapd/configuration/ParameterWidget.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::boolalpha;
  using std::string;
  using std::vector;
  using std::ostringstream;


  ParameterWidget::ParameterWidget ( QObject* parent, Parameter* parameter, const std::string& label, int flags )
    : QObject     (parent)
    , _parameter  (parameter)
    , _labelWidget(new QLabel())
    , _valueWidget(NULL)
    , _flags      (flags)
  {
    setObjectName ( _parameter->getId().c_str() );

    _labelWidget->setText ( label.c_str() );

    switch ( _parameter->getType() ) {
      case Parameter::Unknown:
        break;
      case Parameter::String:
        {
          QLineEdit* lineEdit = NULL;
          if      ( hasFlags(IsFileName) ) lineEdit = new FilePathEdit ( FilePathEdit::FileName );
          else if ( hasFlags(IsPathName) ) lineEdit = new FilePathEdit ( FilePathEdit::PathName );
          else                             lineEdit = new QLineEdit();

          lineEdit->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Fixed );
          lineEdit->setMinimumWidth ( 375 );
          lineEdit->setText ( QString("%1").arg(_parameter->asString().c_str()) );
          _valueWidget = lineEdit;
        }
        break;
      case Parameter::Bool:
        {
          QCheckBox* checkBox = new QCheckBox();
          checkBox->setChecked ( _parameter->asBool() );
          _valueWidget = checkBox;

          connect ( checkBox, SIGNAL(stateChanged(int)), this, SLOT(enableSlaves(int)) );
        }
        break;
      case Parameter::Int:
        {
          if ( hasFlags(UseSpinBox) ) {
            QSpinBox* spinBox = new QSpinBox();
            spinBox->setValue ( _parameter->asInt() );
            _valueWidget = spinBox;
          } else {
            QLineEdit* lineEdit = new QLineEdit();
            lineEdit->setValidator ( new QIntValidator(this) );
            lineEdit->setText ( QString("%1").arg(_parameter->asString().c_str()) );
            _valueWidget = lineEdit;
          }

          int bounds =     (int)(_parameter->hasFlags(Parameter::HasMin))
                       + 2*(int)(_parameter->hasFlags(Parameter::HasMax));

          if ( bounds ) {
            ostringstream toolTip;
            toolTip << "A value in interval ";
            switch ( bounds ) {
              case 1:
                toolTip << "[" << _parameter->getMinInt() << ":<no bound>]";
                break;
              case 2:
                toolTip << "[<no bound>:" << _parameter->getMaxInt() << "]";
                break;
              case 3:
                toolTip << "[" << _parameter->getMinInt()
                        << ":" << _parameter->getMaxInt() << "]";
                break;
            }
            _valueWidget->setToolTip ( toolTip.str().c_str() );
          }
        }
        break; 
      case Parameter::Double:
      case Parameter::Percentage:
        {
          double mult = (_parameter->getType() == Parameter::Double) ? 1.0 : 100.0;

          QLineEdit* lineEdit = new QLineEdit();
          lineEdit->setValidator ( new QDoubleValidator(this) );
          lineEdit->setText ( QString("%1").arg(_parameter->asDouble()*mult) );
        //lineEdit->setValue     ( _parameter->asPercentage() );

          int bounds =     (int)(_parameter->hasFlags(Parameter::HasMin))
                       + 2*(int)(_parameter->hasFlags(Parameter::HasMax));

          if ( bounds ) {
            ostringstream toolTip;
            toolTip << "A value in interval ";


            switch ( bounds ) {
              case 1:
                toolTip << "[" << (_parameter->getMinDouble()*mult) << ":<no bound>]";
                break;
              case 2:
                toolTip << "[<no bound>:" << (_parameter->getMaxDouble()*mult) << "]";
                break;
              case 3:
                toolTip << "[" << (_parameter->getMinDouble()*mult)
                        << ":" << (_parameter->getMaxDouble()*mult) << "]";
                break;
            }
            lineEdit->setToolTip ( toolTip.str().c_str() );
          }
          _valueWidget = lineEdit;
        }
        break;  
      case Parameter::Enumerate:
        {
          int        currentValue = _parameter->asInt();
          int        currentIndex = 0;
          QComboBox* comboBox     = new QComboBox();
        //comboBox->setSizePolicy ( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
          const vector<Parameter::EnumValue>& values = _parameter->getValues();
          for ( size_t i=0 ; i != values.size() ; ++i ) {
            comboBox->addItem ( values[i]._label.c_str(), values[i]._value );
            if ( values[i]._value == currentValue ) currentIndex = i;
          }
          comboBox->setCurrentIndex ( currentIndex );
          _valueWidget = comboBox;
        }
        break;
    }

    string valueId = _parameter->getId() + ".edit";
    _valueWidget->setObjectName ( valueId.c_str() );

    _parameter->registerCb ( boost::bind(&ParameterWidget::onUpdateValueCb,this,_1) );
  }


  void  ParameterWidget::updateValue ()
  {
    if ( _parameter->getType() == Parameter::String )
      {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        _parameter->setString ( lineEdit->displayText().toStdString() );
      }
    else if ( _parameter->getType() == Parameter::Bool )
      {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(_valueWidget);
        _parameter->setBool ( checkBox->isChecked() );
      }
    else if ( _parameter->getType() == Parameter::Int )
      {
        if ( hasFlags(UseSpinBox) ) {
          QSpinBox* spinBox = qobject_cast<QSpinBox*>(_valueWidget);
          int       value   = spinBox->value();

          if ( not _parameter->setInt(value) )
            spinBox->setValue ( _parameter->asInt() );
        } else {
          bool       success;
          QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
          int        value    = lineEdit->displayText().toInt ( &success );

          if ( not success or not _parameter->setInt(value) )
            lineEdit->setText ( _parameter->asString().c_str() );
        }
      }
    else if ( _parameter->getType() == Parameter::Double )
      {
        bool       success;
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        double     value    = lineEdit->displayText().toFloat ( &success );

        if ( not success or not _parameter->setDouble(value) )
          lineEdit->setText ( _parameter->asString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Percentage )
      {
        bool       success;
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        double     value    = lineEdit->displayText().toFloat ( &success );

        if ( not success or not _parameter->setPercentage(value) ) {
          lineEdit->setText ( _parameter->asPercentageString().c_str() );
        }
      }
    else if ( _parameter->getType() == Parameter::Enumerate )
      {
        QComboBox* comboBox = qobject_cast<QComboBox*>(_valueWidget);

        const vector<Parameter::EnumValue>& values = _parameter->getValues();
        _parameter->setInt ( values[comboBox->currentIndex()]._value );
      }
  }


  void  ParameterWidget::onUpdateValueCb ( Parameter* )
  {
  //cerr << "onUpdateValue: " << _parameter->getId() << " -> " << _parameter->asString() << endl;

    if ( _parameter->getType() == Parameter::String )
      {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        lineEdit->setText ( _parameter->asString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Bool )
      {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(_valueWidget);
        checkBox->setChecked ( _parameter->asBool() );
      }
    else if ( _parameter->getType() == Parameter::Int )
      {
        if ( hasFlags(UseSpinBox) ) {
          QSpinBox* spinBox = qobject_cast<QSpinBox*>(_valueWidget);
          spinBox->setValue ( _parameter->asInt() );
        } else {
          QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
          lineEdit->setText ( _parameter->asString().c_str() );
        }
      }
    else if ( _parameter->getType() == Parameter::Double )
      {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        lineEdit->setText ( _parameter->asString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Percentage )
      {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        lineEdit->setText ( _parameter->asPercentageString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Enumerate )
      {
        QComboBox* comboBox = qobject_cast<QComboBox*>(_valueWidget);

        int   value = _parameter->asInt();
        const vector<Parameter::EnumValue>& values = _parameter->getValues();
        for ( size_t ival=0 ; ival < values.size() ; ++ival ) {
          if ( values[ival]._value == value ) {
            comboBox->setCurrentIndex ( ival );
            break;
          }
        }
      }
  }


#if BACKUP
  void  ParameterWidget::onUpdateValueCb ( Parameter* )
  {
    if ( _parameter->getType() == Parameter::String )
      {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        _parameter->setString ( lineEdit->displayText().toStdString() );
      }
    else if ( _parameter->getType() == Parameter::Bool )
      {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(_valueWidget);
        _parameter->setBool ( checkBox->isChecked() );
      }
    else if ( _parameter->getType() == Parameter::Int )
      {
        if ( hasFlags(UseSpinBox) ) {
          QSpinBox* spinBox = qobject_cast<QSpinBox*>(_valueWidget);
          int       value   = spinBox->value();

          if ( _parameter->checkValue(value) ) _parameter->setInt ( value );
          else spinBox->setValue ( _parameter->asInt() );
        } else {
          bool       success;
          QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
          int        value    = lineEdit->displayText().toInt ( &success );

          if ( success ) success = _parameter->checkValue ( value );
          if ( success ) _parameter->setInt ( value );
          else           lineEdit->setText ( _parameter->asString().c_str() );
        }
      }
    else if ( _parameter->getType() == Parameter::Double )
      {
        bool       success;
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        double     value    = lineEdit->displayText().toFloat ( &success );
            
        if ( success ) success = _parameter->checkValue ( value );
        if ( success ) _parameter->setDouble ( value );
        else           lineEdit->setText ( _parameter->asString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Percentage )
      {
        bool       success;
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(_valueWidget);
        double     value    = lineEdit->displayText().toFloat ( &success );

        if ( success ) success = _parameter->checkValue ( value/100.0 );
        if ( success ) _parameter->setPercentage ( value );
        else           lineEdit->setText ( _parameter->asString().c_str() );
      }
    else if ( _parameter->getType() == Parameter::Enumerate )
      {
        QComboBox* comboBox = qobject_cast<QComboBox*>(_valueWidget);

        const vector<Parameter::EnumValue>& values = _parameter->getValues();
        _parameter->setInt ( values[comboBox->currentIndex()]._value );
      }
  }
#endif


  void  ParameterWidget::enableSlaves ( int state )
  {
    ConfigurationWidget* cw = rparent<ConfigurationWidget*> ( this );
    if ( cw == NULL ) {
      cerr << "[ERROR] ParameterWidget::enableSlaves(): Cannot find parent ConfigurationWidget." << endl;
      return;
    }

    bool enabled = ( state != Qt::Unchecked );

    const vector<string>& slaveIds = _parameter->getSlaves();
    for ( size_t islave=0 ; islave<slaveIds.size() ; ++islave ) {
      ParameterWidget* slave = cw->find(slaveIds[islave]);
      if ( slave == NULL ) continue;

      slave->getLabelWidget()->setEnabled ( enabled );
      slave->getValueWidget()->setEnabled ( enabled );
    }
  }


}  // End of Cfg namespace.
