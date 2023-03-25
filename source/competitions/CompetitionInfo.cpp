#include "CompetitionInfo.h"

#include "../simulator/Hill.h"
#include "CompetitionRules.h"
#include "AbstractCompetitionResults.h"

#include <QDate>

CompetitionInfo::CompetitionInfo()
{

}

CompetitionInfo::~CompetitionInfo()
{
    if(results != nullptr){
        delete results;
    }
}

AbstractCompetitionResults *CompetitionInfo::getResults() const
{
    return results;
}

void CompetitionInfo::setResults(AbstractCompetitionResults *newResults)
{
    results = newResults;
}

QDate CompetitionInfo::getDate() const
{
    return date;
}

void CompetitionInfo::setDate(const QDate &newDate)
{
    date = newDate;
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
