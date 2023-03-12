#include "SingleJumpsManager.h"
#include <QDebug>

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QObject>
#include <QDir>

SingleJumpsManager::SingleJumpsManager(int gate, int jumpsCount, const QString &resultsFileName, bool changeableWind, short resultsFormat) :
    jumpsCount(jumpsCount),
    changeableWind(changeableWind),
    resultsFileName(resultsFileName),
    gate(gate),
    resultsFormat(resultsFormat)
{
}

void SingleJumpsManager::simulate()
{
    qDebug()<<"simulation";
    jumpSimulator.setJumper(&jumper);
    jumpSimulator.setHill(&hill);
    jumpSimulator.setGate(&gate);
    jumpSimulator.setWindAverageCalculatingType(getWindAverageCalculatingType());
    jumpSimulator.setWindCompensationDistanceEffect(getWindCompensationDistanceEffect());

    double min = 0, max = 1000, avg = 0;

    QVector<Wind> winds;
    if(changeableWind == false)
    {
        for(const auto & setting : windsGeneratorSettings)
        {
            Wind wind;
            wind.setDirection(setting.getBaseDirection());
            wind.setStrength(setting.getBaseWindStrength());
            winds.push_back(wind);
        }
    }

    for(int i=0; i<jumpsCount; i++){
        if(getChangeableWind() == true){
            windsGenerator.setGenerationSettings(getWindsGeneratorSettings());
            jumpSimulator.setWindsInfo(WindsInfo(windsGenerator.generateWinds()));
        }
        else{
            jumpSimulator.setWindsInfo(WindsInfo(winds));
        }

        jumpSimulator.simulateJump();
        jumps.push_back(jumpSimulator.getJumpData());
        qDebug()<<i+1<<". "<<jumpSimulator.getJumpData();
        double distance = jumpSimulator.getJumpData().getDistance();
        if(min < distance)
            min = distance;
        if(max > distance)
            max = distance;

        avg += distance;
    }
    avg /= jumpsCount;
    qDebug()<<"\n\nRÓŻNICA: "<<max - min<<"m";
    qDebug()<<"NAJKRÓTSZY: "<<max<<"m";
    qDebug()<<"NAJDALSZY: "<<min<<"m";
    qDebug()<<"ŚREDNIA DŁUGOŚĆ SKOKU: "<<avg<<"m";

    if(resultsFileName.isEmpty() == false)
        saveResultsToFile(resultsFormat);
}

bool SingleJumpsManager::saveResultsToFile(short fileFormat)
{
    switch(fileFormat)
    {
    case Json:
        QDir dir(QDir::currentPath());
        dir.mkpath("results/single-jumps");
        QFile file("results/single-jumps/" + getResultsFileName() + ".json");
        if(file.open(QFile::WriteOnly | QFile::Text) == false)
        {
            QMessageBox message(QMessageBox::Icon::Critical, QObject::tr("Nie można zapisać wyników"), QObject::tr("Nie udało się otworzyć pliku results/single-jumps/<podana nazwa pliku>\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia"),  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            return false;
        }
        QJsonDocument document;
        QJsonObject mainObject;
        mainObject.insert("jumper", Jumper::getJumperJsonObject(&jumper, false, true));
        QJsonArray array;
        for (auto & jump : getJumps())
        {
            array.push_back(JumpData::getJumpDataJsonObject(&jump, true, true, true));
        }
        mainObject.insert("jumps", array);
        document.setObject(mainObject);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        file.resize(0);
        file.write(bytes);
        file.close();

        return true;
        break;
    }
    return true;
}

short SingleJumpsManager::getResultsFormat() const
{
    return resultsFormat;
}

void SingleJumpsManager::setResultsFormat(short newResultsFormat)
{
    resultsFormat = newResultsFormat;
}

Jumper SingleJumpsManager::getJumper() const
{
    return jumper;
}

void SingleJumpsManager::setJumper(const Jumper &newJumper)
{
    jumper = newJumper;
}

Hill SingleJumpsManager::getHill() const
{
    return hill;
}

void SingleJumpsManager::setHill(const Hill &newHill)
{
    hill = newHill;
}

int SingleJumpsManager::getJumpsCount() const
{
    return jumpsCount;
}

void SingleJumpsManager::setJumpsCount(int newJumpsCount)
{
    jumpsCount = newJumpsCount;
}

QVector<JumpData> SingleJumpsManager::getJumps() const
{
    return jumps;
}

QVector<JumpData> &SingleJumpsManager::getEditableJumps()
{
    return jumps;
}

void SingleJumpsManager::setJumps(const QVector<JumpData> &newJumps)
{
    jumps = newJumps;
}

QVector<WindGenerationSettings> SingleJumpsManager::getWindsGeneratorSettings() const
{
    return windsGeneratorSettings;
}

void SingleJumpsManager::setWindsGeneratorSettings(const QVector<WindGenerationSettings> &newWindsGeneratorSettings)
{
    windsGeneratorSettings = newWindsGeneratorSettings;
}

bool SingleJumpsManager::getChangeableWind() const
{
    return changeableWind;
}

void SingleJumpsManager::setChangeableWind(bool newChangeableWind)
{
    changeableWind = newChangeableWind;
}

QString SingleJumpsManager::getResultsFileName() const
{
    return resultsFileName;
}

void SingleJumpsManager::setResultsFileName(const QString &newResultsFileName)
{
    resultsFileName = newResultsFileName;
}

int SingleJumpsManager::getGate() const
{
    return gate;
}

void SingleJumpsManager::setGate(int newGate)
{
    gate = newGate;
}

short SingleJumpsManager::getWindAverageCalculatingType() const
{
    return windAverageCalculatingType;
}

void SingleJumpsManager::setWindAverageCalculatingType(short newWindAverageCalculatingType)
{
    windAverageCalculatingType = newWindAverageCalculatingType;
}

short SingleJumpsManager::getWindCompensationDistanceEffect() const
{
    return windCompensationDistanceEffect;
}

void SingleJumpsManager::setWindCompensationDistanceEffect(short newWindCompensationDistanceEffect)
{
    windCompensationDistanceEffect = newWindCompensationDistanceEffect;
}
