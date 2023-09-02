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
#include <QProgressDialog>

#include "../global/CountryFlagsManager.h"

GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;
SeasonDatabaseObjectsManager globalObjectsManager;
SeasonDatabaseObjectsManager calendarPresetsObjectsManager;

GlobalDatabase::GlobalDatabase()
{
    loadedSimulationSaves = false;
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

QVector<JumperFormGeneratorSettingsPreset> GlobalDatabase::getFormGeneratorPresets() const
{
    return formGeneratorPresets;
}

QVector<JumperFormGeneratorSettingsPreset> &GlobalDatabase::getEditableFormGeneratorPresets()
{
    return formGeneratorPresets;
}

void GlobalDatabase::setFormGeneratorPresets(const QVector<JumperFormGeneratorSettingsPreset> &newFormGeneratorPresets)
{
    formGeneratorPresets = newFormGeneratorPresets;
}

bool GlobalDatabase::getLoadedSimulationSaves() const
{
    return loadedSimulationSaves;
}

void GlobalDatabase::setLoadedSimulationSaves(bool newLoadedSimulationSaves)
{
    loadedSimulationSaves = newLoadedSimulationSaves;
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
    return (loadJumpers() && loadHills() && loadCompetitionsRules() && loadPointsForPlacesPresets() && loadCalendarPresets() && loadFormGeneratorPresets());
}

bool GlobalDatabase::writeToJson()
{
    return (writeJumpers() && writeHills() && writeCompetitionsRules() && writeSimulationSaves() && writePointsForPlacesPresets() && writeCalendarPresets() && writeFormGeneratorPresets());
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

bool GlobalDatabase::loadSimulationSaves(bool progressDialog)
{
    globalSimulationSaves.clear();

    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/simulationSaves");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(fileNames.count() * 5);
    dialog.setMinimumDuration(0);
    dialog.setValue(0);
    dialog.setWindowTitle(QObject::tr("Wczytywanie zapisów symulacji"));
    //dialog.setLabelText(QString(QObject::tr("Postęp wczytywania zapisów symulacji: %1 z %2")).arg(QString::number(dialog.value())).arg(QString::number(dialog.maximum())));
    dialog.setModal(true);
    dialog.setWindowModality(Qt::WindowModal);

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
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        QJsonObject object = doc.object().value("simulation-save").toObject();
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        SimulationSave * s = SimulationSave::getFromJson(object, &seasonObjectsManager);
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
        globalSimulationSaves.push_back(s);
        dialog.setValue(dialog.value() + 1);
        QCoreApplication::processEvents();
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
    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/calendarPresets");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        globalObjectsManager.clear();
        QFile file("userData/GlobalDatabase/calendarPresets/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji", "Nie udało się otworzyć pliku simulationSaves/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            ok = false;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object().value("calendar-preset").toObject();
        SeasonCalendarPreset p = SeasonCalendarPreset::getFromJson(object, &globalObjectsManager);
        globalCalendarPresets.push_back(p);
        file.close();
    }
    return ok;
}

bool GlobalDatabase::loadFormGeneratorPresets()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/formGeneratorPresets");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        globalObjectsManager.clear();
        QFile file("userData/GlobalDatabase/formGeneratorPresets/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetem generatora formy", "Nie udało się otworzyć pliku userData/GlobalDatabase/formGeneratorPresets/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            ok = false;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object().value("jumper-form-generator-settings-preset").toObject();
        JumperFormGeneratorSettingsPreset p = JumperFormGeneratorSettingsPreset::getFromJson(object, &globalObjectsManager);
        formGeneratorPresets.push_back(p);
        file.close();
    }
    return ok;
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
    bool ok = true;
    for(auto & preset : globalCalendarPresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/calendarPresets/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writeFormGeneratorPresets()
{
    bool ok = true;
    for(auto & preset : formGeneratorPresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/formGeneratorPresets/") == false)
            ok = false;
    }
    return ok;
}

void GlobalDatabase::setupJumpersFlags()
{
    for(auto & jumper : getEditableGlobalJumpers())
    {
        jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    }
}
