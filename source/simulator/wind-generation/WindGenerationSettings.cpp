#include "WindGenerationSettings.h"

WindGenerationSettings::WindGenerationSettings(short baseDirection, double windDirectionInstability, double baseWindStrength, double windStrengthInstability) : baseDirection(baseDirection),
    windDirectionInstability(windDirectionInstability),
    baseWindStrength(baseWindStrength),
    windStrengthInstability(windStrengthInstability)
{}

double WindGenerationSettings::getBaseDirection() const
{
    return baseDirection;
}

void WindGenerationSettings::setBaseDirection(double newBaseDirection)
{
    baseDirection = newBaseDirection;
}

double WindGenerationSettings::getWindStrengthInstability() const
{
    return windStrengthInstability;
}

void WindGenerationSettings::setWindStrengthInstability(double newWindStrengthInstability)
{
    windStrengthInstability = newWindStrengthInstability;
}

double WindGenerationSettings::getBaseWindStrength() const
{
    return baseWindStrength;
}

void WindGenerationSettings::setBaseWindStrength(double newBaseWindStrength)
{
    baseWindStrength = newBaseWindStrength;
}

double WindGenerationSettings::getWindDirectionInstability() const
{
    return windDirectionInstability;
}

void WindGenerationSettings::setWindDirectionInstability(double newWindDirectionInstability)
{
    windDirectionInstability = newWindDirectionInstability;
}
