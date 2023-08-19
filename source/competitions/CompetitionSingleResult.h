#ifndef COMPETITIONSINGLERESULT_H
#define COMPETITIONSINGLERESULT_H

#include "../utilities/ClassWithID.h"
#include "../simulator/JumpData.h"
#include "../simulator/Team.h"
#include <QVector>
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class CompetitionInfo;

class CompetitionSingleResult : public ClassWithID
{
public:
    CompetitionSingleResult(Jumper * jumper = nullptr, int type = 0);
    CompetitionSingleResult(Team * team, int type = 0);
    inline bool operator<(const CompetitionSingleResult & second) {return pointsSum < second.getPointsSum();}
    //inline bool operator>(const CompetitionSingleResult & second) {return pointsSum > second.getPointsSum();}
    enum Type{
        IndividualResult,
        TeamResult
    };

    static CompetitionSingleResult getFromJson(QJsonObject obj, SeasonDatabaseObjectsManager *objectsManager);
    static QJsonObject getJsonObject(CompetitionSingleResult result);

protected:
    QVector<JumpData> jumps;
    double pointsSum;
    int position;

    int type;
    Jumper * jumper;
    Team * team;
    QVector<CompetitionSingleResult> teamJumpersResults;

public:
    void updatePointsSum();
    void updateTeamJumpersResults();
    CompetitionSingleResult * getTeamJumperResult(Jumper * jumper);

    QVector<JumpData> getJumps() const;
    QVector<JumpData> & getJumpsReference();
    void setJumps(const QVector<JumpData> &newJumps);
    double getPointsSum() const;
    void setPointsSum(double newPointsSum);
    int getType() const;
    void setType(int newType);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    Team *getTeam() const;
    void setTeam(Team *newTeam);
    int getPosition() const;
    void setPosition(int newPosition);
    QVector<CompetitionSingleResult> getTeamJumpersResults() const;
    QVector<CompetitionSingleResult> & getTeamJumpersResultsReference();
    void setTeamJumpersResults(const QVector<CompetitionSingleResult> &newTeamJumpersResults);
};

inline bool operator>(const CompetitionSingleResult & l, const CompetitionSingleResult & r)
{
    if(l.getPointsSum() == r.getPointsSum())
    {
        return l.getID() > r.getID();
    }
    else return l.getPointsSum() > r.getPointsSum();
}

inline bool operator<(const CompetitionSingleResult & l, const CompetitionSingleResult & r)
{
    if(l.getPointsSum() == r.getPointsSum())
    {
        return l.getID() < r.getID();
    }
    else return l.getPointsSum() < r.getPointsSum();
}


#endif // COMPETITIONSINGLERESULT_H
