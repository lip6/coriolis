#ifndef __COMPILATION_WIDGET_H
#define __COMPILATION_WIDGET_H

#include <QWidget>
#include <QProcess>

class QCheckBox;
class QComboBox;
class QTextEdit;
class QPushButton;
class QProgressBar;
class QSettings;
class QStringList;
class QAction;

class CompilationWidget : public QWidget {
    Q_OBJECT

    public:
        CompilationWidget (QWidget *parent =0);
        ~CompilationWidget () {};

    private:
        QCheckBox*    _hurricane;
        QCheckBox*    _io;
        QCheckBox*    _test;
        QCheckBox*    _crlcore;
        QCheckBox*    _chams;
        QCheckBox*    _hurricaneAMS;
        QComboBox*    _mode;
        QCheckBox*    _static;
        QCheckBox*    _svnUp;
        QCheckBox*    _doc;
        QCheckBox*    _clean;
        QPushButton*  _compile;
        QProgressBar* _progressBar;

        QTextEdit*    _console;
        QProcess*     _myProcess;

        QSettings*    _settings;
        QString       _cProgram;
        QString       _sourceRD;
        bool          _hasHurricane;
        bool          _hasIo;
        bool          _hasTest;
        bool          _hasCrlcore;
        bool          _hasChams;
	    bool          _hasHurricaneAMS;

    private:
        void readSettings();
        void writeSettings();
        void runConfigDialog(bool first);

    public slots:
        void compile();
        void updateError();
        void updateText();
        void updateExit(int exitCode, QProcess::ExitStatus exitStatus);
        void updateConfig(QStringList list);
        void reconfig();
        void about();

    signals:
        void progress(int value);
};

#endif
