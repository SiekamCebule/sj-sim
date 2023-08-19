#include "GlobalDatabase.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QCoreApplication>
#include <QJsonValue>
#include <QSysInfo>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

#include "../global/CountryFlagsManager.h"

GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;
SeasonDatabaseObjectsManager globalObjectsManager;
SeasonDatabaseObjectsManager calendarPresetsObjectsManager;

GlobalDatabase::GlobalDatabase()
{

}

GlobalDatabase::~GlobalDatabase()
{
    for(auto & save : globalSimulationSaves)
        delete save;
    for(auto & calendarPreset : globalCalendarPresets){
        for(auto & comp : calendarPreset.getCalendarReference().getCompetitionsReference())
            delete comp;
        for(auto & cls : calendarPreset.getCalendarReference().getClassificationsReference())
            delete cls;
    }
    delete m_globalDatabase;
}

QVector<SeasonCalendarPreset> GlobalDatabase::getGlobalCalendarPresets() const
{
    return globalCalendarPresets;
}

QVector<SeasonCalendarPreset> &GlobalDatabase::getEditableGlobalCalendarPresets()
{
    return globalCalendarPresets;
}

void GlobalDatabase::setGlobalCalendarPresets(const QVector<SeasonCalendarPreset> &newGlobalCalendarPresets)
{
    globalCalendarPresets = newGlobalCalendarPresets;
}

QVector<PointsForPlacesPreset> GlobalDatabase::getGlobalPointsForPlacesPresets() const
{
    return globalPointsForPlacesPresets;
}

QVector<PointsForPlacesPreset> &GlobalDatabase::getEditableGlobalPointsForPlacesPresets()
{
    return globalPointsForPlacesPresets;
}

void GlobalDatabase::setGlobalPointsForPlacesPresets(const QVector<PointsForPlacesPreset> &newGlobalPointsForPlacesPresets)
{
    globalPointsForPlacesPresets = newGlobalPointsForPlacesPresets;
}

QVector<SimulationSave *> GlobalDatabase::getGlobalSimulationSaves() const
{
    return globalSimulationSaves;
}

QVector<SimulationSave *> &GlobalDatabase::getEditableGlobalSimulationSaves()
{
    return globalSimulationSaves;
}

void GlobalDatabase::setGlobalSimulationSaves(const QVector<SimulationSave *> &newGlobalSimulationSaves)
{
    globalSimulationSaves = newGlobalSimulationSaves;
}

QVector<CompetitionRules> GlobalDatabase::getGlobalCompetitionsRules() const
{
    return globalCompetitionsRules;
}

void GlobalDatabase::setGlobalCompetitionsRules(const QVector<CompetitionRules> &newGlobalCompetitionsRules)
{
    globalCompetitionsRules = newGlobalCompetitionsRules;
}

QVector<CompetitionRules> &GlobalDatabase::getEditableCompetitionRules()
{
    return globalCompetitionsRules;
}

QVector<Hill> GlobalDatabase::getGlobalHills() const
{
    return globalHills;
}

void GlobalDatabase::setGlobalHills(const QVector<Hill> &newGlobalHills)
{
    globalHills = newGlobalHills;
}

QVector<Hill> &GlobalDatabase::getEditableGlobalHills()
{
    return globalHills;
}

QVector<Jumper> GlobalDatabase::getGlobalJumpers() const
{
    return globalJumpers;
}

void GlobalDatabase::setGlobalJumpers(const QVector<Jumper> &newGlobalJumpers)
{
    globalJumpers = newGlobalJumpers;
}

QVector<Jumper> &GlobalDatabase::getEditableGlobalJumpers()
{
    return globalJumpers;
}

GlobalDatabase *GlobalDatabase::get()
{
    if(m_globalDatabase == nullptr)
        m_globalDatabase = new GlobalDatabase();
    return m_globalDatabase;
}

void GlobalDatabase::removeJumper(int index)
{
    globalJumpers.remove(index);
}

bool GlobalDatabase::loadFromJson()
{
    globalObjectsManager.clear();
    return (loadJumpers() && loadHills() && loadCompetitionsRules() && loadSimulationSaves() && loadPointsForPlacesPresets() && loadCalendarPresets());
}

bool GlobalDatabase::writeToJson()
{
    return (writeJumpers() && writeHills() && writeCompetitionsRules() && writeSimulationSaves() && writePointsForPlacesPresets() && writeCalendarPresets());
}

