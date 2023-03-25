#ifndef COMPETITIONINFO_H
#define COMPETITIONINFO_H

class Hill;
#include "CompetitionRules.h"
#include "AbstractCompetitionResults.h"
#include "CompetitionInfo.h"

#include <QDate>

class CompetitionInfo
{
public:
    CompetitionInfo();
    ~CompetitionInfo();

private:
    Hill * hill;
    CompetitionRules rules;
    AbstractCompetitionResults * results;
    QDate date;

public:
    QDate getDate() const;
    void setDate(const QDate &newDate);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    CompetitionRules getRules() const;
    CompetitionRules * getRulesPointer() {return &rules;}
    void setRules(const CompetitionRules &newRules);
    AbstractCompetitionResults *getResults() const;
    void setResults(AbstractCompetitionResults *newResults);
};

#endif // COMPETITIONINFO_H
