#include "AbstractCompetitionManager.h"

#include <QDebug>

AbstractCompetitionManager::AbstractCompetitionManager(short type, int startingGate) : type(type), roundStartingGate(startingGate)
{
    actualGate = actualRound = 0;
    results = nullptr;
    competitionRules = nullptr;
    competitionInfo = nullptr;
}

void AbstractCompetitionManager::generateActualWinds()
{
    windsGenerator.setGenerationSettings(actualWindGenerationSettings);
    actualWinds = windsGenerator.generateWinds();
}

bool AbstractCompetitionManager::getIsCoachGate() const
{
    return isCoachGate;
}

void AbstractCompetitionManager::setIsCoachGate(bool newIsCoachGate)
{
    isCoachGate = newIsCoachGate;
}

int AbstractCompetitionManager::getActualCoachGate() const
{
    return actualCoachGate;
}

void AbstractCompetitionManager::setActualCoachGate(int newActualCoachGate)
{
    actualCoachGate = newActualCoachGate;
}

JumpManipulator AbstractCompetitionManager::getActualJumpManipulator() const
{
    return actualJumpManipulator;
}

void AbstractCompetitionManager::setActualJumpManipulator(const JumpManipulator &newActualJumpManipulator)
{
    actualJumpManipulator = newActualJumpManipulator;
}

double AbstractCompetitionManager::getActualJumperPointsToTheLeader() const
{
    return actualJumperPointsToTheLeader;
}

void AbstractCompetitionManager::setActualJumperPointsToTheLeader(double newActualJumperPointsToTheLeader)
{
    actualJumperPointsToTheLeader = newActualJumperPointsToTheLeader;
}

double AbstractCompetitionManager::getToAdvanceDistance() const
{
    return toAdvanceDistance;
}

void AbstractCompetitionManager::setToAdvanceDistance(double newToAdvanceDistance)
{
    toAdvanceDistance = newToAdvanceDistance;
}

int AbstractCompetitionManager::getActualJumperIndex() const
{
    return actualJumperIndex;
}

void AbstractCompetitionManager::setActualJumperIndex(int newActualJumperIndex)
{
    actualJumperIndex = newActualJumperIndex;
    emit actualJumperIndexChanged();
}

Jumper *AbstractCompetitionManager::getActualJumper() const
{
    return actualJumper;
}

void AbstractCompetitionManager::setActualJumper(Jumper *newActualJumper)
{
    actualJumper = newActualJumper;
}

QVector<Wind> AbstractCompetitionManager::getActualWinds() const
{
    return actualWinds;
}

void AbstractCompetitionManager::setActualWinds(const QVector<Wind> &newActualWinds)
{
    actualWinds = newActualWinds;
}

int AbstractCompetitionManager::getRoundStartingGate() const
{
    return roundStartingGate;
}

void AbstractCompetitionManager::setRoundStartingGate(int newRoundStartingGate)
{
    roundStartingGate = newRoundStartingGate;
}

double AbstractCompetitionManager::getToBeatDistance() const
{
    return toBeatDistance;
}

void AbstractCompetitionManager::setToBeatDistance(double newToBeatDistance)
{
    toBeatDistance = newToBeatDistance;
}

bool AbstractCompetitionManager::getLastJump() const
{
    return lastJump;
}

void AbstractCompetitionManager::setLastJump(bool newLastJump)
{
    lastJump = newLastJump;
}

bool AbstractCompetitionManager::getCompetiitonShouldBeEnded() const
{
    return competiitonShouldBeEnded;
}

void AbstractCompetitionManager::setCompetiitonShouldBeEnded(bool newCompetiitonShouldBeEnded)
{
    competiitonShouldBeEnded = newCompetiitonShouldBeEnded;
}

bool AbstractCompetitionManager::getRoundShouldBeEnded() const
{
    return roundShouldBeEnded;
}

void AbstractCompetitionManager::setRoundShouldBeEnded(bool newRoundShouldBeEnded)
{
    roundShouldBeEnded = newRoundShouldBeEnded;
}

int AbstractCompetitionManager::getDSQBaseProbability() const
{
    return DSQBaseProbability;
}

void AbstractCompetitionManager::setDSQBaseProbability(int newDSQBaseProbability)
{
    DSQBaseProbability = newDSQBaseProbability;
}

AbstractCompetitionResults *AbstractCompetitionManager::getResults() const
{
    return results;
}

void AbstractCompetitionManager::setResults(AbstractCompetitionResults *newResults)
{
    results = newResults;
}

CompetitionRules *AbstractCompetitionManager::getCompetitionRules() const
{
    return competitionRules;
}

CompetitionRules *AbstractCompetitionManager::getEditableCompetitionRules()
{
    return competitionRules;
}

void AbstractCompetitionManager::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

QVector<bool> * AbstractCompetitionManager::getCompletedJumpsPointer()
{
    return &completedJumps;
}

QSet<int> * AbstractCompetitionManager::getHasDSQPointer()
{
    return &hasDSQ;
}

QSet<int> * AbstractCompetitionManager::getHasDNSPointer()
{
    return &hasDNS;
}

int AbstractCompetitionManager::getActualRound() const
{
    return actualRound;
}

void AbstractCompetitionManager::setActualRound(int newActualRound)
{
    actualRound = newActualRound;
}

int AbstractCompetitionManager::getActualGate() const
{
    return actualGate;
}

void AbstractCompetitionManager::setActualGate(int newActualGate)
{
    actualGate = newActualGate;
}

short AbstractCompetitionManager::getType() const
{
    return type;
}

void AbstractCompetitionManager::setType(short newType)
{
    type = newType;
}

QVector<WindGenerationSettings> AbstractCompetitionManager::getActualWindGenerationSettings() const
{
    return actualWindGenerationSettings;
}

QVector<WindGenerationSettings> * AbstractCompetitionManager::getActualWindGenerationSettingsPointer()
{
    return &actualWindGenerationSettings;
}

void AbstractCompetitionManager::setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings)
{
    actualWindGenerationSettings = newActualWindGenerationSettings;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

CompetitionInfo *AbstractCompetitionManager::getEditableCompetitionInfo()
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
