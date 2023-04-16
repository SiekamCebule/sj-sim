#include "SeasonsManager.h"

#include "../simulator/Jumper.h"
#include "../simulator/Hill.h"

SeasonsManager::SeasonsManager()
{

}

QVector<Hill> SeasonsManager::getHills() const
{
    return hills;
}

void SeasonsManager::setHills(const QVector<Hill> &newHills)
{
    hills = newHills;
}

void SeasonsManager::updateJumpersActivityVectors()
{

}

QVector<Jumper *> SeasonsManager::getInactiveJumpers() const
{
    return inactiveJumpers;
}

void SeasonsManager::setInactiveJumpers(const QVector<Jumper *> &newInactiveJumpers)
{
    inactiveJumpers = newInactiveJumpers;
}

QVector<Jumper *> SeasonsManager::getActiveJumpers() const
{
    return activeJumpers;
}

void SeasonsManager::setActiveJumpers(const QVector<Jumper *> &newActiveJumpers)
{
    activeJumpers = newActiveJumpers;
}

QVector<Jumper> SeasonsManager::getJumpers() const
{
    return jumpers;
}

void SeasonsManager::setJumpers(const QVector<Jumper> &newJumpers)
{
    jumpers = newJumpers;
}
