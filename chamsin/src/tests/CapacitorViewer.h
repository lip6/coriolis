#ifndef _CAPACITORVIEWER_H
#define _CAPACITORVIEWER_H

#include <QMainWindow>

namespace Hurricane {
  class Library;
  class Record;
  class CellWidget;
  class HPalette;
  class HMousePosition;
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
        void runInspectorOnDataBase();
        void runInspectorOnCell();

    private:
        CapacitorViewer();
        void createActions();
        void createMenus();
        void createLayout();
        void runInspector(Record* record);

        CellWidget* _cellWidget;
        Capacitor* _capacitor;
        QAction* _runInspectorOnDataBase;
        QAction* _runInspectorOnCell;
        QMenu* _viewMenu;
        QMenu* _toolsMenu;
        HPalette* _palette;
        HMousePosition* _mousePosition;
};


#endif /* _CAPACITORVIEWER_H */
