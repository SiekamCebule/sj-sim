#include "AbstractCompetitionManager.h"

#include <QDebug>

AbstractCompetitionManager::AbstractCompetitionManager(short type, int startingGate) : type(type)
{
    actualGate = actualRound = 0;
    results = nullptr;
    competitionRules = nullptr;
    competitionInfo = nullptr;
    toBeatLineDistance = toAdvanceLineDistance = 0;
    coachGateForNextJumper = false;
    actualCoachGate = 0;
    actualStartListIndex = actualCompetitorPointsToTheLeader = 0;
    roundsStartingGates = {startingGate};
}

void AbstractCompetitionManager::updateLeaderResult()
{
    results->sortInDescendingOrder();
    leaderResult = results->getResultByIndex(0);
}

JumpManipulator *AbstractCompetitionManager::getActualJumpManipulator() const
{
    return actualJumpManipulator;
}

void AbstractCompetitionManager::setActualJumpManipulator(JumpManipulator *newActualJumpManipulator)
{
    actualJumpManipulator = newActualJumpManipulator;
}

QVector<Wind> *AbstractCompetitionManager::getActualWinds() const
{
    return actualWinds;
}

void AbstractCompetitionManager::setActualWinds(QVector<Wind> *newActualWinds)
{
    actualWinds = newActualWinds;
}

Jumper *AbstractCompetitionManager::getActualJumper() const
{
    return actualJumper;
}

void AbstractCompetitionManager::setActualJumper(Jumper *newActualJumper)
{
    actualJumper = newActualJumper;
}

short AbstractCompetitionManager::getType() const
{
    return type;
}

void AbstractCompetitionManager::setType(short newType)
{
    type = newType;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}

CompetitionRules *AbstractCompetitionManager::getCompetitionRules() const
{
    return competitionRules;
}

void AbstractCompetitionManager::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

CompetitionResults *AbstractCompetitionManager::getResults() const
{
    return results;
}

void AbstractCompetitionManager::setResults(CompetitionResults *newResults)
{
    results = newResults;
}

QVector<int> AbstractCompetitionManager::getRoundsStartingGates() const
{
    return roundsStartingGates;
}

void AbstractCompetitionManager::setRoundsStartingGates(const QVector<int> &newRoundsStartingGates)
{
    roundsStartingGates = newRoundsStartingGates;
}

int AbstractCompetitionManager::getActualGate() const
{
    return actualGate;
}

void AbstractCompetitionManager::setActualGate(int newActualGate)
{
    actualGate = newActualGate;
}

int AbstractCompetitionManager::getActualRound() const
{
    return actualRound;
}

void AbstractCompetitionManager::setActualRound(int newActualRound)
{
    actualRound = newActualRound;
}

double AbstractCompetitionManager::getToBeatLineDistance() const
{
    return toBeatLineDistance;
}

void AbstractCompetitionManager::setToBeatLineDistance(double newToBeatLineDistance)
{
    toBeatLineDistance = newToBeatLineDistance;
}

double AbstractCompetitionManager::getToAdvanceLineDistance() const
{
    return toAdvanceLineDistance;
}

void AbstractCompetitionManager::setToAdvanceLineDistance(double newToAdvanceLineDistance)
{
    toAdvanceLineDistance = newToAdvanceLineDistance;
}

bool AbstractCompetitionManager::getCoachGateForNextJumper() const
{
    return coachGateForNextJumper;
}

void AbstractCompetitionManager::setCoachGateForNextJumper(bool newCoachGateForNextJumper)
{
    coachGateForNextJumper = newCoachGateForNextJumper;
}

int AbstractCompetitionManager::getActualCoachGate() const
{
    return actualCoachGate;
}

void AbstractCompetitionManager::setActualCoachGate(int newActualCoachGate)
{
    actualCoachGate = newActualCoachGate;
}

int AbstractCompetitionManager::getActualStartListIndex() const
{
    return actualStartListIndex;
}

void AbstractCompetitionManager::setActualStartListIndex(int newActualStartListIndex)
{
    actualStartListIndex = newActualStartListIndex;
}

double AbstractCompetitionManager::getActualCompetitorPointsToTheLeader() const
{
    return actualCompetitorPointsToTheLeader;
}

void AbstractCompetitionManager::setActualCompetitorPointsToTheLeader(double newActualCompetitorPointsToTheLeader)
{
    actualCompetitorPointsToTheLeader = newActualCompetitorPointsToTheLeader;
}

JumpManipulator AbstractCompetitionManager::getActualJumpManipulator() const
{
    return actualJumpManipulator;
}

void AbstractCompetitionManager::setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator)
{
    actualJumpManipulator = newActualJumpManipulator;
}

QVector<StartListCompetitorStatus> AbstractCompetitionManager::getStartListStatus() const
{
    return startListStatuses;
}

void AbstractCompetitionManager::setStartListStatus(const QVector<StartListCompetitorStatus> &newStartListStatus)
{
    startListStatuses = newStartListStatus;
}

bool AbstractCompetitionManager::getRoundShouldBeEnded() const
{
    return roundShouldBeEnded;
}

void AbstractCompetitionManager::setRoundShouldBeEnded(bool newRoundShouldBeEnded)
{
    roundShouldBeEnded = newRoundShouldBeEnded;
}

bool AbstractCompetitionManager::getCompetiitonShouldBeEnded() const
{
    return competiitonShouldBeEnded;
}

void AbstractCompetitionManager::setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded)
{
    competiitonShouldBeEnded = newCompetiitonShouldBeEnded;
}

bool AbstractCompetitionManager::getLastJump() const
{
    return lastJump;
}

void AbstractCompetitionManager::setLastJump(bool newLastJump)
{
    lastJump = newLastJump;
}

CompetitionSingleResult *AbstractCompetitionManager::getLeaderResult() const
{
    return leaderResult;
}

void AbstractCompetitionManager::setLeaderResult(CompetitionSingleResult *newLeaderResult)
{
    leaderResult = newLeaderResult;
}

CompetitionSingleResult *AbstractCompetitionManager::getLastQualifiedResult() const
{
    return lastQualifiedResult;
}

void AbstractCompetitionManager::setLastQualifiedResult(CompetitionSingleResult *newLastQualifiedResult)
{
    lastQualifiedResult = newLastQualifiedResult;
}
