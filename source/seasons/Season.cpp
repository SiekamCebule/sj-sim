#include "Season.h"

Season::Season() :
    ClassWithID()
{
    generateID();
}

Season::~Season()
{
}

QJsonObject Season::getJsonObject(Season &season)
{
    QJsonObject obj;
    obj.insert("id", QString::number(season.getID()));
    obj.insert("season-number", season.getSeasonNumber());
    if(season.getActualCalendar() != nullptr)
        obj.insert("actual-calendar-id", QString::number(season.getActualCalendar()->getID()));
    else
        obj.insert("actual-calendar-id", QString::number(-1));
    QJsonArray array;
    for(auto & cal : season.getCalendarsReference())
    {
        array.push_back(SeasonCalendar::getJsonObject(*cal));
    }
    obj.insert("calendars", array);

    return obj;
}

Season Season::getFromJson(QJsonObject obj, DatabaseObjectsManager * objectsManager)
{
    Season season;
    season.setID(obj.value("id").toString().toULong());
    season.setSeasonNumber(obj.value("season-number").toInt());
    QJsonArray array = obj.value("calendars").toArray();
    for(auto a : array)
    {
        season.getCalendarsReference().push_back(new SeasonCalendar(SeasonCalendar::getFromJson(a.toObject(), objectsManager)));
    }
    if(objectsManager != nullptr)
        objectsManager->fill(&season.getCalendarsReference());

    if(obj.value("actual-calendar-id").toString().toULong() == -1)
        season.setActualCalendar(nullptr);
    else
        season.setActualCalendar(static_cast<SeasonCalendar *>(objectsManager->getObjectByID(obj.value("actual-calendar-id").toString().toULong())));

    return season;
}

bool Season::containsCalendarByName(QString n)
{
    for(auto & c : calendars)
    {
        if(c->getName() == n)
            return true;
    }
    return false;
}

SeasonCalendar *Season::getActualCalendar() const
{
    return actualCalendar;
}

void Season::setActualCalendar(SeasonCalendar *newActualCalendar)
{
    actualCalendar = newActualCalendar;
}

QVector<SeasonCalendar *> Season::getCalendars() const
{
    return calendars;
}

QVector<SeasonCalendar *> &Season::getCalendarsReference()
{
    return calendars;
}

void Season::setCalendars(const QVector<SeasonCalendar *> &newCalendars)
{
    calendars = newCalendars;
}

int Season::getSeasonNumber() const
{
    return seasonNumber;
}

void Season::setSeasonNumber(int newSeasonNumber)
{
    seasonNumber = newSeasonNumber;
}
