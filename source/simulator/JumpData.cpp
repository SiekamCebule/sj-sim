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

bool JumpData::getBeats95HSPercents() const
{
    return beats95HSPercents;
}

void JumpData::setBeats95HSPercents(bool newBeats95HSPercents)
{
    beats95HSPercents = newBeats95HSPercents;
}

int JumpData::getCoachGate() const
{
    return coachGate;
}

void JumpData::setCoachGate(int newCoachGate)
{
    coachGate = newCoachGate;
}

bool JumpData::getHasCoachGate() const
{
    return hasCoachGate;
}

void JumpData::setHasCoachGate(bool newHasCoachGate)
{
    hasCoachGate = newHasCoachGate;
}

bool JumpData::getDNS() const
{
    return DNS;
}

void JumpData::setDNS(bool newDNS)
{
    DNS = newDNS;
}

QVector<Wind> JumpData::getWinds() const
{
    return winds;
}

QVector<Wind> *JumpData::getWindsPointer()
{
    return &winds;
}

void JumpData::setWinds(const QVector<Wind> &newWinds)
{
    winds = newWinds;
}

CompetitionRules *JumpData::getRules() const
{
    return rules;
}

void JumpData::setRules(CompetitionRules *newRules)
{
    rules = newRules;
}

bool JumpData::getIsDSQOccured() const
{
    return isDSQOccured;
}

void JumpData::setIsDSQOccured(bool newIsDSQOccured)
{
    isDSQOccured = newIsDSQOccured;
}

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

void JumpData::reset()
{
    distance = points = gateCompensation = windCompensation = totalCompensation = gate = judgesPoints = 0;
    landing = Landing();
    judges.clear();
    judges.fill(0, 5);
    winds.clear();
    averagedWind = 0;
    jumper = nullptr;
    hill = nullptr;
    simulator = nullptr;
    rules = nullptr;
    isDSQOccured = false;
    simulationData.reset();
}

QJsonObject JumpData::getJumpDataJsonObject(const JumpData &jumpData, bool saveJudges, bool saveSimulationData, bool saveWinds)
{
    QJsonObject object;
    object.insert("distance", jumpData.getDistance());
    object.insert("points", jumpData.getPoints());
    object.insert("gate", jumpData.getGate());
    object.insert("averaged-wind", jumpData.getAveragedWind());
    object.insert("gate-compensation", jumpData.getGateCompensation());
    object.insert("wind-compensation", jumpData.getWindCompensation());
    object.insert("total-compensation", jumpData.getTotalCompensation());
    object.insert("judges-points", jumpData.getJudgesPoints());
    object.insert("landing-type", jumpData.getLanding().getTextLandingType());
    object.insert("landing-imbalance", jumpData.getLanding().getImbalance());
    if(saveJudges == true){
        QJsonArray judgesArray;
        for(const auto & jg : jumpData.getJudges())
        {
            judgesArray.push_back(QJsonValue(jg));
        }
        object.insert("judges", judgesArray);
    }
    if(saveSimulationData == true){
        QJsonObject simulationDataObject;
        simulationDataObject.insert("takeoff-rating", jumpData.getSimulationData().getTakeoffRating());
        simulationDataObject.insert("flight-rating", jumpData.getSimulationData().getFlightRating());
        simulationDataObject.insert("judges-rating", jumpData.getSimulationData().getJudgesRating());
        object.insert("simulation-data", simulationDataObject);
    }
    if(saveWinds == true){
        QJsonArray windsArray;
        int i=0;
        for(const auto & wind : jumpData.getWinds())
        {
            QJsonObject windObject;
            windObject.insert("range", QJsonValue(QString::number(WindsGenerator::getRangeOfWindSector(i+1, jumpData.getHill()->getKPoint()).first) + " - " + QString::number(WindsGenerator::getRangeOfWindSector(i+1, jumpData.getHill()->getKPoint()).second)));
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
    d<<jumpData.getDistance()<<"m ("<<jumpData.getPoints()<<" pts) --> Wiatr: "<<WindsCalculator::getAveragedWind(jumpData.getWinds(), jumpData.getSimulator()->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<<"m/s, ("<<jumpData.getWindCompensation()<<" pts), Belka "<<jumpData.getGate()<<" ("<<jumpData.getGateCompensation()<<"),   |"<<jumpData.getJudges().at(0)<<"|"<<jumpData.getJudges().at(1)<<"|"<<jumpData.getJudges().at(2)<<"|"<<jumpData.getJudges().at(3)<<"|"<<jumpData.getJudges().at(4)<<"|"<<",   Lądowanie: "<<jumpData.getLanding().getTextLandingType();

    return d;
}
