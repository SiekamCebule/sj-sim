#include "DatabaseEditorWindow.h"
#include "ui_DatabaseEditorWindow.h"

#include "../../global/GlobalDatabase.h"

#include "DatabaseListItemWidget.h"

#include <QFont>
#include <QScrollArea>

DatabaseEditorWindow::DatabaseEditorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseEditorWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(GlobalDatabase::get()->getGlobalJumpers().size() > 0)
        fillJumpersWidget();
}

DatabaseEditorWindow::~DatabaseEditorWindow()
{
    delete ui;
}

void DatabaseEditorWindow::when_ItemWidgetDeleteButtonClicked(int index)
{
    GlobalDatabase::get()->removeJumper(index);
    fillJumpersWidget();
}

void DatabaseEditorWindow::fillJumpersWidget()
{
    if(ui->verticalLayout_jumpers != NULL){
        QLayoutItem * item;
        while((item = ui->verticalLayout_jumpers->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    int i=0;
    for(const auto & jumper : GlobalDatabase::get()->getGlobalJumpers())
    {
        DatabaseListItemWidget * itemWidget = new DatabaseListItemWidget;
        itemWidget->setIndex(i);
        itemWidget->setEditorParent(this);

        QLabel * label = new QLabel(QString::number(itemWidget->getIndex()));
        QFont font = label->font();
        font.setFamily("Ubuntu Light");
        font.setPointSize(13);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #000000; margin-right: 11px;}");
        itemWidget->addLabel(label, 0);

        label = new QLabel(jumper.getNameAndSurname());
        font.setFamily("Ubuntu Light");
        font.setPointSize(10);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #452020; margin-right: 5px;}");
        itemWidget->addLabel(label, 1);

        label = new QLabel(jumper.getCountryCode());
        itemWidget->addLabel(label, 2);

        ui->verticalLayout_jumpers->addWidget(itemWidget);
        itemWidget->update();
        i++;
    }
    ui->verticalLayout_jumpers->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Ignored, QSizePolicy::Expanding));
}
