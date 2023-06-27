#include "SeasonCalendar.h"

SeasonCalendar::SeasonCalendar()
{

}

SeasonCalendar::~SeasonCalendar()
{
    for(auto & comp : competitions)
        delete comp;
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo *> &newCompetitions)
{
    competitions = newCompetitions;
}

QVector<CompetitionInfo *> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

QVector<CompetitionInfo *> &SeasonCalendar::getCompetitionsReference()
{
    return competitions;
}
