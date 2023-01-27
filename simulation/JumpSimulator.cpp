#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "JumpMistake.h"

#include <QDebug>
#include <tuple>
#include <QRandomGenerator>

JumpSimulator::JumpSimulator(Jumper *jumper, ConditionsInfo *conditionsInfo, Hill *hill, Competition *competition) : jumper(jumper),
    conditionsInfo(conditionsInfo),
    hill(hill),
    competition(competition)
{
    distance = 0;
    height = 0;
}

void JumpSimulator::simulateAll()
{
    simulateInrun();
    simulateTakeoff();
}

void JumpSimulator::simulateInrun()
{
    //predkosc najazdowa
    speed = hill->getBaseSpeed() + (conditionsInfo->getGate() * hill->getSpeedForGate());
    if(jumperCharacteristicsContains("very-slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 115;
    else if(jumperCharacteristicsContains("slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 170;
    else if(jumperCharacteristicsContains("little-slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 340;
    else if(jumperCharacteristicsContains("little-fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 340;
    else if(jumperCharacteristicsContains("fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 170;
    else if(jumperCharacteristicsContains("very-fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 115;

    speed += randomDouble(-hill->getBaseSpeed() / 450, hill->getBaseSpeed() / 450);

    qDebug()<<"Prędkość najazdowa skoczka: "<<speed<<" km/h";
}

void JumpSimulator::simulateTakeoff()
{
    //wybicie trwa 8% punktu K skoczni z małym mnożnikiem od prędkości
    double takeoffDuration = (hill->getKPoint() * 0.08) * (0.95 + speed / 850);
    qDebug()<<"Czas trwania wybicia: "<<QString::number(takeoffDuration, 'f', 1)<<" metrów";
    distance += takeoffDuration;
    height += hill->getTableHeight() * 0.96 + (double(jumper->getJumperSkills()->getTakeoffPower()) / 28 + double(jumper->getJumperSkills()->getForm()) / 61) * (speed / 240);
    qDebug()<<"Wysokość skoczka po wyjściu z progu: "<<height<<" metrów";

    JumpMistake m;
    m.generateJumpMistake(this, JumpMistake::TakeoffMistake);


    //błędy na progu
    int possibleMistakes = takeoffDuration / 1.75;
    for(int i=0; i<possibleMistakes; i++)
    {
        JumpMistake mistake = JumpMistake::generateJumpMistake(this, JumpMistake::MistakeType::TakeoffMistake);
        if(mistake.getIsOccurred())
        {
            //skutek błędu
        }
    }

}

Competition *JumpSimulator::getCompetition() const
{
    return competition;
}

void JumpSimulator::setCompetition(Competition *newCompetition)
{
    competition = newCompetition;
}

double JumpSimulator::getDistance() const
{
    return distance;
}

bool JumpSimulator::jumperCharacteristicsContains(QString characteristics)
{
    return jumper->getJumperSkills()->getCharacteristics().contains(characteristics);
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
