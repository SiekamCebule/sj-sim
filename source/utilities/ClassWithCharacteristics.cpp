#include "ClassWithCharacteristics.h"
#include "../simulator/Characteristic.h"

QSet<Characteristic> ClassWithCharacteristics::getCharacteristics() const
{
    return characteristics;
}

void ClassWithCharacteristics::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
{
    characteristics = newCharacteristics;
}

ClassWithCharacteristics::ClassWithCharacteristics()
{

}

bool ClassWithCharacteristics::characteristicsContains(const QString &type)
{
    return characteristics.contains(Characteristic(type));
}

void ClassWithCharacteristics::insertCharacteristic(const Characteristic &characteristic)
{
    characteristics.insert(characteristic);
}

void ClassWithCharacteristics::insertCharacteristic(const QString &type, double level)
{
    characteristics.insert(Characteristic(level, type));
}

void ClassWithCharacteristics::removeCharacteristic(const Characteristic &characteristic)
{
    characteristics.remove(characteristic);
}

void ClassWithCharacteristics::removeCharacteristic(const QString &type)
{
    characteristics.remove(Characteristic(0, type));
}

double ClassWithCharacteristics::getLevelOfCharacteristic(const QString &characteristicType)
{
    for(QSet<Characteristic>::iterator it = characteristics.begin(); it != characteristics.end(); it++)
    {
        if(it->getType() == characteristicType)
            return it->getLevel();
    }
    return 0;
}
