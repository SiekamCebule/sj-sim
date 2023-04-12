#ifndef INDIVIDUALCLASSIFICATIONSINGLERESULT_H
#define INDIVIDUALCLASSIFICATIONSINGLERESULT_H

#include <QVector>
#include "../../../competitions/IndividualCompetitions/IndividualCompetitionSingleResult.h"
#include "AbstractClassificationSingleResult.h"


class IndividualClassificationSingleResult : public AbstractClassificationSingleResult
{
public:
    IndividualClassificationSingleResult();

private:
    QVector<IndividualCompetitionSingleResult *> competitionsResults;

public:
    QVector<IndividualCompetitionSingleResult *> getCompetitionsResults() const;
    void setCompetitionsResults(const QVector<IndividualCompetitionSingleResult *> &newCompetitionsResults);
};

#endif // INDIVIDUALCLASSIFICATIONSINGLERESULT_H
