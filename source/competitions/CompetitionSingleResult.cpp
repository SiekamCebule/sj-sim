#include "CompetitionSingleResult.h"

#include "CompetitionInfo.h"

CompetitionSingleResult::CompetitionSingleResult(Jumper *jumper, int type) : type(type), jumper(jumper), team(nullptr), competitionInfo(nullptr)
{}

CompetitionSingleResult::CompetitionSingleResult(Team *team, int type) : type(type), jumper(nullptr), team(team), competitionInfo(nullptr)
{}

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

CompetitionInfo *CompetitionSingleResult::getCompetitionInfo() const
{
    return competitionInfo;
}

void CompetitionSingleResult::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}

void CompetitionSingleResult::updatePointsSum()
{
    pointsSum = 0;
    for(auto & res : jumps){
        qDebug()<<"AAAAAAAAAAAAAAAAAAAAAA: "<<res.getPoints();
        pointsSum += res.getPoints();
    }
}

void CompetitionSingleResult::updateTeamJumpersResults()
{
    teamJumpersResults.clear();
    QSet<Jumper *> jumpers;
    for(auto & jump : getJumpsReference()){
        if(jumpers.contains(jump.getJumper()) == false){
            CompetitionSingleResult result(jump.getJumper());
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
