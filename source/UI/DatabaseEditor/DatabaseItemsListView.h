#ifndef DATABASEITEMSLISTVIEW_H
#define DATABASEITEMSLISTVIEW_H

#include <QWidget>
#include <QAction>
#include <QVector>
#include <QListView>
#include "../../simulator/Jumper.h"
#include "../../simulator/Hill.h"
#include "../../simulator/Team.h"
#include "../../competitions/CompetitionRules.h"
#include "ListModels/CompetitionRulesListModel.h"
#include "ListModels/HillsListModel.h"
#include "ListModels/JumpersListModel.h"
#include "ListModels/TeamsListModel.h"

namespace Ui {
class DatabaseItemsListView;
}

class DatabaseItemsListView : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseItemsListView(int type, bool allowInserting = true, QWidget *parent = nullptr);
    ~DatabaseItemsListView();

    enum Type{
        JumperItems,
        HillItems,
        CompetitionRulesItems,
        TeamItems
    };

    void setupListModel();

signals:
    void listViewDoubleClicked(const QModelIndex &index);
    void up();
    void down();

private:
    Ui::DatabaseItemsListView *ui;
    int type;

    QVector<Jumper> * jumpers;
    QVector<Hill> * hills;
    QVector<CompetitionRules> * competitionRules;
    QVector<Team> * teams;

    QAbstractListModel * listModel;

    QAction * insertAction;
    QAction * removeAction;
    QAction * upAction;
    QAction * downAction;

    bool allowInserting;
    bool teamsEditing;

private slots:
    void onInsertActionTriggered();
    void onRemoveActionTriggered();
    void onUpActionTriggered();
    void onDownActionTriggered();

public:
    QVector<Jumper> *getJumpers() const;
    void setJumpers(QVector<Jumper> *newJumpers);
    QVector<Hill> *getHills() const;
    void setHills(QVector<Hill> *newHills);
    QVector<CompetitionRules> *getCompetitionRules() const;
    void setCompetitionRules(QVector<CompetitionRules> *newCompetitionRules);
    QVector<Team> *getTeams() const;
    void setTeams(QVector<Team> *newTeams);
    bool getAllowInserting() const;
    void setAllowInserting(bool newAllowInserting);
    QListView * getListView();
    QAbstractListModel *getListModel();
};

#endif // DATABASEITEMSLISTVIEW_H
