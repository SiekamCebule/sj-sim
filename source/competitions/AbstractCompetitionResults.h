#ifndef ABSTRACTCOMPETITIONRESULTS_H
#define ABSTRACTCOMPETITIONRESULTS_H
#include "AbstractCompetitionSingleResult.h"
#include <QVector>

class CompetitionInfo;

class AbstractCompetitionResults
{
public:
    AbstractCompetitionResults();
    virtual ~AbstractCompetitionResults() = default;

protected:
    CompetitionInfo * competitionInfo;

public:
    CompetitionInfo *getCompetitionInfo() const;
    void setCompetitionInfo(CompetitionInfo *newCompetitionInfo);
};

#endif // ABSTRACTCOMPETITIONRESULTS_H
