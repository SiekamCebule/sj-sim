#include "SeasonCalendar.h"

#include "../utilities/functions.h"

SeasonCalendar::SeasonCalendar()
{

}

SeasonCalendar::~SeasonCalendar()
{
    for(auto & comp : competitions)
        delete comp;
}

void SeasonCalendar::fixCompetiitonsClassifications(QVector<Classification> * classificationsList)
{
    for(auto & comp : competitions){
        for(auto & classification : comp->getClassificationsReference()){
            if(MyFunctions::vectorContains(*classificationsList, classification) == false){
                //Trzeba usunąć classification z classificationsList ponieważ wcześniej classification zostało usunięte
                qDebug()<<classificationsList->count()<<"  C1";
                MyFunctions::removeFromVector(comp->getClassificationsReference(), classification);
                qDebug()<<classificationsList->count()<<"  C2";
            }
        }
    }
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo *> &newCompetitions)
{
    competitions = newCompetitions;
}

QVector<CompetitionInfo *> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

QVector<CompetitionInfo *> &SeasonCalendar::getCompetitionsReference()
{
    return competitions;
}
