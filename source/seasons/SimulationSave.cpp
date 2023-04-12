#include "SimulationSave.h"

SimulationSave::SimulationSave()
{
    seasonsManager = nullptr;
}

SimulationSave::SimulationSave(SeasonsManager *seasonsManager) :
    seasonsManager(seasonsManager)
{

}

SeasonsManager *SimulationSave::getSeasonsManager() const
{
    return seasonsManager;
}

void SimulationSave::setSeasonsManager(SeasonsManager *newSeasonsManager)
{
    seasonsManager = newSeasonsManager;
}

QVector<Season> SimulationSave::getSeasons() const
{
    return seasons;
}

QVector<Season> &SimulationSave::getSeasonsReferences()
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
