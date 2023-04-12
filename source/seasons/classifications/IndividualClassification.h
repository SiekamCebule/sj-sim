#ifndef INDIVIDUALCLASSIFICATION_H
#define INDIVIDUALCLASSIFICATION_H

#include <QVector>
#include "AbstractClassification.h"
#include "single-results/IndividualClassificationSingleResult.h"

class IndividualClassification
{
public:
    IndividualClassification();

private:
    QVector<IndividualClassificationSingleResult *> jumpersResults;

public:
    QVector<IndividualClassificationSingleResult *> getJumpersResults() const;
    QVector<IndividualClassificationSingleResult *> & getEditableJumpersResults();
    void setJumpersResults(const QVector<IndividualClassificationSingleResult *> &newJumpersResults);
};

#endif // INDIVIDUALCLASSIFICATION_H
