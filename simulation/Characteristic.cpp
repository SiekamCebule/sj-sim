#include "Characteristic.h"

Characteristic::Characteristic(short level, const QString &type) : level(level),
    type(type)
{}

Characteristic::Characteristic(const QString &type) : type(type), level(0)
{
}

short Characteristic::getLevel() const
{
    return level;
}

void Characteristic::setLevel(short newLevel)
{
    level = newLevel;
}

QString Characteristic::getType() const
{
    return type;
}

void Characteristic::setType(const QString &newType)
{
    type = newType;
}
