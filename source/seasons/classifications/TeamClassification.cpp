#include "TeamClassification.h"

TeamClassification::TeamClassification()
{

}

QVector<TeamClassificationSingleResult *> TeamClassification::getTeamsResults() const
{
    return teamsResults;
}

QVector<TeamClassificationSingleResult *> &TeamClassification::getEditableTeamsResults()
{
    return teamsResults;
}

void TeamClassification::setTeamsResults(const QVector<TeamClassificationSingleResult *> &newTeamsResults)
{
    teamsResults = newTeamsResults;
}
