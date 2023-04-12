#include "AbstractClassificationSingleResult.h"
#include "../../../simulator/JumpData.h"

AbstractClassificationSingleResult::AbstractClassificationSingleResult()
{

}

QVector<JumpData *> AbstractClassificationSingleResult::getJumps() const
{
    return jumps;
}

void AbstractClassificationSingleResult::setJumps(const QVector<JumpData *> &newJumps)
{
    jumps = newJumps;
}
