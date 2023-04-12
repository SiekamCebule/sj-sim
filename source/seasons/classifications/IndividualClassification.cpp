#include "IndividualClassification.h"

IndividualClassification::IndividualClassification()
{

}

QVector<IndividualClassificationSingleResult *> IndividualClassification::getJumpersResults() const
{
    return jumpersResults;
}

QVector<IndividualClassificationSingleResult *> &IndividualClassification::getEditableJumpersResults()
{

}

void IndividualClassification::setJumpersResults(const QVector<IndividualClassificationSingleResult *> &newJumpersResults)
{
    jumpersResults = newJumpersResults;
}
