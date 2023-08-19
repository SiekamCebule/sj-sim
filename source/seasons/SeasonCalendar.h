#ifndef SEASONCALENDAR_H
#define SEASONCALENDAR_H

#include <QVector>
#include <QSharedPointer>
#include <QJsonObject>
#include "../competitions/CompetitionInfo.h"

class SeasonCalendar
{
public:
    SeasonCalendar();
    ~SeasonCalendar();

    void fixCompetitionsClassifications();
    void fixCompetitionsHills(QVector<Hill *> *hillsList);
    void fixAdvancementCompetitions();
    void fixAdvancementClassifications();
    void updateCompetitionsQualifyingCompetitions();

    static SeasonCalendar getFromJson(QJsonObject json, SeasonDatabaseObjectsManager *objectsManager);
    static QJsonObject getJsonObject(SeasonCalendar & calendar);
    static int getCompetitionMainIndex(QVector<CompetitionInfo *> & competitions, CompetitionInfo * competition);
    static CompetitionInfo * getMainCompetitionByIndex(QVector<CompetitionInfo *> & competitions, int index);

private:
    QVector<CompetitionInfo *> competitions;
    QVector<Classification *> classifications;

public:
    QVector<CompetitionInfo *> getCompetitions() const;
    QVector<CompetitionInfo *>  & getCompetitionsReference();
    void setCompetitions(const QVector<CompetitionInfo *> &newCompetitions);
    QVector<Classification *> getClassifications() const;
    QVector<Classification *> &getClassificationsReference();
    void setClassifications(const QVector<Classification *> &newClassifications);
};

#endif // SEASONCALENDAR_H
