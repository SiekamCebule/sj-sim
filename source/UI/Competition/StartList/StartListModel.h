#ifndef STARTLISTMODEL_H
#define STARTLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "../../../simulator/Jumper.h"
#include "../../../simulator/Team.h"
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

    QVector<bool> * completedJumps;
    // Pisze, które indexy nie startowały/są zdyskwalifikowane
    QSet<int> * hasDNS;
    QSet<int> * hasDSQ;

    short type;
    enum Type{
        IndividualCompetiton,
        TeamCompetition
    };

public:
    QVector<bool> getCompletedJumps() const;
    void setCompletedJumps(const QVector<bool> &newCompletedJumps);
    QVector<Jumper *> *getJumpers() const;
    void setJumpers(QVector<Jumper *> *newJumpers);
    QVector<Team *> *getTeams() const;
    void setTeams(QVector<Team *> *newTeams);
    short getType() const;
    void setType(short newType);
    QSet<int> *getHasDNS() const;
    void setHasDNS(QSet<int> *newHasDNS);
    QSet<int> *getHasDSQ() const;
    void setHasDSQ(QSet<int> *newHasDSQ);
};

#endif // STARTLISTMODEL_H
