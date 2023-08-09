#include "ClassificationEditorWidget.h"
#include "ui_ClassificationEditorWidget.h"

#include "../../competitions/CompetitionRules.h"
#include "../../global/GlobalDatabase.h"

ClassificationEditorWidget::ClassificationEditorWidget(Classification * classification, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassificationEditorWidget)
{
    ui->setupUi(this);

    if(classification != nullptr)
        pointsForPlaces = this->classification->getPointsForPlacesReference();

    tableModel = new PointsForPlacesTableModel(this);
    tableModel->setPointsForPlaces(&pointsForPlaces);
    ui->tableView_pointsForPlaces->setModel(tableModel);
    ui->tableView_pointsForPlaces->setSelectionMode(QAbstractItemView::SingleSelection);

    action_add = new QAction(this);
    action_add->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_A);
    this->addAction(action_add);
    connect(action_add, &QAction::triggered, this, &ClassificationEditorWidget::addActionTriggered);

    action_remove = new QAction(this);
    action_remove->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_D);
    this->addAction(action_remove);
    connect(action_remove, &QAction::triggered, this, &ClassificationEditorWidget::removeActionTriggered);

    connect(ui->comboBox_points, &QComboBox::currentIndexChanged, this, [this](){
        if(ui->comboBox_points->currentIndex() == Classification::PointsForPlaces)
        {
            ui->tableView_pointsForPlaces->show();
            ui->label_shortcuts->show();
        }
        else if(ui->comboBox_points->currentIndex() == Classification::CompetitionPoints)
        {
            ui->tableView_pointsForPlaces->hide();
            ui->label_shortcuts->hide();
        }
    });

    int i=1;
    ui->comboBox_presets->addItem("BRAK");
    for(auto & preset : GlobalDatabase::get()->getEditableGlobalPointsForPlacesPresets()){
        ui->comboBox_presets->addItem(QString::number(i) + ". " + preset.getName());
        i++;
    }
    connect(ui->comboBox_presets, &QComboBox::activated, this, [this](){
        if(ui->comboBox_presets->currentIndex() == 0)
        {
            tableModel->removeRows(0, tableModel->rowCount());
            pointsForPlaces.clear();
        }
        else
        {
            tableModel->removeRows(0, tableModel->rowCount());
            pointsForPlaces = GlobalDatabase::get()->getEditableGlobalPointsForPlacesPresets()[ui->comboBox_presets->currentIndex() - 1].getPointsForPlacesReference();
            tableModel->insertRows(0, pointsForPlaces.count());
            emit tableModel->dataChanged(tableModel->index(0, 0), tableModel->index(tableModel->rowCount() - 1, tableModel->columnCount() - 1));
        }
    });
}

ClassificationEditorWidget::~ClassificationEditorWidget()
{
    delete ui;
}

void ClassificationEditorWidget::resetInputs()
{
    ui->lineEdit_name->clear();
    ui->comboBox_type->setCurrentIndex(0);
    ui->comboBox_points->setCurrentIndex(0);
    tableModel->removeRows(0, tableModel->rowCount());
    pointsForPlaces.clear();
}

void ClassificationEditorWidget::fillInputs()
{
    ui->lineEdit_name->setText(classification->getName());
    ui->comboBox_type->setCurrentIndex(classification->getClassificationType());
    ui->comboBox_points->setCurrentIndex(classification->getPunctationType());

    pointsForPlaces = classification->getPointsForPlaces();
    tableModel->insertRows(0, pointsForPlaces.count());
    emit tableModel->dataChanged(tableModel->index(0, 0), tableModel->index(tableModel->rowCount() - 1, tableModel->columnCount() - 1));
}

Classification * ClassificationEditorWidget::getClassificationFromInputs()
{
    Classification * classification = new Classification();
    classification->setName(ui->lineEdit_name->text());
    classification->setClassificationType(ui->comboBox_type->currentIndex());
    classification->setPunctationType(ui->comboBox_points->currentIndex());
    classification->setPointsForPlaces(pointsForPlaces);
    return classification;
}

void ClassificationEditorWidget::on_pushButton_submit_clicked()
{
    emit submitted();
}

void ClassificationEditorWidget::addActionTriggered()
{
    int count = pointsForPlaces.count();
    pointsForPlaces.insert(count + 1, 0);
    tableModel->insertRow(count);
    emit tableModel->dataChanged(tableModel->index(count, 0), tableModel->index(count, 1));
}

void ClassificationEditorWidget::removeActionTriggered()
{
    int count = pointsForPlaces.count();
    pointsForPlaces.remove(count);
    tableModel->removeRow(count - 1);
}

Classification *ClassificationEditorWidget::getClassification() const
{
    return classification;
}

void ClassificationEditorWidget::setClassification(Classification *newClassification)
{
    classification = newClassification;
    pointsForPlaces = classification->getPointsForPlacesReference();
}
