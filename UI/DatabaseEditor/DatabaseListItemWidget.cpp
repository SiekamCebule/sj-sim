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

    QPushButton * buttonEdit = new QPushButton;
    buttonEdit->setFlat(true);
    buttonEdit->setFixedSize(20, 20);
    buttonEdit->setIcon(QIcon(":/img/pencil.png"));
    buttonEdit->setIconSize(QSize(20, 20));
    ui->horizontalLayout->addWidget(buttonEdit);
    QPushButton * buttonDelete = new QPushButton;
    buttonDelete->setFlat(true);
    buttonDelete->setFixedSize(20, 20);
    buttonDelete->setIcon(QIcon(":/img/delete.png"));
    buttonDelete->setIconSize(QSize(20, 20));
    ui->horizontalLayout->addWidget(buttonDelete);

    connect(buttonDelete, &QPushButton::clicked, this, [this](){
        dynamic_cast<DatabaseEditorWindow *>(editorParent)->when_ItemWidgetDeleteButtonClicked(this->getIndex());
    });


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

// PROBLEM: Nie wiem jak podłączyć przycisk usuwania do slotu tak, żeby DatabaseEditorWindow wiedział który widget i którego skoczka z globalnej bazy danych ma usunąć. (W skrócie, potrzebuje indexu usuwanego skoczka.
