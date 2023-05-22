#ifndef COMPETITIONSINGLERESULT_H
#define COMPETITIONSINGLERESULT_H

#include "../utilities/ClassWithID.h"
#include "../simulator/JumpData.h"
#include "../simulator/Team.h"
#include <QVector>
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

protected:
    CompetitionInfo * competitionInfo;
    QVector<JumpData> jumps;
    double pointsSum;
    int position;

    int type;
    Jumper * jumper;
    Team * team;

public:
    void updatePointsSum();

    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
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
};

inline bool operator>(const CompetitionSingleResult & l, const CompetitionSingleResult & r) {return l.getPointsSum() > r.getPointsSum();}

#endif // COMPETITIONSINGLERESULT_H
