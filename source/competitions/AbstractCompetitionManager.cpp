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
    setActualStartListIndex(0);
    actualCompetitorPointsToTheLeader = 0;
    roundsStartingGates = {startingGate};
}

bool AbstractCompetitionManager::checkRoundEnd()
{
    return (isAllJumpsAreFinished() == true);
}

bool AbstractCompetitionManager::checkCompetitionEnd()
{
    return (isAllJumpsAreFinished() && (actualRound == competitionRules->getEditableRounds().count() || actualRound == competitionInfo->getExceptionalRoundsCount()));
}

void AbstractCompetitionManager::updateLeaderResult()
{
    results->sortInDescendingOrder();
    leaderResult = results->getResultByIndex(0);
}

int AbstractCompetitionManager::getFirstUnfinishedStartListStatus()
{
    int index = 0;
    for(auto & status : startListStatuses){
        if(status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
            return index;
        else
            index++;
    }
    return index; // 0
}

bool AbstractCompetitionManager::isAllJumpsAreFinished()
{
    for(auto & status : startListStatuses){
        if(status.getJumpStatus() == StartListCompetitorStatus::Unfinished)
            return false;
    }
    return true;
}

QVector<WindGenerationSettings> AbstractCompetitionManager::getWindGenerationSettings() const
{
    return windGenerationSettings;
}

QVector<WindGenerationSettings> & AbstractCompetitionManager::getWindGenerationSettingsReference()
{
    return windGenerationSettings;
}

void AbstractCompetitionManager::setWindGenerationSettings(const QVector<WindGenerationSettings> &newWindGenerationSettings)
{
    windGenerationSettings = newWindGenerationSettings;
}

int AbstractCompetitionManager::getBaseDSQProbability() const
{
    return baseDSQProbability;
}

void AbstractCompetitionManager::setBaseDSQProbability(int newBaseDSQProbability)
{
    baseDSQProbability = newBaseDSQProbability;
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

QVector<int> & AbstractCompetitionManager::getRoundsStartingGatesReference()
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
    emit actualStartListIndexChanged();
}

double AbstractCompetitionManager::getActualCompetitorPointsToTheLeader() const
{
    return actualCompetitorPointsToTheLeader;
}

void AbstractCompetitionManager::setActualCompetitorPointsToTheLeader(double newActualCompetitorPointsToTheLeader)
{
    actualCompetitorPointsToTheLeader = newActualCompetitorPointsToTheLeader;
}

QVector<StartListCompetitorStatus> AbstractCompetitionManager::getStartListStatuses() const
{
    return startListStatuses;
}

QVector<StartListCompetitorStatus> &AbstractCompetitionManager::getStartListStatusesReference()
{
    return startListStatuses;
}

void AbstractCompetitionManager::setStartListStatus(const QVector<StartListCompetitorStatus> &newStartListStatus)
{
    startListStatuses = newStartListStatus;
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
