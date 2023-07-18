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
#include <QProgressDialog>
#include <QObject>
#include <QObject>
#include <QDir>

#include "../simulator/JumpManipulator.h"

SingleJumpsManager::SingleJumpsManager(int gate, int jumpsCount, const QString &resultsFileName, bool changeableWind, short resultsFormat) :
    jumpsCount(jumpsCount),
    changeableWind(changeableWind),
    resultsFileName(resultsFileName),
    gate(gate),
    resultsFormat(resultsFormat)
{
    windAverageCalculatingType = windCompensationDistanceEffect = DSQProbability = 0;
}

void SingleJumpsManager::simulate()
{
    jumpSimulator.setJumper(&jumper);
    jumpSimulator.setHill(&hill);
    jumpSimulator.setGate(gate);
    jumpSimulator.setDSQBaseProbability(getDSQProbability());
    jumpSimulator.setCompetitionRules(&rules);
    jumpSimulator.setManipulator(new JumpManipulator());

    double min = 0, max = 100000, avg = 0;

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
    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(jumpsCount);
    dialog.setWindowTitle("Symulacja pojedynczych skoków");
    dialog.setLabelText(QString("Przesymulowano %1 z %2 skoków").arg(QString::number(dialog.value()).arg(QString::number(dialog.maximum()))));
    dialog.setModal(true);
    dialog.setWindowModality(Qt::WindowModal);

    for(int i=0; i<jumpsCount; i++){
        if(getChangeableWind() == true){
            windsGenerator.setGenerationSettings(getWindsGeneratorSettings());
            jumpSimulator.setWinds(windsGenerator.generateWinds());
        }
        else{
            jumpSimulator.setWinds(winds);
        }
        jumpSimulator.simulateJump();
        jumps.push_back(jumpSimulator.getJumpData());

        dialog.setValue(i+1);
        dialog.setLabelText(QString("Przesymulowano %1 z %2 skoków").arg(QString::number(dialog.value()), QString::number(dialog.maximum())));
    }

    delete jumpSimulator.getManipulator();
}

bool SingleJumpsManager::saveResultsToFile(short fileFormat)
{
    switch(fileFormat)
    {
    case Json:{
        if(getResultsFileName().isEmpty()) setResultsFileName("filee");
        QDir dir(QDir::currentPath());
        dir.mkpath("results/single-jumps");
        QFile file("results/single-jumps/" + getResultsFileName() + ".json");
        file.resize(0);
        if(!(file.open(QIODevice::ReadWrite | QIODevice::Text)))
        {
            QMessageBox message(QMessageBox::Icon::Critical, QObject::tr("Nie można zapisać wyników"), QObject::tr("Nie udało się otworzyć pliku results/single-jumps/<podana nazwa pliku>\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia"),  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            return false;
        }
        QJsonDocument document;
        QJsonObject mainObject;
        mainObject.insert("jumper", Jumper::getJsonObject(jumper));
        QJsonArray array;
        for(auto & jump : getJumpsReference())
        {
            array.push_back(JumpData::getJsonObject(jump));
            array.push_back(QJsonObject());
        }
        mainObject.insert("jumps", array);
        document.setObject(mainObject);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        file.write(bytes);
        file.close();

        break;
    }

    case Csv:
        break;
    }

    return true;
}

CompetitionRules& SingleJumpsManager::getRules()
{
    return rules;
}

void SingleJumpsManager::setRules(const CompetitionRules &newRules)
{
    rules = newRules;
}

int SingleJumpsManager::getDSQProbability() const
{
    return DSQProbability;
}

void SingleJumpsManager::setDSQProbability(int newDSQProbability)
{
    DSQProbability = newDSQProbability;
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

QVector<JumpData> &SingleJumpsManager::getJumpsReference()
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
