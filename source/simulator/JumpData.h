#ifndef JUMPDATA_H
#define JUMPDATA_H

#include <QVector>
#include <QJsonObject>

#include "../utilities/ClassWithID.h"
#include "../competitions/CompetitionRules.h"
#include "WindsCalculator.h"
#include "Landing.h"
#include "Jumper.h"
#include "Hill.h"
#include "JumpSimulationData.h"


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

    QVector<Wind> winds;
    double averagedWind;

    bool isDSQOccured;
    bool DNS;

    Jumper * jumper;
    Hill * hill;
    JumpSimulator * simulator;

public:
    void reset();

    static QJsonObject getJumpDataJsonObject(const JumpData &jumpData, bool saveJudges, bool saveSimulationData, bool saveWinds);

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
    QVector<Wind> getWinds() const;
    QVector<Wind>* getWindsPointer();
    void setWinds(const QVector<Wind> &newWinds);
    bool getDNS() const;
    void setDNS(bool newDNS);
};

QDebug operator<<(QDebug d, const JumpData & jumpData);

#endif // JUMPDATA_H
