#include "IndividualCompetitionResults.h"

IndividualCompetitionResults::IndividualCompetitionResults()
{

}

void IndividualCompetitionResults::updatePositions()
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

IndividualCompetitionSingleResult *IndividualCompetitionResults::getPointerOfExactJumperResults(int index) const
{
    return const_cast<IndividualCompetitionSingleResult *>(&jumpersResults[index]);
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
