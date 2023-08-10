#ifndef COMPETITIONINFO_H
#define COMPETITIONINFO_H

class Hill;
class SeasonCalendar;
#include "CompetitionRules.h"
#include "CompetitionResults.h"
#include "CompetitionInfo.h"
#include "../utilities/ClassWithID.h"
#include "../seasons/Classification.h"
#include "../seasons/SeasonCalendar.h"

#include <QDate>

class CompetitionInfo : public ClassWithID
{
public:
    CompetitionInfo(Hill * hill = nullptr);
    ~CompetitionInfo();

    enum SerieType{
        Competition,
        Qualifications,
        TrialRound,
        Training
    };

    void updateQualifyingCompetitions(SeasonCalendar * calendar);
    bool saveToFile(QString dir, QString name);

    static QVector<CompetitionInfo *> getSpecificTypeCompetitions(QVector<CompetitionInfo *> competitions, int type);

private:
    Hill * hill;
    CompetitionRules rules;
    CompetitionResults results;
    short serieType;
    int exceptionalRoundsCount;

    bool cancelled;
    bool played;

    CompetitionInfo * trialRound;
    QVector<CompetitionInfo *> trainings;
    QVector<Classification *> classifications;

    Classification * advancementClassification;
    CompetitionInfo * advancementCompetition; //Tylko jedno z tych 2 może być "aktywne"
    QVector<CompetitionInfo *> qualifyingCompetitions; //Konkursy dla których ten konkurs jest kwalifikacjami

    QVector<Team> teams;

public:
    static QJsonObject getJsonObject(CompetitionInfo &competition);
    static CompetitionInfo getFromJson(const QJsonObject & json);

public:
    Hill *getHill() const;
    void setHill(Hill *newHill);
    CompetitionRules getRules() const;
    CompetitionRules * getRulesPointer() {return &rules;}
    void setRules(const CompetitionRules &newRules);
    short getSerieType() const;
    void setSerieType(short newSerieType);
    int getExceptionalRoundsCount() const;
    void setExceptionalRoundsCount(int newExceptionalRoundsCount);
    bool getCancelled() const;
    void setCancelled(bool newCancelled);
    CompetitionInfo *getTrialRound() const;
    void setTrialRound(CompetitionInfo *newTrialRound);
    QVector<CompetitionInfo *> getTrainings() const;
    QVector<CompetitionInfo *> & getTrainingsReference();
    void setTrainings(const QVector<CompetitionInfo *> &newTrainings);
    QVector<Classification *> getClassifications() const;
    QVector<Classification *> & getClassificationsReference();
    void setClassifications(const QVector<Classification *> &newClassifications);
    CompetitionResults getResults() const;
    void setResults(const CompetitionResults &newResults);
    CompetitionResults & getResultsReference();
    Classification *getAdvancementClassification() const;
    void setAdvancementClassification(Classification *newAdvancementClassification);
    CompetitionInfo *getAdvancementCompetition() const;
    void setAdvancementCompetition(CompetitionInfo *newAdvancementCompetition);
    QVector<CompetitionInfo *> getQualifyingCompetitions() const;
    QVector<CompetitionInfo *> & getQualifyingCompetitionsReference();
    void setQualifyingCompetitions(const QVector<CompetitionInfo *> &newQualifyingCompetitions);
    bool getPlayed() const;
    void setPlayed(bool newPlayed);
    QVector<Team> getTeams() const;
    QVector<Team> & getTeamsReference();
    void setTeams(const QVector<Team> &newTeams);
};

#endif // COMPETITIONINFO_H
