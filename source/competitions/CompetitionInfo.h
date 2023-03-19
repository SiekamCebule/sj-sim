#ifndef COMPETITIONINFO_H
#define COMPETITIONINFO_H

class Hill;
class CompetitionResults;
class CompetitionRules;

#include <QDate>

class CompetitionInfo
{
public:
    CompetitionInfo();

    QDate getDate() const;
    void setDate(const QDate &newDate);
    CompetitionRules *getRules() const;
    void setRules(CompetitionRules *newRules);
    Hill *getHill() const;
    void setHill(Hill *newHill);

private:
    Hill * hill;
    //CompetitionResults * results;
    CompetitionRules * rules;
    QDate date;
};

#endif // COMPETITIONINFO_H
