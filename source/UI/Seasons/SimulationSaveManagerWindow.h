#ifndef SIMULATIONSAVEMANAGERWINDOW_H
#define SIMULATIONSAVEMANAGERWINDOW_H

#include <QMainWindow>
#include "../DatabaseEditor/DatabaseItemsListView.h"
#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "CalendarEditor/CalendarEditorWidget.h"
#include "../../seasons/SimulationSave.h"

namespace Ui {
class SimulationSaveManagerWindow;
}

class SimulationSaveManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationSaveManagerWindow(SimulationSave * save, QWidget *parent = nullptr);
    ~SimulationSaveManagerWindow();

    void showJumperAndHillsEditingHelp();

private:
    Ui::SimulationSaveManagerWindow *ui;

    SimulationSave * simulationSave;

    DatabaseItemsListView * jumpersListView;
    JumperEditorWidget * jumperEditor;

    DatabaseItemsListView * hillsListView;
    HillEditorWidget * hillEditor;

    DatabaseItemsListView * rulesListView;
    CompetitionRulesEditorWidget * rulesEditor;

    CalendarEditorTableModel * calendarTableModel;
    CalendarEditorWidget * calendarEditor;
};

#endif // SIMULATIONSAVEMANAGERWINDOW_H
