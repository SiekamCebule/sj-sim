#ifndef SINGLEJUMPSMANAGER_H
#define SINGLEJUMPSMANAGER_H

#include <QVector>
#include "../simulator/JumpData.h"
#include "../simulator/Hill.h"
#include "../simulator/ConditionsInfo.h"
#include "../simulator/wind-generation/WindsGenerator.h"
#include "../simulator/JumpSimulator.h"

class SingleJumpsManager
{
public:
    SingleJumpsManager(const Jumper &jumper = Jumper(), const Hill &hill = Hill(), const ConditionsInfo &conditionsInfo = ConditionsInfo(), const WindsGenerator &windsGenerator = WindsGenerator(), int jumpsCount = 0);

    void simulate();

    Jumper getJumper() const;
    void setJumper(const Jumper &newJumper);
    Hill getHill() const;
    void setHill(const Hill &newHill);
    ConditionsInfo getConditionsInfo() const;
    void setConditionsInfo(const ConditionsInfo &newConditionsInfo);
    int getJumpsCount() const;
    void setJumpsCount(int newJumpsCount);
    QVector<JumpData *> getJumps() const;
    void setJumps(const QVector<JumpData *> &newJumps);

private:
    Jumper jumper;
    Hill hill;
    ConditionsInfo conditionsInfo;
    WindsGenerator windsGenerator;
    JumpSimulator jumpSimulator;

    int jumpsCount;
    QVector<JumpData *> jumps;
};

#endif // SINGLEJUMPSMANAGER_H
