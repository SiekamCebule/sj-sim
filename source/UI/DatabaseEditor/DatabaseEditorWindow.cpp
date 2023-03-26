#include "DatabaseEditorWindow.h"
#include "ui_DatabaseEditorWindow.h"

#include "../../global/GlobalDatabase.h"
#include "../../global/CountryFlagsManager.h"
#include "../../simulator/Hill.h"
#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/CompetitionRulesEditorWidget.h"

#include "DatabaseListItemWidget.h"

#include <QFont>
#include <QScrollArea>
#include <QCloseEvent>
#include <QMessageBox>

DatabaseEditorWindow::DatabaseEditorWindow(JumperEditorWidget * jumperEditor, HillEditorWidget *hillEditor, CompetitionRulesEditorWidget * competitionRulesEditor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseEditorWindow),
    jumperEditor(jumperEditor),
    hillEditor(hillEditor),
    competitionRulesEditor(competitionRulesEditor),
    actualElementType(0),
    selectedItemIndex(-1)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    tempGlobalJumpers = GlobalDatabase::get()->getGlobalJumpers();
    tempGlobalHills = GlobalDatabase::get()->getGlobalHills();
    tempGlobalCompetitionRules = GlobalDatabase::get()->getGlobalCompetitionsRules();

    if(GlobalDatabase::get()->getGlobalJumpers().size() > 0)
        fillJumpersWidget();
    if(GlobalDatabase::get()->getGlobalHills().size() > 0)
        fillHillsWidget();
    qDebug()<<GlobalDatabase::get()->getGlobalCompetitionsRules().at(0).getRounds().at(0).getCount()<<" PRZED";
    if(GlobalDatabase::get()->getGlobalCompetitionsRules().size() > 0)
        fillCompetitionRulesWidget();
    qDebug()<<GlobalDatabase::get()->getGlobalCompetitionsRules().at(0).getRounds().at(0).getCount()<<" PO";

    if(this->jumperEditor == nullptr)
        this->jumperEditor = new JumperEditorWidget();
    ui->tab_jumpers->layout()->addWidget(this->jumperEditor);

    if(this->hillEditor == nullptr)
        this->hillEditor = new HillEditorWidget();
    ui->tab_hills->layout()->addWidget(this->hillEditor);

    if(this->competitionRulesEditor == nullptr)
        this->competitionRulesEditor = new CompetitionRulesEditorWidget();
    ui->tab_competitionRules->layout()->addWidget(this->competitionRulesEditor);

    connect(ui->tabWidget_main, &QTabWidget::currentChanged, this, &DatabaseEditorWindow::setActualElementType);
    connect(this->jumperEditor, &JumperEditorWidget::changed, this, &DatabaseEditorWindow::replaceJumperFromJumperEditor);
    connect(this->hillEditor, &HillEditorWidget::changed, this, &DatabaseEditorWindow::replaceHillFromHillEditor);
    connect(this->competitionRulesEditor, &CompetitionRulesEditorWidget::changed, this, &DatabaseEditorWindow::replaceCompetitionRulesFromEditor);
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
    if(actualElementType == ElementType::JumperElement){
        for(auto & item : jumpersListItems)
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
        jumperEditor->fillJumperInputs();
    }
    else if(actualElementType == ElementType::HillElement){
        for(auto & item : hillsListItems)
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
        hillEditor->setHill(const_cast<Hill*>(&GlobalDatabase::get()->getGlobalHills().at(index-1)));
        hillEditor->fillHillInputs();
    }
    else if(actualElementType == ElementType::CompetitionRulesElement){
        for(auto & item : competitionRulesListItems)
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
        competitionRulesEditor->setCompetitionRules(const_cast<CompetitionRules*>(&GlobalDatabase::get()->getGlobalCompetitionsRules().at(index - 1)));
        competitionRulesEditor->fillCompetitionRulesInputs();
    }
}

