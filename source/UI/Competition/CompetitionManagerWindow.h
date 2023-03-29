#ifndef COMPETITIONMANAGERWINDOW_H
#define COMPETITIONMANAGERWINDOW_H

#include <QDialog>
#include "JumperCompetitionResultsWidget.h"
#include "StartList/StartListModel.h"
#include "Results/ResultsTableModel.h"
#include "../../competitions/AbstractCompetitionManager.h"
#include "../../simulator/JumpManipulator.h"

namespace Ui {
class CompetitionManagerWindow;
}

class CompetitionManagerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionManagerWindow(AbstractCompetitionManager * manager = nullptr,  QWidget *parent = nullptr);
    ~CompetitionManagerWindow();

    void updateToBeatDistanceLabel();
    void updateToAdvanceDistanceLabel();
    void updatePointsToTheLeaderLabel();

private:
    Ui::CompetitionManagerWindow *ui;
    AbstractCompetitionManager * manager;
    short type;

    StartListModel * startListModel;
    ResultsTableModel * resultsTableModel;
    JumpManipulator currentInputJumpManipulator;

    JumperCompetitionResultsWidget * jumperResultsWidget;

public:
    AbstractCompetitionManager *getManager() const;
    short getType() const;
    void setType(short newType);
    JumpManipulator getCurrentInputJumpManipulator() const;
    void setCurrentInputJumpManipulator(const JumpManipulator &newCurrentInputJumpManipulator);

private slots:
    void on_pushButton_jump_clicked();
    void on_tableView_results_doubleClicked(const QModelIndex &index);
    void on_pushButton_generateNewWinds_clicked();
    void on_pushButton_windsGeneratorSettings_clicked();
};

#endif // COMPETITIONMANAGERWINDOW_H
