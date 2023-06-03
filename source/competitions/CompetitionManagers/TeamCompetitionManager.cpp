#include "TeamCompetitionManager.h"


TeamCompetitionManager::TeamCompetitionManager(short type) : AbstractCompetitionManager(type)
{

}


void TeamCompetitionManager::setActualTeamToActualJumperTeam()
{
    for(auto & team : getActualRoundTeamsReference())
    {
        if(actualJumper == team->getJumpersReference()[actualGroup - 1])
            actualTeam = team;
    }
}

void TeamCompetitionManager::setupNextRound()
{
    actualRound++; //Przechodzi do następnej rundy
    roundTeams.push_back(getFilteredTeamsForNextRound());
    setActualStartListIndex(0);
    roundsStartingGates.push_back(actualGate);
    setupStartListStatusesForActualRound();
}

void TeamCompetitionManager::setupStartListStatusesForActualRound()
{
    QVector<QVector<StartListCompetitorStatus>> groups;
    for(int i=0; i<competitionRules->getJumpersInTeamCount(); i++){
        QVector<StartListCompetitorStatus> groupStartList;
        for(auto & team : getActualRoundTeamsReference()){
            groupStartList.push_back(StartListCompetitorStatus(team->getJumpersReference()[i]));
        }
        groups.push_back(groupStartList);
    }
    roundsJumpersGroups.push_back(groups);
}

void TeamCompetitionManager::updateCompetitorsAdvanceStatuses()
{
    teamsAdvanceStatuses.clear();
    results->updatePositions();
    for(auto & team : getActualRoundTeamsReference())
    {
        int status = StartListCompetitorStatus::Waiting;
        if(actualRound == competitionRules->getRounds().count())
            status = StartListCompetitorStatus::Waiting;
        else if(getActualRoundTeamsReference().count() <= competitionRules->getRounds().at(actualRound).getCount()){
            status = StartListCompetitorStatus::SureAdvanced;
        }

        else if(lastQualifiedResult == nullptr)
            status = StartListCompetitorStatus::Waiting;
        else{
            if(results->getResultOfTeam(team)->getPosition() <= lastQualifiedResult->getPosition()){
                status = StartListCompetitorStatus::SureAdvanced;
            }
            else{
                int lastWaiting = competitionRules->getRounds().at(actualRound).getCount();
                results->sortInDescendingOrder();
                QVector<CompetitionSingleResult> * res = &results->getResultsReference();
                int lastPosition = 0;
                for(int i = res->count() - 1; i>=0; i--){
                    if(res->at(i).getPosition() < competitionRules->getRounds().at(actualRound).getCount()){
                        if(res->at(i).getPosition() == lastPosition) //mamy egzekwo
                        {
                            int exaequos = 0;
                            for(int i = res->count() - 1; i>=0; i--){
                                if(res->at(i).getPosition() == lastPosition){
                                    exaequos++;
                                    lastPosition = res->at(i).getPosition();
                                }
                                else
                                    break;
                            }
                            if(exaequos >= (competitionRules->getRounds().at(actualRound - 1).getCount() - lastPosition))
                                lastWaiting += exaequos;
                        }
                    }
                }
                if(results->getResultOfTeam(team)->getPosition() <= lastWaiting)
                    status = StartListCompetitorStatus::Waiting;
                else
                    status = StartListCompetitorStatus::SureDroppedOut;
            }
        }
        teamsAdvanceStatuses.push_back(QPair<Team *, int>(team, status));
    }
}

int TeamCompetitionManager::getAdvanceStatusOfTeam(Team *team)
{
    for(auto & status : teamsAdvanceStatuses){
        if(status.first == team)
            return status.second;
    }
    return (-1); // nie znaleziono
}

QVector<Team *> TeamCompetitionManager::getFilteredTeamsForNextRound()
{
    QVector<Team *> teams;
    if(competitionRules->getRounds().at(actualRound - 1).getSortStartList() == true){
        results->sortInAscendingOrder();
        QVector<CompetitionSingleResult *> roundResults;
        for(auto & res : results->getResultsReference()){
            if(getAdvanceStatusOfTeam(res.getTeam()) != (-1))
                roundResults.push_back(&res);
        }

        for(auto & res : roundResults){
            if(getAdvanceStatusOfTeam(res->getTeam()) == StartListCompetitorStatus::SureAdvanced)
                teams.push_back(res->getTeam());
        }
        results->sortInDescendingOrder();
    }
    else{
        for(auto & status : teamsAdvanceStatuses){
            if(status.second == StartListCompetitorStatus::SureAdvanced)
                teams.push_back(status.first);
        }
    }
    return teams;
}

QVector<QVector<QVector<StartListCompetitorStatus> > > TeamCompetitionManager::getRoundsJumpersGroups() const
{
    return roundsJumpersGroups;
}

QVector<QVector<QVector<StartListCompetitorStatus> > > &TeamCompetitionManager::getRoundsJumpersGroupsReference()
{
    return roundsJumpersGroups;
}

QVector<QVector<StartListCompetitorStatus> > &TeamCompetitionManager::getActualRoundJumpersGroupsReference()
{
    return roundsJumpersGroups[actualRound - 1];
}

QVector<StartListCompetitorStatus> &TeamCompetitionManager::getActualGroupStartListReference()
{
    return roundsJumpersGroups[actualRound - 1][actualGroup - 1];
}

void TeamCompetitionManager::setRoundsJumpersGroups(const QVector<QVector<QVector<StartListCompetitorStatus> > > &newRoundsJumpersGroups)
{
    roundsJumpersGroups = newRoundsJumpersGroups;
}

QVector<QPair<Team *, int> > TeamCompetitionManager::getTeamsAdvanceStatuses() const
{
    return teamsAdvanceStatuses;
}

void TeamCompetitionManager::setTeamsAdvanceStatuses(const QVector<QPair<Team *, int> > &newTeamsAdvanceStatuses)
{
    teamsAdvanceStatuses = newTeamsAdvanceStatuses;
}

QVector<QVector<Team *> > TeamCompetitionManager::getRoundsTeams() const
{
    return roundTeams;
}

QVector<QVector<Team *> > &TeamCompetitionManager::getRoundsTeamsReference()
{
    return roundTeams;
}

void TeamCompetitionManager::setRoundsTeams(const QVector<QVector<Team *> > &newRoundTeams)
{
    roundTeams = newRoundTeams;
}

QVector<Team *> & TeamCompetitionManager::getActualRoundTeamsReference()
{
    return roundTeams[actualRound - 1];
}

Team *TeamCompetitionManager::getActualTeam() const
{
    return actualTeam;
}

void TeamCompetitionManager::setActualTeam(Team *newActualTeam)
{
    actualTeam = newActualTeam;
}
