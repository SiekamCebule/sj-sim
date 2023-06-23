#include "SeasonCalendar.h"

SeasonCalendar::SeasonCalendar()
{

}

QVector<QPair<Hill *, QVector<CompetitionInfo *> > > SeasonCalendar::getCompetitionWeekendsVector()
{
    QVector<QPair<Hill *, QVector<CompetitionInfo *>>> competitionWeekends;
    for(auto & competition : competitions){
        if(competitions.count() == 0 || competitionWeekends.last().first != competition.getHill()){
            QPair<Hill *, QVector<CompetitionInfo *>> weekend;
            weekend.first = competition.getHill();
            weekend.second = {&competition};

            competitionWeekends.push_back(weekend);
        }
        else{
            competitionWeekends.last().second.push_back(&competition);
        }
    }

    return competitionWeekends;
}

QVector<CompetitionInfo> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

QVector<CompetitionInfo> &SeasonCalendar::getCompetitionsReference()
{
    return competitions;
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo> &newCompetitions)
{
    competitions = newCompetitions;
}
