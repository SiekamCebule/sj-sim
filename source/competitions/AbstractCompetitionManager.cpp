#include "AbstractCompetitionManager.h"

#include <QDebug>

AbstractCompetitionManager::AbstractCompetitionManager(short type) : type(type)
{
    actualGate = actualRound = runQualificationsForSingleCompetition = 0;
    results = nullptr;
    competitionRules = nullptr;
    competitionInfo = nullptr;
}

void AbstractCompetitionManager::initActualRound()
{
    if(getRunQualificationsForSingleCompetition() == true)
        actualRound = 0;
    else actualRound = 1;
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

void AbstractCompetitionManager::setCompetitionRules(CompetitionRules *newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

AbstractCompetitionResults AbstractCompetitionManager::getQualificationsResults() const
{
    return qualificationsResults;
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

bool AbstractCompetitionManager::getRunQualificationsForSingleCompetition() const
{
    return runQualificationsForSingleCompetition;
}

void AbstractCompetitionManager::setRunQualificationsForSingleCompetition(bool newRunQualificationsForSingleCompetition)
{
    runQualificationsForSingleCompetition = newRunQualificationsForSingleCompetition;
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

void AbstractCompetitionManager::setActualWindGenerationSettings(const QVector<WindGenerationSettings> &newActualWindGenerationSettings)
{
    actualWindGenerationSettings = newActualWindGenerationSettings;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
