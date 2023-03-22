#include "IndividualCompetitionResult.h"

IndividualCompetitionResult::IndividualCompetitionResult()
{

}

Jumper *IndividualCompetitionResult::getJumper() const
{
    return jumper;
}

void IndividualCompetitionResult::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

QVector<JumpData> IndividualCompetitionResult::getJumps() const
{
    return jumps;
}

void IndividualCompetitionResult::setJumps(const QVector<JumpData> &newJumps)
{
    jumps = newJumps;
}
