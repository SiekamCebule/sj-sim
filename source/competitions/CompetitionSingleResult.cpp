#include "CompetitionSingleResult.h"
#include "../global/SeasonDatabaseObjectsManager.h"
#include "CompetitionInfo.h"
extern SeasonDatabaseObjectsManager seasonObjectsManager;

CompetitionSingleResult::CompetitionSingleResult(CompetitionInfo *competition, Jumper *jumper, int type) : type(type), jumper(jumper), team(nullptr), competition(competition),
    ClassWithID()
{}

CompetitionSingleResult::CompetitionSingleResult(CompetitionInfo *competition, Team *team, int type) : type(type), jumper(nullptr), team(team), competition(competition),
    ClassWithID()
{}

CompetitionSingleResult CompetitionSingleResult::getFromJson(QJsonObject obj, SeasonDatabaseObjectsManager * objectsManager)
{
    CompetitionSingleResult result(nullptr);
    result.setID(obj.value("id").toString().toULong());
    //result.setCompetition(static_cast<CompetitionInfo *>(objectsManager->getObjectByID(obj.value("competition-id").toString().toULong())));
    result.setPosition(obj.value("position").toInt());
    result.setPointsSum(obj.value("points-sum").toDouble());
    result.setType(obj.value("type").toInt());
    result.setJumper(static_cast<Jumper *>(objectsManager->getObjectByID(obj.value("jumper-id").toString().toULong())));
    result.setTeam(static_cast<Team *>(objectsManager->getObjectByID(obj.value("team-id").toString().toULong())));

    QJsonArray jumpsArray = obj.value("jumps").toArray();
    for(auto jump : jumpsArray){
        result.getJumpsReference().push_back(JumpData::getFromJson(jump.toObject(), objectsManager));
    }
    result.updateTeamJumpersResults();
    return result;
}

QJsonObject CompetitionSingleResult::getJsonObject(CompetitionSingleResult result)
{
    QJsonObject resultObject;
    resultObject.insert("id", QString::number(result.getID()));
    //resultObject.insert("competition-id", QString::number(result.getCompetition()->getID()));
    resultObject.insert("position", result.getPosition());
    resultObject.insert("points-sum", result.getPointsSum());
    resultObject.insert("type", result.getType());
    if(result.getJumper() != nullptr)
        resultObject.insert("jumper-id", QString::number(result.getJumper()->getID()));
    if(result.getTeam() != nullptr)
        resultObject.insert("team-id", QString::number(result.getTeam()->getID()));

    QJsonArray jumpsArray;
    for(auto & jump : qAsConst(result.getJumpsReference())){
        QJsonObject jumpObject = JumpData::getJsonObject(jump);
        jumpsArray.push_back(jumpObject);
    }
    resultObject.insert("jumps", jumpsArray);

    return resultObject;
}

QVector<CompetitionSingleResult *> CompetitionSingleResult::getFilteredSingleResults(QVector<CompetitionInfo *> &competitions, Jumper *jumper, QSet<int> serieTypes)
{
    QVector<CompetitionSingleResult *> singleResults;
    for(auto & comp : competitions)
    {
        if(serieTypes.contains(comp->getSerieType()))
        {
            if(comp->getResultsReference().getResultOfIndividualJumper(jumper) != nullptr)
            {
                singleResults.push_back(comp->getResultsReference().getResultOfIndividualJumper(jumper));
                continue;
            }
        }
    }
    return singleResults;
}

CompetitionInfo *CompetitionSingleResult::getCompetition() const
{
    return competition;
}

void CompetitionSingleResult::setCompetition(CompetitionInfo *newCompetition)
{
    competition = newCompetition;
}

QVector<CompetitionSingleResult> CompetitionSingleResult::getTeamJumpersResults() const
{
    return teamJumpersResults;
}

QVector<CompetitionSingleResult> &CompetitionSingleResult::getTeamJumpersResultsReference()
{
    return teamJumpersResults;
}

void CompetitionSingleResult::setTeamJumpersResults(const QVector<CompetitionSingleResult> &newTeamJumpersResults)
{
    teamJumpersResults = newTeamJumpersResults;
}

int CompetitionSingleResult::getPosition() const
{
    return position;
}

void CompetitionSingleResult::setPosition(int newPosition)
{
    position = newPosition;
}

Team *CompetitionSingleResult::getTeam() const
{
    return team;
}

void CompetitionSingleResult::setTeam(Team *newTeam)
{
    team = newTeam;
}

Jumper *CompetitionSingleResult::getJumper() const
{
    return jumper;
}

void CompetitionSingleResult::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

int CompetitionSingleResult::getType() const
{
    return type;
}

void CompetitionSingleResult::setType(int newType)
{
    type = newType;
}

double CompetitionSingleResult::getPointsSum() const
{
    return pointsSum;
}

void CompetitionSingleResult::setPointsSum(double newPointsSum)
{
    pointsSum = newPointsSum;
}

QVector<JumpData> CompetitionSingleResult::getJumps() const
{
    return jumps;
}

QVector<JumpData> &CompetitionSingleResult::getJumpsReference()
{
    return jumps;
}

void CompetitionSingleResult::setJumps(const QVector<JumpData> &newJumps)
{
    jumps = newJumps;
}

void CompetitionSingleResult::updatePointsSum()
{
    pointsSum = 0;
    for(auto & res : jumps){
        pointsSum += res.getPoints();
    }
}

void CompetitionSingleResult::updateTeamJumpersResults()
{
    teamJumpersResults.clear();
    QSet<Jumper *> jumpers;
    for(auto & jump : getJumpsReference()){
        if(jumpers.contains(jump.getJumper()) == false){
            CompetitionSingleResult result(competition, jump.getJumper());
            result.getJumpsReference().push_back(jump);
            result.updatePointsSum();

            teamJumpersResults.push_back(result);
            jumpers.insert(jump.getJumper());
        }
        else{
            for(auto & result : teamJumpersResults){
                if(jump.getJumper() == result.getJumper()){
                    result.getJumpsReference().push_back(jump);
                    result.updatePointsSum();
                }
            }
        }
    }
}

CompetitionSingleResult *CompetitionSingleResult::getTeamJumperResult(Jumper *jumper)
{
    for(auto & res : teamJumpersResults)
    {
        if(res.getJumper() == jumper)
            return &res;
    }
}
