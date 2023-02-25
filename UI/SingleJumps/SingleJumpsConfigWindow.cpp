#include "SingleJumpsConfigWindow.h"
#include "ui_SingleJumpsConfigWindow.h"

#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"

#include "../../global/GlobalDatabase.h"

#include <QStringList>
#include <QStringListModel>
#include <QScrollArea>

SingleJumpsConfigWindow::SingleJumpsConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsConfigWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    jumperEditor = new JumperEditorWidget;
    hillEditor = new HillEditorWidget;
    scrollArea = new QScrollArea;


    QStringList jumpersStrings;
    jumpersStrings.push_back("BRAK");
    for(const auto & jumper : GlobalDatabase::get()->getGlobalJumpers())
    {
        jumpersStrings.push_back(jumper.getNameAndSurname() + " (" + jumper.getCountryCode() + ")");
    }
    QStringListModel * jumpersStringsModel = new QStringListModel(jumpersStrings);
    ui->comboBox_existingJumper->setModel(jumpersStringsModel);

    QStringList hillsStrings;
    hillsStrings.push_back("BRAK");
    for(const auto & hill : GlobalDatabase::get()->getGlobalHills())
    {
        hillsStrings.push_back(hill.getName() + " HS" + QString::number(hill.getHSPoint()) + " (" + hill.getCountryCode() + ")");
    }
    QStringListModel * hillsStringsModel = new QStringListModel(hillsStrings);
    ui->comboBox_existingHill->setModel(hillsStringsModel);

    ui->verticalLayout_existingJumper->addWidget(jumperEditor);
    ui->verticalLayout_existingHill->addWidget(hillEditor);
}

SingleJumpsConfigWindow::~SingleJumpsConfigWindow()
{
    delete ui;
}

void SingleJumpsConfigWindow::on_comboBox_existingJumper_currentIndexChanged(int index)
{
    if(index > 0){
        jumperEditor->setJumper(const_cast<Jumper *>(&GlobalDatabase::get()->getGlobalJumpers().at(index - 1)));
        jumperEditor->fillJumperInputs();
    }
    else jumperEditor->resetJumperInputs();
}

void SingleJumpsConfigWindow::on_comboBox_existingHill_currentIndexChanged(int index)
{
    if(index > 0){
        hillEditor->setHill(const_cast<Hill *>(&GlobalDatabase::get()->getGlobalHills().at(index - 1)));
        hillEditor->fillHillInputs();
    }
    else hillEditor->resetHillInputs();
}
