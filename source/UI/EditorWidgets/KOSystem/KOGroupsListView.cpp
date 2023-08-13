#include "KOGroupsListView.h"
#include "ui_KOGroupsListView.h"

#include "../../../utilities/functions.h"

KOGroupsListView::KOGroupsListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KOGroupsListView)
{
    ui->setupUi(this);
}

KOGroupsListView::~KOGroupsListView()
{
    delete ui;
}

void KOGroupsListView::fillListLayout()
{
    for(auto & widget : groupWidgets)
    {
        MyFunctions::removeFromVector(groupWidgets, widget);
        ui->verticalLayout_groupsWidgets->removeWidget(widget);
        delete widget;
    }
    qDebug()<<"KOGROUPS COUNT: "<<KOGroups->count();
    for(auto & group : *KOGroups)
    {
        KOGroupInfoWidget * widget = new KOGroupInfoWidget(&group, this);
        widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        widget->updateWidget();
        ui->verticalLayout_groupsWidgets->addWidget(widget);
        groupWidgets.push_back(widget);
    }
}

QVector<KOGroup> *KOGroupsListView::getKOGroups() const
{
    return KOGroups;
}

void KOGroupsListView::setKOGroups(QVector<KOGroup> *newKOGroups)
{
    KOGroups = newKOGroups;
}