bool GlobalDatabase::loadJumpers()
{
    QFile file("userData/GlobalDatabase/globalJumpers.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalJumpers.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    /*QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    QJsonArray jumpersArray = obj.value("jumpers");*/
    globalJumpers = Jumper::getVectorFromJson(file.readAll());
    globalObjectsManager.fill(&globalJumpers);
    file.close();
    return true;
}

bool GlobalDatabase::loadHills()
{
    QFile file("userData/GlobalDatabase/globalHills.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku ze skoczniami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalHills.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalHills = Hill::getVectorFromJson(file.readAll());
    globalObjectsManager.fill(&globalHills);
    file.close();
    return true;
}

bool GlobalDatabase::loadCompetitionsRules()
{
    QFile file("userData/GlobalDatabase/globalCompetitionsRules.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zasadami konkursów", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCompetitionsRules.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalCompetitionsRules = CompetitionRules::getCompetitionRulesVectorFromJson(file.readAll());
    globalObjectsManager.fill(&globalCompetitionsRules);
    file.close();
    return true;
}

bool GlobalDatabase::loadSimulationSaves()
{
    globalSimulationSaves.clear();

    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/simulationSaves");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        SeasonDatabaseObjectsManager * sdom = &seasonObjectsManager;
        seasonObjectsManager.clear();
        QFile file("simulationSaves/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji", "Nie udało się otworzyć pliku simulationSaves/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            ok = false;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object().value("simulation-save").toObject();
        SimulationSave * s = SimulationSave::getFromJson(object, &seasonObjectsManager);
        globalSimulationSaves.push_back(s);
        file.close();
    }
    return ok;
}

bool GlobalDatabase::loadPointsForPlacesPresets()
{
    QFile file("userData/GlobalDatabase/globalPointsForPlacesPresets.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetami punktów za miejsca", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalPointsForPlacesPresets.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalPointsForPlacesPresets = PointsForPlacesPreset::getPointsForPlacesPresetsVectorFromJson(file.readAll());
    file.close();
    return true;
}

bool GlobalDatabase::loadCalendarPresets()
{
    QFile file("userData/GlobalDatabase/globalCalendarPresets.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetami kalendarzy", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCalendarPresets.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalCalendarPresets = SeasonCalendarPreset::getVectorFromJson(file.readAll());
    file.close();
    return true;
}

bool GlobalDatabase::writeJumpers()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(auto & jumper : getGlobalJumpers())
    {
        array.push_back(QJsonValue(Jumper::getJsonObject(jumper)));
    }
    mainObject.insert("jumpers", QJsonValue(array));
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalJumpers.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalJumpers.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));

    file.close();
    return true;
}

bool GlobalDatabase::writeHills()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(auto & hill : getGlobalHills())
    {
        array.push_back(QJsonValue(Hill::getJsonObject(hill)));
    }
    mainObject.insert("hills", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalHills.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku ze skoczniami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalHills.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GlobalDatabase::writeCompetitionsRules()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;

    for(auto & rules : getGlobalCompetitionsRules())
    {
        array.push_back(QJsonValue(CompetitionRules::getJsonObject(rules)));
    }
    mainObject.insert("competitionsRules", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalCompetitionsRules.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zasadami konkursów", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCompetitionsRules.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GlobalDatabase::writeSimulationSaves()
{
    bool ok = true;
    for(auto & save : globalSimulationSaves)
    {
        if(save->saveToFile("simulationSaves/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writePointsForPlacesPresets()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;

    for(auto & preset : globalPointsForPlacesPresets)
    {
        array.push_back(QJsonValue(PointsForPlacesPreset::getPointsForPlacesPresetJsonObject(preset)));
    }
    mainObject.insert("pointsForPlacesPresets", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalPointsForPlacesPresets.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetami punktów za miejsca", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalPointsForPlacesPresets.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GlobalDatabase::writeCalendarPresets()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;

    for(auto & preset : globalCalendarPresets)
    {
        array.push_back(QJsonValue(SeasonCalendarPreset::getJsonObject(preset)));
    }
    mainObject.insert("calendarPresets", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalCalendarPresets.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetami kalendarzy", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCalendarPresets.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void GlobalDatabase::setupJumpersFlags()
{
    for(auto & jumper : getEditableGlobalJumpers())
    {
        jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    }
}
