#ifndef TEAMCOMPETITIONMANAGER_H
#define TEAMCOMPETITIONMANAGER_H

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Team.h"

class TeamCompetitionManager : public AbstractCompetitionManager
{
    Q_OBJECT
public:
    TeamCompetitionManager();
    void setActualTeamToActualJumperTeam();

    void setupNextRound();
    void setupStartListStatusesForActualRound(); // zrobienie grup list startowych
    void updateCompetitorsAdvanceStatuses();

    bool checkCompetitionEnd() override;
    bool checkRoundEnd() override;
    bool checkGroupEnd();
    void setupNextGroup();

    int getAdvanceStatusOfTeam(Team * team);
    QVector<Team *> getFilteredTeamsForNextRound();

private:
    int actualGroup;
    Team * actualTeam;
    QVector<QVector<Team *>> roundsTeams;
    QVector<QPair<Team *, int>> teamsAdvanceStatuses;

public:
    Team *getActualTeam() const;
    void setActualTeam(Team *newActualTeam);
    QVector<QVector<Team *> > getRoundsTeams() const;
    QVector<QVector<Team *> > & getRoundsTeamsReference();
    void setRoundsTeams(const QVector<QVector<Team *> > &newRoundTeams);
    QVector<Team *>  & getActualRoundTeamsReference();
    QVector<QPair<Team *, int> > getTeamsAdvanceStatuses() const;
     QVector<QPair<Team *, int> > & getTeamsAdvanceStatusesReference();
    void setTeamsAdvanceStatuses(const QVector<QPair<Team *, int> > &newTeamsAdvanceStatuses);
    int getActualGroup() const;
    void setActualGroup(int newActualGroup);

signals:
    void actualGroupChanged();
};

#endif // TEAMCOMPETITIONMANAGER_H
