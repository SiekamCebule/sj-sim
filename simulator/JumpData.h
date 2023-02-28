#ifndef JUMPDATA_H
#define JUMPDATA_H

#include <QVector>

#include "../utilities/ClassWithID.h"
#include "WindsInfo.h"
#include "Landing.h"
#include "Jumper.h"
#include "Hill.h"


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

    WindsInfo windsInfo;
    double averagedWind;

    Jumper * jumper;
    Hill * hill;

    JumpSimulator * simulator;

public:
    void reset();

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
};

QDebug operator<<(QDebug d, const JumpData & jumpData);

#endif // JUMPDATA_H
