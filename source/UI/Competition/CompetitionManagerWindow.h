#ifndef COMPETITIONMANAGERWINDOW_H
#define COMPETITIONMANAGERWINDOW_H

#include <QDialog>
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

private:
    Ui::CompetitionManagerWindow *ui;
    AbstractCompetitionManager * manager;
    short type;

    StartListModel * startListModel;
    ResultsTableModel * resultsTableModel;
    JumpManipulator currentInputJumpManipulator;

public:
    AbstractCompetitionManager *getManager() const;
    short getType() const;
    void setType(short newType);
    JumpManipulator getCurrentInputJumpManipulator() const;
    void setCurrentInputJumpManipulator(const JumpManipulator &newCurrentInputJumpManipulator);

private slots:
    void on_pushButton_jump_clicked();
};

#endif // COMPETITIONMANAGERWINDOW_H
