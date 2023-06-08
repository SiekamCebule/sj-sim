#include "Season.h"

Season::Season()
{

}

int Season::getSeasonNumber() const
{
    return seasonNumber;
}

void Season::setSeasonNumber(int newSeasonNumber)
{
    seasonNumber = newSeasonNumber;
}

QVector<AbstractClassification *> Season::getClassifications() const
{
    return classifications;
}

void Season::setClassifications(const QVector<AbstractClassification *> &newClassifications)
{
    classifications = newClassifications;
}

SeasonSettings Season::getSettings() const
{
    return settings;
}

void Season::setSettings(const SeasonSettings &newSettings)
{
    settings = newSettings;
}

SeasonCalendar Season::getCalendar() const
{
    return calendar;
}

void Season::setCalendar(const SeasonCalendar &newCalendar)
{
    calendar = newCalendar;
}