#include "IndividualClassificationSingleResult.h"

IndividualClassificationSingleResult::IndividualClassificationSingleResult()
{

}

QVector<IndividualCompetitionSingleResult *> IndividualClassificationSingleResult::getCompetitionsResults() const
{
    return competitionsResults;
}

void IndividualClassificationSingleResult::setCompetitionsResults(const QVector<IndividualCompetitionSingleResult *> &newCompetitionsResults)
{
    competitionsResults = newCompetitionsResults;
}
