#include "AbstractCompetitionSingleResult.h"

#include "CompetitionInfo.h"

AbstractCompetitionSingleResult::AbstractCompetitionSingleResult()
{

}

double AbstractCompetitionSingleResult::getPointsSum() const
{
    return pointsSum;
}

void AbstractCompetitionSingleResult::setPointsSum(double newPointsSum)
{
    pointsSum = newPointsSum;
}

CompetitionInfo *AbstractCompetitionSingleResult::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionSingleResult::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
