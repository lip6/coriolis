#ifndef __TECHNOROW_H
#define __TECHNOROW_H

#include <QWidget>

class QPushButton;
class QLineEdit;

class TechnoRow : public QWidget {
    Q_OBJECT

    public:
        TechnoRow (int id, QWidget *parent =0);
        ~TechnoRow () {};

        QString getName();
        QString getFile();
        void setName(QString&);
        void setFile(QString&);

        int           _id;
        QLineEdit*    _name;
        QLineEdit*    _file;
        QPushButton*  _suppress;
        QPushButton*  _browse;

    signals:
        void suppressed(int);
    
    public slots:
        void suppress();
        void chooseFile();
};

#endif
