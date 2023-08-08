#include "ClassificationResultsTableView.h"
#include "ui_ClassificationResultsTableView.h"

ClassificationResultsTableView::ClassificationResultsTableView(bool allowShowDetails, Classification * classification, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassificationResultsTableView),
    allowShowDetails(allowShowDetails),
    classification(classification)
{
    ui->setupUi(this);

    tableModel = new ClassificationResultsTableModel(nullptr, this);

    connect(ui->tableView, &QTableView::doubleClicked, this, [this](const QModelIndex & index){
        if(this->allowShowDetails == true)
        {
           // Pokaż szczegóły wyniku w klasyfikacji po double click-u.
        }
    });
}

ClassificationResultsTableView::~ClassificationResultsTableView()
{
    delete ui;
}

void ClassificationResultsTableView::fillTable()
{
    tableModel->setClassification(classification);
    qDebug()<<tableModel->getClassification()->getResultsReference().count();
    ui->tableView->setModel(tableModel);
    ui->tableView->resizeColumnsToContents();
}

bool ClassificationResultsTableView::getAllowShowDetails() const
{
    return allowShowDetails;
}

void ClassificationResultsTableView::setAllowShowDetails(bool newAllowShowDetails)
{
    allowShowDetails = newAllowShowDetails;
}

Classification *ClassificationResultsTableView::getClassification() const
{
    return classification;
}

void ClassificationResultsTableView::setClassification(Classification *newClassification)
{
    classification = newClassification;
}
