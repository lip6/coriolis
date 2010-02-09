#include <iostream>
using namespace std;

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QByteArray>
#include <QTextEdit>
#include <QSettings>
#include <QMessageBox>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QScrollBar>

#include "CompilationWidget.h"
#include "CompileHighlighter.h"
#include "ConfigDialog.h"

CompilationWidget::CompilationWidget(QWidget *parent) 
    : QWidget(parent)
    , _cProgram ("")
    , _sourceRD ("")
    , _hasHurricane (false)
    , _hasIo (false)
    , _hasTest (false)
    , _hasCrlcore (false)
    , _hasChams (false)
    , _hasHurricaneAMS (false) {

    setStyleSheet("font-weight: normal;");

    _hurricane    = new QCheckBox ( "hurricane"   , this );
    _io           = new QCheckBox ( "io"          , this );
    _test         = new QCheckBox ( "test"        , this );
    _crlcore      = new QCheckBox ( "crlcore"     , this );
    _chams        = new QCheckBox ( "chams"       , this );
    _hurricaneAMS = new QCheckBox ( "hurricaneAMS", this );   

    readSettings();    

    QVBoxLayout* vBoxOutilsGroup = new QVBoxLayout();
    vBoxOutilsGroup->addWidget(_hurricane);
    vBoxOutilsGroup->addWidget(_io);
    vBoxOutilsGroup->addWidget(_test);
    vBoxOutilsGroup->addWidget(_crlcore);
    vBoxOutilsGroup->addWidget(_chams);
    vBoxOutilsGroup->addWidget(_hurricaneAMS);
    if (!_hasHurricane)    _hurricane->setVisible(false);
    if (!_hasIo)           _io->setVisible(false);
    if (!_hasTest)         _test->setVisible(false);
    if (!_hasCrlcore)      _crlcore->setVisible(false);
    if (!_hasChams)        _chams->setVisible(false);
    if (!_hasHurricaneAMS) _hurricaneAMS->setVisible(false);

    QGroupBox* outilsGroup = new QGroupBox ( "Outils", this );
    outilsGroup->setLayout(vBoxOutilsGroup);

    _mode = new QComboBox (this);
    QStringList modes;
    modes << "Release" << "Debug";
    _mode->addItems(modes);
    _static = new QCheckBox ("static", this);
    _svnUp  = new QCheckBox ("svn update", this);
    _doc    = new QCheckBox ("build doc", this);
    _clean  = new QCheckBox ("clean build", this);

    QVBoxLayout* vBoxOptionsGroup = new QVBoxLayout();
    vBoxOptionsGroup->addWidget(_mode);
    vBoxOptionsGroup->addWidget(_static);
    vBoxOptionsGroup->addWidget(_doc);
    vBoxOptionsGroup->addWidget(_svnUp);
    vBoxOptionsGroup->addWidget(_clean);

    QGroupBox* optionsGroup = new QGroupBox ( "Options", this );
    optionsGroup->setLayout(vBoxOptionsGroup);

    _compile = new QPushButton ( "Compile !", this );
    connect ( _compile, SIGNAL(clicked()), this, SLOT(compile()) );

    _progressBar = new QProgressBar ( this );
    _progressBar->setRange(0,100);
    _progressBar->setTextVisible(true);
    connect ( this, SIGNAL(progress(int)), _progressBar, SLOT(setValue(int)) );

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(outilsGroup);
    vBoxLayout->addWidget(optionsGroup);
    vBoxLayout->addWidget(_compile);
    vBoxLayout->addWidget(_progressBar);

    QWidget* menu = new QWidget ( this );
    menu->setLayout(vBoxLayout);

    _console = new QTextEdit(this);
    _console->setFixedSize(800,400);
    CompileHighlighter* highlight = new CompileHighlighter(_console->document());

    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(menu);
    hBoxLayout->addWidget(_console);
    setLayout(hBoxLayout);
    setWindowTitle("Easy compile for chams");

}

void CompilationWidget::compile() {
    QStringList arguments;

    _progressBar->reset();
    _console->clear();

    _myProcess = new QProcess(this);
    QStringList env = QProcess::systemEnvironment();
    //QString currentInstallRD = _installRD;
    //if (_static->isChecked()) currentInstallRD += "/"+_mode->currentText()+".Static";
    //else                      currentInstallRD += "/"+_mode->currentText()+".Shared";
    //cerr << _installRD.toStdString() << endl;
    //env << "HURRICANE_TOP=" + currentInstallRD;
    //env << "CHAMS_TOP=" + currentInstallRD;
    //env << "IO_TOP=" + currentInstallRD;
    //env << "DYLD_LIBRARY_PATH=" + currentInstallRD + "/lib";
    //_myProcess->setEnvironment(env);
    connect(_myProcess, SIGNAL(readyReadStandardError()) , this, SLOT(updateError()));
    connect(_myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(updateText()));
    connect(_myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(updateExit(int, QProcess::ExitStatus)));
    //arguments << "-s " + _sourceRD;
    //arguments << "-i " + currentInstallRD;
    //arguments << "-b " + _buildRD;
    arguments << "-p" + _sourceRD;
    arguments << "-m" + _mode->currentText();
    if (_static->isChecked())       arguments << "-s";
    if (_svnUp->isChecked())        arguments << "-u";
    if (_doc->isChecked())          arguments << "-d";
    if (_clean->isChecked())        arguments << "-c";
    if (_hurricane->isChecked())    arguments << "-t hurricane";
    if (_io->isChecked())           arguments << "-t io";
    if (_test->isChecked())         arguments << "-t test";
    if (_crlcore->isChecked())      arguments << "-t crlcore";
    if (_chams->isChecked())        arguments << "-t chams";
    if (_hurricaneAMS->isChecked()) arguments << "-t hurricaneAMS";
    
    //for (int i = 0; i < arguments.size(); ++i)
    //      cout << arguments.at(i).toLocal8Bit().constData() << endl;

    _myProcess->start(_cProgram, arguments);
}

