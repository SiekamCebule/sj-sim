#include "AppSettingsWindow.h"
#include "ui_AppSettingsWindow.h"

#include <QTranslator>
#include <QCloseEvent>

#include "../mainwindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../global/GlobalTranslators.h"
#include "../../global/CountryFlagsManager.h"
#include "../../global/GlobalSimulationSettings.h"

AppSettingsWindow::AppSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowFlags(Qt::Window);

    ui->spinBox_skillsRange->setValue(GlobalSimulationSettings::get()->getMaxSkills());
    ui->spinBox_dsqProbability->setValue(GlobalSimulationSettings::get()->getBaseDsqProbability());
}

AppSettingsWindow::~AppSettingsWindow()
{
    delete ui;
}

void AppSettingsWindow::setupLanguagesComboBox()
{
    ui->comboBox_language->addItem(QIcon(CountryFlagsManager::getFlagPixmap("pl")), QObject::tr("Polski"));
    ui->comboBox_language->addItem(QIcon(CountryFlagsManager::getFlagPixmap("gb")), QObject::tr("Angielski"));

    ui->comboBox_language->setCurrentIndex(GlobalAppSettings::get()->getLanguageID());
    connect(ui->comboBox_language, &QComboBox::currentIndexChanged, this, &AppSettingsWindow::myConnect_on_comboBox_language_currentIndexChanged);
}

void AppSettingsWindow::on_pushButton_defaultSettings_clicked()
{
    ui->comboBox_language->setCurrentIndex(0);
}

MainWindow *AppSettingsWindow::getMainWindowParent() const
{
    return mainWindowParent;
}

void AppSettingsWindow::setMainWindowParent(MainWindow *newMainWindowParent)
{
    mainWindowParent = newMainWindowParent;
}

void AppSettingsWindow::closeEvent(QCloseEvent *event)
{
    GlobalSimulationSettings::get()->updateSimulationRandomMultiplier();
    GlobalSimulationSettings::get()->writeToFile();
    event->accept();
}

void AppSettingsWindow::myConnect_on_comboBox_language_currentIndexChanged(int index)
{
    switch(index)
    {
    case GlobalAppSettings::Polish:
        dynamic_cast<MainWindow *>(getMainWindowParent())->getParentApplication()->removeTranslator(const_cast<QTranslator *>(GlobalTranslators::get()->getGlobalTranslations().at(0)));
        ui->retranslateUi(this);
        GlobalAppSettings::get()->setLanguageID(GlobalAppSettings::Polish);
        break;
    case GlobalAppSettings::English:
        dynamic_cast<MainWindow *>(getMainWindowParent())->getParentApplication()->installTranslator(const_cast<QTranslator *>(GlobalTranslators::get()->getGlobalTranslations().at(0)));
        ui->retranslateUi(this);
        GlobalAppSettings::get()->setLanguageID(GlobalAppSettings::English);
        break;
    }
}

void AppSettingsWindow::on_spinBox_dsqProbability_valueChanged(int arg1)
{
    GlobalSimulationSettings::get()->setBaseDsqProbability(arg1);
}

void AppSettingsWindow::on_spinBox_skillsRange_valueChanged(int arg1)
{
    GlobalSimulationSettings::get()->setMaxSkills(arg1);
}

