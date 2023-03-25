#ifndef INDIVIDUALCOMPETITIONRESULTS_H
#define INDIVIDUALCOMPETITIONRESULTS_H

#include "../AbstractCompetitionResults.h"
#include "IndividualCompetitionSingleResult.h"
#include <QVector>

class IndividualCompetitionResults : public AbstractCompetitionResults
{
public:
    IndividualCompetitionResults();

private:
    QVector<IndividualCompetitionSingleResult> jumpersResults;

public:
    QVector<IndividualCompetitionSingleResult> getJumpersResults() const;
    QVector<IndividualCompetitionSingleResult> &getEditableJumpersResults();
    void setJumpersResults(const QVector<IndividualCompetitionSingleResult> &newJumpersResults);
    IndividualCompetitionSingleResult *getResultsOfJumper(Jumper * jumper);
};

#endif // INDIVIDUALCOMPETITIONRESULTS_H
