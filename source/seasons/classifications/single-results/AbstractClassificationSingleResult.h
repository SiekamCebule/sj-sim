#ifndef ABSTRACTCLASSIFICATIONSINGLERESULT_H
#define ABSTRACTCLASSIFICATIONSINGLERESULT_H

#include <QVector>
class JumpData;

class AbstractClassificationSingleResult
{
public:
    AbstractClassificationSingleResult();

protected:
    QVector<JumpData *> jumps;

public:
    QVector<JumpData *> getJumps() const;
    void setJumps(const QVector<JumpData *> &newJumps);
};

#endif // ABSTRACTCLASSIFICATIONSINGLERESULT_H
