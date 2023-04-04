#include "IndividualCompetitionResults.h"

IndividualCompetitionResults::IndividualCompetitionResults()
{

}

void IndividualCompetitionResults::updatePositions()
{
    double previousJumperPoints = 0;
    int actualPosition = 1;
    int add = 1;
    int i=0;
    for(auto & result : jumpersResults){
        if(i==0){
            result.setPosition(1);
            previousJumperPoints = result.getPointsSum();
            i++;
            continue;
        }

        if(previousJumperPoints == result.getPointsSum())
        {
            result.setPosition(actualPosition);
            add += 1;
        }
        else{
            actualPosition += add;
            add = 1;
            result.setPosition(actualPosition);
        }

        previousJumperPoints = result.getPointsSum();
        i++;
    }
}

void IndividualCompetitionResults::sortJumpersResultsInDescendingOrder()
{
    std::sort(jumpersResults.begin(), jumpersResults.end(), std::greater<IndividualCompetitionSingleResult>());
    updatePositions();
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
