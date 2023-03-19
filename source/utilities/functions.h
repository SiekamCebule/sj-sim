#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>

template<typename T>
void moveItemToVectorBack(QVector<T> & vector, size_t itemIndex)
{
    auto it = vector.begin() + itemIndex;
    std::rotate(it, it+1, vector.end());
}

double roundDoubleToHalf(double number);
double roundDoubleToOnePlace(double number);
double roundDoubleToTwoPlaces(double number);


#endif // FUNCTIONS_H
