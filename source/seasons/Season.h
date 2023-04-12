#ifndef SEASON_H
#define SEASON_H

#include <QVector>
#include "SeasonCalendar.h"
#include "classifications/AbstractClassification.h"
#include "SeasonSettings.h"

class Season
{
public:
    Season();

private:
    SeasonCalendar calendar;
    SeasonSettings settings;
    QVector<AbstractClassification *> classifications;
    int seasonNumber;

public:
    SeasonCalendar getCalendar() const;
    void setCalendar(const SeasonCalendar &newCalendar);
    SeasonSettings getSettings() const;
    void setSettings(const SeasonSettings &newSettings);
    QVector<AbstractClassification *> getClassifications() const;
    void setClassifications(const QVector<AbstractClassification *> &newClassifications);
    int getSeasonNumber() const;
    void setSeasonNumber(int newSeasonNumber);
};

#endif // SEASON_H
