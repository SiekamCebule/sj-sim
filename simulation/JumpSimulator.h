#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QList>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "ConditionsInfo.h"
#include "Hill.h"

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, ConditionsInfo *conditionsInfo = nullptr, Hill *hill = nullptr, Competition *competition = nullptr);
private:
    Jumper * jumper;
    ConditionsInfo * conditionsInfo;
    Hill * hill;
    Competition * competition;

public:
    //skok
    void simulate();
    void setupTakeoffEffect();


    JumpData getJumpData();


    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    ConditionsInfo *getConditionsInfo() const;
    void setConditionsInfo(ConditionsInfo *newConditionsInfo);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    Competition *getCompetition() const;
    void setCompetition(Competition *newCompetition);


    struct JumpEffect
    {
        JumpEffect();

        int duration;
        QVector<QPair<double, double>> effects; // wysokosc, predkosc
        int actualIndex;



    public:
        int getDuration() const;
    };

private:
    QVector<JumpEffect> allEffects;
    JumpEffect takeoffEffect;
};

#endif // JUMPSIMULATOR_H
