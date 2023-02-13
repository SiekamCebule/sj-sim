#ifndef WINDSGENERATOR_H
#define WINDSGENERATOR_H

#include "WindGenerationSettings.h"
#include "../Wind.h"

#include <QVector>

class WindsGenerator
{
public:
    WindsGenerator(int windsCount = 0, const QVector<WindGenerationSettings> &generationSettings = QVector<WindGenerationSettings>());
private:
    int windsCount;
    QVector<WindGenerationSettings> generationSettings;
public:
    QVector<Wind> generateWinds();
};

#endif // WINDSGENERATOR_H
