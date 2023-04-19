#include "NewSimulationSaveConfigurationWindow.h"
#include "ui_NewSimulationSaveConfigurationWindow.h"
#include <QDate>

NewSimulationSaveConfigurationWindow::NewSimulationSaveConfigurationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSimulationSaveConfigurationWindow)
{
    ui->setupUi(this);
    ui->spinBox_seasonNumber->setValue(QDate::currentDate().year());
    setWindowFlags(Qt::Window);
}

NewSimulationSaveConfigurationWindow::~NewSimulationSaveConfigurationWindow()
{
    delete ui;
}

QString NewSimulationSaveConfigurationWindow::getNameFromInput() const
{
    return ui->lineEdit_name->text();
}

int NewSimulationSaveConfigurationWindow::getSeasonNumberFromInput() const
{
    return ui->spinBox_seasonNumber->value();
}

void NewSimulationSaveConfigurationWindow::on_pushButton_submit_clicked()
{
    accept();
}

