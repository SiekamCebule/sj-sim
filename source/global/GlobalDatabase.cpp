#include "GlobalDatabase.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

#include "../global/CountryFlagsManager.h"

GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;

GlobalDatabase::GlobalDatabase()
{

}

GlobalDatabase::~GlobalDatabase()
{
    delete m_globalDatabase;
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
    return (loadJumpers() && loadHills() && loadCompetitionsRules());
}

bool GlobalDatabase::writeToJson()
{
    return (writeJumpers() && writeHills() && writeCompetitionsRules());
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
    globalJumpers = Jumper::getJumpersVectorFromJson(file.readAll());
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
    globalHills = Hill::getHillsVectorFromJson(file.readAll());
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
        array.push_back(QJsonValue(Jumper::getJumperJsonObject(jumper, true, true)));
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
        array.push_back(QJsonValue(Hill::getHillJsonObject(hill, true, true, true)));
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
        array.push_back(QJsonValue(CompetitionRules::getCompetitionRulesJsonObject(rules)));
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

void GlobalDatabase::setupJumpersFlags()
{
    for(auto & jumper : getEditableGlobalJumpers())
    {
        jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    }
}

void GlobalDatabase::setupHillsFlags()
{
    for(auto & hill : getEditableGlobalHills())
    {
        hill.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())));
    }
}
