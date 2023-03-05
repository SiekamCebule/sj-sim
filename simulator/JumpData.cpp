#include "JumpData.h"
#include "JumpSimulator.h"
#include "wind-generation/WindsGenerator.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

JumpData::JumpData(Jumper *jumper, Hill *hill) : jumper(jumper),
    hill(hill)
{}

JumpSimulationData JumpData::getSimulationData() const
{
    return simulationData;
}

void JumpData::setSimulationData(const JumpSimulationData &newSimulationData)
{
    simulationData = newSimulationData;
}

double JumpData::getAveragedWind() const
{
    return averagedWind;
}

void JumpData::setAveragedWind(double newAveragedWind)
{
    averagedWind = newAveragedWind;
}

int JumpData::getGate() const
{
    return gate;
}

void JumpData::setGate(int newGate)
{
    gate = newGate;
}

JumpSimulator *JumpData::getSimulator() const
{
    return simulator;
}

void JumpData::setSimulator(JumpSimulator *newSimulator)
{
    simulator = newSimulator;
}

WindsInfo JumpData::getWindsInfo() const
{
    return windsInfo;
}

void JumpData::setWindsInfo(const WindsInfo &newWindsInfo)
{
    windsInfo = newWindsInfo;
}

void JumpData::reset()
{
    distance = points = gateCompensation = windCompensation = totalCompensation = gate = 0;
    landing = Landing();
    judges.clear();
    windsInfo = WindsInfo();
    averagedWind = 0;
    jumper = nullptr;
    hill = nullptr;
    simulator = nullptr;
}

QJsonObject JumpData::getJumpDataJsonObject(JumpData * jumpData, bool saveJudges, bool saveSimulationData, bool saveWinds)
{
    QJsonObject object;
    object.insert("distance", jumpData->getDistance());
    object.insert("points", jumpData->getPoints());
    object.insert("gate-compensation", jumpData->getGateCompensation());
    object.insert("wind-compensation", jumpData->getWindCompensation());
    object.insert("total-compensation", jumpData->getTotalCompensation());
    object.insert("judges-points", jumpData->getJudgesPoints());
    object.insert("landing-type", jumpData->getLanding().getTextLandingType());
    object.insert("landing-imbalance", jumpData->getLanding().getImbalance());
    if(saveJudges == true){
        QJsonArray judgesArray;
        for(const auto & jg : jumpData->getJudges())
        {
            judgesArray.push_back(QJsonValue(jg));
        }
        object.insert("judges", judgesArray);
    }
    if(saveSimulationData == true){
        QJsonObject simulationDataObject;
        simulationDataObject.insert("takeoff-rating", jumpData->getSimulationData().getTakeoffRating());
        simulationDataObject.insert("flight-rating", jumpData->getSimulationData().getFlightRating());
        simulationDataObject.insert("judges-rating", jumpData->getSimulationData().getJudgesRating());
        object.insert("simulation-data", simulationDataObject);
    }
    if(saveWinds == true){
        QJsonArray windsArray;
        int i=0;
        for(const auto & wind : jumpData->getWindsInfo().getWinds())
        {
            QJsonObject windObject;
            windObject.insert("range", QJsonValue(QString::number(WindsGenerator::getRangeOfWindSector(i+i, jumpData->getHill()->getKPoint()).first) + " - " + QString::number(WindsGenerator::getRangeOfWindSector(i+i, jumpData->getHill()->getKPoint()).second)));
            windObject.insert("direction", wind.getDirection());
            windObject.insert("strength", wind.getStrength());
            windsArray.push_back(windObject);
            i++;
        }
        object.insert("winds", windsArray);
    }

    return object;
}

Jumper *JumpData::getJumper() const
{
    return jumper;
}

void JumpData::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

Hill *JumpData::getHill() const
{
    return hill;
}

void JumpData::setHill(Hill *newHill)
{
    hill = newHill;
}

double JumpData::getJudgesPoints() const
{
    return judgesPoints;
}

void JumpData::setJudgesPoints(double newJudgesPoints)
{
    judgesPoints = newJudgesPoints;
}

QVector<double> JumpData::getJudges() const
{
    return judges;
}

void JumpData::setJudges(const QVector<double> &newJudges)
{
    judges = newJudges;
}

Landing JumpData::getLanding() const
{
    return landing;
}

void JumpData::setLanding(const Landing &newLanding)
{
    landing = newLanding;
}

double JumpData::getTotalCompensation() const
{
    return totalCompensation;
}

void JumpData::setTotalCompensation(double newTotalCompensation)
{
    totalCompensation = newTotalCompensation;
}

double JumpData::getWindCompensation() const
{
    return windCompensation;
}

void JumpData::setWindCompensation(double newWindCompensation)
{
    windCompensation = newWindCompensation;
}

double JumpData::getGateCompensation() const
{
    return gateCompensation;
}

void JumpData::setGateCompensation(double newGateCompensation)
{
    gateCompensation = newGateCompensation;
}

double JumpData::getPoints() const
{
    return points;
}

void JumpData::setPoints(double newPoints)
{
    points = newPoints;
}

double JumpData::getDistance() const
{
    return distance;
}

void JumpData::setDistance(double newDistance)
{
    distance = newDistance;
}

QDebug operator<<(QDebug d, const JumpData & jumpData)
{
    Jumper * jumper = jumpData.getJumper();
    d <<jumper->getNameAndSurname()<<" ("<<jumper->getCountryCode()<<")";
    d<<jumpData.getDistance()<<"m ("<<jumpData.getPoints()<<" pts) --> Wiatr: "<<jumpData.getWindsInfo().getAveragedWind(jumpData.getSimulator()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<<"m/s, ("<<jumpData.getWindCompensation()<<" pts), Belka "<<jumpData.getGate()<<" ("<<jumpData.getGateCompensation()<<"),   |"<<jumpData.getJudges().at(0)<<"|"<<jumpData.getJudges().at(1)<<"|"<<jumpData.getJudges().at(2)<<"|"<<jumpData.getJudges().at(3)<<"|"<<jumpData.getJudges().at(4)<<"|"<<",   Lądowanie: "<<jumpData.getLanding().getTextLandingType();

    return d;
}
