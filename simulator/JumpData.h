#ifndef JUMPDATA_H
#define JUMPDATA_H

#include <QVector>

#include "../utilities/ClassWithID.h"
#include "ConditionsInfo.h"
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
    double distance;
    double points;
    double gateCompensation;
    double windCompensation;
    double totalCompensation;
    double judgesPoints;

    Landing landing;
    QVector<double> judges;

    ConditionsInfo conditionsInfo;


    Jumper * jumper;
    Hill * hill;

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
    ConditionsInfo getConditionsInfo() const;
    void setConditionsInfo(const ConditionsInfo &newConditionsInfo);
    double getJudgesPoints() const;
    void setJudgesPoints(double newJudgesPoints);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
};

#endif // JUMPDATA_H
