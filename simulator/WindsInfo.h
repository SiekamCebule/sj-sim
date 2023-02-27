#ifndef WINDSINFO_H
#define WINDSINFO_H

#include "../utilities/ClassWithID.h"
#include "Wind.h"
#include "Hill.h"

#include <QVector>


class WindsInfo : public ClassWithID
{
public:
    WindsInfo(const QVector<Wind> & winds =QVector<Wind>());
private:
    QVector<Wind> winds; // łącznie 1 wiatr na 10 metrów.
    QVector<ulong> windsIDs;

    //pomiary:
    /*
     * 10% punktu K - 10%
     * 30% punktu K - 10
     * 45% punktu K - 20
     * 60% punktu K - 20
     * 75% punktu K - 15
     * 90% punktu K - 15
     * 105% punktu K - 10
     */

public:
    enum AverageWindType{
        Original,
        Improved
    };

    Wind getAveragedWind(short type);

    ulong getWindID(int index) const;
    QVector<Wind> & getWinds();
    void setWinds(const QVector<Wind> &newWinds);
    QVector<ulong> getWindsIDs() const;
    void setWindsIDs(const QVector<ulong> &newWindsIDs);
    void setVectorWindID(int index, ulong newID);
};

#endif // WINDSINFO_H
