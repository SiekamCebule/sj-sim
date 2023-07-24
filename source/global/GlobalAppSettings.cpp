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

bool GlobalAppSettings::getShowSeasonJumpersAndHillsHelp() const
{
    return showSeasonJumpersAndHillsHelp;
}

void GlobalAppSettings::setShowSeasonJumpersAndHillsHelp(bool newShowSeasonJumpersAndHillsHelp)
{
    showSeasonJumpersAndHillsHelp = newShowSeasonJumpersAndHillsHelp;
}

bool GlobalAppSettings::getShowCalendarEditorHelp() const
{
    return showCalendarEditorHelp;
}

void GlobalAppSettings::setShowCalendarEditorHelp(bool newShowCalendarEditorHelp)
{
    showCalendarEditorHelp = newShowCalendarEditorHelp;
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
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu ustawień aplikacji", "Nie udało się wczytać ustawień aplikacji z pliku userData/Settings/appSettings.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }

    QJsonObject object = document.object();
    QJsonValue value = object.value("settings");
    if(!value.isObject())
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu ustawień aplikacji", "Prawdopodobnie w tym pliku nie ma obiektu z ustawieniami\nUpewnij się, że wybrałeś właściwy plik!");
        message.setModal(true);
        message.exec();
        return false;
    }
    setLanguageID(value.toObject().value("language-id").toInt());
    setShowCalendarEditorHelp(value.toObject().value("show-calendar-editor-help").toBool(true));
    setShowSeasonJumpersAndHillsHelp(value.toObject().value("show-season-jumpers-and-hills-help").toBool(true));

    return true;
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
    QJsonObject settingsObject;
    settingsObject.insert("language-id", getLanguageID());
    settingsObject.insert("show-calendar-editor-help", getShowCalendarEditorHelp());
    settingsObject.insert("show-season-jumpers-and-hills-help", getShowSeasonJumpersAndHillsHelp());

    mainObject.insert("settings", settingsObject);
    document.setObject(mainObject);

    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}
