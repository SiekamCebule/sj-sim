#include "JumpSimulator.h"
#include "../utilities/functions.h"
#include "JumpMistake.h"

#include <QDebug>
#include <tuple>
#include <QRandomGenerator>
#include <math.h>

JumpSimulator::JumpSimulator(Jumper *jumper, ConditionsInfo *conditionsInfo, Hill *hill, Competition *competition) : jumper(jumper),
    conditionsInfo(conditionsInfo),
    hill(hill),
    competition(competition)
{
    takeoffDuration = 0;
    distance = 0;
    relativeHeight = 0;
    speed = 0;
    aerodynamicPosition = 0;
    takeoffMistakeHeightEffect = 0;
    takeoffMistakeSpeedEffect = 0;
    takeoffMistakeAerodynamicPositionEffect = 0;
    heightAboveLandingHill = 0;
    isLanding = false;
}

void JumpSimulator::simulateJump()
{
    simulateInrun();
    simulateTakeoff();
    setAerodynamicPositionAfterTakeoff();
    simulateFlight();
    qDebug()<<"Skok zakończył się na "<<(std::floor((distance * 2) + 0.5) / 2)<<" metrze.";
}

void JumpSimulator::resetTemporaryParameters()
{
    takeoffDuration = 0;
    distance = 0;
    relativeHeight = 0;
    speed = 0;
    aerodynamicPosition = 0;
    takeoffMistakeHeightEffect = 0;
    takeoffMistakeSpeedEffect = 0;
    takeoffMistakeAerodynamicPositionEffect = 0;
    heightAboveLandingHill = 0;
    isLanding = false;
}

void JumpSimulator::simulateInrun()
{
    //predkosc najazdowa
    speed = hill->getBaseSpeed() + (conditionsInfo->getGate() * hill->getSpeedForGate());
    speed += randomDouble(-hill->getBaseSpeed() / 460, hill->getBaseSpeed() / 460);

    if(jumperCharacteristicsContains(Characteristic("inrun-speed")))
        speed += 0.28 * jumper->getJumperSkills()->getLevelOfCharacteristic("inrun-speed");

    qDebug()<<"Prędkość najazdowa skoczka: "<<speed<<" km/h";
}

void JumpSimulator::simulateTakeoff()
{
    //wybicie trwa 6% punktu K skoczni z małym mnożnikiem od prędkości
    takeoffDuration = (hill->getKPoint() * 0.06) * (0.95 + speed / 850);
    //qDebug()<<"Czas trwania wybicia: "<<QString::number(takeoffDuration, 'f', 1)<<" metrów";
    //distance += takeoffDuration;
    relativeHeight += hill->getTableHeight() * 0.96 + (double(jumper->getJumperSkills()->getTakeoffPower()) / 28 + double(jumper->getJumperSkills()->getForm()) / 61) * (speed / 240);
    qDebug()<<"Wysokość skoczka po wyjściu z progu: "<<relativeHeight<<" metrów";

    JumpMistake m;
    m.generateJumpMistake(this, JumpMistake::TakeoffMistake);;

    JumpMistake mistake = JumpMistake::generateJumpMistake(this, JumpMistake::MistakeType::TakeoffMistake);
    if(mistake.getIsOccurred())
    {
        takeoffMistakeHeightEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::Height);
        takeoffMistakeSpeedEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::Speed);
        takeoffMistakeAerodynamicPositionEffect = JumpMistake::generateJumpMistakeEffect(this, &mistake, JumpMistake::AerodynamicPosition);
        qDebug()<<"Popełniono błąd na progu: "<<mistake.getNote()<<" (Szkodliwość błędu "<<mistake.getHarmfulness()<<"/ 10) --> Skutki błędu: utracono "<<QString::number(takeoffMistakeHeightEffect, 'f', 2)<<"metrów wysokości, "<<QString::number(takeoffMistakeSpeedEffect, 'f', 2)<<"km/h prędkości"<<", i pogorszono pozycję aerodynamiczną o "<<takeoffMistakeAerodynamicPositionEffect;

        relativeHeight -= takeoffMistakeHeightEffect;
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
    //qDebug()<<QString::number(takeoffDuration, 'f', 1)<<"metr (wybicie) --> Prędkość: "<<speed<<", wysokość: "<<relativeHeight;
    int whichIteration = 1;
    distance += takeoffDuration;
    while(isLanding == false)
    {
        speed += (double(aerodynamicPosition) - 28.5) / 180; // zmiana prędkości przez pozycję aerodynamiczną
        aerodynamicPosition += 0; // Zmiana pozycji aerodynamicznej
        relativeHeight -= 0.0282; // bazowe odjęcie;
        relativeHeight -= 0; // wiatr w plecy
        relativeHeight += 0; // wiatr pod narty
        if(distance > hill->getLandingZoneStart())
            relativeHeight -= hill->getRelativeHeightSubstractAfterLandingZone(distance);
        //qDebug()<<"Odjecie: "<<hill->getRelativeHeightSubstractAfterLandingZone(distance);

        /*Co wpływa na zmianę wysokości względnej?
         * (czynniki zewnętrzne)
         * 1. Grawitacja
         * (skoczek)
         * 1. Wiatr w plecy
         * 2. Wiatr pod narty (Podwyższenie wysokości)
         * 3. Błędy zawodnika
         * 4. Cechy charakterystyczne dot. wysokości
         * (skocznia)
         * 1. Profil zeskoku
        */

        double distanceChange = 0.865 + ((speed - 88) / 27);
        if(distanceChange < 0.02) distanceChange = 0.02;
        distance += distanceChange; // Zmiana odległości (zawsze przynajmniej 0.04 metra)

        //qDebug()<<QString::number(distance, 'f', 1)<<"metr --> Prędkość: "<<speed<<", wysokość: "<<relativeHeight;


        if(relativeHeight < 0.25) isLanding = true;

        whichIteration++;
    }
}

double JumpSimulator::getRelativeHeight() const
{
    return relativeHeight;
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

short JumpSimulator::getAerodynamicPosition() const
{
    return aerodynamicPosition;
}

bool JumpSimulator::jumperCharacteristicsContains(const Characteristic & characteristics)
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
