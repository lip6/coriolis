#ifndef _ANALOGICVIEWER_H
#define _ANALOGICVIEWER_H

#include <QMainWindow>

namespace Hurricane {
  class Library;
  class Transistor;
  class CellWidget;
}
using namespace Hurricane;

class AnalogicViewer : public QMainWindow {
    Q_OBJECT
    public:
        AnalogicViewer(Library* library);
    private slots:
        void lvalueChanged(int value);
        void wvalueChanged(int value);
        void transistorTypeChanged(int value);

    private:
        AnalogicViewer();
        CellWidget* _cellWidget;
        Transistor* _transistor;
};


#endif /* _ANALOGICVIEWER_H */
