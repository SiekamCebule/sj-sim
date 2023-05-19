#include "IndividualClassificationSingleResult.h"

IndividualClassificationSingleResult::IndividualClassificationSingleResult()
{

}

QVector<CompetitionSingleResult *> IndividualClassificationSingleResult::getCompetitionsResults() const
{
    return competitionsResults;
}

void IndividualClassificationSingleResult::setCompetitionsResults(const QVector<CompetitionSingleResult *> &newCompetitionsResults)
{
    competitionsResults = newCompetitionsResults;
}
