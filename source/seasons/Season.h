#ifndef SEASON_H
#define SEASON_H

#include <QVector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "SeasonCalendar.h"
#include "Classification.h"
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
    int seasonNumber;

public:
    SeasonCalendar getCalendar() const;
    SeasonCalendar & getCalendarReference();
    void setCalendar(const SeasonCalendar &newCalendar);
    int getSeasonNumber() const;
    void setSeasonNumber(int newSeasonNumber);
};

#endif // SEASON_H
