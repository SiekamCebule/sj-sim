#include "CompetitionStartListDisplayWidget.h"
#include "ui_CompetitionStartListDisplayWidget.h"

#include <QInputDialog>
#include <QEventLoop>
#include <QDialog>
#include <QLayout>
#include "../../../utilities/functions.h"
#include "../../EditorWidgets/JumperEditorWidget.h"

CompetitionStartListDisplayWidget::CompetitionStartListDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompetitionStartListDisplayWidget)
{
    ui->setupUi(this);
}

CompetitionStartListDisplayWidget::~CompetitionStartListDisplayWidget()
{
    delete ui;
}
void CompetitionStartListDisplayWidget::fillItemsLayout()
{
    if(ui->verticalLayout_items != nullptr)
    {
        QLayoutItem * item;
        while((item = ui->verticalLayout_items->takeAt(0)) != nullptr)
        {
            if(QString(item->widget()->metaObject()->className()) == QString("CompetitionStartListDisplayItemWidget")){
                disconnect(dynamic_cast<CompetitionStartListDisplayItemWidget *>(item->widget()), &CompetitionStartListDisplayItemWidget::doubleClicked, this,
                           &CompetitionStartListDisplayWidget::askForNewIndex);
            }
            //delete item->widget();
            item->widget()->hide();
            delete item;
        }
    }
    items.clear();

    int index = 1;
    for(auto & jumper : *jumpers)
    {
        CompetitionStartListDisplayItemWidget * item = new CompetitionStartListDisplayItemWidget(Jumpers, this);
        item->setJumper(&jumper);
        item->setIndexInParentList(index);
        item->setIsActive(competitiorsActivity.at(index - 1));
        item->fillWidget();

        connect(item, &CompetitionStartListDisplayItemWidget::doubleClicked, this, &CompetitionStartListDisplayWidget::askForNewIndex);
        connect(item, &CompetitionStartListDisplayItemWidget::activityChanged, this, [this, item](){
            competitiorsActivity.replace(item->getIndexInParentList() - 1, item->getIsActive());
            if(item->getIsActive() == false)
            {
                int indexOfItem = item->getIndexInParentList() - 1;
                switch(getCompetitiorsType())
                {
                case Jumpers:
                    moveItemToVectorBack(*jumpers, indexOfItem);
                }
                moveItemToVectorBack(competitiorsActivity, indexOfItem);
                fillItemsLayout();
            }
        });
        connect(item, &CompetitionStartListDisplayItemWidget::selected, this, [this, item](){
            selectedItem = item->getIndexInParentList() - 1;
            updateItemsSelection();
        });

        items.push_back(item);
        ui->verticalLayout_items->addWidget(item);
        //dynamic_cast<CompetitionStartListDisplayItemWidget *>(item)->uninstallEventFilterForThis();

        index++;
    }
    //ui->verticalLayout_items->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void CompetitionStartListDisplayWidget::setJumpers(QVector<Jumper> *newJumpers)
{
    jumpers = newJumpers;
}

void CompetitionStartListDisplayWidget::fillCompetitiorsActivity()
{
    competitiorsActivity.fill(true, jumpers->size());
}

void CompetitionStartListDisplayWidget::askForNewIndex()
{
    int countOfCompetitiors = 0;
    switch(getCompetitiorsType())
    {
    case Jumpers:
        countOfCompetitiors = jumpers->count();
        break;
    case Teams:
        countOfCompetitiors = 0;
        break;
    }

    QInputDialog dialog;
    dialog.setWindowTitle(tr("Zmiana miejsca na liście startowej"));
    dialog.setLabelText(tr("Podaj nowy numer startowy uczestnika na liście startowej"));
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setIntMaximum(countOfCompetitiors);
    dialog.setModal(true);
    dialog.show();
    int val = dialog.exec();
    if(val == QDialog::Accepted)
    {
        int newIndex = dialog.intValue();
        if(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() != newIndex){
            switch(getCompetitiorsType())
            {
            case Jumpers:
                jumpers->swapItemsAt(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() - 1, newIndex - 1);
                break;
            case Teams:
                break;
            }
            competitiorsActivity.swapItemsAt(dynamic_cast<CompetitionStartListDisplayItemWidget *>(sender())->getIndexInParentList() - 1, newIndex - 1);
            fillItemsLayout();
        }
    }
}

void CompetitionStartListDisplayWidget::on_pushButton_up_clicked()
{
    if((!(selectedItem < 1)) && selectedItem > (-1))
    {
        switch(getCompetitiorsType())
        {
        case Jumpers:
            jumpers->swapItemsAt(selectedItem, selectedItem - 1);
            break;
        case Teams:
            break;
        }
        competitiorsActivity.swapItemsAt(selectedItem, selectedItem - 1);
        fillItemsLayout();
        selectedItem = selectedItem - 1;
        emit items.at(selectedItem)->selected();
    }
}

void CompetitionStartListDisplayWidget::updateItemsSelection()
{
    int index = 0;
    for(auto & item : items)
    {
        if((item->getIndexInParentList() - 1) != selectedItem)
        {
            item->setIsSelected(false);
        }
        else item->setIsSelected(true);
        index++;
    }
}
void CompetitionStartListDisplayWidget::on_pushButton_down_clicked()
{
    if((!(selectedItem + 1 >= jumpers->size())) && selectedItem > (-1))
    {
        switch(getCompetitiorsType())
        {
        case Jumpers:
            jumpers->swapItemsAt(selectedItem, selectedItem + 1);
            break;
        case Teams:
            break;
        }
        competitiorsActivity.swapItemsAt(selectedItem, selectedItem + 1);
        fillItemsLayout();
        selectedItem = selectedItem + 1;
        emit items.at(selectedItem)->selected();
    }
}

short CompetitionStartListDisplayWidget::getCompetitiorsType() const
{
    return competitiorsType;
}

void CompetitionStartListDisplayWidget::setCompetitiorsType(short newCompetitiorsType)
{
    competitiorsType = newCompetitiorsType;
}

void CompetitionStartListDisplayWidget::on_pushButton_edit_clicked()
{
    if(competitiorsActivity.at(selectedItem) == true){
        QDialog * dialog = new QDialog;
        dialog->setWindowFlags(Qt::Window);
        dialog->setWindowTitle("Edytuj uczestnika na liście startowej");
        dialog->setStyleSheet("background-color: rgb(210, 210, 210);");
        dialog->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        dialog->setFixedSize(dialog->size());
        dialog->setLayout(new QVBoxLayout(dialog));

        switch(getCompetitiorsType())
        {
        case Jumpers:
            JumperEditorWidget * editor = new JumperEditorWidget(const_cast<Jumper*>(&jumpers->at(selectedItem)));
            editor->fillJumperInputs();
            dialog->layout()->addWidget(editor);

            connect(editor, &JumperEditorWidget::submitted, dialog, &QDialog::accept);
            if(dialog->exec() == QDialog::Accepted)
            {
                jumpers->replace(selectedItem, editor->getJumperFromWidgetInput());
                fillItemsLayout();
            }
            delete editor;
            break;
        }
    }
}

