#ifndef INDIVIDUALCLASSIFICATIONSINGLERESULT_H
#define INDIVIDUALCLASSIFICATIONSINGLERESULT_H

#include <QVector>
#include "../../../competitions/IndividualCompetitions/IndividualCompetitionResult.h"
#include "AbstractClassificationSingleResult.h"


class IndividualClassificationSingleResult : public AbstractClassificationSingleResult
{
public:
    IndividualClassificationSingleResult();

private:
    QVector<IndividualCompetitionResult *> competitionsResults;

public:
    QVector<IndividualCompetitionResult *> getCompetitionsResults() const;
    void setCompetitionsResults(const QVector<IndividualCompetitionResult *> &newCompetitionsResults);
};

#endif // INDIVIDUALCLASSIFICATIONSINGLERESULT_H
