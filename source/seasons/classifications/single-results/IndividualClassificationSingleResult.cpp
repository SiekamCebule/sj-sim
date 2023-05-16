#include "IndividualClassificationSingleResult.h"

IndividualClassificationSingleResult::IndividualClassificationSingleResult()
{

}

QVector<IndividualCompetitionResult *> IndividualClassificationSingleResult::getCompetitionsResults() const
{
    return competitionsResults;
}

void IndividualClassificationSingleResult::setCompetitionsResults(const QVector<IndividualCompetitionResult *> &newCompetitionsResults)
{
    competitionsResults = newCompetitionsResults;
}
