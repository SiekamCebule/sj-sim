#ifndef JUMPDATA_H
#define JUMPDATA_H

#include <QVector>

#include "../utilities/ClassWithID.h"
#include "../competitions/CompetitionRules.h"
#include "WindsInfo.h"
#include "Landing.h"
#include "Jumper.h"
#include "Hill.h"
#include "JumpSimulationData.h"
#include <QJsonObject>


class JumpSimulator;


class JumpData : public ClassWithID
{
public:
    friend class JumpSimulator;
    JumpData(Jumper *jumper = nullptr, Hill *hill = nullptr);
private:
    int gate;
    double distance;
    double points;
    double gateCompensation;
    double windCompensation;
    double totalCompensation;
    double judgesPoints;

    Landing landing;
    QVector<double> judges;

    JumpSimulationData simulationData;
    CompetitionRules * rules;

    WindsInfo windsInfo;
    double averagedWind;

    bool isDSQOccured;

    Jumper * jumper;
    Hill * hill;
    JumpSimulator * simulator;

public:
    void reset();

    static QJsonObject getJumpDataJsonObject(JumpData *jumpData, bool saveJudges, bool saveSimulationData, bool saveWinds);

    double getDistance() const;
    void setDistance(double newDistance);
    double getPoints() const;
    void setPoints(double newPoints);
    double getGateCompensation() const;
    void setGateCompensation(double newGateCompensation);
    double getWindCompensation() const;
    void setWindCompensation(double newWindCompensation);
    double getTotalCompensation() const;
    void setTotalCompensation(double newTotalCompensation);
    Landing getLanding() const;
    void setLanding(const Landing &newLanding);
    QVector<double> getJudges() const;
    void setJudges(const QVector<double> &newJudges);
    double getJudgesPoints() const;
    void setJudgesPoints(double newJudgesPoints);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    WindsInfo getWindsInfo() const;
    void setWindsInfo(const WindsInfo &newWindsInfo);
    JumpSimulator *getSimulator() const;
    void setSimulator(JumpSimulator *newSimulator);
    int getGate() const;
    void setGate(int newGate);
    double getAveragedWind() const;
    void setAveragedWind(double newAveragedWind);
    JumpSimulationData getSimulationData() const;
    void setSimulationData(const JumpSimulationData &newSimulationData);
    bool getIsDSQOccured() const;
    void setIsDSQOccured(bool newIsDSQOccured);
    CompetitionRules *getRules() const;
    void setRules(CompetitionRules *newRules);
};

QDebug operator<<(QDebug d, const JumpData & jumpData);

#endif // JUMPDATA_H
