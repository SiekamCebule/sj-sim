#include "SingleJumpsManager.h"
#include <QDebug>

SingleJumpsManager::SingleJumpsManager(int gate, int jumpsCount, bool saveResultsToFile, const QString &resultsFileName, bool changeableWind, const QVector<WindGenerationSettings> &windsGeneratorSettings, const Jumper &jumper, const Hill &hill) : jumper(jumper),
    hill(hill),
    windsGeneratorSettings(windsGeneratorSettings),
    jumpsCount(jumpsCount),
    changeableWind(changeableWind),
    saveResultsToFile(saveResultsToFile),
    resultsFileName(resultsFileName),
    gate(gate)
{}

void SingleJumpsManager::simulate()
{
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
            wind.setValue(setting.getBaseWindStrength());
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

bool SingleJumpsManager::getSaveResultsToFile() const
{
    return saveResultsToFile;
}

void SingleJumpsManager::setSaveResultsToFile(bool newSaveResultsToFile)
{
    saveResultsToFile = newSaveResultsToFile;
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
