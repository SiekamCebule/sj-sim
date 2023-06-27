#include "CompetitionInfo.h"

#include "../simulator/Hill.h"
#include "CompetitionRules.h"
#include "CompetitionResults.h"

#include <QDate>

CompetitionInfo::CompetitionInfo(Hill *hill) : hill(hill)
{
    exceptionalRoundsCount = (-1);
    cancelled = false;
    results = nullptr;
    serieType = Competition;
    trialRound = nullptr;
}

CompetitionInfo::~CompetitionInfo()
{
    if(results != nullptr){
        delete results;
    }
    /*for(auto & t : trainings)
        if(t != nullptr)
            delete t;
    if(trialRound != nullptr)
        delete trialRound;*/
}

QVector<CompetitionInfo *> CompetitionInfo::getTrainings() const
{
    return trainings;
}

QVector<CompetitionInfo *> &CompetitionInfo::getTrainingsReference()
{
    return trainings;
}

void CompetitionInfo::setTrainings(const QVector<CompetitionInfo *> &newTrainings)
{
    trainings = newTrainings;
}

CompetitionInfo *CompetitionInfo::getTrialRound() const
{
    return trialRound;
}

void CompetitionInfo::setTrialRound(CompetitionInfo *newTrialRound)
{
    trialRound = newTrialRound;
}

bool CompetitionInfo::getCancelled() const
{
    return cancelled;
}

void CompetitionInfo::setCancelled(bool newCancelled)
{
    cancelled = newCancelled;
}

int CompetitionInfo::getExceptionalRoundsCount() const
{
    return exceptionalRoundsCount;
}

void CompetitionInfo::setExceptionalRoundsCount(int newExceptionalRoundsCount)
{
    exceptionalRoundsCount = newExceptionalRoundsCount;
}

short CompetitionInfo::getSerieType() const
{
    return serieType;
}

void CompetitionInfo::setSerieType(short newSerieType)
{
    serieType = newSerieType;
}

CompetitionResults *CompetitionInfo::getResults() const
{
    return results;
}

void CompetitionInfo::setResults(CompetitionResults *newResults)
{
    results = newResults;
}

Hill *CompetitionInfo::getHill() const
{
    return hill;
}

void CompetitionInfo::setHill(Hill *newHill)
{
    hill = newHill;
}

CompetitionRules CompetitionInfo::getRules() const
{
    return rules;
}

void CompetitionInfo::setRules(const CompetitionRules &newRules)
{
    rules = newRules;
}
