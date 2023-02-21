#include "Characteristic.h"

#include <QDebug>
#include <QObject>

Characteristic::Characteristic(double level, const QString &type) : level(level),
    type(type)
{
    if(level < -10) level = -10;
    if(level > 10) level = 10;
}

Characteristic::Characteristic(const QString &type) :
    level(0),
    type(type)
{
}

double Characteristic::getLevel() const
{
    return level;
}

void Characteristic::setLevel(double newLevel)
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

QStringList Characteristic::characteristicTypesForSpecificParent(short parentType, bool transformToDisplay)
{
    QStringList characteristicTypes;
    switch(parentType)
    {
    case Jumper:
        characteristicTypes.push_back("takeoff-height");
        characteristicTypes.push_back("flight-height");
        characteristicTypes.push_back("landing-skill");
        break;
    case Hill:
        characteristicTypes.push_back("real-hs-point");
        characteristicTypes.push_back("takeoff-technique-effect");
        characteristicTypes.push_back("takeoff-power-effect");
        characteristicTypes.push_back("takeoff-form-effect");
        characteristicTypes.push_back("takeoff-randomness-effect");
        characteristicTypes.push_back("flight-technique-effect");
        characteristicTypes.push_back("flight-form-effect");
        characteristicTypes.push_back("flight-randomness-effect");
        break;
    case WindsGeneratorSettings:
        characteristicTypes.push_back("back-wind-probability");
        characteristicTypes.push_back("back-side-wind-probability");
        characteristicTypes.push_back("side-wind-probability");
        characteristicTypes.push_back("front-side-wind-probability");
        characteristicTypes.push_back("front-wind-probability");
    }

    if(transformToDisplay == true){
        for(auto & type : characteristicTypes)
        {
            type = getTypeToDisplay(Characteristic(0, type));
        }
    }

    return characteristicTypes;
}

