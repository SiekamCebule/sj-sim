#ifndef SIMULATIONSAVE_H
#define SIMULATIONSAVE_H

#include <QVector>
#include <QString>
#include "Season.h"
#include "SeasonsManager.h"

class SimulationSave
{
public:
    SimulationSave();
    SimulationSave(SeasonsManager * seasonsManager);

private:
    QString name;
    QVector<Season> seasons;
    SeasonsManager * seasonsManager;

public:
    QString getName() const;
    void setName(const QString &newName);
    QVector<Season> getSeasons() const;
    QVector<Season>& getSeasonsReference();
    void setSeasons(const QVector<Season> &newSeasons);
    SeasonsManager *getSeasonsManager() const;
    void setSeasonsManager(SeasonsManager *newSeasonsManager);
};

#endif // SIMULATIONSAVE_H
