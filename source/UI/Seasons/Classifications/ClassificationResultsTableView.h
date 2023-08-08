#ifndef CLASSIFICATIONRESULTSTABLEVIEW_H
#define CLASSIFICATIONRESULTSTABLEVIEW_H

#include <QWidget>
#include "ClassificationResultsTableModel.h"
#include "../../../seasons/Classification.h"

namespace Ui {
class ClassificationResultsTableView;
}

class ClassificationResultsTableView : public QWidget
{
    Q_OBJECT

public:
    explicit ClassificationResultsTableView(bool allowShowDetails, Classification *classification, QWidget *parent = nullptr);
    ~ClassificationResultsTableView();

    void fillTable();

private:
    Ui::ClassificationResultsTableView *ui;

    ClassificationResultsTableModel * tableModel;
    Classification * classification;
    bool allowShowDetails;
public:
    Classification *getClassification() const;
    void setClassification(Classification *newClassification);
    bool getAllowShowDetails() const;
    void setAllowShowDetails(bool newAllowShowDetails);
};

#endif // CLASSIFICATIONRESULTSTABLEVIEW_H
