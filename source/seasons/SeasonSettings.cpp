#include "SeasonSettings.h"

SeasonSettings::SeasonSettings()
{
    resultsSaveLevel = 0;
    seasonArchiving = 0;
}

bool SeasonSettings::getSeasonArchiving() const
{
    return seasonArchiving;
}

void SeasonSettings::setSeasonArchiving(bool newSeasonArchiving)
{
    seasonArchiving = newSeasonArchiving;
}

short SeasonSettings::getResultsSaveLevel() const
{
    return resultsSaveLevel;
}

void SeasonSettings::setResultsSaveLevel(short newResultsSaveLevel)
{
    resultsSaveLevel = newResultsSaveLevel;
}
