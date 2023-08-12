#ifndef KOGROUP_H
#define KOGROUP_H

#include <QVector>
#include <QJsonObject>
#include "../../simulator/Jumper.h"
#include "../../simulator/Team.h"
#include "../../utilities/ClassWithID.h"

class CompetitionInfo;

class KOGroup : public ClassWithID
{
public:
    KOGroup();

    static QJsonObject getJsonObject(KOGroup &group);
    static KOGroup getFromJson(const QJsonObject & json);

private:
    CompetitionInfo * competition;
    int competitionType;

    int number;

    //Aktywny jeden z tych vectorów w zależności competitionType
    QVector<Jumper *> jumpers;
    QVector<Team *> teams;
public:
    CompetitionInfo *getCompetition() const;
    void setCompetition(CompetitionInfo *newCompetition);
    int getCompetitionType() const;
    int getNumber() const;
    void setNumber(int newNumber);
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper *> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    QVector<Team *> & getTeamsReference();
    void setTeams(const QVector<Team *> &newTeams);
    void setCompetitionType(int newCompetitionType);
};

#endif // KOGROUP_H
