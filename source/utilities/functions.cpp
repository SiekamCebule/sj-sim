#include "functions.h"
#include <math.h>

double roundDoubleToHalf(double number)
{
    return (std::floor((number * 2) + 0.5) / 2);
}

double roundDoubleToOnePlace(double number)
{
    return static_cast<float>(static_cast<int>(number * 10.)) / 10.;
}

double roundDoubleToTwoPlaces(double number)
{
    return std::ceil(number * 100.0) / 100.0;
}

QVector<QWidget *> MyFunctions::getWidgetsVector(QWidget *parent, QString search)
{
    QRegularExpression exp(search);
    return parent->findChildren<QWidget *>(exp);
}
