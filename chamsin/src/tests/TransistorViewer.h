#ifndef _TRANSISTORVIEWER_H
#define _TRANSISTORVIEWER_H

#include <QMainWindow>

namespace Hurricane {
  class Library;
  class CellWidget;
}
using namespace Hurricane;

class MetaTransistor;

class TransistorViewer : public QMainWindow {
    Q_OBJECT
    public:
        TransistorViewer(Library* library);
    private slots:
        void lvalueChanged(int value);
        void wvalueChanged(int value);
        void transistorTypeChanged(int value);
        void mvalueChanged(int value);

    private:
        TransistorViewer();
        CellWidget* _cellWidget;
        MetaTransistor* _transistor;
};


#endif /* _TRANSISTORVIEWER_H */
