#ifndef SEASON_H
#define SEASON_H

#include <QVector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "SeasonCalendar.h"
#include "Classification.h"
#include "SeasonSettings.h"
#include "../global/SeasonDatabaseObjectsManager.h"

extern SeasonDatabaseObjectsManager seasonObjectsManager;

class Season : public ClassWithID
{
public:
    Season();

    static QJsonObject getJsonObject(Season & season);
    static Season getFromJson(QJsonObject obj, SeasonDatabaseObjectsManager *objectsManager);

private:
    SeasonCalendar calendar;
    SeasonSettings settings;
    int seasonNumber;

public:
    SeasonCalendar getCalendar() const;
    SeasonCalendar & getCalendarReference();
    void setCalendar(const SeasonCalendar &newCalendar);
    SeasonSettings getSettings() const;
    SeasonSettings & getSettingsReference();
    void setSettings(const SeasonSettings &newSettings);
    int getSeasonNumber() const;
    void setSeasonNumber(int newSeasonNumber);
};

#endif // SEASON_H
