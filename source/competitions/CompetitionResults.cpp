#include "CompetitionResults.h"

#include "CompetitionInfo.h"
#include "CompetitionSingleResult.h"

extern SeasonDatabaseObjectsManager seasonObjectsManager;


CompetitionResults::CompetitionResults() :
    ClassWithID()
{

}

CompetitionInfo *CompetitionResults::getCompetition() const
{
    return competition;
}

void CompetitionResults::setCompetition(CompetitionInfo *newCompetition)
{
    competition = newCompetition;
}

QVector<QVector<JumpData *> > CompetitionResults::constructRoundsJumps(QVector<RoundInfo> *rounds)
{
    QVector<QVector<JumpData *>> roundsJumps;
    for(int i=0; i<rounds->count(); i++){
        QVector<JumpData *> jumps;
        for(auto & jp : results){
            if(jp.getJumpsReference().count() < i){
                jumps.push_back(&jp.getJumpsReference()[i]);
            }
        }
        roundsJumps.push_back(jumps);
    }
    return roundsJumps;
}

CompetitionResults CompetitionResults::constructRoundsResults(QVector<RoundInfo> *roundsInfos, QVector<int> rounds)
{
    CompetitionResults results;
    for(auto & round : rounds){
        if(round - 1 >= results.getResultsReference().count())
            break;
        else{
            for(auto & jump : constructRoundsJumps(roundsInfos)[round - 1]){
                results.addJump(jump->getJumper(), *jump);
            }
        }
    }
    results.sortInDescendingOrder();
    return results;
}

CompetitionResults CompetitionResults::getFromJson(QJsonObject obj, SeasonDatabaseObjectsManager * objectsManager)
{
    CompetitionResults results;
    results.setID(obj.value("id").toString().toULong());

    QJsonArray resultsArray = obj.value("results").toArray();
    for(auto res : resultsArray){
        results.getResultsReference().push_back(CompetitionSingleResult::getFromJson(res.toObject(), objectsManager));
    }
    objectsManager->fill(&results.getResultsReference());

    return results;
}

QJsonObject CompetitionResults::getJsonObject(CompetitionResults &results)
{
    QJsonObject object;
    object.insert("id", QString::number(results.getID()));

    QJsonArray resultsArray;
    for(auto & res : qAsConst(results.getResultsReference()))
    {
        QJsonObject o = CompetitionSingleResult::getJsonObject(res);
        resultsArray.push_back(o);
    }
    object.insert("results", resultsArray);

    return object;
}

QVector<int> CompetitionResults::getJumpersPositions(const QVector<Jumper *> * jumpers) const
{
    QVector<int> positions;

    double previousResultPoints = 0;
    int actualPosition = 1;
    int add = 1;
    int i=0;
    for(auto & result : results){
        if(jumpers->contains(result.getJumper())){
            int pos = 0;
            if(i==0){
                positions.push_back(1);
                previousResultPoints = result.getPointsSum();
                i++;
                continue;
            }

            if(previousResultPoints == result.getPointsSum())
            {
                pos = actualPosition;;
                add += 1;
            }
            else{
                actualPosition += add;
                add = 1;
                pos = actualPosition;
            }

            previousResultPoints = result.getPointsSum();
            i++;
            positions.push_back(pos);
        }
    }

    return positions;
}

void CompetitionResults::sortJumpersByResults(QVector<Jumper *> &jumpers)
{
    QVector<Jumper *> temp;
    for(auto & res : results)
    {
        if(jumpers.contains(res.getJumper()))
        {
            temp.push_back(res.getJumper());
        }
    }
    jumpers = temp;
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
        if(result.getJumper() == jumper){
            return &result;
        }
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
    if(results.count() > index){
        return &results[index];
    }
    return nullptr;
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

int CompetitionResults::howManyJumpersJumpedInTeam(Team *team)
{
    CompetitionSingleResult * res = getResultOfTeam(team);
    int i = 0;
    QSet<Jumper *> jumpers;
    for(auto & jump : res->getJumpsReference())
    {
        if(jumpers.contains(jump.getJumper()) == false)
        {
            jumpers.insert(jump.getJumper());
            i++;
        }
    }
    return i;
}

int CompetitionResults::getIndexOfJumperResult(Jumper *jumper)
{
    int index = 0;
    for(auto & result : results)
    {
        if(result.getJumper() == jumper)
            return index;
        index++;
    }
    return -1;
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
    // for(auto & result : results){
    //   qDebug()<<result.getJumper()->getNameAndSurname()<<": "<<result.getPosition();
    //}
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
