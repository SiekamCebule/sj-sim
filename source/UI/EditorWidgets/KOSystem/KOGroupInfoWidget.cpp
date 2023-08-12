#include "KOGroupInfoWidget.h"
#include "ui_KOGroupListView.h"

KOGroupInfoWidget::KOGroupInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KOGroupListView)
{
    ui->setupUi(this);
    connect(ui->listView, &QListView::doubleClicked, this, &KOGroupInfoWidget::listViewDoubleClicked);
}

KOGroupInfoWidget::~KOGroupInfoWidget()
{
    delete ui;
}
