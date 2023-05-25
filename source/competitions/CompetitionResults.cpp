#include "CompetitionResults.h"

#include "CompetitionInfo.h"

CompetitionResults::CompetitionResults(CompetitionInfo *competitionInfo) : competitionInfo(competitionInfo)
{

}

CompetitionResults::~CompetitionResults()
{

}

QVector<CompetitionSingleResult> CompetitionResults::getResults() const
{
    return results;
}

QVector<CompetitionSingleResult> &CompetitionResults::getResultsReference()
{
    return results;
}

void CompetitionResults::setResults(const QVector<CompetitionSingleResult> &newResults)
{
    results = newResults;
}

CompetitionSingleResult *CompetitionResults::getResultOfIndividualJumper(Jumper *jumper)
{
    for(auto & result : results)
    {
        if(result.getJumper() == jumper)
            return &result;
    }
    return nullptr;
}

CompetitionSingleResult *CompetitionResults::getResultOfTeam(Team *team)
{
    for(auto & result : results)
    {
        if(result.getTeam() == team)
            return &result;
    }
    return nullptr;
}

CompetitionSingleResult *CompetitionResults::getResultByIndex(int index)
{
    return const_cast<CompetitionSingleResult *>(&results[index]);
}

void CompetitionResults::addJump(Jumper *jumper, JumpData &jump, int jumpNumber)
{
    CompetitionSingleResult *result = nullptr;
    for(auto & res : results){
        if(res.getJumper() == jumper){
            result = &res;
            break;
        }
    }
    if(result == nullptr){
        results.push_back(CompetitionSingleResult(jumper, CompetitionSingleResult::IndividualResult));
        result = &results[results.count() - 1];
    }
    qDebug()<<result->getJumper()->getNameAndSurname();
    int index = jumpNumber;
    if(jumpNumber == -1 || jumpNumber >= result->getJumpsReference().count())
        result->getJumpsReference().push_back(jump);
    else{
        if(jumpNumber < 0)
            index = 0;

        result->getJumpsReference().insert(index, jump);
    }
    result->updatePointsSum();
}

void CompetitionResults::addJump(Team *team, JumpData &jump, int jumpNumber)
{
    CompetitionSingleResult *result = nullptr;
    for(auto & res : results){
        if(res.getTeam() == team){
            result = &res;
            break;
        }
    }
    if(result == nullptr){
        results.push_back(CompetitionSingleResult(team, CompetitionSingleResult::TeamResult));
        result = &results[results.count() - 1];
    }
    int index = jumpNumber;
    if(jumpNumber == -1 || jumpNumber >= result->getJumpsReference().count())
        result->getJumpsReference().push_back(jump);
    else{
        if(jumpNumber < 0)
            index = 0;

        result->getJumpsReference().insert(index, jump);
    }
    result->updatePointsSum();
}

void CompetitionResults::addJump(CompetitionSingleResult *result, JumpData &jump, int jumpNumber)
{
    int index = jumpNumber;
    if(jumpNumber == -1 || jumpNumber >= result->getJumpsReference().count())
        result->getJumpsReference().push_back(jump);
    else{
        if(jumpNumber < 0)
            index = 0;

        result->getJumpsReference().insert(index, jump);
    }
    result->updatePointsSum();
}

void CompetitionResults::updatePositions()
{
    double previousResultPoints = 0;
    int actualPosition = 1;
    int add = 1;
    int i=0;
    for(auto & result : results){
        if(i==0){
            result.setPosition(1);
            previousResultPoints = result.getPointsSum();
            i++;
            continue;
        }

        if(previousResultPoints == result.getPointsSum())
        {
            result.setPosition(actualPosition);
            add += 1;
        }
        else{
            actualPosition += add;
            add = 1;
            result.setPosition(actualPosition);
        }

        previousResultPoints = result.getPointsSum();
        i++;
    }
}

void CompetitionResults::sortInDescendingOrder()
{
    std::sort(results.begin(), results.end(), std::greater<CompetitionSingleResult>());
    updatePositions();
}

void CompetitionResults::sortInAscendingOrder()
{
    std::sort(results.begin(), results.end(), std::less<CompetitionSingleResult>());
    updatePositions();
}

CompetitionInfo *CompetitionResults::getCompetitionInfo() const
{
    return competitionInfo;
}

void CompetitionResults::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
