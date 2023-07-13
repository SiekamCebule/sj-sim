#ifndef SIMULATIONSAVE_H
#define SIMULATIONSAVE_H

#include <QVector>
#include <QString>
#include "Season.h"

class SimulationSave
{
public:
    SimulationSave();

private:
    QString name;
    QVector<Season> seasons;
    QVector<Jumper> jumpers;
    QVector<Hill> hills;

    QVector<Jumper *> activeJumpers;
    QVector<Jumper *> inactiveJumpers;

public:
    QString getName() const;
    void setName(const QString &newName);
    QVector<Season> getSeasons() const;
    QVector<Season>& getSeasonsReference();
    void setSeasons(const QVector<Season> &newSeasons);
    QVector<Jumper> getJumpers() const;
    QVector<Jumper> & getJumpersReference();
    void setJumpers(const QVector<Jumper> &newJumpers);
    QVector<Hill> & getHillsReference();
    void setHills(const QVector<Hill> &newHills);
    QVector<Jumper *> getActiveJumpers() const;
    QVector<Jumper *> & getActiveJumpers();
    void setActiveJumpers(const QVector<Jumper *> &newActiveJumpers);
    QVector<Jumper *> getInactiveJumpers() const;
    QVector<Jumper *> & getInactiveJumpersReference();
    void setInactiveJumpers(const QVector<Jumper *> &newInactiveJumpers);
    QVector<Hill> getHills() const;
};

#endif // SIMULATIONSAVE_H

//Co muszę zrobić?
//Znaleźć sposób na wczytanie pewnych rzeczy np. CompetitionInfo tak żeby następne wczytane CompetitionInfo mogły mieć ustawione treningi które zostały wczytane tuż przed aktualnym CompetitionInfo
//Rozwiązanie:
//1. Zrobić w CompetitionInfo funkcję zwracającą sam obiekt a nie vector
//2. Zrobić w klasie Calendar funkcję loadCalendar na podstawie QJsonObject-a która po każdym wczytanym competition daje go do seasonObjectsManagera
