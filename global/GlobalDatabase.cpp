#include "GlobalDatabase.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>

#include <QMessageBox>


GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;

GlobalDatabase::GlobalDatabase()
{

}

GlobalDatabase::~GlobalDatabase()
{
}

QVector<Hill> GlobalDatabase::getGlobalHills() const
{
    return globalHills;
}

void GlobalDatabase::setGlobalHills(const QVector<Hill> &newGlobalHills)
{
    globalHills = newGlobalHills;
}

QVector<Jumper> GlobalDatabase::getGlobalJumpers() const
{
    return globalJumpers;
}

void GlobalDatabase::setGlobalJumpers(const QVector<Jumper> &newGlobalJumpers)
{
    globalJumpers = newGlobalJumpers;
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
    globalJumpers.clear();
    globalHills.clear();

    if(loadJumpers() == false)
        return false;

    return true;
}

bool GlobalDatabase::loadJumpers()
{
    QFile file("userData/GlobalDatabase/globalJumpers.json");
    file.open(QFile::ReadOnly | QFile::Text);
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu zawodników", "Nie udało się wczytać zawodników z pliku userData/GlobalDatabase/globalJumpers.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("jumpers");
    QJsonArray array = value.toArray();

    for(const auto & val : array)
    {
        QJsonObject obj = val.toObject();
        Jumper jumper;
        jumper.setJumperSkills(new JumperSkills);
        jumper.setName(obj.value("name").toString());
        jumper.setSurname(obj.value("surname").toString());
        jumper.setCountryCode(obj.value("countryCode").toString());
        jumper.getJumperSkills()->setTakeoffPower(obj.value("takeoff-power").toDouble());
        jumper.getJumperSkills()->setTakeoffTechnique(obj.value("takeoff-technique").toDouble());
        jumper.getJumperSkills()->setFlightTechnique(obj.value("flight-technique").toDouble());
        jumper.getJumperSkills()->setFlightStyle(obj.value("flight-style").toDouble());
        jumper.getJumperSkills()->setLandingStyle(obj.value("landing-style").toDouble());
        jumper.getJumperSkills()->setForm(obj.value("form").toDouble());

        QJsonArray characteristicsArray = obj.value("characteristics").toArray();
        for(const auto & val : characteristicsArray){
            jumper.getJumperSkills()->insertCharacteristic(val.toObject().value("type").toString(), val.toObject().value("level").toDouble());
        }

        globalJumpers.push_back(jumper);
    }

    for(const auto & jp : globalJumpers)
    {
        qDebug()<<jp.getNameAndSurname()<<" ("<<jp.getCountryCode()<<")";
        JumperSkills * jps = jp.getJumperSkills();
        qDebug()<<jps->getTakeoffPower()<<", "<<jps->getTakeoffTechnique()<<", "<<jps->getFlightTechnique()<<", "<<jps->getFlightStyle()<<", "<<jps->getLandingStyle()<<", "<<jps->getForm();
        for(const auto & ch : jps->getCharacteristics())
            qDebug()<<ch.getType()<<", "<<ch.getLevel();
        qDebug()<<"\n\n";
    }

    return true;
}


