#ifndef SEASON_H
#define SEASON_H

#include <QVector>
#include "SeasonCalendar.h"
#include "Classification.h"
#include "SeasonSettings.h"

class Season
{
public:
    Season();

private:
    SeasonCalendar calendar;
    SeasonSettings settings;
    int seasonNumber;

public:
    SeasonCalendar getCalendar() const;
    void setCalendar(const SeasonCalendar &newCalendar);
    SeasonSettings getSettings() const;
    void setSettings(const SeasonSettings &newSettings);
    int getSeasonNumber() const;
    void setSeasonNumber(int newSeasonNumber);
};

#endif // SEASON_H
