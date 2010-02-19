#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <vector>

#include <QDialog>

class QVBoxLayout;
class QPushButton;
class QLineEdit;
class QStringList;
class TechnoRow;

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    ConfigDialog();
    void setProgram(QString);
    void setLibrary(QString);

signals:
    void configDone(QStringList&, std::vector<std::pair<QString, QString> >&);

public slots:
    void addRow();
    void addRow(QString&, QString&);
    void removeRow(int);
    void doConfig();
    void chooseProgram();
    void chooseLibrary();


private:
    QLineEdit*    _programE;
    QLineEdit*    _libraryE;
    static int    _rowID;
    QPushButton*  _add;
    QVBoxLayout*  _rowLayout;
    std::vector<TechnoRow*> _rows;
};

#endif
