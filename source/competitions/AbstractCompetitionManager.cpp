#include "AbstractCompetitionManager.h"

#include <QDebug>

AbstractCompetitionManager::AbstractCompetitionManager(short type) : type(type)
{
    actualGate = 0;
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

CompetitionRules AbstractCompetitionManager::getCompetitionRules() const
{
    return competitionRules;
}

void AbstractCompetitionManager::setCompetitionRules(const CompetitionRules &newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

CompetitionInfo *AbstractCompetitionManager::getCompetitionInfo() const
{
    return competitionInfo;
}

void AbstractCompetitionManager::setCompetitionInfo(CompetitionInfo *newCompetitionInfo)
{
    competitionInfo = newCompetitionInfo;
}
