#ifndef INDIVIDUALCLASSIFICATIONSINGLERESULT_H
#define INDIVIDUALCLASSIFICATIONSINGLERESULT_H

#include <QVector>
#include "../../../competitions/CompetitionResults.h"
#include "AbstractClassificationSingleResult.h"


class IndividualClassificationSingleResult : public AbstractClassificationSingleResult
{
public:
    IndividualClassificationSingleResult();

private:
    QVector<CompetitionSingleResult *> competitionsResults;

public:
    QVector<CompetitionSingleResult *> getCompetitionsResults() const;
    void setCompetitionsResults(const QVector<CompetitionSingleResult *> &newCompetitionsResults);
};

#endif // INDIVIDUALCLASSIFICATIONSINGLERESULT_H
