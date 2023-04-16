#ifndef SEASONSMANAGER_H
#define SEASONSMANAGER_H

#include <QVector>
class Jumper;
class Hill;


class SeasonsManager
{
public:
    SeasonsManager();

private:
    QVector<Jumper> jumpers;
    QVector<Jumper *> activeJumpers;
    QVector<Jumper *> inactiveJumpers;
    QVector<Hill> hills;
public:
    QVector<Jumper> getJumpers() const;
    void setJumpers(const QVector<Jumper> &newJumpers);
    QVector<Jumper *> getActiveJumpers() const;
    void setActiveJumpers(const QVector<Jumper *> &newActiveJumpers);
    QVector<Jumper *> getInactiveJumpers() const;
    void setInactiveJumpers(const QVector<Jumper *> &newInactiveJumpers);
    QVector<Hill> getHills() const;
    void setHills(const QVector<Hill> &newHills);
    void updateJumpersActivityVectors();
};

#endif // SEASONSMANAGER_H
