#ifndef INDIVIDUALCOMPETITIONMANAGER_H
#define INDIVIDUALCOMPETITIONMANAGER_H

#include <QVector>
#include <QObject>

#include "../AbstractCompetitionManager.h"
#include "../../simulator/Jumper.h"

class IndividualCompetitionManager : public AbstractCompetitionManager
{
    Q_OBJECT
public:
    IndividualCompetitionManager();
    void simulateNext(const JumpManipulator &manipulator = JumpManipulator());

signals:
    void actualJumperIndexChanged();

private:
    QVector<Jumper *> startingJumpers;
    QVector<Jumper *> actualRoundJumpers;
    Jumper * actualJumper;
    int actualJumperIndex;

public:
    void setupFirstRoundJumpers(CompetitionResults * qualificationsResults);

public:
    void setActualJumperIndex(int newActualJumperIndex);
    int getActualJumperIndex() const;
    QVector<Jumper *> getStartingJumpers() const;
    void setStartingJumpers(const QVector<Jumper *> &newStartingJumpers);
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
