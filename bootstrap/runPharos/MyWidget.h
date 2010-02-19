#ifndef __MY_WIDGET_H
#define __MY_WIDGET_H

#include <vector>

#include <QWidget>
#include <QProcess>
#include <QString>

class QComboBox;
class QPushButton;
class QTextEdit;
class QSettings;
class QStringList;

class MyWidget : public QWidget {
    Q_OBJECT

    public:
        MyWidget (QWidget *parent =0);
        ~MyWidget () {};

        QComboBox*    _technoCB;
        QPushButton*  _execute;
        QPushButton*  _stop;
        QPushButton*  _show;
        QPushButton*  _clear;
        QTextEdit*    _console;

        QProcess*     _myProcess;

        QSettings*    _settings;
        QString       _program;
        QString       _library;
        int           _nbTechno;
        std::vector<std::pair<QString, QString> > _technos;

    private:
        void readSettings();
        void writeSettings();
        void runConfigDialog(bool);
        void updateComboBox();

    public slots:
        void about();
        void reconfig();
        void execute();
        void stop();
        void finished(int exitCode, QProcess::ExitStatus exitStatus);
        void showOrHide();
        void updateError();
        void updateText();
        void updateConfig(QStringList&, std::vector<std::pair<QString, QString> >&);
};

#endif
