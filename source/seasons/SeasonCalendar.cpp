#include "SeasonCalendar.h"

SeasonCalendar::SeasonCalendar()
{

}

QVector<CompetitionInfo> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo> &newCompetitions)
{
    competitions = newCompetitions;
}
