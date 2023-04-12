#ifndef TEAMCLASSIFICATION_H
#define TEAMCLASSIFICATION_H

#include <QVector>
#include "AbstractClassification.h"
#include "single-results/TeamClassificationSingleResult.h"

class TeamClassification : public AbstractClassification
{
public:
    TeamClassification();

private:
    QVector<TeamClassificationSingleResult *> teamsResults;

public:
    QVector<TeamClassificationSingleResult *> getTeamsResults() const;
    QVector<TeamClassificationSingleResult *> & getEditableTeamsResults();
    void setTeamsResults(const QVector<TeamClassificationSingleResult *> &newTeamsResults);
};

#endif // TEAMCLASSIFICATION_H
