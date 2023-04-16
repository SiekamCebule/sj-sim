#include "SeasonSettings.h"

SeasonSettings::SeasonSettings()
{
    resultsSaveLevel = 0;
}

short SeasonSettings::getResultsSaveLevel() const
{
    return resultsSaveLevel;
}

void SeasonSettings::setResultsSaveLevel(short newResultsSaveLevel)
{
    resultsSaveLevel = newResultsSaveLevel;
}
