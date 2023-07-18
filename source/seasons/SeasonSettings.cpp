#include "SeasonSettings.h"

SeasonSettings::SeasonSettings()
{
}

SeasonSettings SeasonSettings::getFromJson(QJsonObject json)
{
    SeasonSettings settings;

    return settings;
}

QJsonObject SeasonSettings::getJsonObject(SeasonSettings &settings)
{
    QJsonObject obj;

    return obj;
}
