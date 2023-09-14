#include "CompetitionSingleResult.h"
#include "../global/DatabaseObjectsManager.h"
#include "CompetitionInfo.h"

CompetitionSingleResult::CompetitionSingleResult(CompetitionInfo *competition, Jumper *jumper, int type) : type(type), jumper(jumper), team(nullptr), competition(competition),
    ClassWithID()
{}

CompetitionSingleResult::CompetitionSingleResult(CompetitionInfo *competition, Team *team, int type) : type(type), jumper(nullptr), team(team), competition(competition),
    ClassWithID()
{}

CompetitionSingleResult CompetitionSingleResult::getFromJson(QJsonObject obj, DatabaseObjectsManager * objectsManager)
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

CompetitionSingleResult CompetitionSingleResult::getFromJsonValue(QJsonValue val, DatabaseObjectsManager *objectsManager)
{
    return CompetitionSingleResult::getFromJson(val.toObject(), objectsManager);
}

QVector<CompetitionSingleResult *> CompetitionSingleResult::getFilteredSingleResults(QVector<CompetitionInfo *> &competitions, Jumper *jumper, QSet<int> serieTypes, QSet<int> hillTypes, QVector<Classification *> classifications, bool skipClassifications)
{
    QVector<CompetitionSingleResult *> singleResults;
    for(auto & comp : competitions)
    {
        bool ok = false;
        for(auto & compCls : comp->getClassificationsReference()){
            for(auto & cls : classifications){
                if(compCls->getName() == cls->getName())
                {
                    ok = true;
                    break;
                }
            }
        }
        if(skipClassifications == true)
            ok = true;
        if(serieTypes.contains(comp->getSerieType()) && hillTypes.contains(comp->getHill()->getHillType()) && ok)
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

QHash<Jumper *, QVector<CompetitionSingleResult *> > CompetitionSingleResult::getJumpersFilteredSingleResults(QVector<Jumper *> &jumpers, QVector<CompetitionInfo *> &competitions, QSet<int> serieTypes,QSet<int> hillTypes,  QVector<Classification *> classifications, bool skipClassifications)
{
    QHash<Jumper *, QVector<CompetitionSingleResult *>> toReturn;
    for(auto & jumper : jumpers)
    {
        toReturn.insert(jumper, CompetitionSingleResult::getFilteredSingleResults(competitions, jumper, serieTypes, hillTypes, classifications, skipClassifications));
    }
    return toReturn;
}

QString CompetitionSingleResult::getCsvResultsObject()
{
    QString s;
    s += QString::number(position)+";";
    s += jumper->getNameAndSurname()+";";
    s += jumper->getCountryCode()+";";
    s += QString::number(pointsSum)+";";
    for(auto & round : competition->getRulesPointer()->getRoundsReference())
    {
        int i=0;
        s += QString::number(jumps[i].getDistance())+";";
        s += QString::number(jumps[i].getGate())+";";
        s += QString::number(jumps[i].getAveragedWind())+";";
        s += QString::number(jumps[i].getTotalCompensation())+";";
        s += QString::number(jumps[i].getJudgesPoints())+";";
        s += QString::number(jumps[i].getPoints())+"";
        i++;
    }
    return s;
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
