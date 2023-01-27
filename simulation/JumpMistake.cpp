#include "JumpMistake.h"
#include "JumperSkills.h"
#include "JumpSimulator.h"

#include "../utilities/functions.h"

#include <QVector>
#include <QDebug>

JumpMistake::JumpMistake(short type, short harmuflness, bool isOccurred, double distance) : type(type),
    harmfulness(harmfulness),
    isOccurred(isOccurred),
    distance(distance)
{

}

bool JumpMistake::getIsOccurred() const
{
    return isOccurred;
}

void JumpMistake::setIsOccurred(bool newIsOccurred)
{
    isOccurred = newIsOccurred;
}

short JumpMistake::getHarmfulness() const
{
    return harmfulness;
}

void JumpMistake::setHarmfulness(short newHarmfulness)
{
    harmfulness = newHarmfulness;
}

JumpMistake JumpMistake::generateJumpMistake(JumpSimulator * const simulator, short mistakeType)
{
    QVector<double> probabilities;
    probabilities.fill(0, 10);
    double drawSum = 0;
    JumpMistake mistake;
    JumperSkills * const jumperSkills = simulator->getJumper()->getJumperSkills();

    if(mistakeType == MistakeType::TakeoffMistake)
    {
        // Dla skoczka:
        /* Technika wybicia 25
         * Forma: 25
         * Bez żadnych cech
         */
        probabilities = {3, 6.8, 19, 52, 135, 400, 1200, 3550, 9800, 30000};
        drawSum = 0.78 + ((double(jumperSkills->getTakeoffTechnique()) - 26) / 100 + (double(jumperSkills->getForm()) - 26) / 100);
    }

    for(short i=9; i>=0; i--)
    {
        double chance = 1 / (probabilities[i] * drawSum) * 100;
        double random = randomDouble(0, 100);
        if(random < chance) // jeśli tak, skoczek popełnił błąd.
        {
            mistake.setIsOccurred(true);
            mistake.setType(MistakeType::TakeoffMistake);
            mistake.setDistance(simulator->getDistance());
            mistake.setHarmfulness(i+1);
        }
    }
    return mistake;
}

double JumpMistake::getDistance() const
{
    return distance;
}

void JumpMistake::setDistance(double newDistance)
{
    distance = newDistance;
}

short JumpMistake::getType() const
{
    return type;
}

void JumpMistake::setType(short newType)
{
    type = newType;
}
