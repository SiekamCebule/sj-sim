#ifndef SEASONSETTINGS_H
#define SEASONSETTINGS_H

#include <QVector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../global/SeasonDatabaseObjectsManager.h"

class SeasonSettings
{
public:
    SeasonSettings();

    static SeasonSettings getFromJson(QJsonObject json);
    static QJsonObject getJsonObject(SeasonSettings & settings);
private:
};

#endif // SEASONSETTINGS_H