void DatabaseEditorWindow::fillJumpersWidget()
{
    if(ui->verticalLayout_jumpers != nullptr){
        QLayoutItem * item;
        while((item = ui->verticalLayout_jumpers->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    jumpersListItems.clear();
    int i=1;
    for(const auto & jumper : GlobalDatabase::get()->getGlobalJumpers())
    {
        DatabaseListItemWidget * itemWidget = new DatabaseListItemWidget;
        itemWidget->setParent(this);
        itemWidget->setIndex(i);
        itemWidget->setEditorParent(this);

        QLabel * label = new QLabel(QString::number(itemWidget->getIndex()));
        QFont font = label->font();
        font.setFamily("Ubuntu Light");
        font.setPointSize(15);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #000000; margin-right: 11px;}");
        itemWidget->addLabel(label, 0);

        label = new QLabel(jumper.getNameAndSurname());
        label->setObjectName("main-label");
        font.setFamily("Ubuntu Light");
        font.setPointSize(13);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #452020; margin-right: 5px;}");
        itemWidget->addLabel(label, 1);

        label = new QLabel();
        label->setPixmap(jumper.getFlagPixmap().scaled(CountryFlagsManager::getFlagPixmapSize()));
        itemWidget->addLabel(label, 2);

        ui->verticalLayout_jumpers->addWidget(itemWidget);
        itemWidget->update();
        jumpersListItems.push_back(itemWidget);

        connect(itemWidget, &DatabaseListItemWidget::itemSelected, this, &DatabaseEditorWindow::updateItemsSelection);
        i++;
    }
    ui->verticalLayout_jumpers->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Ignored, QSizePolicy::Expanding));
}

void DatabaseEditorWindow::fillHillsWidget()
{
    if(ui->verticalLayout_hills != nullptr){
        QLayoutItem * item;
        while((item = ui->verticalLayout_hills->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    hillsListItems.clear();
    int i=1;
    for(const auto & hill : GlobalDatabase::get()->getGlobalHills())
    {
        DatabaseListItemWidget * itemWidget = new DatabaseListItemWidget;
        itemWidget->setParent(this);
        itemWidget->setIndex(i);
        itemWidget->setEditorParent(this);

        QLabel * label = new QLabel(QString::number(itemWidget->getIndex()));
        QFont font = label->font();
        font.setFamily("Ubuntu Light");
        font.setPointSize(15);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #000000; margin-right: 11px;}");
        itemWidget->addLabel(label, 0);

        label = new QLabel(hill.getName() + " HS" +QString::number(hill.getHSPoint()));
        label->setObjectName("main-label");
        font.setFamily("Ubuntu Light");
        font.setPointSize(13);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #452020; margin-right: 5px;}");
        itemWidget->addLabel(label, 1);

        label = new QLabel();
        label->setPixmap(hill.getFlagPixmap().scaled(CountryFlagsManager::getFlagPixmapSize()));
        itemWidget->addLabel(label, 2);

        ui->verticalLayout_hills->addWidget(itemWidget);
        itemWidget->update();
        hillsListItems.push_back(itemWidget);

        connect(itemWidget, &DatabaseListItemWidget::itemSelected, this, &DatabaseEditorWindow::updateItemsSelection);
        i++;
    }
    ui->verticalLayout_hills->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Ignored, QSizePolicy::Expanding));
}

void DatabaseEditorWindow::fillCompetitionRulesWidget()
{
    if(ui->verticalLayout_competitionsRules != nullptr)
    {
        QLayoutItem * item;
        while((item = ui->verticalLayout_competitionsRules->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
    }
    competitionRulesListItems.clear();
    int i=1;
    for(const auto & rules : GlobalDatabase::get()->getGlobalCompetitionsRules())
    {
        DatabaseListItemWidget * itemWidget = new DatabaseListItemWidget(this);
        itemWidget->setIndex(i);
        itemWidget->setEditorParent(this);

        QLabel * label = new QLabel(QString::number(itemWidget->getIndex()));
        QFont font = label->font();
        font.setFamily("Ubuntu Light");
        font.setPointSize(15);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #000000; margin-right: 11px;}");
        itemWidget->addLabel(label, 0);

        label = new QLabel(rules.getName());
        label->setObjectName("main-label");
        font.setFamily("Ubuntu Light");
        font.setPointSize(13);
        label->setFont(font);
        label->setStyleSheet("QLabel{color: #452020; margin-right: 5px;}");
        itemWidget->addLabel(label, 1);

        ui->verticalLayout_competitionsRules->addWidget(itemWidget);
        itemWidget->update();
        competitionRulesListItems.push_back(itemWidget);

        connect(itemWidget, &DatabaseListItemWidget::itemSelected, this, &DatabaseEditorWindow::updateItemsSelection);
        i++;
    }
    ui->verticalLayout_competitionsRules->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Ignored, QSizePolicy::Expanding));
}

void DatabaseEditorWindow::updateIndexes()
{
    int index;
    if(actualElementType == JumperElement){
        index = 0;
        for (auto & jum : GlobalDatabase::get()->getEditableGlobalJumpers())
        {
            dynamic_cast<DatabaseListItemWidget *>(ui->verticalLayout_jumpers->itemAt(index)->widget())->setIndex(index+1);
            index++;
        }
    }
    else if(actualElementType == HillElement){
        index = 0;
        for (auto & hill : GlobalDatabase::get()->getEditableGlobalHills())
        {
            dynamic_cast<DatabaseListItemWidget *>(ui->verticalLayout_hills->itemAt(index)->widget())->setIndex(index+1);
            index++;
        }
    }
    else if(actualElementType == CompetitionRulesElement){
        index = 0;
        for (auto & rule : GlobalDatabase::get()->getEditableCompetitionRules())
        {
            dynamic_cast<DatabaseListItemWidget *>(ui->verticalLayout_competitionsRules->itemAt(index)->widget())->setIndex(index+1);
            index++;
        }
    }
    qDebug()<<"no";
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
        GlobalDatabase::get()->setGlobalJumpers(tempGlobalJumpers);
        GlobalDatabase::get()->setGlobalHills(tempGlobalHills);
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
    selectedItemIndex = -1;
    actualElementType = newActualElementType;
}

void DatabaseEditorWindow::on_pushButton_add_clicked()
{
    int index = 0;

    if(actualElementType == JumperElement){
        if(GlobalDatabase::get()->getGlobalJumpers().size() > 0 && selectedItemIndex > (-1))
            index = selectedItemIndex - 1;
        else index = -1;

        GlobalDatabase::get()->getEditableGlobalJumpers().insert(index + 1, Jumper("Name", "Surname", "XXX", JumperSkills()));
        fillJumpersWidget();
    }
    else if(actualElementType == HillElement)
    {
        if(GlobalDatabase::get()->getGlobalHills().size() > 0 && selectedItemIndex > (-1))
            index = selectedItemIndex - 1;
        else index = -1;

        GlobalDatabase::get()->getEditableGlobalHills().insert(index + 1, Hill("Hill", "XXX", 0, 0));
        fillHillsWidget();
    }
    else if(actualElementType == CompetitionRulesElement)
    {
        if(GlobalDatabase::get()->getGlobalCompetitionsRules().size() > 0 && selectedItemIndex > (-1))
            index = selectedItemIndex - 1;
        else index = -1;

        GlobalDatabase::get()->getEditableCompetitionRules().insert(index + 1, CompetitionRules("Rules"));
        fillCompetitionRulesWidget();
    }
    updateIndexes();
    updateItemsSelection(index+2);
}


void DatabaseEditorWindow::on_pushButton_remove_clicked()
{
    if(actualElementType == JumperElement){
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
    }
    else if(actualElementType == HillElement){
        if(GlobalDatabase::get()->getGlobalHills().size() > 0 && selectedItemIndex > (-1))
        {
            int index = selectedItemIndex - 1;
            GlobalDatabase::get()->getEditableGlobalHills().remove(index, 1);
            fillHillsWidget();
            updateIndexes();

            if(GlobalDatabase::get()->getGlobalHills().size() > 0)
            {
                if(index == GlobalDatabase::get()->getGlobalHills().size())
                    updateItemsSelection(index);
                else updateItemsSelection(index + 1);
            }
        }
    }
    else if(actualElementType == CompetitionRulesElement){
        if(GlobalDatabase::get()->getGlobalCompetitionsRules().size() > 0 && selectedItemIndex > (-1))
        {
            int index = selectedItemIndex - 1;
            GlobalDatabase::get()->getEditableCompetitionRules().remove(index, 1);
            fillCompetitionRulesWidget();
            updateIndexes();

            if(GlobalDatabase::get()->getGlobalHills().size() > 0)
            {
                if(index == GlobalDatabase::get()->getGlobalCompetitionsRules().size())
                    updateItemsSelection(index);
                else updateItemsSelection(index + 1);
            }
        }
    }
}

void DatabaseEditorWindow::replaceJumperFromJumperEditor()
{
    if(selectedItemIndex > (-1)){
        GlobalDatabase::get()->getEditableGlobalJumpers().replace(selectedItemIndex - 1, jumperEditor->getJumperFromWidgetInput());
        fillJumpersWidget();
        updateItemsSelection(selectedItemIndex);
    }
}

void DatabaseEditorWindow::replaceHillFromHillEditor()
{
    if(selectedItemIndex > (-1)){
        GlobalDatabase::get()->getEditableGlobalHills().replace(selectedItemIndex - 1, hillEditor->getHillFromWidgetInput());
        fillHillsWidget();
        updateItemsSelection(selectedItemIndex);
    }
}

void DatabaseEditorWindow::replaceCompetitionRulesFromEditor()
{
    if(selectedItemIndex > (-1)){
        GlobalDatabase::get()->getEditableCompetitionRules().replace(selectedItemIndex - 1, competitionRulesEditor->getCompetitionRulesFromWidgetInputs());
        fillCompetitionRulesWidget();
        updateItemsSelection(selectedItemIndex);
    }
}

QVector<CompetitionRules> DatabaseEditorWindow::getTempGlobalCompetitionRules() const
{
    return tempGlobalCompetitionRules;
}

void DatabaseEditorWindow::setTempGlobalCompetitionRules(const QVector<CompetitionRules> &newTempGlobalCompetitionRules)
{
    tempGlobalCompetitionRules = newTempGlobalCompetitionRules;
}

QVector<Hill> DatabaseEditorWindow::getTempGlobalHills() const
{
    return tempGlobalHills;
}

void DatabaseEditorWindow::setTempGlobalHills(const QVector<Hill> &newTempGlobalHills)
{
    tempGlobalHills = newTempGlobalHills;
}

QVector<Jumper> DatabaseEditorWindow::getTempGlobalJumpers() const
{
    return tempGlobalJumpers;
}

void DatabaseEditorWindow::setTempGlobalJumpers(const QVector<Jumper> &newTempGlobalJumpers)
{
    tempGlobalJumpers = newTempGlobalJumpers;
}

//zawsze jak zostaje 1 zawodnik, i się go usuwa, wtedy program sie crashuje.


void DatabaseEditorWindow::on_pushButton_up_clicked()
{
    int index = selectedItemIndex - 1;
    if(actualElementType == JumperElement){
        if((!(index < 1)) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(index, index - 1);
            fillJumpersWidget();
            updateIndexes();
            updateItemsSelection(index);
        }
    }
    else if(actualElementType == HillElement){
        if((!(index < 1)) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableGlobalHills().swapItemsAt(index, index - 1);
            fillHillsWidget();
            updateIndexes();
            updateItemsSelection(index);
        }
    }
    else if(actualElementType == CompetitionRulesElement){
        if((!(index < 1)) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableCompetitionRules().swapItemsAt(index, index - 1);
            fillCompetitionRulesWidget();
            updateIndexes();
            updateItemsSelection(index);
        }
    }
}

void DatabaseEditorWindow::on_pushButton_down_clicked()
{
    int index = selectedItemIndex - 1;
    if(actualElementType == JumperElement){
        if((!(index + 1 >= GlobalDatabase::get()->getGlobalJumpers().size())) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableGlobalJumpers().swapItemsAt(index, index + 1);
            fillJumpersWidget();
            updateIndexes();
            updateItemsSelection(index + 2);
        }
    }
    else if(actualElementType == HillElement){
        if((!(index + 1 >= GlobalDatabase::get()->getGlobalHills().size())) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableGlobalHills().swapItemsAt(index, index + 1);
            fillHillsWidget();
            updateIndexes();
            updateItemsSelection(index + 2);
        }
    }
    else if(actualElementType == CompetitionRulesElement){
        if((!(index + 1 >= GlobalDatabase::get()->getGlobalCompetitionsRules().size())) && selectedItemIndex > (-1))
        {
            GlobalDatabase::get()->getEditableCompetitionRules().swapItemsAt(index, index + 1);
            fillCompetitionRulesWidget();
            updateIndexes();
            updateItemsSelection(index + 2);
        }
    }
}

