#ifndef STARTLISTMODEL_H
#define STARTLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "../../../simulator/Jumper.h"
#include "../../../simulator/Team.h"
#include "../../../competitions/StartListCompetitorStatus.h"
#include "../../../competitions/AbstractCompetitionManager.h"

class StartListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit StartListModel(AbstractCompetitionManager * manager, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QVector<Jumper *> * jumpers;
    QVector<Team *> * teams;

    QVector<StartListCompetitorStatus> * startListStatuses;

    short type;
public:
    enum Type{
        IndividualCompetiton,
        TeamCompetition
    };

    QVector<Jumper *> *getJumpers() const;
    void setJumpers(QVector<Jumper *> *newJumpers);
    QVector<Team *> *getTeams() const;
    void setTeams(QVector<Team *> *newTeams);
    short getType() const;
    void setType(short newType);
    QVector<StartListCompetitorStatus> *getStartListStatuses() const;
    void setStartListStatuses(QVector<StartListCompetitorStatus> *newStartListStatuses);
};

#endif // STARTLISTMODEL_H