QString Characteristic::getTypeToDisplay(const Characteristic &characteristic)
{
    if(characteristic.getLevel() > 0){
        /// Jumper
        if(characteristic.getType() == "takeoff-height")
            return QObject::tr("Wysoko wychodzi z progu");
        else if(characteristic.getType() == "flight-height")
            return QObject::tr("Lata wysoko nad zeskokiem");
        else if(characteristic.getType() == "landing-skill")
            return QObject::tr("Dobrze ląduje");

        /// Hill
        else if(characteristic.getType() == "real-hs-point")
            return QObject::tr("Bezpieczne skoki za punktem HS");
        else if(characteristic.getType() == "takeoff-technique-effect")
            return QObject::tr("Zwiększony wpływ techniki wybicia");
        else if(characteristic.getType() == "takeoff-power-effect")
            return QObject::tr("Zwiększony wpływ siły wybicia");
        else if(characteristic.getType() == "takeoff-form-effect")
            return QObject::tr("Zwiększony wpływ formy przy wybiciu");
        else if(characteristic.getType() == "takeoff-randomness-effect")
            return QObject::tr("Zwiększony wpływ losowości przy wybiciu");
        else if(characteristic.getType() == "flight-technique-effect")
            return QObject::tr("Zwiększony wpływ techniki lotu");
        else if(characteristic.getType() == "flight-form-effect")
            return QObject::tr("Zwiększony wpływ formy w locie");
        else if(characteristic.getType() == "flight-randomness-effect")
            return QObject::tr("Zwiększony wpływ losowości w locie");

        /// WindsGenerator
        else if(characteristic.getType() == "back-wind-probability")
            return QObject::tr("Zwiększona szansa na tylni wiatr");
        else if(characteristic.getType() == "back-side-wind-probability")
            return QObject::tr("Zwiększona szansa na tylnio-boczny wiatr");
        else if(characteristic.getType() == "side-wind-probability")
            return QObject::tr("Zwiększona szansa na boczny wiatr");
        else if(characteristic.getType() == "front-side-wind-probability")
            return QObject::tr("Zwiększona szansa na przednio-boczny wiatr");
        else if(characteristic.getType() == "front-wind-probability")
            return QObject::tr("Zwiększona szansa na przedni wiatr");
    }
    else if(characteristic.getLevel() < 0)
    {
        /// Jumper
        if(characteristic.getType() == "takeoff-height")
            return QObject::tr("Nisko wychodzi z progu");
        else if(characteristic.getType() == "flight-height")
            return QObject::tr("Lata nisko nad zeskokiem");
        else if(characteristic.getType() == "landing-skill")
            return QObject::tr("Słabo ląduje");

        /// Hill
        else if(characteristic.getType() == "real-hs-point")
            return QObject::tr("Niebezpieczne skoki za punktem HS");
        else if(characteristic.getType() == "takeoff-technique-effect")
            return QObject::tr("Zmniejszony wpływ techniki wybicia");
        else if(characteristic.getType() == "takeoff-power-effect")
            return QObject::tr("Zmniejszony wpływ siły wybicia");
        else if(characteristic.getType() == "takeoff-form-effect")
            return QObject::tr("Zmniejszony wpływ formy przy wybiciu");
        else if(characteristic.getType() == "takeoff-randomness-effect")
            return QObject::tr("Zmniejszony wpływ losowości przy wybiciu");
        else if(characteristic.getType() == "flight-technique-effect")
            return QObject::tr("Zmniejszony wpływ techniki lotu");
        else if(characteristic.getType() == "flight-form-effect")
            return QObject::tr("Zmniejszony wpływ formy w locie");
        else if(characteristic.getType() == "flight-randomness-effect")
            return QObject::tr("Zmniejszony wpływ losowości w locie");

        /// WindsGenerator
        else if(characteristic.getType() == "back-wind-probability")
            return QObject::tr("Zmniejszona szansa na tylni wiatr");
        else if(characteristic.getType() == "back-side-wind-probability")
            return QObject::tr("Zmniejszona szansa na tylnio-boczny wiatr");
        else if(characteristic.getType() == "side-wind-probability")
            return QObject::tr("Zmniejszona szansa na boczny wiatr");
        else if(characteristic.getType() == "front-side-wind-probability")
            return QObject::tr("Zmniejszona szansa na przednio-boczny wiatr");
        else if(characteristic.getType() == "front-wind-probability")
            return QObject::tr("Zmniejszona szansa na przedni wiatr");
    }
    else if(characteristic.getLevel() == 0)
    {
        /// Jumper
        if(characteristic.getType() == "takeoff-height")
            return QObject::tr("Wysokość wyjścia z progu");
        else if(characteristic.getType() == "flight-height")
            return QObject::tr("Wysokość lotu");
        else if(characteristic.getType() == "landing-skill")
            return QObject::tr("Umiejętność lądowania");

        /// Hill
        else if(characteristic.getType() == "real-hs-point")
            return QObject::tr("Bezpieczeństwo skoków za punktem HS");
        else if(characteristic.getType() == "takeoff-technique-effect")
            return QObject::tr("Wpływ techniki wybicia");
        else if(characteristic.getType() == "takeoff-power-effect")
            return QObject::tr("Wpływ siły wybicia");
        else if(characteristic.getType() == "takeoff-form-effect")
            return QObject::tr("Wpływ formy przy wybiciu");
        else if(characteristic.getType() == "takeoff-randomness-effect")
            return QObject::tr("Wpływ losowości przy wybiciu");
        else if(characteristic.getType() == "flight-technique-effect")
            return QObject::tr("Wpływ techniki lotu");
        else if(characteristic.getType() == "flight-form-effect")
            return QObject::tr("Wpływ formy w locie");
        else if(characteristic.getType() == "flight-randomness-effect")
            return QObject::tr("Wpływ losowości w locie");

        /// WindsGenerator
        else if(characteristic.getType() == "back-wind-probability")
            return QObject::tr("Szansa na tylni wiatr");
        else if(characteristic.getType() == "back-side-wind-probability")
            return QObject::tr("Szansa na tylnio-boczny wiatr");
        else if(characteristic.getType() == "side-wind-probability")
            return QObject::tr("Szansa na boczny wiatr");
        else if(characteristic.getType() == "front-side-wind-probability")
            return QObject::tr("Szansa na przednio-boczny wiatr");
        else if(characteristic.getType() == "front-wind-probability")
            return QObject::tr("Szansa na przedni wiatr");
    }
    return "<Nieprawidłowa cecha>";
}

int Characteristic::getCharacteristicsForParentTypeCount(short parentType)
{
    return characteristicTypesForSpecificParent(parentType).count();
}

int Characteristic::getAdditionToCharacteristicComboBoxIndexToGetCharacteristic(short parentType)
{
    int addition = 0;
    for(int i=0; i < parentType + 1; i++)
    {
        addition += getCharacteristicsForParentTypeCount(i);
    }
    qDebug()<<"addition: "<<addition;
    return addition;
}
