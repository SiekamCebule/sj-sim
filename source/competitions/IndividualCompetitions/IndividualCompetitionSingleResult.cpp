#include "IndividualCompetitionSingleResult.h"

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult()
{
    pointsSum = 0;
}

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult(const JumpData &jump)
{
    jumps.push_back(jump);
    setJumper(jump.getJumper());
}

bool IndividualCompetitionSingleResult::operator >(const IndividualCompetitionSingleResult &r) const
{
    return getPointsSum() > r.getPointsSum();
}

bool IndividualCompetitionSingleResult::operator <(const IndividualCompetitionSingleResult &r) const
{
    return getPointsSum() < r.getPointsSum();
}

void IndividualCompetitionSingleResult::updatePointsSum()
{
    pointsSum = 0;
    for(auto & res : jumps){
        pointsSum += res.getPoints();
    }
}

Jumper *IndividualCompetitionSingleResult::getJumper() const
{
    return jumper;
}

void IndividualCompetitionSingleResult::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

QVector<JumpData> IndividualCompetitionSingleResult::getJumps() const
{
    return jumps;
}

void IndividualCompetitionSingleResult::setJumps(const QVector<JumpData> &newJumps)
{
    jumps = newJumps;
}

QVector<JumpData> &IndividualCompetitionSingleResult::getEditableJumps()
{
    return jumps;
}
