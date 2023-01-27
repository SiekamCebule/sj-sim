#include "WindsGenerator.h"

WindsGenerator::WindsGenerator(double baseWind, double windChange, const QSet<QString> &userSettings, QVector<Wind> *winds) : winds(winds),
    baseWind(baseWind),
    windChange(windChange),
    userSettings(userSettings)
{
}

QVector<Wind> *WindsGenerator::getWinds() const
{
    return winds;
}

void WindsGenerator::setWinds(QVector<Wind> *newWinds)
{
    winds = newWinds;
}

double WindsGenerator::getBaseWind() const
{
    return baseWind;
}

void WindsGenerator::setBaseWind(double newBaseWind)
{
    baseWind = newBaseWind;
}

double WindsGenerator::getWindChange() const
{
    return windChange;
}

void WindsGenerator::setWindChange(double newWindChange)
{
    windChange = newWindChange;
}

QSet<QString> & WindsGenerator::getUserSettings()
{
    return userSettings;
}

void WindsGenerator::setUserSettings(const QSet<QString> &newUserSettings)
{
    userSettings = newUserSettings;
}
