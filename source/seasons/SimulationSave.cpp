#include "SimulationSave.h"

SimulationSave::SimulationSave()
{

}

QVector<Hill> SimulationSave::getHills() const
{
    return hills;
}

QVector<Jumper *> SimulationSave::getInactiveJumpers() const
{
    return inactiveJumpers;
}

QVector<Jumper *> &SimulationSave::getInactiveJumpersReference()
{
    return inactiveJumpers;
}

void SimulationSave::setInactiveJumpers(const QVector<Jumper *> &newInactiveJumpers)
{
    inactiveJumpers = newInactiveJumpers;
}

QVector<Jumper *> &SimulationSave::getActiveJumpers()
{
    return activeJumpers;
}

void SimulationSave::setActiveJumpers(const QVector<Jumper *> &newActiveJumpers)
{
    activeJumpers = newActiveJumpers;
}

void SimulationSave::setHills(const QVector<Hill> &newHills)
{
    hills = newHills;
}

QVector<Jumper> SimulationSave::getJumpers() const
{
    return jumpers;
}

QVector<Jumper> &SimulationSave::getJumpersReference()
{
    return jumpers;
}

void SimulationSave::setJumpers(const QVector<Jumper> &newJumpers)
{
    jumpers = newJumpers;
}

QVector<Hill> &SimulationSave::getHillsReference()
{
    return hills;
}

QVector<Season> SimulationSave::getSeasons() const
{
    return seasons;
}

QVector<Season> &SimulationSave::getSeasonsReference()
{
    return seasons;
}

void SimulationSave::setSeasons(const QVector<Season> &newSeasons)
{
    seasons = newSeasons;
}

QString SimulationSave::getName() const
{
    return name;
}

void SimulationSave::setName(const QString &newName)
{
    name = newName;
}
