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
    baseDsqProbability = 0;
    simulationRandomMultiplier = 0;
}

double GlobalSimulationSettings::getSimulationRandomMultiplier() const
{
    return simulationRandomMultiplier;
}

void GlobalSimulationSettings::setSimulationRandomMultiplier(double newSimulationRandomMultiplier)
{
    simulationRandomMultiplier = newSimulationRandomMultiplier;
}

int GlobalSimulationSettings::getBaseDsqProbability() const
{
    return baseDsqProbability;
}

void GlobalSimulationSettings::setBaseDsqProbability(int newBaseDsqProbability)
{
    baseDsqProbability = newBaseDsqProbability;
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
    this->setSimulationRandomMultiplier(value.toObject().value("simulation-random-multiplier").toDouble());
    this->setBaseDsqProbability(value.toObject().value("base-dsq-probability").toDouble());

    return true;
}

bool GlobalSimulationSettings::writeToFile()
{
    QFile file("userData/Settings/globalSimulationSettings.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z ustawieniami symulacji", "Nie udało się otworzyć pliku userData/Settings/globalSimulationSettings.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonObject settingsObject;
    settingsObject.insert("simulation-random-multiplier", getSimulationRandomMultiplier());
    settingsObject.insert("base-dsq-probability", getBaseDsqProbability());
    mainObject.insert("settings", settingsObject);
    document.setObject(mainObject);
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}
