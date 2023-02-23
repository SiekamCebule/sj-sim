#include "DatabaseEditorWindow.h"
#include "ui_DatabaseEditorWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../EditorWidgets/JumperEditorWidget.h"

#include "DatabaseListItemWidget.h"

#include <QFont>
#include <QScrollArea>
#include <QCloseEvent>
#include <QMessageBox>

DatabaseEditorWindow::DatabaseEditorWindow(JumperEditorWidget * jumperEditor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseEditorWindow),
    jumperEditor(jumperEditor),
    actualElementType(0),
    selectedItemIndex(-1)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    if(GlobalDatabase::get()->getGlobalJumpers().size() > 0)
        fillJumpersWidget();

    if(this->jumperEditor == nullptr)
        this->jumperEditor = new JumperEditorWidget();

    ui->tab_jumpers->layout()->addWidget(this->jumperEditor);

    connect(ui->tabWidget_main, &QTabWidget::currentChanged, this, &DatabaseEditorWindow::setActualElementType);
    connect(this->jumperEditor, &JumperEditorWidget::submitted, this, &DatabaseEditorWindow::replaceJumperFromJumperEdit);

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

void DatabaseEditorWindow::updateItemsSelection(int index)
{
    for(auto & item : listItems)
    {
        if(item->getIndex() != index)
        {
            item->setIsSelected(false);
            item->setStyleSheet("QLabel{}");
        }
        else
        {
            item->setIsSelected(true);
            this->setSelectedItemIndex(item->getIndex());
            item->setStyleSheet("QLabel#main-label{border-radius: 8px; border: 1px solid rgb(20, 59, 23);background-color: rgb(248, 255, 250);padding: 2px;}");
        }
    }

    jumperEditor->setJumper(const_cast<Jumper *>(&GlobalDatabase::get()->getGlobalJumpers().at(index-1)));
    jumperEditor->fillJumperInfo();
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
    listItems.clear();
    int i=1;
    for(const auto & jumper : GlobalDatabase::get()->getGlobalJumpers())
    {
        DatabaseListItemWidget * itemWidget = new DatabaseListItemWidget;
        itemWidget->setIndex(i);
        itemWidget->setEditorParent(this);

        QLabel * label = new QLabel(QString::number(itemWidget->getIndex()));
        QFont font = label->font();
        font.setFamily("Ubuntu Light");
        font.setPointSize(17);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #000000; margin-right: 11px;}");
        itemWidget->addLabel(label, 0);

        label = new QLabel(jumper.getNameAndSurname());
        label->setObjectName("main-label");
        font.setFamily("Ubuntu Light");
        font.setPointSize(15);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #452020; margin-right: 5px;}");
        itemWidget->addLabel(label, 1);

        label = new QLabel(jumper.getCountryCode());
        itemWidget->addLabel(label, 2);

        ui->verticalLayout_jumpers->addWidget(itemWidget);
        itemWidget->update();
        listItems.push_back(itemWidget);

        connect(itemWidget, &DatabaseListItemWidget::itemSelected, this, &DatabaseEditorWindow::updateItemsSelection);
        i++;
    }
    ui->verticalLayout_jumpers->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Ignored, QSizePolicy::Expanding));
}

void DatabaseEditorWindow::updateIndexes()
{
    int index = 0;
    for (auto & jum : GlobalDatabase::get()->getEditableGlobalJumpers())
    {
        dynamic_cast<DatabaseListItemWidget *>(ui->verticalLayout_jumpers->itemAt(index)->widget())->setIndex(index+1);
        index++;
    }
}

int DatabaseEditorWindow::getSelectedItemIndex() const
{
    return selectedItemIndex;
}

void DatabaseEditorWindow::setSelectedItemIndex(int newSelectedItemIndex)
{
    selectedItemIndex = newSelectedItemIndex;
}

void DatabaseEditorWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox message;
    message.setStyleSheet("background-color: white; color: black;");
    QMessageBox::StandardButton button;
    button = message.question(this, "Wyjście z edytora bazy danych", "Czy zapisać zmiany w bazie danych?", QMessageBox::StandardButton::No | QMessageBox::StandardButton::Cancel | QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::Yes);
    if(button == QMessageBox::Yes)
    {
        GlobalDatabase::get()->writeToJson();
        event->accept();
    }
    else if(button == QMessageBox::Cancel)
    {
        event->ignore();
    }
    else if(button == QMessageBox::No)
    {
        event->accept();
    }
    else
        qDebug()<<"invalid button in database editor close event";
}

short DatabaseEditorWindow::getActualElementType() const
{
    return actualElementType;
}

void DatabaseEditorWindow::setActualElementType(short newActualElementType)
{
    actualElementType = newActualElementType;
}

void DatabaseEditorWindow::on_pushButton_add_clicked()
{
    int index = 0;
    if(GlobalDatabase::get()->getGlobalJumpers().size() > 0 && selectedItemIndex > (-1))
        index = selectedItemIndex - 1;
    else index = -1;

    GlobalDatabase::get()->getEditableGlobalJumpers().insert(index + 1, Jumper("Name", "Surname", "XXX", JumperSkills()));
    fillJumpersWidget();
    updateIndexes();
    updateItemsSelection(index+2);
}


void DatabaseEditorWindow::on_pushButton_remove_clicked()
{
    if(GlobalDatabase::get()->getGlobalJumpers().size() > 0 && selectedItemIndex > (-1))
    {
        int index = selectedItemIndex - 1;
        GlobalDatabase::get()->getEditableGlobalJumpers().remove(index, 1);
        fillJumpersWidget();
        updateIndexes();

        if(GlobalDatabase::get()->getGlobalJumpers().size() > 0)
        {
            if(index == GlobalDatabase::get()->getGlobalJumpers().size())
                updateItemsSelection(index);
            else updateItemsSelection(index + 1);
        }
    }
    else qDebug()<<"error";
}

void DatabaseEditorWindow::replaceJumperFromJumperEdit()
{
    GlobalDatabase::get()->getEditableGlobalJumpers().replace(selectedItemIndex - 1, jumperEditor->getJumperFromWidget());
    fillJumpersWidget();
}

//zawsze jak zostaje 1 zawodnik, i się go usuwa, wtedy program sie crashuje.


void DatabaseEditorWindow::on_pushButton_up_clicked()
{
    int index = selectedItemIndex - 1;
    if((!(index < 1)) && selectedItemIndex > (-1))
    {
        GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(index, index - 1);
        fillJumpersWidget();
        updateIndexes();
        updateItemsSelection(index);
    }
}


void DatabaseEditorWindow::on_pushButton_down_clicked()
{
    int index = selectedItemIndex - 1;
    if((!(index + 1 >= GlobalDatabase::get()->getGlobalJumpers().size())) && selectedItemIndex > (-1))
    {
        GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(index, index + 1);
        fillJumpersWidget();
        updateIndexes();
        updateItemsSelection(index + 2);
    }
}

