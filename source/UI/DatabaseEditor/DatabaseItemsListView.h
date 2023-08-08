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
#include "../../seasons/Classification.h"
#include "../../global/PointsForPlacesPreset.h"
#include "ListModels/CompetitionRulesListModel.h"
#include "ListModels/HillsListModel.h"
#include "ListModels/JumpersListModel.h"
#include "ListModels/ClassificationsListModel.h"
#include "ListModels/PointsForPlacesPresetsListModel.h"
#include "ListModels/SeasonJumpersListModel.h"

namespace Ui {
class DatabaseItemsListView;
}

class DatabaseItemsListView : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseItemsListView(int type, bool allowInserting = true, bool allowRemoving = true, bool allowMoving = true, QWidget *parent = nullptr);
    ~DatabaseItemsListView();

    enum Type{
        SeasonJumpersItems,
        JumperItems,
        HillItems,
        CompetitionRulesItems,
        ClassificationItems,
        PointsForPlacesPresetsItems
    };

    void setupListModel();
    void selectOnlyFirstRow();

signals:
    void listViewDoubleClicked(const QModelIndex &index);
    void listViewClicked(const QModelIndex &index);
    void up();
    void down();
    void insert();
    void remove();

private:
    Ui::DatabaseItemsListView *ui;
    int type;

    QVector<Jumper *> * seasonJumpers;
    QVector<Jumper> * jumpers;
    QVector<Hill> * hills;
    QVector<CompetitionRules> * competitionRules;
    QVector<Classification *> * classifications;
    QVector<PointsForPlacesPreset> * pointsForPlacesPresets;

    QAbstractListModel * listModel;

    QAction * insertAction;
    QAction * removeAction;
    QAction * upAction;
    QAction * downAction;

    bool allowInserting;
    bool allowRemoving;
    bool allowMoving;
    bool insertLast;
    int lastDoubleClickedIndex;

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
    QVector<PointsForPlacesPreset> *getPointsForPlacesPresets() const;
    void setPointsForPlacesPresets(QVector<PointsForPlacesPreset> *newPointsForPlacesPresets);
    int getLastDoubleClickedIndex() const;
    void setLastDoubleClickedIndex(int newLastDoubleClickedIndex);
    QVector<Jumper *> *getSeasonJumpers() const;
    void setSeasonJumpers(QVector<Jumper *> *newSeasonJumpers);
    int getType() const;
    void setType(int newType);
    bool getAllowMoving() const;
    void setAllowMoving(bool newAllowMoving);
    bool getInsertLast() const;
    void setInsertLast(bool newInsertLast);
    QVector<Classification *> *getClassifications() const;
    void setClassifications(QVector<Classification *> *newClassifications);
};

#endif // DATABASEITEMSLISTVIEW_H
