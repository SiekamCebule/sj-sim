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

    enum SerieType{
        Competition,
        Qualifications,
        TrialRound,
        Training
    };

private:
    Hill * hill;
    CompetitionRules rules;
    AbstractCompetitionResults * results;
    short serieType;
    int exceptionalRoundsCount;

public:
    Hill *getHill() const;
    void setHill(Hill *newHill);
    CompetitionRules getRules() const;
    CompetitionRules * getRulesPointer() {return &rules;}
    void setRules(const CompetitionRules &newRules);
    AbstractCompetitionResults *getResults() const;
    void setResults(AbstractCompetitionResults *newResults);
    short getSerieType() const;
    void setSerieType(short newSerieType);
    int getExceptionalRoundsCount() const;
    void setExceptionalRoundsCount(int newExceptionalRoundsCount);
};

#endif // COMPETITIONINFO_H
