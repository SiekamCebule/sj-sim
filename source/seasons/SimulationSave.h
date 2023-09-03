#ifndef SIMULATIONSAVE_H
#define SIMULATIONSAVE_H

#include <QVector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "Season.h"
#include "../utilities/ClassWithID.h"
#include "../global/DatabaseObjectsManager.h"
#include "../form-generator/JumperFormTendence.h"

class SimulationSave : public ClassWithID
{
public:
    SimulationSave();
    ~SimulationSave();

    static SimulationSave *getFromJson(QJsonObject obj, DatabaseObjectsManager *objectsManager);
    static QJsonObject getJsonObject(SimulationSave & save);

    bool saveToFile(QString dir = "");
    static SimulationSave *loadFromFile(QString fileName);
    void updateNextCompetitionIndex();
    void repairDatabase();
    void fixJumpersFormTendences();
    JumperFormTendence * getJumperTendence(Jumper * jumper);

private:
    QString name;
    QVector<Season> seasons;
    Season * actualSeason;
    QVector<Jumper *> jumpers;
    QVector<Hill *> hills;
    QVector<CompetitionRules> competitionRules;
    QVector<JumperFormTendence> jumpersFormTendences;

    CompetitionInfo * nextCompetition;
    int nextCompetitionIndex;

    //SETTINGS
    bool showForm;
    bool saveFileSizeReduce;

public:
    QString getName() const;
    void setName(const QString &newName);
    QVector<Season> getSeasons() const;
    QVector<Season>& getSeasonsReference();
    void setSeasons(const QVector<Season> &newSeasons);
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper *> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    QVector<Hill *> & getHillsReference();
    void setHills(const QVector<Hill *> &newHills);
    QVector<Hill *> getHills() const;
    QVector<CompetitionRules> & getCompetitionRulesReference();
    void setCompetitionRules(const QVector<CompetitionRules> &newCompetitionRules);
    Season *getActualSeason() const;
    void setActualSeason(Season *newActualSeason);
    int getNextCompetitionIndex() const;
    void setNextCompetitionIndex(int newNextCompetitionIndex);
    CompetitionInfo *getNextCompetition() const;
    void setNextCompetition(CompetitionInfo *newNextCompetition);
    QVector<JumperFormTendence> getJumpersFormTendences() const;
    QVector<JumperFormTendence> & getJumpersFormTendencesReference();
    void setJumpersFormTendences(const QVector<JumperFormTendence> &newJumpersFormTendences);
    bool getShowForm() const;
    void setShowForm(bool newShowForm);
    bool getSaveFileSizeReduce() const;
    void setSaveFileSizeReduce(bool newSaveFileSizeReduce);
};

#endif // SIMULATIONSAVE_H

//Co muszę zrobić?
//Znaleźć sposób na wczytanie pewnych rzeczy np. CompetitionInfo tak żeby następne wczytane CompetitionInfo mogły mieć ustawione treningi które zostały wczytane tuż przed aktualnym CompetitionInfo
//Rozwiązanie:
//1. Zrobić w CompetitionInfo funkcję zwracającą sam obiekt a nie vector
//2. Zrobić w klasie Calendar funkcję loadCalendar na podstawie QJsonObject-a która po każdym wczytanym competition daje go do seasonObjectsManagera
