#include "KOGroupsListView.h"
#include "ui_KOGroupsListView.h"

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

QVector<KOGroup> *KOGroupsListView::getKOGroups() const
{
    return KOGroups;
}

void KOGroupsListView::setKOGroups(QVector<KOGroup> *newKOGroups)
{
    KOGroups = newKOGroups;
}
