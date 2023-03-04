#include "SingleJumpsConfigWindow.h"
#include "ui_SingleJumpsConfigWindow.h"

#include "../EditorWidgets/JumperEditorWidget.h"
#include "../EditorWidgets/HillEditorWidget.h"
#include "../EditorWidgets/WindsGeneratorSettingsEditorWidget.h"

#include "../../global/GlobalDatabase.h"

#include <QStringList>
#include <QStringListModel>
#include <QScrollArea>

SingleJumpsConfigWindow::SingleJumpsConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleJumpsConfigWindow)
{
    ui->setupUi(this);

    ui->lineEdit_resultsFileName->setAlignment(Qt::AlignHCenter);

    setWindowFlags(Qt::Window);

    jumperEditor = new JumperEditorWidget;
    jumperEditor->removeSubmitButton();

    hillEditor = new HillEditorWidget;
    hillEditor->removeSubmitButton();

    windsGeneratorSettingsEditor = new WindsGeneratorSettingsEditorWidget;
    windsGeneratorSettingsEditor->setRemovingSubmitButtons(true);


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

    ui->verticalLayout_jumperContent->addWidget(jumperEditor);
    ui->verticalLayout_hillContent->addWidget(hillEditor);
    ui->verticalLayout_windsGeneratorSettings->addWidget(windsGeneratorSettingsEditor);
    connect(hillEditor, &HillEditorWidget::KPointInputChanged, windsGeneratorSettingsEditor, &WindsGeneratorSettingsEditorWidget::fillSettingsInputs);

    ui->lineEdit_resultsFileName->setEnabled(false);

    ui->toolBox->setCurrentIndex(0);
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
        emit hillEditor->KPointInputChanged(const_cast<Hill *>(&GlobalDatabase::get()->getGlobalHills().at(index - 1))->getKPoint());
    }
    else hillEditor->resetHillInputs();
}

void SingleJumpsConfigWindow::on_pushButton_submit_clicked()
{
    accept();
}

WindsGeneratorSettingsEditorWidget *SingleJumpsConfigWindow::getWindsGeneratorSettingsEditor() const
{
    return windsGeneratorSettingsEditor;
}

int SingleJumpsConfigWindow::getJumpsCountFromInput()
{
    return ui->spinBox_jumpsCount->value();
}

bool SingleJumpsConfigWindow::getChangeableWindFromInput()
{
    return ui->checkBox_changeableWind->isChecked();
}

bool SingleJumpsConfigWindow::getSaveResultsToFileFromInput()
{
    return ui->checkBox_saveResultsFile->isChecked();
}

QString SingleJumpsConfigWindow::getResultsFileName()
{
    return ui->lineEdit_resultsFileName->text();
}

short SingleJumpsConfigWindow::getWindAverageCalculatingType()
{
    return ui->comboBox_windAverageCalculatingType->currentIndex();
}

short SingleJumpsConfigWindow::getWindCompensationDistanceEffect()
{
    return ui->comboBox_windCompensationDistanceEffect->currentIndex();
}

int SingleJumpsConfigWindow::getGateFromInput()
{
    return ui->spinBox_gate->value();
}

HillEditorWidget *SingleJumpsConfigWindow::getHillEditor() const
{
    return hillEditor;
}

JumperEditorWidget *SingleJumpsConfigWindow::getJumperEditor() const
{
    return jumperEditor;
}


void SingleJumpsConfigWindow::on_checkBox_saveResultsFile_stateChanged(int arg1)
{
    if(arg1 == 2)
        ui->lineEdit_resultsFileName->setEnabled(true);
    else ui->lineEdit_resultsFileName->setEnabled(false);
}

