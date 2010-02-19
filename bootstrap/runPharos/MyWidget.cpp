#include <iostream>
using namespace std;

#include <QWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QByteArray>
#include <QComboBox>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>

#include "MyWidget.h"
#include "ConfigDialog.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {
    
    setStyleSheet("font-weight: normal;");

    _technoCB = new QComboBox(this);
    _technoCB->setFixedWidth(120);
    readSettings();
    updateComboBox();

    _console = new QTextEdit(this);
    _console->setFixedSize(600,350);
    _console->setFontPointSize(10);

    _execute = new QPushButton ( "&Execute", this );
    _execute->setDefault(true);
    _stop = new QPushButton ( "&Stop", this );
    _stop->setEnabled(false);
    _show = new QPushButton ( "&Show console", this );
    _clear = new QPushButton ( "&Clear", this );

    QDialogButtonBox* buttons = new QDialogButtonBox();
    buttons->addButton ( _execute, QDialogButtonBox::RejectRole );
    buttons->addButton ( _stop   , QDialogButtonBox::AcceptRole );
    buttons->addButton ( _show   , QDialogButtonBox::ResetRole  );
    buttons->addButton ( _clear  , QDialogButtonBox::ResetRole  );

    connect ( _execute, SIGNAL(clicked()), this    , SLOT(execute())    );
    connect ( _stop   , SIGNAL(clicked()), this    , SLOT(stop())       );
    connect ( _show   , SIGNAL(clicked()), this    , SLOT(showOrHide()) );
    connect ( _clear  , SIGNAL(clicked()), _console, SLOT(clear())      );

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(buttons, Qt::AlignRight);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(_technoCB);
    layout->addWidget(_console);
    layout->addLayout(hLayout);
    setLayout(layout);
    _clear->hide();
    _console->hide();
    setWindowTitle("Run Pharos");
    setFixedSize(400,110);
}

void MyWidget::readSettings() {
    _settings = new QSettings("chams", "runPharos");

    if ( !_settings->contains("Program") ) {
        QMessageBox::warning(this, tr("runPharos"), tr("It seems you do not have configured the application. Let's configure it now."), QMessageBox::Ok, QMessageBox::Ok);
        runConfigDialog(true);
    } else {
        _program  = _settings->value("Program").toString();
        _library  = _settings->value("Library").toString();
        _nbTechno = _settings->value("NbTechno").toInt();
        _technos.clear();
        for ( int i = 0 ; i < _nbTechno ; i++ ) {
            QString name = QString("name_%1").arg(i);
            QString file = QString("file_%1").arg(i);
            _technos.push_back(pair<QString, QString>(_settings->value(name).toString(), _settings->value(file).toString()));
        }
    }
}

void MyWidget::runConfigDialog(bool first) {
    ConfigDialog* cfg = new ConfigDialog;
    connect(cfg, SIGNAL(configDone(QStringList&, std::vector<std::pair<QString, QString> >&)), this, SLOT(updateConfig(QStringList&, std::vector<std::pair<QString, QString> >&)));
    if(!first) {
        cfg->setProgram(_program);
        cfg->setLibrary(_library);
        // on enleve la row par defaut vide
        cfg->removeRow(0);
        // on rajoute toutes les technos lues dans les settings:
        for ( size_t i = 0 ; i < _technos.size() ; i++ ) {
            cfg->addRow(_technos[i].first, _technos[i].second);
        }
    }
    cfg->exec();
    writeSettings();
}

void MyWidget::reconfig() {
    runConfigDialog(false);
}

void MyWidget::writeSettings() {
    _settings->setValue("Program"      , _program);
    _settings->setValue("Library"      , _library);
    _settings->setValue("NbTechno"     , _nbTechno);
    for ( size_t i = 0 ; i < _technos.size() ; i++ ) {
        QString name = QString("name_%1").arg(i);
        QString file = QString("file_%1").arg(i);
        _settings->setValue(name, _technos[i].first);
        _settings->setValue(file, _technos[i].second);
    }
}

void MyWidget::updateConfig(QStringList& list, vector<pair<QString, QString> >& technos) {
    _program  = list.value(0);
    _library  = list.value(1);
    _technos.clear();
    for ( size_t i = 0 ; i < technos.size() ; i++ ) {
        _technos.push_back(pair<QString, QString>(technos[i].first, technos[i].second));
    }
    _nbTechno = _technos.size();
    updateComboBox();
}

void MyWidget::updateComboBox() {
    if (_technoCB->count() != 0)
        _technoCB->clear();
    QStringList choices;
    for ( size_t i = 0 ; i < _technos.size() ; i++ ) {
        choices << _technos[i].first;
    }
    _technoCB->addItems(choices);
}

void MyWidget::about() {
    QMessageBox::about(this, tr("About runPharos"), tr("<p><h1 align=\"center\">runPharos</h1></p>"
                                                       "<p>This tool allows every one to easily run pharos on a specified technology.</p>"
                                                       "<p align=\"center\">Version 1.0<br>19/02/2010<br>by <b>D.Dupuis</b></p>"));

}

void MyWidget::execute() {
    QStringList arguments;
    QString techno ("");
    QString name = _technoCB->currentText();
    for ( size_t i = 0 ; i < _technos.size() ; i++ ) {
        if ( _technos[i].first == name )
            techno = _technos[i].second;
    }
    if ( techno == "" )
        return;
    arguments <<  techno;

    _myProcess = new QProcess(this);
    QStringList env = QProcess::systemEnvironment();
    QString DYLD = QString("DYLD_LIBRARY_PATH=%1").arg(_library);
    QString LD = QString("LD_LIBRARY_PATH=%1").arg(_library);
    QString PP = QString("PYTHONPATH=%1/python").arg(_library);
    env << DYLD << LD << PP;
    _myProcess->setEnvironment(env);
    connect(_myProcess, SIGNAL(readyReadStandardError()) , this, SLOT(updateError()));
    connect(_myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(updateText()));
    connect(_myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)) );
    _myProcess->start(_program, arguments);
    _execute->setEnabled(false);
    _stop->setEnabled(true);
}

void MyWidget::stop() {
    _myProcess->terminate();
    _execute->setEnabled(true);
    _stop->setEnabled(false);
}

void MyWidget::showOrHide() {
    if ( _console->isHidden() ) {
        _show->setText("&Hide console");
        _console->show();
        _clear->show();
        setFixedSize(620,450);
    }
    else {
        _show->setText("&Show console");
        _console->hide();
        _clear->hide();
        setFixedSize(400,110);
    }
}

void MyWidget::updateError() {
    QByteArray data = _myProcess->readAllStandardError();
    _console->insertPlainText(QString(data));
}

void MyWidget::updateText() {
    QByteArray data = _myProcess->readAllStandardOutput();
    _console->insertPlainText(QString(data));
}

void MyWidget::finished(int exitCode, QProcess::ExitStatus exitStatus) {
    _execute->setEnabled(true);
    _stop->setEnabled(false);
}
