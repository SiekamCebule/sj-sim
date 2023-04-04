#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>
#include <QWidget>
#include <QRegularExpression>

template<typename T>
void moveItemToVectorBack(QVector<T> & vector, size_t itemIndex)
{
    auto it = vector.begin() + itemIndex;
    std::rotate(it, it+1, vector.end());
}

class MyFunctions{
public:
    static QVector<QWidget *> getWidgetsVector(QWidget *parent, QString search);

    template<typename T>
    static bool pointersVectorContains(const QVector<T *> & vector, T * item){
        for(auto & it : vector){
            if(it == item)
                return true;
        }
        return false;
    }
};

double roundDoubleToHalf(double number);
double roundDoubleToOnePlace(double number);
double roundDoubleToTwoPlaces(double number);


#endif // FUNCTIONS_H
