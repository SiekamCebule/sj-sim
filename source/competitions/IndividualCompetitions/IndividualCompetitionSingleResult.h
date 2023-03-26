#ifndef INDIVIDUALCOMPETITIONSINGLERESULT_H
#define INDIVIDUALCOMPETITIONSINGLERESULT_H

#include "../AbstractCompetitionSingleResult.h"
#include "../../simulator/JumpData.h"
#include <QVector>


class IndividualCompetitionSingleResult : public AbstractCompetitionSingleResult
{
public:
    IndividualCompetitionSingleResult();
    IndividualCompetitionSingleResult(const JumpData & jump);

    bool operator >(const IndividualCompetitionSingleResult & r) const;
    bool operator <(const IndividualCompetitionSingleResult & r) const;
    void updatePointsSum();

private:
    QVector<JumpData> jumps;
    Jumper * jumper;
    int position;

public:
    int jumpsCount() {return jumps.count();}
    QVector<JumpData> getJumps() const;
    void setJumps(const QVector<JumpData> &newJumps);
    QVector<JumpData> & getEditableJumps();
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    int getPosition() const;
    void setPosition(int newPosition);
};

#endif // INDIVIDUALCOMPETITIONSINGLERESULT_H
