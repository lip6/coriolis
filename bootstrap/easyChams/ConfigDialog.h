#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class QLabel;
class QStringList;

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    ConfigDialog();
    inline void setCompileProgram(QString);
    inline void setSourceRootDirectory(QString);
    inline void setHasHurricane(bool);
    inline void setHasIo(bool);
    inline void setHasTest(bool);
    inline void setHasCrlcore(bool);
    inline void setHasChams(bool);
    inline void setHasHurricaneAMS(bool);

public slots:
    void chooseCProgram();
    void chooseSourceRD();
    void doConfig();

signals:
    void configDone (QStringList list);

private:
    QLineEdit* _compileE;
    QLineEdit* _sourceE;
    QCheckBox* _hurCB;
    QCheckBox* _ioCB; 
    QCheckBox* _testCB;
    QCheckBox* _crlCB;
    QCheckBox* _chamsCB;
    QCheckBox* _hurAMSCB;
};

inline void ConfigDialog::setCompileProgram      (QString cProgram  ) { _compileE->setText(cProgram); };
inline void ConfigDialog::setSourceRootDirectory (QString rDirectory) { _sourceE->setText(rDirectory); };

inline void ConfigDialog::setHasHurricane(bool state) { 
    if (state) 
        _hurCB->setCheckState(Qt::Checked);
    else 
        _hurCB->setCheckState(Qt::Unchecked);
};

inline void ConfigDialog::setHasIo(bool state) { 
    if (state) 
        _ioCB->setCheckState(Qt::Checked);
    else 
        _ioCB->setCheckState(Qt::Unchecked);
};

inline void ConfigDialog::setHasTest(bool state) { 
    if (state) 
        _testCB->setCheckState(Qt::Checked);
    else 
        _testCB->setCheckState(Qt::Unchecked);
};

inline void ConfigDialog::setHasCrlcore(bool state) { 
    if (state) 
        _crlCB->setCheckState(Qt::Checked);
    else 
        _crlCB->setCheckState(Qt::Unchecked);
};

inline void ConfigDialog::setHasChams(bool state) { 
    if (state) 
        _chamsCB->setCheckState(Qt::Checked);
    else 
        _chamsCB->setCheckState(Qt::Unchecked);
};

inline void ConfigDialog::setHasHurricaneAMS(bool state) {
    if (state) 
        _hurAMSCB->setCheckState(Qt::Checked);
    else 
        _hurAMSCB->setCheckState(Qt::Unchecked);
};
#endif
