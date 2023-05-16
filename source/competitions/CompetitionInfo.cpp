#include "CompetitionInfo.h"

#include "../simulator/Hill.h"
#include "CompetitionRules.h"
#include "CompetitionResults.h"

#include <QDate>

CompetitionInfo::CompetitionInfo()
{
    exceptionalRoundsCount = (-1);
}

CompetitionInfo::~CompetitionInfo()
{
    if(results != nullptr){
        delete results;
    }
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
