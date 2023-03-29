#include "IndividualCompetitionResults.h"

IndividualCompetitionResults::IndividualCompetitionResults()
{

}

void IndividualCompetitionResults::updatePositions()
{
    double oldPoints = 0;
    int actualPosition = 0;
    int add = 1;
    for(auto & result : jumpersResults){
        if(oldPoints == result.getPointsSum()){
            if(actualPosition > (0)){
                result.setPosition(actualPosition);
                add +=1;
            }
        }
        else{
            actualPosition += add;
            result.setPosition(actualPosition);
            add = 1;
        }
        oldPoints = result.getPointsSum();
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
