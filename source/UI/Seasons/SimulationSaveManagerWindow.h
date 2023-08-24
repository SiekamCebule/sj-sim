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
#include "ItemModels/CompetitionsArchiveListModel.h"
#include "ItemModels/ClassificationsArchiveListModel.h"
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

    void setupNextSeasonConfigButton();
    void configNextSeason();

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

    CompetitionsArchiveListModel * competitionsArchiveModel;
    ClassificationsArchiveListModel * classificationsArchiveModel;
    ClassificationResultsTableView * archiveClassificationResults;

    void whenClassificationsComboBoxIndexChanged(int index);

public:
    SimulationSave *getSimulationSave() const;
private slots:
    void on_pushButton_competitionConfig_clicked();
    void on_pushButton_saveToFile_clicked();
    void on_pushButton_repairDatabase_clicked();
    void on_comboBox_archiveSeason_currentIndexChanged(int index);
    void on_listView_competitionsArchive_doubleClicked(const QModelIndex &index);
    void on_listView_classificationsArchive_doubleClicked(const QModelIndex &index);
    void on_pushButton_jumperStats_clicked();
    void on_pushButton_clicked();
};

#endif // SIMULATIONSAVEMANAGERWINDOW_H
