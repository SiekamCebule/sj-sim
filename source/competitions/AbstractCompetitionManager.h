#ifndef ABSTRACTCOMPETITIONMANAGER_H
#define ABSTRACTCOMPETITIONMANAGER_H

#include <QObject>
#include "../simulator/JumpSimulator.h"
#include "../simulator/JumpManipulator.h"
#include "CompetitionRules.h"
#include "CompetitionInfo.h"
#include "CompetitionResults.h"
#include "../simulator/wind-generation/WindGenerationSettings.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "StartListCompetitorStatus.h"

class AbstractCompetitionManager : public QObject
{
    Q_OBJECT
public:
    AbstractCompetitionManager(short type = CompetitionRules::Individual, int startingGate = 0);

    bool checkRoundEnd();
    bool checkCompetitionEnd();

    virtual void updateToBeatLineDistance() {}
    virtual void updateToAdvanceLineDistance() {}
    void updateLeaderResult();
    virtual void updateLastQualifiedResult() {}
    virtual void updateActualCompetitorPointsToTheLeader() {}

    int getFirstUnfinishedStartListStatus();
    bool isAllJumpsAreFinished();

signals:
    void actualJumperIndexChanged();
    void roundEnd();
    void competitionEnd();

protected:
    short type;

    CompetitionInfo * competitionInfo;
    CompetitionRules * competitionRules;
    CompetitionResults * results;

    QVector<int> roundsStartingGates;
    int actualGate;
    int actualRound; //liczone od 1.
    QVector<Wind> * actualWinds;

    double toBeatLineDistance;
    double toAdvanceLineDistance;
    double actualCompetitorPointsToTheLeader;

    bool coachGateForNextJumper;
    int actualCoachGate;

    Jumper * actualJumper;
    int actualStartListIndex;
    JumpManipulator * actualJumpManipulator;

    QVector<StartListCompetitorStatus> startListStatuses;

    CompetitionSingleResult * leaderResult;
    CompetitionSingleResult * lastQualifiedResult;

public:
    short getType() const;
    void setType(short newType);
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
    CompetitionResults *getResults() const;
    void setResults(CompetitionResults *newResults);
    QVector<int> getRoundsStartingGates() const;
    void setRoundsStartingGates(const QVector<int> &newRoundsStartingGates);
    int getActualGate() const;
    void setActualGate(int newActualGate);
    int getActualRound() const;
    void setActualRound(int newActualRound);
    double getToBeatLineDistance() const;
    void setToBeatLineDistance(double newToBeatLineDistance);
    double getToAdvanceLineDistance() const;
    void setToAdvanceLineDistance(double newToAdvanceLineDistance);
    bool getCoachGateForNextJumper() const;
    void setCoachGateForNextJumper(bool newCoachGateForNextJumper);
    int getActualCoachGate() const;
    void setActualCoachGate(int newActualCoachGate);
    int getActualStartListIndex() const;
    void setActualStartListIndex(int newActualStartListIndex);
    double getActualCompetitorPointsToTheLeader() const;
    void setActualCompetitorPointsToTheLeader(double newActualCompetitorPointsToTheLeader);
    QVector<StartListCompetitorStatus> getStartListStatus() const;
    void setStartListStatus(const QVector<StartListCompetitorStatus> &newStartListStatus);
    CompetitionSingleResult *getLeaderResult() const;
    void setLeaderResult(CompetitionSingleResult *newLeaderResult);
    CompetitionSingleResult *getLastQualifiedResult() const;
    void setLastQualifiedResult(CompetitionSingleResult *newLastQualifiedResult);
    Jumper *getActualJumper() const;
    void setActualJumper(Jumper *newActualJumper);
    QVector<Wind> *getActualWinds() const;
    void setActualWinds(QVector<Wind> *newActualWinds);
    JumpManipulator *getActualJumpManipulator() const;
    void setActualJumpManipulator(JumpManipulator *newActualJumpManipulator);
};

#endif // ABSTRACTCOMPETITIONMANAGER_H
