#include "TeamCompetitionManager.h"


TeamCompetitionManager::TeamCompetitionManager() : AbstractCompetitionManager(CompetitionRules::Team)
{
    actualGroup = 0;
    connect(this, &TeamCompetitionManager::actualStartListIndexChanged, this, [this](){
        if(roundsTeams.count() > 0){
            actualJumper = startListStatuses[actualStartListIndex].getJumper();
            actualTeam = roundsTeams[actualRound - 1][actualStartListIndex];
        }
    });
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
    roundsTeams.push_back(getFilteredTeamsForNextRound());
    setActualStartListIndex(0);
    roundsStartingGates.push_back(actualGate);
    setupStartListStatusesForActualRound();
}

void TeamCompetitionManager::setupStartListStatusesForActualRound()
{
    startListStatuses.clear();
    for(auto & team : getActualRoundTeamsReference()){
        startListStatuses.push_back(StartListCompetitorStatus(team->getJumpersReference()[actualGroup - 1]));
    }
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

int TeamCompetitionManager::getActualGroup() const
{
    return actualGroup;
}

void TeamCompetitionManager::setActualGroup(int newActualGroup)
{
    actualGroup = newActualGroup;
    emit actualGroupChanged();
}

QVector<QPair<Team *, int> > TeamCompetitionManager::getTeamsAdvanceStatuses() const
{
    return teamsAdvanceStatuses;
}

QVector<QPair<Team *, int> > & TeamCompetitionManager::getTeamsAdvanceStatusesReference()
{
    return teamsAdvanceStatuses;
}

void TeamCompetitionManager::setTeamsAdvanceStatuses(const QVector<QPair<Team *, int> > &newTeamsAdvanceStatuses)
{
    teamsAdvanceStatuses = newTeamsAdvanceStatuses;
}

QVector<QVector<Team *> > TeamCompetitionManager::getRoundsTeams() const
{
    return roundsTeams;
}

QVector<QVector<Team *> > &TeamCompetitionManager::getRoundsTeamsReference()
{
    return roundsTeams;
}

void TeamCompetitionManager::setRoundsTeams(const QVector<QVector<Team *> > &newRoundTeams)
{
    roundsTeams = newRoundTeams;
}

QVector<Team *> & TeamCompetitionManager::getActualRoundTeamsReference()
{
    return roundsTeams[actualRound - 1];
}

Team *TeamCompetitionManager::getActualTeam() const
{
    return actualTeam;
}

void TeamCompetitionManager::setActualTeam(Team *newActualTeam)
{
    actualTeam = newActualTeam;
}
