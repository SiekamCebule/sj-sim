#ifndef SIMULATIONSAVEMANAGERWINDOW_H
#define SIMULATIONSAVEMANAGERWINDOW_H

#include <QMainWindow>
#include "../DatabaseEditor/DatabaseItemsListView.h"
#include "../EditorWidgets/JumperEditorWidget.h"
#include "../../seasons/SimulationSave.h"

namespace Ui {
class SimulationSaveManagerWindow;
}

class SimulationSaveManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationSaveManagerWindow(SimulationSave * simulationSave, QWidget *parent = nullptr);
    ~SimulationSaveManagerWindow();

private:
    Ui::SimulationSaveManagerWindow *ui;

    SimulationSave * simulationSave;

    DatabaseItemsListView * jumpersListView;
    JumperEditorWidget * jumperEditor;

public:

};

#endif // SIMULATIONSAVEMANAGERWINDOW_H
