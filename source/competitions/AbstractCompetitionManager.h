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

    virtual void simulateNext() {}
    virtual bool checkRoundEnd() {return false;}
    virtual bool checkCompetitionEnd() {return false;}

    virtual void setupNextRound() {}
    virtual void fillCompletedJumpsToStartOfRound() {}

    virtual int getCompetitiorsCountForActualRound() {return 0;}
    virtual bool getSortStartListForActualRound() {return false;}

    void updateToBeatDistance();
    void updateToAdvanceDistance();
    void updateLeaderResult();
    void updateLastQualifiedResult();
    virtual void updateActualCompetitorPointsToTheLeader() {}

signals:
    void roundEnd();
    void competitionEnd();

    void actualJumperIndexChanged();

protected:
    short type;

    CompetitionInfo * competitionInfo;
    CompetitionRules * competitionRules;
    CompetitionResults * results;

    QVector<int> roundsStartingGates;
    int actualGate;
    int actualRound; //liczone od 1.
    double toBeatLineDistance;
    double toAdvanceLineDistance;

    bool coachGateForNextJumper;
    int actualCoachGate;

    int actualStartListIndex;
    double actualCompetitorPointsToTheLeader;
    JumpManipulator actualJumpManipulator;

    QVector<StartListCompetitorStatus> startListStatus;

    bool roundShouldBeEnded;
    bool competiitonShouldBeEnded;
    bool lastJump;

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
    JumpManipulator getActualJumpManipulator() const;
    void setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator);
    QVector<StartListCompetitorStatus> getStartListStatus() const;
    void setStartListStatus(const QVector<StartListCompetitorStatus> &newStartListStatus);
    bool getRoundShouldBeEnded() const;
    void setRoundShouldBeEnded(bool newRoundShouldBeEnded);
    bool getCompetiitonShouldBeEnded() const;
    void setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded);
    bool getLastJump() const;
    void setLastJump(bool newLastJump);
    CompetitionSingleResult *getLeaderResult() const;
    void setLeaderResult(CompetitionSingleResult *newLeaderResult);
    CompetitionSingleResult *getLastQualifiedResult() const;
    void setLastQualifiedResult(CompetitionSingleResult *newLastQualifiedResult);
};

#endif // ABSTRACTCOMPETITIONMANAGER_H
