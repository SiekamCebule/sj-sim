#ifndef SEASONCALENDAR_H
#define SEASONCALENDAR_H

#include <QVector>
#include "../competitions/CompetitionInfo.h"

class SeasonCalendar
{
public:
    SeasonCalendar();

private:
    QVector<CompetitionInfo> competitions;

public:
    QVector<QPair<Hill *, QVector<CompetitionInfo *>>> getCompetitionWeekendsVector();
public:
    QVector<CompetitionInfo> getCompetitions() const;
    QVector<CompetitionInfo> & getCompetitionsReference();
    void setCompetitions(const QVector<CompetitionInfo> &newCompetitions);
};

#endif // SEASONCALENDAR_H
