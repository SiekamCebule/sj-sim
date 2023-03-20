#include "GlobalSimulationSettings.h"

#include <QFile>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

GlobalSimulationSettings* GlobalSimulationSettings::m_globalSimulationSettings = nullptr;

GlobalSimulationSettings::GlobalSimulationSettings()
{
    takeoffRatingTakeoffHeightAbsValue = takeoffRatingBaseRandomBaseValue = takeoffRatingBaseRandomDeviationBaseValue = takeoffRatingBaseRandomDeviationSubstractCharacteristicDivider = takeoffRatingLogRandomBaseValue = takeoffRatingLogRandomDeviationBaseValue = takeoffRatingLogRandomDeviationJumpsEqualityDivider = flightRatingBaseRandomBaseValue = flightRatingBaseRandomDeviationBaseValue = flightRatingBaseRandomDeviationSubstractCharacteristicDivider = flightRatingLogRandomBaseValue = flightRatingLogRandomDeviationBaseValue = flightRatingLogRandomDeviationJumpsEqualityDivider = flightRatingFlightHeightAbsValue = 0;
}

double GlobalSimulationSettings::getFlightRatingFlightHeightAbsValue() const
{
    return flightRatingFlightHeightAbsValue;
}

void GlobalSimulationSettings::setFlightRatingFlightHeightAbsValue(double newFlightRatingFlightHeightAbsValue)
{
    flightRatingFlightHeightAbsValue = newFlightRatingFlightHeightAbsValue;
}

double GlobalSimulationSettings::getTakeoffRatingTakeoffHeightAbsValue() const
{
    return takeoffRatingTakeoffHeightAbsValue;
}

void GlobalSimulationSettings::setTakeoffRatingTakeoffHeightAbsValue(double newTakeoffRatingTakeoffHeightAbsValue)
{
    takeoffRatingTakeoffHeightAbsValue = newTakeoffRatingTakeoffHeightAbsValue;
}

GlobalSimulationSettings *GlobalSimulationSettings::get()
{
    if(m_globalSimulationSettings == nullptr)
        m_globalSimulationSettings = new GlobalSimulationSettings();

    return m_globalSimulationSettings;
}

