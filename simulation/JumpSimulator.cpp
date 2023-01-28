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
    speed = 0;
    aerodynamicPosition = 0;
    takeoffMistakeHeightEffect = 0;
    takeoffMistakeSpeedEffect = 0;
    takeoffMistakeAerodynamicPositionEffect = 0;
    heightAboveLandingHill = 0;
    isLanding = false;
}

void JumpSimulator::simulateAll()
{
    simulateInrun();
    simulateTakeoff();
    setAerodynamicPositionAfterTakeoff();
    simulateFlight();
}

void JumpSimulator::simulateInrun()
{
    //predkosc najazdowa
    speed = hill->getBaseSpeed() + (conditionsInfo->getGate() * hill->getSpeedForGate());
    if(jumperCharacteristicsContains("very-slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 150;
    else if(jumperCharacteristicsContains("slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 225;
    else if(jumperCharacteristicsContains("little-slow-inrun-speed"))
        speed -= hill->getBaseSpeed() / 360;
    else if(jumperCharacteristicsContains("little-fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 360;
    else if(jumperCharacteristicsContains("fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 225;
    else if(jumperCharacteristicsContains("very-fast-inrun-speed"))
        speed += hill->getBaseSpeed() / 150;

    speed += randomDouble(-hill->getBaseSpeed() / 450, hill->getBaseSpeed() / 450);

    qDebug()<<"Prędkość najazdowa skoczka: "<<speed<<" km/h";
}

void JumpSimulator::simulateTakeoff()
{
    //wybicie trwa 6% punktu K skoczni z małym mnożnikiem od prędkości
    double takeoffDuration = (hill->getKPoint() * 0.06) * (0.95 + speed / 850);
    //qDebug()<<"Czas trwania wybicia: "<<QString::number(takeoffDuration, 'f', 1)<<" metrów";
    distance += takeoffDuration;
    height += hill->getTableHeight() * 0.96 + (double(jumper->getJumperSkills()->getTakeoffPower()) / 28 + double(jumper->getJumperSkills()->getForm()) / 61) * (speed / 240);
    qDebug()<<"Wysokość skoczka po wyjściu z progu: "<<height<<" metrów";

    JumpMistake m;
    m.generateJumpMistake(this, JumpMistake::TakeoffMistake);

    JumpMistake mistake = JumpMistake::generateJumpMistake(this, JumpMistake::MistakeType::TakeoffMistake);
    if(mistake.getIsOccurred())
    {
        takeoffMistakeHeightEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::Height);
        takeoffMistakeSpeedEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::Speed);
        takeoffMistakeAerodynamicPositionEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::AerodynamicPosition);
        qDebug()<<"Popełniono błąd na progu: "<<mistake.getNote()<<" (Szkodliwość błędu "<<mistake.getHarmfulness()<<"/ 10) --> Skutki błędu: utracono "<<QString::number(takeoffMistakeHeightEffect, 'f', 2)<<"metrów wysokości, "<<QString::number(takeoffMistakeSpeedEffect, 'f', 2)<<"km/h prędkości"<<", i pogorszono pozycję aerodynamiczną o "<<takeoffMistakeAerodynamicPositionEffect;

        height -= takeoffMistakeHeightEffect;
        speed -= takeoffMistakeSpeedEffect;
        aerodynamicPosition -= takeoffMistakeAerodynamicPositionEffect;
    }
    else qDebug()<<"Idealne wybicie";

}

void JumpSimulator::setAerodynamicPositionAfterTakeoff()
{
    JumperSkills * const jumperSkills = jumper->getJumperSkills();
    aerodynamicPosition += (double(jumperSkills->getTakeoffTechnique()) / 2.52) + (double(jumperSkills->getFlightTechnique()) / 7.3) + (double(jumperSkills->getForm()) / 2.68) + randomInt(-2, 2);
    qDebug()<<"Pozycja aerodynamiczna skoczka po wyjściu z progu: "<<aerodynamicPosition<<"/ 50";
}

void JumpSimulator::simulateFlight()
{
    isLanding = true; //tymczasowo
    int whichIteration = 1;
    while(isLanding == false)
    {
        speed += (aerodynamicPosition - 27) / 101;
        aerodynamicPosition += 0;
        distance += 0;

        whichIteration++;
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

double JumpSimulator::getSpeed() const
{
    return speed;
}

double JumpSimulator::getHeight() const
{
    return height;
}

short JumpSimulator::getAerodynamicPosition() const
{
    return aerodynamicPosition;
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
