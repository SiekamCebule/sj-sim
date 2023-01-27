#include "functions.h"

#include <QRandomGenerator>
#include <QDebug>
#include <cmath>

double randomDouble(double min, double max, short precision)
{
    QVector<double> results;
    ulong toMultipy = std::pow(10, precision);
    for(int i=0; i<1000; i++)
    {
    double result = (QRandomGenerator::global()->bounded(int(min * toMultipy), int(max * toMultipy)));
    result = double(result / toMultipy);
    results.push_back(result);
    }
    return results[QRandomGenerator::global()->bounded(0, results.size() - 1)];
}

int randomInt(int min, int max)
{
    QVector<int> results;
    for(int i=0; i<1000; i++)
    {
        int result = QRandomGenerator::global()->bounded(min, max);
        results.push_back(result);
    }
    return results[QRandomGenerator::global()->bounded(0, results.size() - 1)];
}
