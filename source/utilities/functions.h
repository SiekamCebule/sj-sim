#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QVector>
#include <QWidget>
#include <vector>
#include <QRegularExpression>
#include <QMessageLogContext>
#include <QString>
#include <QMessageLogContext>
#include <QtGlobal>
#include <QVarLengthArray>
#include "../competitions/CompetitionInfo.h"

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
    static bool vectorContains(const QVector<T *> & vector, T * item)
    {
        for(auto & it : vector){
            if(it == item)
                return true;
        }
        return false;
    }
    template<typename T>
    static bool vectorContains(const QVector<T> & vector, T * item)
    {
        for(auto & it : vector){
            if(&it == item)
                return true;
        }
        return false;
    }
    template<typename T>
    static bool vectorContainsByID(const QVector<T> & vector, T * item)
    {
        for(auto & it : vector){
            if(it.getID() == item->getID())
                return true;
        }
        return false;
    }
    template<typename T>
    static bool vectorContainsByID(const QVector<T *> & vector, T * item)
    {
        for(auto & it : vector){
            if(it->getID() == item->getID())
                return true;
        }
        return false;
    }

    template<typename T>
    static void removeFromVector(QVector<T*> & vector, T * item)
    {
        int i=0;
        for(auto & it : vector)
        {
            if(it == item){
                vector.remove(i);
                break;
            }
            i++;
        }
    }

    template<typename T>
    static void removeFromVector(QVector<T> & vector, T * item)
    {
        int i=0;
        for(auto & it : vector)
        {
            if(&it == item){
                vector.remove(i);
                break;
            }
            i++;
        }
    }

    template<typename T>
    static void removeFromVectorByID(QVector<T> & vector, ulong id)
    {
        int i=0;
        for(auto & it : vector)
        {
            if(it->getID() == id){
                vector.remove(i);
                break;
            }
            i++;
        }
    }

    template <typename Type, typename SourceType>
    static Type static_cast_checked (SourceType *item)
    {
        assert (!item || dynamic_cast<Type> (item)); // sprawdź czy item jest nullptr lub poprawnym wskaźnikiem na Type
        return static_cast<Type> (item); // zwróć static_cast jako wynik
    }

    static void fileMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg);
};

double roundDoubleToHalf(double number);
double roundDoubleToOnePlace(double number);
double roundDoubleToTwoPlaces(double number);


#endif // FUNCTIONS_H
