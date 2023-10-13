#ifndef SEASONCALENDAR_H
#define SEASONCALENDAR_H

#include <QVector>
#include <QSharedPointer>
#include <QJsonObject>
#include "../competitions/CompetitionInfo.h"
#include "../utilities/ClassWithID.h"

class SeasonCalendar : public ClassWithID
{
public:
    SeasonCalendar(QString name = "");
    ~SeasonCalendar();

    void fixCompetitionsClassifications();
    void fixCompetitionsHills(QVector<Hill *> *hillsList, Hill *defaultHill);
    void fixAdvancementCompetitions();
    void fixAdvancementClassifications();
    void updateCompetitionsQualifyingCompetitions();

    static SeasonCalendar getFromJson(QJsonObject json, DatabaseObjectsManager *objectsManager);
    static QJsonObject getJsonObject(SeasonCalendar & calendar);
    static int getCompetitionMainIndex(QVector<CompetitionInfo *> & competitions, CompetitionInfo * competition);
    static CompetitionInfo * getMainCompetitionByIndex(QVector<CompetitionInfo *> & competitions, int index);
    static CompetitionInfo * getCompetitionByIndexAndType(QVector<CompetitionInfo *> & competitions, int index, int type);
    static CompetitionInfo * getCompetitionAfterCompetitionFilteredByType(QVector<CompetitionInfo *> & competitions, int actualIndex, int type, int howMany);

    bool getAllPlayed();

private:
    QVector<CompetitionInfo *> competitions;
    QVector<Classification *> classifications;
    QString name;

public:
    QVector<CompetitionInfo *> getCompetitions() const;
    QVector<CompetitionInfo *>  & getCompetitionsReference();
    void setCompetitions(const QVector<CompetitionInfo *> &newCompetitions);
    QVector<Classification *> getClassifications() const;
    QVector<Classification *> &getClassificationsReference();
    void setClassifications(const QVector<Classification *> &newClassifications);
    QString getName() const;
    void setName(const QString &newName);
};

#endif // SEASONCALENDAR_H
