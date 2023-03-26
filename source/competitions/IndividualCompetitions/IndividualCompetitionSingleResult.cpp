#include "IndividualCompetitionSingleResult.h"

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult()
{
    pointsSum = position = 0;
    jumper = nullptr;
}

IndividualCompetitionSingleResult::IndividualCompetitionSingleResult(const JumpData &jump)
{
    pointsSum = position = 0;
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

int IndividualCompetitionSingleResult::getPosition() const
{
    return position;
}

void IndividualCompetitionSingleResult::setPosition(int newPosition)
{
    position = newPosition;
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
