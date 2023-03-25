#include "AbstractCompetitionResults.h"

#include "CompetitionInfo.h"

AbstractCompetitionResults::AbstractCompetitionResults()
{

}

CompetitionInfo *AbstractCompetitionResults::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionResults::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
