#ifndef TEAMCOMPETITIONMANAGER_H
#define TEAMCOMPETITIONMANAGER_H

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Team.h"

class TeamCompetitionManager : public AbstractCompetitionManager
{
public:
    TeamCompetitionManager(short type = CompetitionRules::Individual);
    void setActualTeamToActualJumperTeam();

    void setupNextRound();
    void setupStartListStatusesForActualRound(); // zrobienie grup list startowych
    void updateCompetitorsAdvanceStatuses();

    int getAdvanceStatusOfTeam(Team * team);
    QVector<Team *> getFilteredTeamsForNextRound();

private:
    int actualGroup;
    Team * actualTeam;
    QVector<QVector<Team *>> roundTeams;
    QVector<QVector<QVector<StartListCompetitorStatus>>> roundsJumpersGroups; //Dla każdej rundy vector z vectorami dla każdej grupy. QVector<StartListCompetitorStatus> to lista startowa dla danej grupy (np 8-osobowa)
    QVector<QPair<Team *, int>> teamsAdvanceStatuses;

public:
    Team *getActualTeam() const;
    void setActualTeam(Team *newActualTeam);
    QVector<QVector<Team *> > getRoundsTeams() const;
    QVector<QVector<Team *> > & getRoundsTeamsReference();
    void setRoundsTeams(const QVector<QVector<Team *> > &newRoundTeams);
    QVector<Team *>  & getActualRoundTeamsReference();
    QVector<QPair<Team *, int> > getTeamsAdvanceStatuses() const;
    void setTeamsAdvanceStatuses(const QVector<QPair<Team *, int> > &newTeamsAdvanceStatuses);

    QVector<QVector<QVector<StartListCompetitorStatus> > > getRoundsJumpersGroups() const;
    QVector<QVector<QVector<StartListCompetitorStatus> > > &getRoundsJumpersGroupsReference();
    void setRoundsJumpersGroups(const QVector<QVector<QVector<StartListCompetitorStatus> > > &newRoundsJumpersGroups);
    QVector<QVector<StartListCompetitorStatus> > &getActualRoundJumpersGroupsReference();
    QVector<StartListCompetitorStatus> &getActualGroupStartListReference();
};

#endif // TEAMCOMPETITIONMANAGER_H
