#ifndef _CAPACITORVIEWER_H
#define _CAPACITORVIEWER_H

#include <QMainWindow>

namespace Hurricane {
  class Library;
  class CellWidget;
}
using namespace Hurricane;

class Capacitor;

class CapacitorViewer : public QMainWindow {
    Q_OBJECT
    public:
        CapacitorViewer(Library* library);
    private slots:
        void lvalueChanged(int value);
        void wvalueChanged(int value);

    private:
        CapacitorViewer();
        CellWidget* _cellWidget;
        Capacitor* _capacitor;
};


#endif /* _CAPACITORVIEWER_H */
