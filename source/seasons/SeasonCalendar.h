#ifndef SEASONCALENDAR_H
#define SEASONCALENDAR_H

#include <QVector>
#include <QSharedPointer>
#include "../competitions/CompetitionInfo.h"

class SeasonCalendar
{
public:
    SeasonCalendar();
    ~SeasonCalendar();

    void fixCompetiitonsClassifications(QVector<Classification> *classificationsList);

private:
    QVector<CompetitionInfo *> competitions;

public:
    QVector<CompetitionInfo *> getCompetitions() const;
    QVector<CompetitionInfo *>  & getCompetitionsReference();
    void setCompetitions(const QVector<CompetitionInfo *> &newCompetitions);
};

#endif // SEASONCALENDAR_H
