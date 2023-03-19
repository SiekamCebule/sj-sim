#include "DatabaseListItemWidget.h"
#include "ui_DatabaseListItemWidget.h"

#include <QPushButton>
#include "DatabaseEditorWindow.h"
#include <QIcon>

DatabaseListItemWidget::DatabaseListItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseListItemWidget)
{
    ui->setupUi(this);
}

DatabaseListItemWidget::~DatabaseListItemWidget()
{
    delete ui;
}

QVector<QLabel *> DatabaseListItemWidget::getLabels() const
{
    return labels;
}

void DatabaseListItemWidget::addLabel(QLabel *label, int index)
{
    labels.insert(index, label);
}

void DatabaseListItemWidget::update()
{
    for(int i=0; i<labels.size(); i++)
    {
        delete ui->horizontalLayout->itemAt(i);
    }
    for(const auto & label : labels)
    {
        ui->horizontalLayout->addWidget(label);
    }
    ui->horizontalLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Maximum, QSizePolicy::Maximum));
    ui->horizontalLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

// parent zamiast wskazywać na DatabaseEditorWindow, wskazuje na jakiś layout

int DatabaseListItemWidget::getIndex() const
{
    return index;
}

void DatabaseListItemWidget::setIndex(int newIndex)
{
    index = newIndex;
}

DatabaseEditorWindow *DatabaseListItemWidget::getEditorParent() const
{
    return editorParent;
}

void DatabaseListItemWidget::setEditorParent(DatabaseEditorWindow *newEditorParent)
{
    editorParent = newEditorParent;
}

void DatabaseListItemWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit itemSelected(index);
    }
}

bool DatabaseListItemWidget::getIsSelected() const
{
    return isSelected;
}

void DatabaseListItemWidget::setIsSelected(bool newIsSelected)
{
    isSelected = newIsSelected;
    if(isSelected == false)
        setStyleSheet("QWidget{background-color: #ffffff;margin: 0px;}");
}

// PROBLEM: Nie wiem jak podłączyć przycisk usuwania do slotu tak, żeby DatabaseEditorWindow wiedział który widget i którego skoczka z globalnej bazy danych ma usunąć. (W skrócie, potrzebuje indexu usuwanego skoczka.
