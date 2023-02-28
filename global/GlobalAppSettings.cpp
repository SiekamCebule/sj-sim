#include "GlobalAppSettings.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

GlobalAppSettings * GlobalAppSettings::m_globalAppSettings = nullptr;

GlobalAppSettings::GlobalAppSettings()
{
}

GlobalAppSettings *GlobalAppSettings::get()
{
    if(m_globalAppSettings == nullptr)
        m_globalAppSettings = new GlobalAppSettings;
    return m_globalAppSettings;
}

short GlobalAppSettings::getLanguageID() const
{
    return languageID;
}

void GlobalAppSettings::setLanguageID(const short &newLanguageID)
{
    languageID = newLanguageID;
}

bool GlobalAppSettings::loadFromJson()
{
    QFile file("userData/Settings/appSettings.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z ustawieniami aplikacji", "Nie udało się otworzyć pliku userData/Settings/appSettings.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonParseError error;
    QByteArray bytes = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);

    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu ustawień aplikacji", "Nie udało się wczytać ustawień aplikacji z pliku userData/Settings/appSettings.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }

    QJsonObject object = document.object();
    setLanguageID(object.value("language-id").toInt());
    file.close();
}

bool GlobalAppSettings::writeToJson()
{
    QFile file("userData/Settings/appSettings.json");
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z ustawieniami aplikacji", "Nie udało się otworzyć pliku userData/Settings/appSettings.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonDocument document;
    QJsonObject mainObject;
    mainObject.insert("language-id", getLanguageID());
    document.setObject(mainObject);

    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
}
