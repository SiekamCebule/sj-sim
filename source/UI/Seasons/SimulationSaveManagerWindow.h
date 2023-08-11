#ifndef SIMULATIONSAVEMANAGERWINDOW_H
#define SIMULATIONSAVEMANAGERWINDOW_H

#include <QMainWindow>
#include "../DatabaseEditor/DatabaseItemsListView.h"
#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"
#include "../EditorWidgets/ClassificationEditorWidget.h"
#include "CalendarEditor/CalendarEditorWidget.h"
#include "Classifications/ClassificationResultsTableView.h"
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
    void fillNextCompetitionInformations();

    void setupClassificationsComboBox();
    void connectComboBox();
    void disconnectComboBox();

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

    DatabaseItemsListView * classificationsListView;
    int classificationsListViewActualElement;
    ClassificationEditorWidget * classificationEditor;

    ClassificationResultsTableView * classificationResultsTableView;


    void whenClassificationsComboBoxIndexChanged(int index);

public:
    SimulationSave *getSimulationSave() const;
private slots:
    void on_pushButton_competitionConfig_clicked();
    void on_pushButton_saveToFile_clicked();
    void on_pushButton_repairDatabase_clicked();
};

#endif // SIMULATIONSAVEMANAGERWINDOW_H
