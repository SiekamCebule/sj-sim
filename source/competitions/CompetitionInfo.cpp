#include "CompetitionInfo.h"

#include "../simulator/Hill.h"
#include "CompetitionRules.h"

#include <QDate>

CompetitionInfo::CompetitionInfo()
{

}

QDate CompetitionInfo::getDate() const
{
    return date;
}

void CompetitionInfo::setDate(const QDate &newDate)
{
    date = newDate;
}

CompetitionRules *CompetitionInfo::getRules() const
{
    return rules;
}

void CompetitionInfo::setRules(CompetitionRules *newRules)
{
    rules = newRules;
}

Hill *CompetitionInfo::getHill() const
{
    return hill;
}

void CompetitionInfo::setHill(Hill *newHill)
{
    hill = newHill;
}
