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
    bool checkRoundEnd();
    bool checkCompetitionEnd();

    void setupNextRound();
    void fillCompletedJumpsToStartOfRound();

    int getCompetitiorsCountForActualRound();
    bool getSortStartListForActualRound();

signals:
    void actualJumperIndexChanged();

private:
    QVector<Jumper *> startingJumpers;
    QVector<Jumper *> actualRoundJumpers;
    Jumper * actualJumper;
    int actualJumperIndex;

public:
    void setupJumpersForActualRound();

public:
    void setActualJumperIndex(int newActualJumperIndex);
    int getActualJumperIndex() const;
    QVector<Jumper *> getStartingJumpers() const;
    void setStartingJumpers(const QVector<Jumper *> &newStartingJumpers);
    QVector<Jumper *> *getActualRoundJumpersPointer() const;
    void setActualRoundJumpers(const QVector<Jumper *> &newActualRoundJumpers);
    void inncrementActualJumperIndex();
    void decrementActualJumperIndex();
    Jumper *getActualJumper() const;
    QVector<Jumper *> getActualRoundJumpers() const;
};

#endif // INDIVIDUALCOMPETITIONMANAGER_H
