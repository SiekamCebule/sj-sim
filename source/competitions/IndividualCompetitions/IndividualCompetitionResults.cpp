#include "IndividualCompetitionResults.h"

IndividualCompetitionResults::IndividualCompetitionResults()
{

}

QVector<IndividualCompetitionSingleResult> IndividualCompetitionResults::getJumpersResults() const
{
    return jumpersResults;
}

QVector<IndividualCompetitionSingleResult> &IndividualCompetitionResults::getEditableJumpersResults()
{
    return jumpersResults;
}

void IndividualCompetitionResults::setJumpersResults(const QVector<IndividualCompetitionSingleResult> &newJumpersResults)
{
    jumpersResults = newJumpersResults;
}

IndividualCompetitionSingleResult *IndividualCompetitionResults::getResultsOfJumper(Jumper *jumper)
{
    for(auto & result : getEditableJumpersResults())
    {
        if(result.getJumper() == jumper)
            return &result;
    }

    return nullptr;
}
