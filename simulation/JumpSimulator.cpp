#include "JumpSimulator.h"

#include <QDebug>

JumpSimulator::JumpSimulator(Jumper *jumper, ConditionsInfo *conditionsInfo, Hill *hill, Competition *competition) : jumper(jumper),
    conditionsInfo(conditionsInfo),
    hill(hill),
    competition(competition)
{}

Competition *JumpSimulator::getCompetition() const
{
    return competition;
}

void JumpSimulator::setCompetition(Competition *newCompetition)
{
    competition = newCompetition;
}

JumpData JumpSimulator::getJumpData()
{
    JumpData jumpData;
    return jumpData;
}

Hill *JumpSimulator::getHill() const
{
    return hill;
}

void JumpSimulator::setHill(Hill *newHill)
{
    hill = newHill;
}

ConditionsInfo *JumpSimulator::getConditionsInfo() const
{
    return conditionsInfo;
}

void JumpSimulator::setConditionsInfo(ConditionsInfo *newConditionsInfo)
{
    conditionsInfo = newConditionsInfo;
}

Jumper *JumpSimulator::getJumper() const
{
    return jumper;
}

void JumpSimulator::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

void JumpSimulator::simulate()
{
    QVector<JumpEffect> effects;

    double height = 0;
    double speed = 0;
    uint distance = 0;

    //TAKEOFF
    JumpEffect takeoffEffect;
    takeoffEffect.duration = 10; //10 metrow
    QPair<double, double> pair1 = {0.25, 0.30};
    QPair<double, double> pair2 = {0.95, 0.22};

    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair1);
    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair1);
    takeoffEffect.effects.push_back(pair1);
    takeoffEffect.effects.push_back(pair1);
    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair2);
    takeoffEffect.effects.push_back(pair1);


    //PĘTLA KAŻDEGO METRA

}

JumpSimulator::JumpEffect::JumpEffect()
{
    actualIndex = 0;
    duration = 0;
}
