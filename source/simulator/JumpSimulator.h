#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QVector>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "WindsInfo.h"
#include "Hill.h"
#include "Landing.h"

#include <QMap>

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, const WindsInfo & conditionsInfo = WindsInfo(), Hill *hill = nullptr);
private:
    Jumper * jumper;
    JumperSkills * jumperSkills; //automatycznie od jumper
    WindsInfo windsInfo;
    Hill * hill;
    int * gate;
    double DSQBaseProbability;

    CompetitionRules * jumpDataCompetitionRules;

public:
    //skok
    void simulateJump();

    void resetTemporaryParameters();
    void updateJumperSkills();
public:

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    JumperSkills *getJumperSkills() const;
    QVector<double> getJudges() const;
    void setJudges(const QVector<double> &newJudges);
    JumpData getJumpData();
    void setJumpData(const JumpData &newJumpData);
    int *getGate() const;
    void setGate(int *newGate);
    WindsInfo getWindsInfo() const;
    void setWindsInfo(const WindsInfo &newWindsInfo);
    short getWindAverageCalculatingType() const;
    void setWindAverageCalculatingType(short newWindAverageCalculatingType);
    short getWindCompensationDistanceEffect() const;
    void setWindCompensationDistanceEffect(short newWindCompensationDistanceEffect);
    double getDSQBaseProbability() const;
    void setDSQBaseProbability(double newDSQBaseProbability);

private:
    enum RandomParameters{
        TakeoffRating,
        FlightRating,
        LandingInstability
    };
    static double getRandomForJumpSimulation(short parameter, Jumper * jumper);

    bool generateDSQ();
    void generateTakeoffRating();
    void generateFlightRating();
    double getMultiplierForFlightStyleEffect();

    void generateDistance();
    void generateWindEffects();
    void generateLanding();
    void generateJudges();
    void calculateCompensations();
    void calculatePoints();

    ///symulacja
    JumpData jumpData;
    void setupJumpData();

    JumpSimulationData * simulationData;

    double getWindSegmentDistance();

    short windAverageCalculatingType;
    short windCompensationDistanceEffect;

public:
    enum WindCompensationDistanceEffect{
        Disabled,
        Original,
        Full
    };
    CompetitionRules *getJumpDataCompetitionRules() const;
    void setJumpDataCompetitionRules(CompetitionRules *newJumpDataCompetitionRules);
};

#endif // JUMPSIMULATOR_H