void CompilationWidget::readSettings() {
    _settings = new QSettings ( "chams", "easyChams" );

    if ( !_settings->contains("Compilation program") ) {
        QMessageBox::warning(this, tr("easyChams"), tr("It seems you do not have configured the application. Let's configure it now."), QMessageBox::Ok, QMessageBox::Ok);
        runConfigDialog(true);
    }
    else {
        _cProgram        = _settings->value("Compilation program").toString();
        _sourceRD        = _settings->value("Source root").toString();
        _hasHurricane    = _settings->value("Hurricane").toBool();
        _hasIo           = _settings->value("Io").toBool();
        _hasTest         = _settings->value("Test").toBool();
        _hasCrlcore      = _settings->value("Crlcore").toBool();
        _hasChams        = _settings->value("Chams").toBool();
        _hasHurricaneAMS = _settings->value("HurricaneAMS").toBool();
    }
}

void CompilationWidget::runConfigDialog(bool first) {
    ConfigDialog* cfg = new ConfigDialog;
    connect ( cfg, SIGNAL(configDone(QStringList)), this, SLOT(updateConfig(QStringList)) );
    if (!first) {
        cfg->setCompileProgram(_cProgram);
        cfg->setSourceRootDirectory(_sourceRD);
        cfg->setHasHurricane(_hasHurricane);
        cfg->setHasIo(_hasIo);
        cfg->setHasTest(_hasTest);
        cfg->setHasCrlcore(_hasCrlcore);
        cfg->setHasChams(_hasChams);
        cfg->setHasHurricaneAMS(_hasHurricaneAMS);
    }
    cfg->exec();
    writeSettings();
}

void CompilationWidget::reconfig() {
    runConfigDialog(false);
}

void CompilationWidget::writeSettings() {
    _settings->setValue("Compilation program", _cProgram);
    _settings->setValue("Source root"        , _sourceRD);
    _settings->setValue("Hurricane"          , _hasHurricane);
    _settings->setValue("Io"                 , _hasIo);
    _settings->setValue("Test"               , _hasTest);
    _settings->setValue("Crlcore"            , _hasCrlcore);
    _settings->setValue("Chams"              , _hasChams);
    _settings->setValue("HurricaneAMS"       , _hasHurricaneAMS);
}

void CompilationWidget::updateConfig(QStringList list) {
    _cProgram        = list.value(0);                          // First   value is compilation program
    _sourceRD        = list.value(1);                          // Second  value is source root directory
    _hasHurricane    = (list.value(2) == "ON") ? true : false; // Third   value is hasHurricane
    _hasIo           = (list.value(3) == "ON") ? true : false; // Forth   value is hasIo
    _hasTest         = (list.value(4) == "ON") ? true : false; // Fifth   value is hasTest
    _hasCrlcore      = (list.value(5) == "ON") ? true : false; // Sixth   value is hasCrlcore
    _hasChams        = (list.value(6) == "ON") ? true : false; // Seventh value is hasChams
    _hasHurricaneAMS = (list.value(7) == "ON") ? true : false; // Eight   value is hasHurricaneAMS
    // updates Tools visibility :
    if (_hasHurricane)     { _hurricane->setVisible(true); }
    else                   { _hurricane->setVisible(false); _hurricane->setCheckState(Qt::Unchecked); }
    if (_hasIo)            { _io->setVisible(true); }
    else                   { _io->setVisible(false); _io->setCheckState(Qt::Unchecked); }
    if (_hasTest)          { _test->setVisible(true); }
    else                   { _test->setVisible(false); _test->setCheckState(Qt::Unchecked); }
    if (_hasCrlcore)       { _crlcore->setVisible(true); }
    else                   { _crlcore->setVisible(false); _crlcore->setCheckState(Qt::Unchecked); }
    if (_hasChams)         { _chams->setVisible(true); }
    else                   { _chams->setVisible(false); _chams->setCheckState(Qt::Unchecked); }
    if (_hasHurricaneAMS)  { _hurricaneAMS->setVisible(true); }
    else                   { _hurricaneAMS->setVisible(false); _hurricaneAMS->setCheckState(Qt::Unchecked); }
}


void CompilationWidget::updateError() {
    QByteArray data = _myProcess->readAllStandardError();
    _console->insertPlainText(QString(data));
    int end = _console->verticalScrollBar()->maximum();
    _console->verticalScrollBar()->setValue(end);
}

void CompilationWidget::updateText() {
    QByteArray data = _myProcess->readAllStandardOutput();
    QString str(data);
    QRegExp rx = QRegExp ( "([0-9]+)%" );
    if ( str.contains(rx) ) {
        bool ok;
        int value = rx.cap(1).toInt ( &ok, 10 );
        emit(progress(value));
    }
    _console->insertPlainText(QString(data));
    int end = _console->verticalScrollBar()->maximum();
    _console->verticalScrollBar()->setValue(end);
}

void CompilationWidget::updateExit(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::NormalExit) {
    _console->append("Completed Successfully");
  }
  else {
    _console->append("Error");
  }
}

void CompilationWidget::about()
{
    QMessageBox::about(this, tr("About easyChams"), tr("<p><h1 align=\"center\">easyChams</h1></p>"
                                                       "<p>This tool allows every one to easily update and compile chams project's sources.</p>"
                                                       "<p align=\"center\">Version 0.6<br>29/01/2010<br>by <b>D.Dupuis</b></p>"));
}
