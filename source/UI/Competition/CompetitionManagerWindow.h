#ifndef COMPETITIONMANAGERWINDOW_H
#define COMPETITIONMANAGERWINDOW_H

#include <QDialog>
#include "StartList/StartListModel.h"
#include "../../competitions/AbstractCompetitionManager.h"

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

public:
    AbstractCompetitionManager *getManager() const;
    short getType() const;
    void setType(short newType);
};

#endif // COMPETITIONMANAGERWINDOW_H
