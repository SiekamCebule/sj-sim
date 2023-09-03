#include "Season.h"

Season::Season() :
    ClassWithID()
{

}

QJsonObject Season::getJsonObject(Season &season)
{
    QJsonObject obj;
    obj.insert("id", QString::number(season.getID()));
    obj.insert("season-number", season.getSeasonNumber());
    obj.insert("calendar", SeasonCalendar::getJsonObject(season.getCalendarReference()));

    return obj;
}

Season Season::getFromJson(QJsonObject obj, DatabaseObjectsManager * objectsManager)
{
    Season season;
    season.setID(obj.value("id").toString().toULong());
    season.setSeasonNumber(obj.value("season-number").toInt());
    season.setCalendar(SeasonCalendar::getFromJson(obj.value("calendar").toObject(), objectsManager));

    return season;
}

// Gdzie powinny być zapisywane CompetitionResults i dlaczego w CompetitionResults to jest wskaźnik a nie obiekt?
// W sumie nie wiem dlaczego resultsy nie są jako obiekt w CompetitionInfo. Chyba powinniśmy tak zrobić bo inaczej będzie trzeba robić jako new i usuwać.

int Season::getSeasonNumber() const
{
    return seasonNumber;
}

void Season::setSeasonNumber(int newSeasonNumber)
{
    seasonNumber = newSeasonNumber;
}

SeasonCalendar Season::getCalendar() const
{
    return calendar;
}

SeasonCalendar &Season::getCalendarReference()
{
    return calendar;
}

void Season::setCalendar(const SeasonCalendar &newCalendar)
{
    calendar = newCalendar;
}