bool GlobalSimulationSettings::loadFromFile()
{
    QFile file("userData/Settings/globalSimulationSettings.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z ustawieniami symulacji", "Nie udało się otworzyć pliku userData/Settings/globalSimulationSettings.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QByteArray bytes = file.readAll();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu ustawień symulacji", "Nie udało się wczytać ustawień symulacji z pliku userData/Settings/globalSimulationSettings.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("settings");
    if(!value.isObject())
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu ustawień symulacji", "Prawdopodobnie w tym pliku nie ma obiektu z ustawieniami\nUpewnij się, że wybrałeś właściwy plik!");
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonObject takeoffRatingRandomObject = value.toObject().value("takeoff-rating").toObject().value("base-random").toObject();
    this->setTakeoffRatingBaseRandomBaseValue(takeoffRatingRandomObject.value("random-base-value").toDouble());
    this->setTakeoffRatingBaseRandomDeviationBaseValue(takeoffRatingRandomObject.value("random-deviation-base-value").toDouble());
    this->setTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider(takeoffRatingRandomObject.value("random-deviation-substract-characteristic-divider").toDouble());

    QJsonObject takeoffRatingLogRandomObject = value.toObject().value("takeoff-rating").toObject().value("log-random").toObject();
    this->setTakeoffRatingLogRandomBaseValue(takeoffRatingLogRandomObject.value("random-base-value").toDouble());
    this->setTakeoffRatingLogRandomDeviationBaseValue(takeoffRatingLogRandomObject.value("random-deviation-base-value").toDouble());
    this->setTakeoffRatingLogRandomDeviationJumpsEqualityDivider(takeoffRatingLogRandomObject.value("random-deviation-jumps-equality-divider").toDouble());

    QJsonObject flightRatingRandomObject = value.toObject().value("flight-rating").toObject().value("base-random").toObject();
    this->setFlightRatingBaseRandomBaseValue(flightRatingRandomObject.value("random-base-value").toDouble());
    this->setFlightRatingBaseRandomDeviationBaseValue(flightRatingRandomObject.value("random-deviation-base-value").toDouble());
    this->setFlightRatingBaseRandomDeviationSubstractCharacteristicDivider(flightRatingRandomObject.value("random-deviation-substract-characteristic-divider").toDouble());

    QJsonObject flightRatingLogRandomObject = value.toObject().value("flight-rating").toObject().value("log-random").toObject();
    this->setFlightRatingLogRandomBaseValue(flightRatingLogRandomObject.value("random-base-value").toDouble());
    this->setFlightRatingLogRandomDeviationBaseValue(flightRatingLogRandomObject.value("random-deviation-base-value").toDouble());
    this->setFlightRatingLogRandomDeviationJumpsEqualityDivider(flightRatingLogRandomObject.value("random-deviation-jumps-equality-divider").toDouble());
}

bool GlobalSimulationSettings::writeToFile()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonObject settingsObject;

    QJsonObject takeoffRatingObject;
    QJsonObject takeoffRatingNormalRandomObject;
    takeoffRatingNormalRandomObject.insert("random-base-value", getTakeoffRatingBaseRandomBaseValue());
    takeoffRatingNormalRandomObject.insert("random-deviation-base-value", getTakeoffRatingBaseRandomDeviationBaseValue());
    takeoffRatingNormalRandomObject.insert("random-deviation-substract-characteristic-divider", getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider());
    QJsonObject takeoffRatingLogRandomObject;
    takeoffRatingLogRandomObject.insert("random-base-value", getTakeoffRatingLogRandomBaseValue());
    takeoffRatingLogRandomObject.insert("random-deviation-base-value", getTakeoffRatingLogRandomDeviationBaseValue());
    takeoffRatingLogRandomObject.insert("random-deviation-jumps-equality-divider", getTakeoffRatingLogRandomDeviationJumpsEqualityDivider());
    takeoffRatingObject.insert("base-random", takeoffRatingNormalRandomObject);
    takeoffRatingObject.insert("log-random", takeoffRatingLogRandomObject);
    //
    QJsonObject flightRatingObject;
    QJsonObject flightRatingNormalRandomObject;
    flightRatingNormalRandomObject.insert("random-base-value", getFlightRatingBaseRandomBaseValue());
    flightRatingNormalRandomObject.insert("random-deviation-base-value", getFlightRatingBaseRandomDeviationBaseValue());
    flightRatingNormalRandomObject.insert("random-deviation-substract-characteristic-divider", getFlightRatingBaseRandomDeviationSubstractCharacteristicDivider());
    QJsonObject flightRatingLogRandomObject;
    flightRatingLogRandomObject.insert("random-base-value", getFlightRatingLogRandomBaseValue());
    flightRatingLogRandomObject.insert("random-deviation-base-value", getFlightRatingLogRandomDeviationBaseValue());
    flightRatingLogRandomObject.insert("random-deviation-jumps-equality-divider", getFlightRatingLogRandomDeviationJumpsEqualityDivider());
    flightRatingObject.insert("base-random", flightRatingNormalRandomObject);
    flightRatingObject.insert("log-random", flightRatingLogRandomObject);

    settingsObject.insert("takeoff-rating", takeoffRatingObject);
    settingsObject.insert("flight-rating", flightRatingObject);
    mainObject.insert("settings", settingsObject);
    document.setObject(mainObject);

    QFile file("userData/Settings/globalSimulationSettings.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z ustawieniami symulacji", "Nie udało się otworzyć pliku userData/Settings/globalSimulationSettings.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}


double GlobalSimulationSettings::getTakeoffRatingBaseRandomBaseValue() const
{
    return takeoffRatingBaseRandomBaseValue;
}

void GlobalSimulationSettings::setTakeoffRatingBaseRandomBaseValue(double newTakeoffRatingBaseRandomBaseValue)
{
    takeoffRatingBaseRandomBaseValue = newTakeoffRatingBaseRandomBaseValue;
}

double GlobalSimulationSettings::getTakeoffRatingBaseRandomDeviationBaseValue() const
{
    return takeoffRatingBaseRandomDeviationBaseValue;
}

void GlobalSimulationSettings::setTakeoffRatingBaseRandomDeviationBaseValue(double newTakeoffRatingBaseRandomDeviationBaseValue)
{
    takeoffRatingBaseRandomDeviationBaseValue = newTakeoffRatingBaseRandomDeviationBaseValue;
}

double GlobalSimulationSettings::getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider() const
{
    return takeoffRatingBaseRandomDeviationSubstractCharacteristicDivider;
}

void GlobalSimulationSettings::setTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider(double newTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider)
{
    takeoffRatingBaseRandomDeviationSubstractCharacteristicDivider = newTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider;
}

double GlobalSimulationSettings::getTakeoffRatingLogRandomBaseValue() const
{
    return takeoffRatingLogRandomBaseValue;
}

void GlobalSimulationSettings::setTakeoffRatingLogRandomBaseValue(double newTakeoffRatingLogRandomBaseValue)
{
    takeoffRatingLogRandomBaseValue = newTakeoffRatingLogRandomBaseValue;
}

double GlobalSimulationSettings::getTakeoffRatingLogRandomDeviationBaseValue() const
{
    return takeoffRatingLogRandomDeviationBaseValue;
}

void GlobalSimulationSettings::setTakeoffRatingLogRandomDeviationBaseValue(double newTakeoffRatingLogRandomDeviationBaseValue)
{
    takeoffRatingLogRandomDeviationBaseValue = newTakeoffRatingLogRandomDeviationBaseValue;
}

double GlobalSimulationSettings::getTakeoffRatingLogRandomDeviationJumpsEqualityDivider() const
{
    return takeoffRatingLogRandomDeviationJumpsEqualityDivider;
}

void GlobalSimulationSettings::setTakeoffRatingLogRandomDeviationJumpsEqualityDivider(double newTakeoffRatingLogRandomDeviationJumpsEqualityDivider)
{
    takeoffRatingLogRandomDeviationJumpsEqualityDivider = newTakeoffRatingLogRandomDeviationJumpsEqualityDivider;
}

double GlobalSimulationSettings::getFlightRatingBaseRandomBaseValue() const
{
    return flightRatingBaseRandomBaseValue;
}

void GlobalSimulationSettings::setFlightRatingBaseRandomBaseValue(double newFlightRatingBaseRandomBaseValue)
{
    flightRatingBaseRandomBaseValue = newFlightRatingBaseRandomBaseValue;
}

double GlobalSimulationSettings::getFlightRatingBaseRandomDeviationBaseValue() const
{
    return flightRatingBaseRandomDeviationBaseValue;
}

void GlobalSimulationSettings::setFlightRatingBaseRandomDeviationBaseValue(double newFlightRatingBaseRandomDeviationBaseValue)
{
    flightRatingBaseRandomDeviationBaseValue = newFlightRatingBaseRandomDeviationBaseValue;
}

double GlobalSimulationSettings::getFlightRatingBaseRandomDeviationSubstractCharacteristicDivider() const
{
    return flightRatingBaseRandomDeviationSubstractCharacteristicDivider;
}

void GlobalSimulationSettings::setFlightRatingBaseRandomDeviationSubstractCharacteristicDivider(double newFlightRatingBaseRandomDeviationSubstractCharacteristicDivider)
{
    flightRatingBaseRandomDeviationSubstractCharacteristicDivider = newFlightRatingBaseRandomDeviationSubstractCharacteristicDivider;
}

double GlobalSimulationSettings::getFlightRatingLogRandomBaseValue() const
{
    return flightRatingLogRandomBaseValue;
}

void GlobalSimulationSettings::setFlightRatingLogRandomBaseValue(double newFlightRatingLogRandomBaseValue)
{
    flightRatingLogRandomBaseValue = newFlightRatingLogRandomBaseValue;
}

double GlobalSimulationSettings::getFlightRatingLogRandomDeviationBaseValue() const
{
    return flightRatingLogRandomDeviationBaseValue;
}

void GlobalSimulationSettings::setFlightRatingLogRandomDeviationBaseValue(double newFlightRatingLogRandomDeviationBaseValue)
{
    flightRatingLogRandomDeviationBaseValue = newFlightRatingLogRandomDeviationBaseValue;
}

double GlobalSimulationSettings::getFlightRatingLogRandomDeviationJumpsEqualityDivider() const
{
    return flightRatingLogRandomDeviationJumpsEqualityDivider;
}

void GlobalSimulationSettings::setFlightRatingLogRandomDeviationJumpsEqualityDivider(double newFlightRatingLogRandomDeviationJumpsEqualityDivider)
{
    flightRatingLogRandomDeviationJumpsEqualityDivider = newFlightRatingLogRandomDeviationJumpsEqualityDivider;
}
