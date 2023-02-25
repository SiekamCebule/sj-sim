#include "SingleJumpsManager.h"

Jumper SingleJumpsManager::getJumper() const
{
    return jumper;
}

void SingleJumpsManager::setJumper(const Jumper &newJumper)
{
    jumper = newJumper;
}

Hill SingleJumpsManager::getHill() const
{
    return hill;
}

void SingleJumpsManager::setHill(const Hill &newHill)
{
    hill = newHill;
}

ConditionsInfo SingleJumpsManager::getConditionsInfo() const
{
    return conditionsInfo;
}

void SingleJumpsManager::setConditionsInfo(const ConditionsInfo &newConditionsInfo)
{
    conditionsInfo = newConditionsInfo;
}

int SingleJumpsManager::getJumpsCount() const
{
    return jumpsCount;
}

void SingleJumpsManager::setJumpsCount(int newJumpsCount)
{
    jumpsCount = newJumpsCount;
}

QVector<JumpData *> SingleJumpsManager::getJumps() const
{
    return jumps;
}

void SingleJumpsManager::setJumps(const QVector<JumpData *> &newJumps)
{
    jumps = newJumps;
}

SingleJumpsManager::SingleJumpsManager(const Jumper &jumper, const Hill &hill, const ConditionsInfo &conditionsInfo, const WindsGenerator &windsGenerator, int jumpsCount) : jumper(jumper),
    hill(hill),
    conditionsInfo(conditionsInfo),
    windsGenerator(windsGenerator),
    jumpsCount(jumpsCount)
{}
