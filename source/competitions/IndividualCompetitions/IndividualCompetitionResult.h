#ifndef INDIVIDUALCOMPETITIONRESULT_H
#define INDIVIDUALCOMPETITIONRESULT_H

#include "../AbstractCompetitionResult.h"
#include "../../simulator/JumpData.h"
#include <QVector>

class IndividualCompetitionResult : AbstractCompetitionResult
{
public:
    IndividualCompetitionResult();

private:
    QVector<JumpData> jumps;
    Jumper * jumper;

public:
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    QVector<JumpData> getJumps() const;
    void setJumps(const QVector<JumpData> &newJumps);
};

#endif // INDIVIDUALCOMPETITIONRESULT_H
