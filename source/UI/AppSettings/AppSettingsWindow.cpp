#include "AppSettingsWindow.h"
#include "ui_AppSettingsWindow.h"

#include <QTranslator>
#include <QCloseEvent>

#include "../mainwindow.h"
#include "../../global/GlobalAppSettings.h"
#include "../../global/GlobalTranslators.h"
#include "../../global/CountryFlagsManager.h"
#include "../../global/GlobalSimulationSettings.h"
#include "SimulationSettingsWindow.h"

AppSettingsWindow::AppSettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsWindow)
{
    ui->setupUi(this);
    setFixedSize(size());

    setWindowFlags(Qt::Window);
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
    event->accept();
    accept();
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
void AppSettingsWindow::on_pushButton_simulationSettings_clicked()
{
    SimulationSettingsWindow window;
    window.setWindowFlags(Qt::Window);
    window.fillInputs();
    connect(&window, &SimulationSettingsWindow::submitted, &SimulationSettingsWindow::accept);
    if(window.exec() == QDialog::Accepted)
    {
        GlobalSimulationSettings * s = GlobalSimulationSettings::get();
        s->setTakeoffRatingBaseRandomBaseValue(window.getTakeoffRatingBaseRandomBaseValueFromInput());
        s->setTakeoffRatingBaseRandomDeviationBaseValue(window.getTakeoffRatingBaseRandomDeviationBaseValueFromInput());
        s->setTakeoffRatingBaseRandomDeviationSubstractCharacteristicDivider(window.getTakeoffRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput());
        s->setTakeoffRatingLogRandomBaseValue(window.getTakeoffRatingLogRandomBaseValueFromInput());
        s->setTakeoffRatingLogRandomDeviationBaseValue(window.getTakeoffRatingLogRandomDeviationBaseValueFromInput());
        s->setTakeoffRatingLogRandomDeviationJumpsEqualityDivider(window.getTakeoffRatingLogRandomDeviationJumpsEqualityDividerFromInput());
        s->setTakeoffRatingTakeoffHeightAbsValue(window.getTakeoffRatingTakeoffHeightAbsValueFromInput());
        //
        s->setFlightRatingBaseRandomBaseValue(window.getFlightRatingBaseRandomBaseValueFromInput());
        s->setFlightRatingBaseRandomDeviationBaseValue(window.getFlightRatingBaseRandomDeviationBaseValueFromInput());
        s->setFlightRatingBaseRandomDeviationSubstractCharacteristicDivider(window.getFlightRatingBaseRandomDeviationSubstractCharacteristicDividerFromInput());
        s->setFlightRatingLogRandomBaseValue(window.getFlightRatingLogRandomBaseValueFromInput());
        s->setFlightRatingLogRandomDeviationBaseValue(window.getFlightRatingLogRandomDeviationBaseValueFromInput());
        s->setFlightRatingLogRandomDeviationJumpsEqualityDivider(window.getFlightRatingLogRandomDeviationJumpsEqualityDividerFromInput());
        s->setFlightRatingFlightHeightAbsValue(window.getFlightRatingFlightHeightAbsValueFromInput());

        s->writeToFile();
    }
}

